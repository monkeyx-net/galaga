#include "assets/extract.h"

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>
#define mkdir(dir, mode) _mkdir(dir)
#endif

#include "assets/png.h"
#include "emu/board.h"
#include "gfx/galaga.h"

/* The programs' tables the game in C reads, by address (tables.txt). */
static const struct {
    unsigned first, last;
    const char *what;
} tables[] = {
    { 0, 0, NULL }, /* none yet: the C phases add them */
};
#define TABLES ((int)(sizeof tables / sizeof tables[0]))

/* the attract mode's screens for the previews (the emulated board's frames) */
static const struct {
    long frame;
    const char *name;
} screens[] = { { 1000, "title" }, { 1300, "score_table" }, { 2500, "demo" } };
#define SCREENS ((int)(sizeof screens / sizeof screens[0]))

/* ---- files ----------------------------------------------------------------- */

static int mkdir_p(const char *path)
{
    char tmp[1100];
    snprintf(tmp, sizeof tmp, "%s", path);
    for (char *p = tmp + 1; *p; p++) {
        if (*p != '/') continue;
        *p = '\0';
        if (mkdir(tmp, 0755) && errno != EEXIST) return -1;
        *p = '/';
    }
    return mkdir(tmp, 0755) && errno != EEXIST ? -1 : 0;
}

/* Write `size` bytes to <dir>/<name> through a temp file, so a failure leaves no partial file. */
static int write_file(const char *dir, const char *name, const void *data, size_t size, char *err, int errlen)
{
    char path[1200], tmp[1210];
    snprintf(path, sizeof path, "%s/%s", dir, name);
    snprintf(tmp, sizeof tmp, "%s.tmp", path);
    FILE *f = fopen(tmp, "wb");
    if (!f || fwrite(data, 1, size, f) != size || fclose(f)) {
        if (f) fclose(f);
        remove(tmp);
        snprintf(err, (size_t)errlen, "cannot write %s", path);
        return -1;
    }
#ifdef _WIN32
    remove(path); /* Windows rename() does not replace */
#endif
    if (rename(tmp, path)) {
        remove(tmp);
        snprintf(err, (size_t)errlen, "cannot replace %s", path);
        return -1;
    }
    return 0;
}

/* A growing text buffer. */
typedef struct {
    char *s;
    size_t len, cap;
} Text;

static void put(Text *t, const char *fmt, ...) __attribute__((format(printf, 2, 3)));
static void put(Text *t, const char *fmt, ...)
{
    va_list ap;
    for (;;) {
        va_start(ap, fmt);
        int n = vsnprintf(t->s ? t->s + t->len : NULL, t->s ? t->cap - t->len : 0, fmt, ap);
        va_end(ap);
        if (n < 0) return;
        if (t->s && t->len + (size_t)n < t->cap) {
            t->len += (size_t)n;
            return;
        }
        size_t cap = t->cap ? t->cap * 2 : 4096;
        while (cap < t->len + (size_t)n + 1) cap *= 2;
        char *s = realloc(t->s, cap);
        if (!s) abort();
        t->s = s;
        t->cap = cap;
    }
}

static int write_text(const char *dir, const char *name, Text *t, char *err, int errlen)
{
    int rc = write_file(dir, name, t->s ? t->s : "", t->len, err, errlen);
    free(t->s);
    memset(t, 0, sizeof *t);
    return rc;
}

/* An RGB picture (0xRRGGBB) as a PNG under <dir>/gfx. */
static int write_png(const char *dir, const char *name, const uint32_t *px, int w, int h, char *err, int errlen)
{
    uint8_t *rgba = malloc((size_t)w * (size_t)h * 4);
    if (!rgba) {
        snprintf(err, (size_t)errlen, "out of memory");
        return -1;
    }
    for (long i = 0; i < (long)w * h; i++) {
        rgba[i * 4] = (uint8_t)(px[i] >> 16);
        rgba[i * 4 + 1] = (uint8_t)(px[i] >> 8);
        rgba[i * 4 + 2] = (uint8_t)px[i];
        rgba[i * 4 + 3] = 0xFF;
    }
    char path[1200];
    snprintf(path, sizeof path, "%s/gfx/%s", dir, name);
    int rc = png_write(path, rgba, w, h);
    free(rgba);
    if (rc) snprintf(err, (size_t)errlen, "cannot write %s", path);
    return rc ? -1 : 0;
}

/* ---- graphics ---------------------------------------------------------------- */

static GalagaVideo video;

/* A preview's colours: the four pens in grey. */
static const uint32_t grey[4] = { 0x000000, 0x555555, 0xAAAAAA, 0xFFFFFF };

static int extract_graphics(const char *dir, char *err, int errlen)
{
    char gfx[1100];
    snprintf(gfx, sizeof gfx, "%s/gfx", dir);
    Text t = {0};
    put(&t, "# the 256 tiles of gg1_9 as the player sees them: 8 rows of 8 pens\n");
    for (int n = 0; n < GALAGA_TILES; n++) {
        put(&t, "tile %02X\n", n);
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) put(&t, "%d", video.tiles[n][y][x]);
            put(&t, "\n");
        }
    }
    if (write_text(gfx, "tiles.txt", &t, err, errlen)) return -1;
    put(&t, "# the 128 sprites of gg1_11 and gg1_10 as the player sees them: 16 rows of 16 pens\n");
    for (int n = 0; n < GALAGA_SPRITES; n++) {
        put(&t, "sprite %02X\n", n);
        for (int y = 0; y < 16; y++) {
            for (int x = 0; x < 16; x++) put(&t, "%d", video.sprites[n][y][x]);
            put(&t, "\n");
        }
    }
    if (write_text(gfx, "sprites.txt", &t, err, errlen)) return -1;
    put(&t, "# the 32 colours of prom-5\n");
    for (int i = 0; i < 32; i++) put(&t, "%06X\n", (unsigned)video.palette[i]);
    put(&t, "# the tiles' colour lookup (prom-4): colour c, pen p at 4c + p; palette entry 16 + this, 15 transparent\n");
    for (int i = 0; i < 256; i++) put(&t, "%X%s", video.tile_lookup[i], i % 32 == 31 ? "\n" : "");
    put(&t, "# the sprites' colour lookup (prom-3): palette entry 0 + this, 15 transparent\n");
    for (int i = 0; i < 256; i++) put(&t, "%X%s", video.sprite_lookup[i], i % 32 == 31 ? "\n" : "");
    if (write_text(gfx, "palette.txt", &t, err, errlen)) return -1;

    static uint32_t px[256 * 128];
    for (int n = 0; n < GALAGA_TILES; n++) /* 16 x 16 tiles */
        for (int y = 0; y < 8; y++)
            for (int x = 0; x < 8; x++) px[((n / 16) * 8 + y) * 128 + (n % 16) * 8 + x] = grey[video.tiles[n][y][x]];
    if (write_png(dir, "tiles.png", px, 128, 128, err, errlen)) return -1;
    for (int n = 0; n < GALAGA_SPRITES; n++) /* 16 x 8 sprites */
        for (int y = 0; y < 16; y++)
            for (int x = 0; x < 16; x++) px[((n / 16) * 16 + y) * 256 + (n % 16) * 16 + x] = grey[video.sprites[n][y][x]];
    return write_png(dir, "sprites.png", px, 256, 128, err, errlen);
}

static int extract_font(const char *dir, char *err, int errlen)
{
    static const char chars[] = EXTRACT_FONT_CHARS;
    Text t = {0};
    put(&t, "# the game's characters: tile, character\n");
    for (int i = 0; chars[i]; i++) put(&t, "%02X %s\n", i, chars[i] == ' ' ? "space" : (char[2]){ chars[i], 0 });
    char gfx[1100];
    snprintf(gfx, sizeof gfx, "%s/gfx", dir);
    return write_text(gfx, "font.txt", &t, err, errlen);
}

/* ---- sound ------------------------------------------------------------------- */

static int extract_waves(const RomSet *rom, const char *dir, char *err, int errlen)
{
    char sound[1100];
    snprintf(sound, sizeof sound, "%s/sound", dir);
    if (mkdir_p(sound)) {
        snprintf(err, (size_t)errlen, "cannot create %s", sound);
        return -1;
    }
    Text t = {0};
    put(&t, "# the WSG's 8 waveforms of 32 samples (prom-1.1d)\n");
    for (int w = 0; w < 8; w++) {
        put(&t, "wave %d ", w);
        for (int i = 0; i < 32; i++) put(&t, "%X", rom->namco[w * 32 + i] & 15);
        put(&t, "\n");
    }
    return write_text(sound, "waves.txt", &t, err, errlen);
}

int extract_load_waves(const char *dir, uint8_t *waves)
{
    char path[1200], line[256];
    snprintf(path, sizeof path, "%s/sound/waves.txt", dir);
    FILE *f = fopen(path, "r");
    if (!f) return -1;
    int got = 0;
    while (fgets(line, sizeof line, f)) {
        int w, k;
        if (sscanf(line, "wave %d %n", &w, &k) != 1 || w < 0 || w > 7) continue;
        for (int i = 0; i < 32; i++) {
            char c = line[k + i];
            int v = c >= '0' && c <= '9' ? c - '0' : c >= 'A' && c <= 'F' ? c - 'A' + 10 : -1;
            if (v < 0) {
                fclose(f);
                return -1;
            }
            waves[w * 32 + i] = (uint8_t)v;
        }
        got |= 1 << w;
    }
    fclose(f);
    return got == 0xFF ? 0 : -1;
}

/* ---- tables ------------------------------------------------------------------ */

static int table_byte(const RomSet *rom, unsigned a)
{
    if (a < EXTRACT_SUB_BASE) return rom->main[a];
    if (a < EXTRACT_SUB2_BASE) return rom->sub[a - EXTRACT_SUB_BASE];
    return rom->sub2[a - EXTRACT_SUB2_BASE];
}

int extract_is_table(unsigned addr)
{
    for (int i = 0; i < TABLES; i++)
        if (tables[i].what && addr >= tables[i].first && addr <= tables[i].last) return 1;
    return 0;
}

static int extract_tables(const RomSet *rom, const char *dir, char *err, int errlen)
{
    Text t = {0};
    put(&t, "# the programs' tables the game in C reads (main; sub +$4000; sub2 +$5000)\n");
    for (int i = 0; i < TABLES; i++) {
        if (!tables[i].what) continue;
        put(&t, "# %s\n", tables[i].what);
        for (unsigned a = tables[i].first; a <= tables[i].last; a += 16) {
            put(&t, "%04X:", a);
            for (unsigned b = a; b < a + 16 && b <= tables[i].last; b++) put(&t, " %02X", table_byte(rom, b));
            put(&t, "\n");
        }
    }
    return write_text(dir, "tables.txt", &t, err, errlen);
}

int extract_load_tables(const char *dir, uint8_t *data, unsigned size)
{
    char path[1200], line[256];
    snprintf(path, sizeof path, "%s/tables.txt", dir);
    FILE *f = fopen(path, "r");
    if (!f) return -1;
    memset(data, 0, size);
    unsigned n = 0;
    while (fgets(line, sizeof line, f)) {
        unsigned a, v;
        int k;
        if (line[0] == '#' || sscanf(line, "%x:%n", &a, &k) != 1) continue;
        for (const char *p = line + k; sscanf(p, "%x%n", &v, &k) == 1; p += k, a++)
            if (a < size) data[a] = (uint8_t)v, n++;
    }
    fclose(f);
    unsigned want = 0;
    for (int i = 0; i < TABLES; i++)
        if (tables[i].what) want += tables[i].last - tables[i].first + 1;
    return n == want ? 0 : -1;
}

/* ---- the attract mode's screens ------------------------------------------------ */

static Board board;

static int extract_screens(const RomSet *rom, const char *dir, char *err, int errlen)
{
    if (!rom->has_51xx || !rom->has_54xx) return 0; /* the board needs them */
    board_init(&board, rom);
    static uint32_t px[GALAGA_W * GALAGA_H];
    int rc = 0;
    for (int i = 0; i < SCREENS && !rc; i++) {
        while (board.frame < screens[i].frame) board_run_frame(&board);
        GalagaFrame fr = { board.video, { board.ram[0], board.ram[1], board.ram[2] }, board.vlatch, board.star_origin };
        galaga_draw(&video, &fr);
        for (int y = 0; y < GALAGA_H; y++)
            for (int x = 0; x < GALAGA_W; x++) px[y * GALAGA_W + x] = video.screen[y][x] & 0xFFFFFF;
        char name[64];
        snprintf(name, sizeof name, "%s.png", screens[i].name);
        rc = write_png(dir, name, px, GALAGA_W, GALAGA_H, err, errlen);
    }
    return rc;
}

int extract_all(const RomSet *rom, const char *out_dir, char *err, int errlen)
{
    char path[1100];
    if (!rom->loaded) {
        snprintf(err, (size_t)errlen, "no ROM set loaded");
        return -1;
    }
    snprintf(path, sizeof path, "%s/%s", out_dir, EXTRACT_MANIFEST);
    remove(path); /* until the end, the folder is incomplete */
    snprintf(path, sizeof path, "%s/gfx", out_dir);
    if (mkdir_p(path)) {
        snprintf(err, (size_t)errlen, "cannot create %s", path);
        return -1;
    }
    galaga_video_init(&video, rom);
    if (extract_graphics(out_dir, err, errlen) || extract_font(out_dir, err, errlen) ||
        extract_waves(rom, out_dir, err, errlen) || extract_tables(rom, out_dir, err, errlen) ||
        extract_screens(rom, out_dir, err, errlen))
        return -1;
    Text t = {0};
    put(&t, "galaga assets %d\n", EXTRACT_VERSION);
    return write_text(out_dir, EXTRACT_MANIFEST, &t, err, errlen);
}
