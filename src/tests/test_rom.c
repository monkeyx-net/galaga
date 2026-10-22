/*
 * The ROM set loader against the user's galaga set (label rom):
 *   test_rom <galaga.zip or folder> <scratch dir>
 * Loads it as given, then from a folder of the same ROMs under other names
 * (matching is by CRC), with the device ROMs in it, then without them
 * (they are optional), then checks an empty folder fails with a message.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "assets/rom.h"

#ifdef _WIN32
#include <direct.h>
#define mkdir(dir, mode) _mkdir(dir)
#endif

static int failures;
#define CHECK(cond, ...)                                                    \
    do {                                                                    \
        if (!(cond)) {                                                      \
            failures++;                                                     \
            fprintf(stderr, "FAIL %s:%d: ", __FILE__, __LINE__);            \
            fprintf(stderr, __VA_ARGS__);                                   \
            fputc('\n', stderr);                                            \
        }                                                                   \
    } while (0)

static RomSet a, b;

static void write_rom(const char *dir, const char *name, const uint8_t *data, size_t len)
{
    char path[1200];
    snprintf(path, sizeof path, "%s/%s", dir, name);
    FILE *f = fopen(path, "wb");
    CHECK(f && fwrite(data, 1, len, f) == len, "cannot write %s", path);
    if (f) fclose(f);
}

/* The set (and the device ROMs if with_mcus) under made-up names in dir. */
static void write_renamed(const char *dir, int with_mcus)
{
    mkdir(dir, 0755);
    for (int i = 0; i < 4; i++) {
        char name[32];
        snprintf(name, sizeof name, "main%d.bin", (i * 3) % 4);
        write_rom(dir, name, a.main + i * 0x1000, 0x1000);
    }
    write_rom(dir, "cpu_b.rom", a.sub, sizeof a.sub);
    write_rom(dir, "cpu_c.rom", a.sub2, sizeof a.sub2);
    write_rom(dir, "chars.rom", a.gfx1, sizeof a.gfx1);
    write_rom(dir, "obj_hi.rom", a.gfx2 + 0x1000, 0x1000);
    write_rom(dir, "obj_lo.rom", a.gfx2, 0x1000);
    write_rom(dir, "lookup_b.prom", a.proms + 0x120, 0x100);
    write_rom(dir, "lookup_a.prom", a.proms + 0x020, 0x100);
    write_rom(dir, "colours.prom", a.proms, 0x20);
    write_rom(dir, "wave.prom", a.namco, 0x100);
    write_rom(dir, "timing.prom", a.namco + 0x100, 0x100);
    if (with_mcus) {
        write_rom(dir, "io_mcu.bin", a.mcu51, sizeof a.mcu51);
        write_rom(dir, "noise_mcu.bin", a.mcu54, sizeof a.mcu54);
    }
    uint8_t junk[0x1000];
    memset(junk, 0x5A, sizeof junk);
    write_rom(dir, "unrelated.bin", junk, sizeof junk);
    write_rom(dir, "unrelated.prom", junk, 0x100);
}

static int same_set(const RomSet *x, const RomSet *y)
{
    return !memcmp(x->main, y->main, sizeof x->main) && !memcmp(x->sub, y->sub, sizeof x->sub) &&
           !memcmp(x->sub2, y->sub2, sizeof x->sub2) && !memcmp(x->gfx1, y->gfx1, sizeof x->gfx1) &&
           !memcmp(x->gfx2, y->gfx2, sizeof x->gfx2) && !memcmp(x->proms, y->proms, sizeof x->proms) &&
           !memcmp(x->namco, y->namco, sizeof x->namco);
}

int main(int argc, char **argv)
{
    if (argc != 3) {
        fprintf(stderr, "usage: test_rom <galaga.zip|folder> <scratch dir>\n");
        return 2;
    }
    char err[512];
    CHECK(!rom_load(&a, argv[1], err, sizeof err), "load %s: %s", argv[1], err);
    CHECK(rom_byte(&a, 0x0000) == a.main[0] && rom_byte(&a, 0x3FFF) == a.main[0x3FFF], "program ROM mapping");
    CHECK(rom_byte(&a, 0x4000) == -1 && rom_byte(&a, 0x8000) == -1, "RAM addresses read as ROM");
    unsigned size;
    CHECK(rom_region(&a, 0, &size) == a.main && size == 0x4000, "region maincpu");
    CHECK(rom_region(&a, 4, &size) == a.gfx2 && size == 0x2000, "region gfx2");
    CHECK(!strcmp(rom_region_name(6), "namco") && !rom_region_name(ROM_REGIONS), "region names");
    if (!a.has_51xx || !a.has_54xx)
        printf("rom: the device ROMs are not next to %s (namco51.zip, namco54.zip): their checks skipped\n",
               argv[1]);

    /* the same ROMs under other names in a folder, with unrelated files */
    mkdir(argv[2], 0755);
    char dir[1100];
    snprintf(dir, sizeof dir, "%s/renamed", argv[2]);
    write_renamed(dir, a.has_51xx && a.has_54xx);
    CHECK(!rom_load(&b, dir, err, sizeof err), "load renamed folder: %s", err);
    CHECK(same_set(&a, &b), "renamed folder loaded different ROMs");
    if (a.has_51xx && a.has_54xx)
        CHECK(b.has_51xx && b.has_54xx && !memcmp(a.mcu51, b.mcu51, sizeof a.mcu51) &&
                  !memcmp(a.mcu54, b.mcu54, sizeof a.mcu54),
              "renamed folder: device ROMs");

    /* without the device ROMs: still a set, the regions absent */
    snprintf(dir, sizeof dir, "%s/no_mcus", argv[2]);
    write_renamed(dir, 0);
    CHECK(!rom_load(&b, dir, err, sizeof err), "load folder without device ROMs: %s", err);
    CHECK(same_set(&a, &b) && !b.has_51xx && !b.has_54xx, "folder without device ROMs");
    CHECK(!rom_region(&b, 7, &size) && !rom_region(&b, 8, &size), "absent device ROMs have regions");

    snprintf(dir, sizeof dir, "%s/empty", argv[2]);
    mkdir(dir, 0755);
    err[0] = '\0';
    CHECK(rom_load(&b, dir, err, sizeof err) == -1 && strstr(err, "missing gg1_1b.3p"),
          "empty folder: rc/err '%s'", err);
    CHECK(!b.loaded && rom_byte(&b, 0) == -1 && !rom_region(&b, 0, &size), "failed load left the set usable");

    if (failures) {
        fprintf(stderr, "%d failure(s)\n", failures);
        return 1;
    }
    puts("rom: all checks passed");
    return 0;
}
