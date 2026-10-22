#include "assets/rom.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "assets/zip.h"

/* MAME `galaga` (0.289), then the device ROMs: name, CRC32, size, and
   where the file goes. */
#define ROM_FILES 16
#define SET_FILES 14 /* the galaga set; the device ROMs follow */
enum { R_MAIN, R_SUB, R_SUB2, R_GFX1, R_GFX2, R_PROMS, R_NAMCO, R_51XX, R_54XX };
static const struct {
    const char *mame;
    uint32_t crc;
    unsigned size;
    int region;      /* R_* */
    unsigned offset; /* within the region */
} files[ROM_FILES] = {
    { "gg1_1b.3p", 0xab036c9f, 0x1000, R_MAIN, 0x0000 },
    { "gg1_2b.3m", 0xd9232240, 0x1000, R_MAIN, 0x1000 },
    { "gg1_3.2m", 0x753ce503, 0x1000, R_MAIN, 0x2000 },
    { "gg1_4b.2l", 0x499fcc76, 0x1000, R_MAIN, 0x3000 },
    { "gg1_5b.3f", 0xbb5caae3, 0x1000, R_SUB, 0 },
    { "gg1_7b.2c", 0xd016686b, 0x1000, R_SUB2, 0 },
    { "gg1_9.4l", 0x58b2f47c, 0x1000, R_GFX1, 0 },
    { "gg1_11.4d", 0xad447c80, 0x1000, R_GFX2, 0x0000 },
    { "gg1_10.4f", 0xdd6f1afc, 0x1000, R_GFX2, 0x1000 },
    { "prom-5.5n", 0x54603c6b, 0x20, R_PROMS, 0x000 },
    { "prom-4.2n", 0x59b6edab, 0x100, R_PROMS, 0x020 },
    { "prom-3.1c", 0x4a04bb6b, 0x100, R_PROMS, 0x120 },
    { "prom-1.1d", 0x7a2815b4, 0x100, R_NAMCO, 0x000 },
    { "prom-2.5c", 0x77245b66, 0x100, R_NAMCO, 0x100 },
    { "51xx.bin", 0xc2f57ef8, 0x400, R_51XX, 0 },
    { "54xx.bin", 0xee7357e0, 0x400, R_54XX, 0 },
};
#define MAX_FILE_SIZE 0x1000

static const char *const region_names[ROM_REGIONS] = { "maincpu", "sub", "sub2", "gfx1", "gfx2",
                                                       "proms", "namco", "51xx", "54xx" };

static uint32_t crc32(const uint8_t *p, size_t n)
{
    uint32_t c = 0xFFFFFFFFu;
    while (n--) {
        c ^= *p++;
        for (int k = 0; k < 8; k++) c = c & 1 ? 0xEDB88320u ^ (c >> 1) : c >> 1;
    }
    return c ^ 0xFFFFFFFFu;
}

static uint8_t *region_base(RomSet *rom, int region)
{
    switch (region) {
    case R_MAIN: return rom->main;
    case R_SUB: return rom->sub;
    case R_SUB2: return rom->sub2;
    case R_GFX1: return rom->gfx1;
    case R_GFX2: return rom->gfx2;
    case R_PROMS: return rom->proms;
    case R_NAMCO: return rom->namco;
    case R_51XX: return rom->mcu51;
    default: return rom->mcu54;
    }
}

static uint8_t *slot(RomSet *rom, int i) { return region_base(rom, files[i].region) + files[i].offset; }

/* Every file of a set file's size in a folder, kept where its CRC matches one. */
static void scan_folder(RomSet *rom, const char *dir, int *found)
{
    DIR *d = opendir(dir);
    if (!d) return;
    struct dirent *e;
    uint8_t buf[MAX_FILE_SIZE];
    while ((e = readdir(d))) {
        char path[1100];
        snprintf(path, sizeof path, "%s/%s", dir, e->d_name);
        struct stat st;
        if (stat(path, &st) || !S_ISREG(st.st_mode)) continue;
        if (st.st_size != 0x1000 && st.st_size != 0x400 && st.st_size != 0x100 && st.st_size != 0x20) continue;
        FILE *f = fopen(path, "rb");
        if (!f) continue;
        size_t got = fread(buf, 1, (size_t)st.st_size, f);
        fclose(f);
        if (got != (size_t)st.st_size) continue;
        uint32_t crc = crc32(buf, got);
        for (int i = 0; i < ROM_FILES; i++)
            if (!found[i] && crc == files[i].crc && got == files[i].size) {
                memcpy(slot(rom, i), buf, got);
                found[i] = 1;
            }
    }
    closedir(d);
}

/* The files still missing from a zip, by CRC. Returns -1 if it cannot be read. */
static int scan_zip(RomSet *rom, const char *zip, int *found, char *err, int errlen)
{
    for (int i = 0; i < ROM_FILES; i++) {
        if (found[i]) continue;
        uint8_t *data;
        size_t len;
        int rc = zip_read_crc(zip, files[i].crc, &data, &len, err, errlen);
        if (rc == -1) return -1;
        if (rc == 0) {
            if (len == files[i].size) {
                memcpy(slot(rom, i), data, len);
                found[i] = 1;
            }
            free(data);
        }
    }
    return 0;
}

/* The device ROMs as MAME keeps them: namco51.zip / namco54.zip, or
   namco51/ and namco54/ folders, in dir. Unreadable zips are skipped. */
static void scan_devices(RomSet *rom, const char *dir, int *found)
{
    static const char *const names[2] = { "namco51", "namco54" };
    for (int k = 0; k < 2; k++) {
        if (found[SET_FILES + k]) continue;
        char path[1100], err[256];
        struct stat st;
        snprintf(path, sizeof path, "%s/%s.zip", dir, names[k]);
        if (stat(path, &st) == 0 && S_ISREG(st.st_mode)) scan_zip(rom, path, found, err, sizeof err);
        snprintf(path, sizeof path, "%s/%s", dir, names[k]);
        if (stat(path, &st) == 0 && S_ISDIR(st.st_mode)) scan_folder(rom, path, found);
    }
}

/* The folder holding path (a zip), or "." */
static void parent_dir(const char *path, char *out, size_t outlen)
{
    snprintf(out, outlen, "%s", path);
    char *slash = strrchr(out, '/');
#ifdef _WIN32
    char *bslash = strrchr(out, '\\');
    if (bslash > slash) slash = bslash;
#endif
    if (slash == out) out[1] = '\0';
    else if (slash) *slash = '\0';
    else snprintf(out, outlen, ".");
}

int rom_load(RomSet *rom, const char *src, char *err, int errlen)
{
    memset(rom, 0, sizeof *rom);
    struct stat st;
    if (stat(src, &st) != 0) {
        snprintf(err, (size_t)errlen, "%s: not found", src);
        return -1;
    }
    int found[ROM_FILES] = { 0 };
    char dir[1024];
    if (S_ISREG(st.st_mode)) {
        if (scan_zip(rom, src, found, err, errlen)) return -1;
        parent_dir(src, dir, sizeof dir);
        scan_devices(rom, dir, found);
    } else {
        scan_folder(rom, src, found);
        scan_devices(rom, src, found);
        snprintf(dir, sizeof dir, "%s/..", src);
        scan_devices(rom, dir, found);
    }
    for (int i = 0; i < SET_FILES; i++)
        if (!found[i]) {
            snprintf(err, (size_t)errlen, "%s: missing %s (CRC %08x); the MAME 'galaga' set is required", src,
                     files[i].mame, (unsigned)files[i].crc);
            memset(rom, 0, sizeof *rom);
            return -1;
        }
    rom->has_51xx = found[SET_FILES];
    rom->has_54xx = found[SET_FILES + 1];
    rom->loaded = 1;
    return 0;
}

int rom_byte(const RomSet *rom, unsigned addr)
{
    if (!rom->loaded || addr >= ROM_MAIN_SIZE) return -1;
    return rom->main[addr];
}

const char *rom_region_name(int i) { return i >= 0 && i < ROM_REGIONS ? region_names[i] : NULL; }

const uint8_t *rom_region(const RomSet *rom, int i, unsigned *size)
{
    static const unsigned sizes[ROM_REGIONS] = { ROM_MAIN_SIZE,  ROM_SUB_SIZE,   ROM_SUB2_SIZE,
                                                 ROM_GFX1_SIZE,  ROM_GFX2_SIZE,  ROM_PROMS_SIZE,
                                                 ROM_NAMCO_SIZE, ROM_MCU_SIZE,   ROM_MCU_SIZE };
    if (i < 0 || i >= ROM_REGIONS || !rom->loaded) return NULL;
    if ((i == R_51XX && !rom->has_51xx) || (i == R_54XX && !rom->has_54xx)) return NULL;
    *size = sizes[i];
    return region_base((RomSet *)rom, i);
}

/* want_dir: a folder; else a regular file */
static int exists(const char *path, int want_dir)
{
    struct stat st;
    return stat(path, &st) == 0 && (want_dir ? S_ISDIR(st.st_mode) : S_ISREG(st.st_mode));
}

int rom_locate(const char *argv0, char *out, size_t outlen)
{
    const char *env = getenv("GALAGA_ROM");
    if (env && *env && (exists(env, 0) || exists(env, 1))) {
        snprintf(out, outlen, "%s", env);
        return 0;
    }
    char exe_dir[900] = ".";
    if (argv0) parent_dir(argv0, exe_dir, sizeof exe_dir);
    const char *const dirs[2] = { exe_dir, "." };
    /* the folder must be a folder: the program itself is called galaga too */
    const char *const names[2] = { "galaga.zip", "galaga" };
    for (int d = 0; d < 2; d++)
        for (int n = 0; n < 2; n++) {
            snprintf(out, outlen, "%s/%s", dirs[d], names[n]);
            if (exists(out, n == 1)) return 0;
        }
#ifdef GALAGA_SOURCE_DIR
    for (int n = 0; n < 2; n++) { /* development builds: the source tree's roms/ */
        snprintf(out, outlen, "%s/roms/%s", GALAGA_SOURCE_DIR, names[n]);
        if (exists(out, n == 1)) return 0;
    }
#endif
    out[0] = '\0';
    return -1;
}
