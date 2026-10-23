#include "gfx/galaga.h"

#include <stdio.h>
#include <string.h>

static uint32_t prom_colour(uint8_t v)
{
    int r = 33 * (v & 1) + 71 * (v >> 1 & 1) + 151 * (v >> 2 & 1);
    int g = 33 * (v >> 3 & 1) + 71 * (v >> 4 & 1) + 151 * (v >> 5 & 1);
    int b = 81 * (v >> 6 & 1) + 174 * (v >> 7 & 1);
    return (uint32_t)(r << 16 | g << 8 | b);
}

/* The starfield: its 256 stars, from the shift register (not the ROM set) */
#define STAR_FIRST 40198 /* the register's steps from 1 to the sequence's start */

static void make_stars(GalagaVideo *v)
{
    static const uint8_t level[4] = { 0x00, 0x47, 0x97, 0xDE }, blue[4] = { 0x00, 0x51, 0xAE, 0xFF };
    uint32_t s = 1;
    int n = 0;
    for (int t = 0; t < GALAGA_STAR_PERIOD; t++) {
        if (!(s & 0x0429) && (s >> 12) == 15 && n < GALAGA_STARS) {
            uint32_t w = ~s;
            int r = (w >> 6 & 1) | (w >> 7 & 1) << 1, g = (w >> 8 & 1) | (w >> 1 & 1) << 1, b = (w >> 2 & 1) | (w >> 4 & 1) << 1;
            v->stars[n].step = (uint16_t)((t - STAR_FIRST + GALAGA_STAR_PERIOD) % GALAGA_STAR_PERIOD);
            v->stars[n].latch = (uint8_t)(s >> 11 & 1);
            v->stars[n].want = (uint8_t)(s >> 9 & 1);
            v->stars[n].rgb = (uint32_t)(level[r] << 16 | level[g] << 8 | blue[b]);
            n++;
        }
        s = s >> 1 | (uint32_t)__builtin_parity(s & 0x0429) << 15;
    }
}

int galaga_star_advance(int origin, const uint8_t *vlatch)
{
    static const int step[8] = { -1, -2, -3, -4, 3, 2, 1, 0 };
    int speed = (vlatch[0] & 1) | (vlatch[1] & 1) << 1 | (vlatch[2] & 1) << 2;
    return (origin + step[speed] + GALAGA_STAR_PERIOD) % GALAGA_STAR_PERIOD;
}

static void draw_stars(GalagaVideo *v, const GalagaFrame *f)
{
    if (!(f->vlatch[5] & 1)) return;
    for (int i = 0; i < GALAGA_STARS; i++) {
        if (!v->stars[i].rgb || (f->vlatch[3 + v->stars[i].latch] & 1) != v->stars[i].want) continue;
        int n = (v->stars[i].step + f->star_origin) % GALAGA_STAR_PERIOD;
        if (n >= 224 * 256) continue;
        v->screen[16 + n % 256][223 - n / 256] = v->stars[i].rgb;
    }
}

void galaga_video_init(GalagaVideo *v, const RomSet *rom)
{
    memset(v, 0, sizeof *v);
    make_stars(v);
    for (int c = 0; c < GALAGA_TILES; c++)
        for (int py = 0; py < 8; py++)
            for (int px = 0; px < 8; px++) {
                uint8_t byte = rom->gfx1[c * 16 + 7 - px + (py < 4 ? 8 : 0)];
                int b = 3 - (py & 3);
                v->tiles[c][py][px] = (uint8_t)((byte >> b & 1) | (byte >> (b + 4) & 1) << 1);
            }
    for (int c = 0; c < GALAGA_SPRITES; c++)
        for (int py = 0; py < 16; py++)
            for (int px = 0; px < 16; px++) {
                int y = 15 - px;
                uint8_t byte = rom->gfx2[c * 64 + (y < 8 ? y : 24 + y) + 8 * (py >> 2)];
                int b = 3 - (py & 3);
                v->sprites[c][py][px] = (uint8_t)((byte >> b & 1) | (byte >> (b + 4) & 1) << 1);
            }
    for (int i = 0; i < 256; i++) {
        v->tile_lookup[i] = rom->proms[0x020 + i] & 15;
        v->sprite_lookup[i] = rom->proms[0x120 + i] & 15;
    }
    for (int i = 0; i < 32; i++) v->palette[i] = prom_colour(rom->proms[i]);
}

/* Rows of pens after each "<word> NN" heading in <dir>/gfx/<name> */
static int load_pens(const char *dir, const char *name, const char *word, uint8_t *pens, int count, int size)
{
    char path[1200], line[512];
    snprintf(path, sizeof path, "%s/gfx/%s", dir, name);
    FILE *f = fopen(path, "r");
    if (!f) return -1;
    int got = 0, n = -1, row = 0;
    size_t wl = strlen(word);
    while (fgets(line, sizeof line, f)) {
        unsigned k;
        if (!strncmp(line, word, wl) && sscanf(line + wl, " %x", &k) == 1 && (int)k < count) {
            n = (int)k;
            row = 0;
            continue;
        }
        if (n < 0 || row >= size || line[0] == '#') continue;
        for (int x = 0; x < size; x++) {
            if (line[x] < '0' || line[x] > '3') {
                fclose(f);
                return -1;
            }
            pens[((size_t)n * (size_t)size + (size_t)row) * (size_t)size + (size_t)x] = (uint8_t)(line[x] - '0');
        }
        if (++row == size) got++;
    }
    fclose(f);
    return got == count ? 0 : -1;
}

int galaga_video_load(GalagaVideo *v, const char *dir)
{
    memset(v, 0, sizeof *v);
    make_stars(v);
    if (load_pens(dir, "tiles.txt", "tile", &v->tiles[0][0][0], GALAGA_TILES, 8) ||
        load_pens(dir, "sprites.txt", "sprite", &v->sprites[0][0][0], GALAGA_SPRITES, 16))
        return -1;
    char path[1200], line[512];
    snprintf(path, sizeof path, "%s/gfx/palette.txt", dir);
    FILE *f = fopen(path, "r");
    if (!f) return -1;
    int colours = 0, lookups = 0;
    while (fgets(line, sizeof line, f)) {
        unsigned rgb;
        if (line[0] == '#') continue;
        if (colours < 32) {
            if (sscanf(line, "%6x", &rgb) == 1) v->palette[colours++] = rgb;
            continue;
        }
        for (const char *p = line; *p && *p != '\n' && lookups < 512; p++) {
            int d = *p >= '0' && *p <= '9' ? *p - '0' : *p >= 'A' && *p <= 'F' ? *p - 'A' + 10 : -1;
            if (d < 0) break;
            if (lookups < 256) v->tile_lookup[lookups] = (uint8_t)d;
            else v->sprite_lookup[lookups - 256] = (uint8_t)d;
            lookups++;
        }
    }
    fclose(f);
    return colours == 32 && lookups == 512 ? 0 : -1;
}

/* the video RAM offset of tile cell (x, y) */
static int tile_offset(int x, int y)
{
    if (y < 2) return 0x3C0 + y * 0x20 + 29 - x;
    if (y >= 34) return (y - 34) * 0x20 + 29 - x;
    return 0x40 + (27 - x) * 32 + (y - 2);
}

/* One 16x16 sprite piece; flip_v / flip_h turn it top to bottom / left to right */
static void draw_piece(GalagaVideo *v, int code, int colour, int sx, int sy, int flip_v, int flip_h)
{
    const uint8_t(*s)[16] = v->sprites[code & 0x7F];
    for (int y = 0; y < 16; y++) {
        int py = sy + y;
        if (py < 0 || py >= GALAGA_H) continue;
        for (int x = 0; x < 16; x++) {
            int px = sx + x;
            if (px < 0 || px >= GALAGA_W) continue;
            uint8_t lk = v->sprite_lookup[(colour * 4 + s[flip_v ? 15 - y : y][flip_h ? 15 - x : x]) & 0xFF];
            if (lk != 15) {
                v->screen[py][px] = v->palette[lk];
                v->drawn[py][px] = 1;
            }
        }
    }
}

/* Flags bit 2 doubles a sprite top to bottom, bit 3 left to right: code bit
   0 is then its lower half, bit 1 its left half (each turned by its flip). */
static void draw_sprites(GalagaVideo *v, const GalagaFrame *f)
{
    for (int i = 0; i < 64; i++) {
        int a = 0x380 + 2 * i;
        int code = f->ram[0][a] & 0x7F, colour = f->ram[0][a + 1] & 0x3F;
        uint8_t flags = f->ram[2][a];
        int sx = f->ram[1][a] - 17;
        int sy = f->ram[1][a + 1] + 256 * (f->ram[2][a + 1] & 1) - 40;
        int flip_v = flags & 1, flip_h = flags >> 1 & 1, tall = flags >> 2 & 1, wide = flags >> 3 & 1;
        if (f->vlatch[7] & 1) flip_v ^= 1, flip_h ^= 1; /* flipped: each sprite turned round where it is */
        for (int qy = 0; qy <= tall; qy++)
            for (int qx = 0; qx <= wide; qx++) {
                int lower = tall && (qy ^ flip_v), left = wide && !(qx ^ flip_h);
                draw_piece(v, code + lower + 2 * left, colour, sx + 16 * qx, sy + 16 * qy, flip_v, flip_h);
            }
    }
}

static void draw_tiles(GalagaVideo *v, const GalagaFrame *f)
{
    for (int cy = 0; cy < 36; cy++)
        for (int cx = 0; cx < 28; cx++) {
            int o = tile_offset(cx, cy);
            const uint8_t(*t)[8] = v->tiles[f->video[o]];
            int colour = f->video[0x400 + o] & 0x3F;
            for (int y = 0; y < 8; y++)
                for (int x = 0; x < 8; x++) {
                    uint8_t lk = v->tile_lookup[colour * 4 + t[y][x]];
                    if (lk != 15) {
                        int py = cy * 8 + y, px = cx * 8 + x;
                        if (f->vlatch[7] & 1) py = GALAGA_H - 1 - py, px = GALAGA_W - 1 - px; /* flipped: turned round */
                        v->screen[py][px] = v->palette[16 + lk];
                        v->drawn[py][px] = 1;
                    }
                }
        }
}

void galaga_draw(GalagaVideo *v, const GalagaFrame *f)
{
    memset(v->screen, 0, sizeof v->screen);
    memset(v->drawn, 0, sizeof v->drawn);
    draw_stars(v, f);
    draw_sprites(v, f);
    draw_tiles(v, f);
}
