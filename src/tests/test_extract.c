/*
 * The extractor against the user's galaga set (label rom):
 *   test_extract <galaga.zip or folder> <scratch dir>
 * Two extractions are byte-identical and hold every file; the graphics,
 * palette and colour lookups read back from them (gfx/galaga.c) are the
 * ROM set's, the waveforms too, and the tables (the ROMs' bytes in their
 * ranges); with the device ROMs, the font reads the title screen the
 * emulated board draws ("GALAGA", "SCORE", "HIGH SCORE", "CREDIT"), each
 * letter's pixels on screen its tile's; and with no ROM set nothing is
 * written, not even a manifest.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "assets/extract.h"
#include "assets/rom.h"
#include "emu/board.h"
#include "gfx/galaga.h"

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

static const char *const files[] = {
    "manifest.txt", "gfx/tiles.txt", "gfx/sprites.txt", "gfx/palette.txt", "gfx/font.txt",
    "gfx/tiles.png", "gfx/sprites.png", "sound/waves.txt", "tables.txt",
};
#define FILES ((int)(sizeof files / sizeof files[0]))
static const char *const screens[] = { "gfx/title.png", "gfx/score_table.png", "gfx/demo.png" };
#define SCREENS ((int)(sizeof screens / sizeof screens[0]))

static RomSet rom, none;
static GalagaVideo from_rom, from_assets;
static Board board;

static long read_all(const char *path, char **out)
{
    FILE *f = fopen(path, "rb");
    if (!f) return -1;
    fseek(f, 0, SEEK_END);
    long n = ftell(f);
    fseek(f, 0, SEEK_SET);
    *out = malloc((size_t)n + 1);
    if (fread(*out, 1, (size_t)n, f) != (size_t)n) n = -1;
    fclose(f);
    return n;
}

static int same_file(const char *a_dir, const char *b_dir, const char *name)
{
    char pa[1200], pb[1200], *a = NULL, *b = NULL;
    snprintf(pa, sizeof pa, "%s/%s", a_dir, name);
    snprintf(pb, sizeof pb, "%s/%s", b_dir, name);
    long na = read_all(pa, &a), nb = read_all(pb, &b);
    int same = na >= 0 && na == nb && !memcmp(a, b, (size_t)na);
    free(a);
    free(b);
    return same;
}

/* The characters of the font file by tile code */
static void load_font(const char *dir, char *chars)
{
    char path[1200], line[256];
    memset(chars, 0, 256);
    snprintf(path, sizeof path, "%s/gfx/font.txt", dir);
    FILE *f = fopen(path, "r");
    if (!f) return;
    while (fgets(line, sizeof line, f)) {
        unsigned n;
        char c[16];
        if (line[0] == '#' || sscanf(line, "%x %15s", &n, c) != 2 || n > 255) continue;
        chars[n] = !strcmp(c, "space") ? ' ' : c[0];
    }
    fclose(f);
}

/* A row of the screen (28 cells) as text through the font; '?' where the
   tile is no character */
static void row_text(const uint8_t *video, const char *chars, int y, char *out)
{
    for (int x = 0; x < 28; x++) {
        int o = y < 2 ? 0x3C0 + y * 0x20 + 29 - x : y >= 34 ? (y - 34) * 0x20 + 29 - x : 0x40 + (27 - x) * 32 + (y - 2);
        out[x] = chars[video[o]] ? chars[video[o]] : '?';
    }
    out[28] = 0;
}

static int screen_has(const uint8_t *video, const char *chars, const char *word, int *where)
{
    char text[29];
    for (int y = 0; y < 36; y++) {
        row_text(video, chars, y, text);
        char *p = strstr(text, word);
        if (p) {
            *where = y * 28 + (int)(p - text);
            return 1;
        }
    }
    return 0;
}

int main(int argc, char **argv)
{
    if (argc != 3) {
        fprintf(stderr, "usage: test_extract <galaga.zip|folder> <scratch dir>\n");
        return 2;
    }
    char err[512], a[1100], b[1100];
    if (rom_load(&rom, argv[1], err, sizeof err)) {
        fprintf(stderr, "%s\n", err);
        return 1;
    }
    mkdir(argv[2], 0755);
    snprintf(a, sizeof a, "%s/a", argv[2]);
    snprintf(b, sizeof b, "%s/b", argv[2]);
    CHECK(!extract_all(&rom, a, err, sizeof err), "extract to %s: %s", a, err);
    CHECK(!extract_all(&rom, b, err, sizeof err), "extract to %s: %s", b, err);
    int nscreens = rom.has_51xx && rom.has_54xx ? SCREENS : 0;
    for (int i = 0; i < FILES + nscreens; i++) {
        const char *name = i < FILES ? files[i] : screens[i - FILES];
        CHECK(same_file(a, b, name), "%s: missing or not byte-identical", name);
    }

    /* read back */
    galaga_video_init(&from_rom, &rom);
    CHECK(!galaga_video_load(&from_assets, a), "gfx: cannot load the extracted graphics");
    CHECK(!memcmp(from_rom.tiles, from_assets.tiles, sizeof from_rom.tiles), "tiles differ from the ROM's");
    CHECK(!memcmp(from_rom.sprites, from_assets.sprites, sizeof from_rom.sprites), "sprites differ from the ROM's");
    CHECK(!memcmp(from_rom.palette, from_assets.palette, sizeof from_rom.palette), "palette differs from the PROM's");
    CHECK(!memcmp(from_rom.tile_lookup, from_assets.tile_lookup, sizeof from_rom.tile_lookup) &&
              !memcmp(from_rom.sprite_lookup, from_assets.sprite_lookup, sizeof from_rom.sprite_lookup),
          "colour lookups differ from the PROMs'");
    uint8_t waves[256];
    CHECK(!extract_load_waves(a, waves), "sound/waves.txt: cannot load");
    int wave_bad = 0;
    for (int i = 0; i < 256; i++) wave_bad += waves[i] != (rom.namco[i] & 15);
    CHECK(!wave_bad, "%d waveform samples differ from prom-1.1d's", wave_bad);
    static uint8_t tables[EXTRACT_SPACE];
    CHECK(!extract_load_tables(a, tables, sizeof tables), "tables.txt: cannot load");
    for (unsigned addr = 0; addr < EXTRACT_SPACE; addr++) {
        int want = !extract_is_table(addr) ? 0
                   : addr < EXTRACT_SUB_BASE ? rom.main[addr]
                   : addr < EXTRACT_SUB2_BASE ? rom.sub[addr - EXTRACT_SUB_BASE] : rom.sub2[addr - EXTRACT_SUB2_BASE];
        if (tables[addr] != want) {
            CHECK(0, "tables.txt: $%04X is %02X, not %02X", addr, tables[addr], want);
            break;
        }
    }

    /* the font against the title screen the board draws */
    if (nscreens) {
        char chars[256];
        load_font(a, chars);
        board_init(&board, &rom);
        while (board.frame < 1000) board_run_frame(&board);
        static const char *const words[] = { "GALAGA", "SCORE", "HIGH SCORE", "CREDIT" };
        uint8_t no_stars[8];
        memcpy(no_stars, board.vlatch, sizeof no_stars);
        no_stars[5] = 0;
        GalagaFrame fr = { board.video, { board.ram[0], board.ram[1], board.ram[2] }, no_stars, 0 };
        galaga_draw(&from_assets, &fr);
        for (int w = 0; w < 4; w++) {
            int where = 0;
            CHECK(screen_has(board.video, chars, words[w], &where), "the title screen has no \"%s\" through the font", words[w]);
            if (w) continue;
            /* each letter of GALAGA on screen: its tile's pixels in its colour */
            int cy = where / 28, cx = where % 28;
            for (int i = 0; i < 6; i++) {
                int o = 0x40 + (27 - (cx + i)) * 32 + (cy - 2), code = board.video[o], colour = board.video[0x400 + o] & 0x3F;
                for (int y = 0; y < 8; y++)
                    for (int x = 0; x < 8; x++) {
                        int lk = from_assets.tile_lookup[colour * 4 + from_assets.tiles[code][y][x]];
                        uint32_t want = lk == 15 ? 0 : from_assets.palette[16 + lk];
                        if ((from_assets.screen[cy * 8 + y][(cx + i) * 8 + x] & 0xFFFFFF) != want) {
                            CHECK(0, "\"GALAGA\" letter %d: pixel %d,%d is not its tile's", i, x, y);
                            y = x = 8;
                        }
                    }
            }
        }
    } else {
        printf("extract: no device ROMs: the screens and the font's check skipped\n");
    }

    /* no ROM set: nothing written */
    char c[1100], manifest[1200];
    snprintf(c, sizeof c, "%s/none", argv[2]);
    snprintf(manifest, sizeof manifest, "%s/%s", c, EXTRACT_MANIFEST);
    remove(manifest);
    CHECK(extract_all(&none, c, err, sizeof err) == -1, "extraction without a ROM set succeeded");
    struct stat st;
    CHECK(stat(manifest, &st) != 0, "a manifest was written without a ROM set");

    if (failures) {
        fprintf(stderr, "%d failure(s)\n", failures);
        return 1;
    }
    puts("extract: all checks passed");
    return 0;
}
