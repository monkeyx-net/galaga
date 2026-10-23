/*
 * The port's picture drawing against a MAME snapshot (label mame):
 *   test_capture <galaga.zip|folder> <frame_N.txt> <N.png> <N>
 * Draws frame N from the video RAM, RAM and latches capture_probe.lua
 * dumped at its vertical blank (gfx/galaga.c), and compares the picture
 * with MAME's (224x288) pixel for pixel, bar the stars where nothing else
 * is drawn (the starfield depends on the frames since power-on: test_emu
 * checks them). TEST_CAPTURE_OUT=<file.png> writes the port's picture.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assets/png.h"
#include "assets/rom.h"
#include "gfx/galaga.h"

static RomSet rom;
static GalagaVideo video;
static uint8_t vram[0x800], ram[3][0x400], vlatch[8];

static int hex(char c)
{
    return c >= '0' && c <= '9' ? c - '0' : c >= 'A' && c <= 'F' ? c - 'A' + 10 : c >= 'a' && c <= 'f' ? c - 'a' + 10 : -1;
}

static int hex_bytes(const char *p, uint8_t *out, int n)
{
    for (int i = 0; i < n; i++) {
        int hi = hex(p[2 * i]), lo = hex(p[2 * i + 1]);
        if (hi < 0 || lo < 0) return -1;
        out[i] = (uint8_t)(hi << 4 | lo);
    }
    return 0;
}

/* a latch from the dump ("--": never written, 0) */
static int latch(const char *l, int i) { return hex(l[2 * i]) < 0 ? 0 : (hex(l[2 * i]) << 4 | hex(l[2 * i + 1])) & 1; }

/* A star's colour: red and green 0, $47, $97 or $DE, blue 0, $51, $AE or $FF */
static int is_star(const uint8_t *p)
{
    for (int i = 0; i < 2; i++)
        if (p[i] && p[i] != 0x47 && p[i] != 0x97 && p[i] != 0xDE) return 0;
    if (p[2] && p[2] != 0x51 && p[2] != 0xAE && p[2] != 0xFF) return 0;
    return 1;
}

int main(int argc, char **argv)
{
    if (argc != 5) {
        fprintf(stderr, "usage: test_capture <rom> <frame_N.txt> <N.png> <N>\n");
        return 2;
    }
    char err[512];
    if (rom_load(&rom, argv[1], err, sizeof err)) {
        fprintf(stderr, "%s\n", err);
        return 1;
    }
    galaga_video_init(&video, &rom);
    FILE *f = fopen(argv[2], "r");
    if (!f) {
        fprintf(stderr, "cannot read %s\n", argv[2]);
        return 1;
    }
    static char line[8192];
    int have = 0;
    while (fgets(line, sizeof line, f)) {
        if (!strncmp(line, "video ", 6)) have |= !hex_bytes(line + 6, vram, 0x800);
        else if (!strncmp(line, "ram1 ", 5)) have |= !hex_bytes(line + 5, ram[0], 0x400) << 1;
        else if (!strncmp(line, "ram2 ", 5)) have |= !hex_bytes(line + 5, ram[1], 0x400) << 2;
        else if (!strncmp(line, "ram3 ", 5)) have |= !hex_bytes(line + 5, ram[2], 0x400) << 3;
        else if (!strncmp(line, "latch ", 6) && strlen(line) >= 6 + 32) {
            for (int i = 0; i < 8; i++) vlatch[i] = (uint8_t)latch(line + 6, 8 + i); /* $6820-$6827, $A000-$A007 */
            have |= 16;
        }
    }
    fclose(f);
    if (have != 31) {
        fprintf(stderr, "%s: not a capture frame\n", argv[2]);
        return 1;
    }
    vlatch[5] = 0; /* no stars: where the starfield stands is not in the dump */
    GalagaFrame fr = { vram, { ram[0], ram[1], ram[2] }, vlatch, 0 };
    galaga_draw(&video, &fr);
    if (getenv("TEST_CAPTURE_OUT")) {
        static uint8_t rgba[GALAGA_H * GALAGA_W * 4];
        for (int y = 0; y < GALAGA_H; y++)
            for (int x = 0; x < GALAGA_W; x++) {
                uint8_t *q = rgba + ((size_t)y * GALAGA_W + (size_t)x) * 4;
                q[0] = (uint8_t)(video.screen[y][x] >> 16), q[1] = (uint8_t)(video.screen[y][x] >> 8);
                q[2] = (uint8_t)video.screen[y][x], q[3] = 255;
            }
        png_write(getenv("TEST_CAPTURE_OUT"), rgba, GALAGA_W, GALAGA_H);
    }

    Image shot;
    const char *perr;
    if (png_load(argv[3], &shot, &perr)) {
        fprintf(stderr, "%s: %s\n", argv[3], perr);
        return 1;
    }
    if (shot.width != GALAGA_W || shot.height != GALAGA_H) {
        fprintf(stderr, "%s: %dx%d, not MAME's 224x288\n", argv[3], shot.width, shot.height);
        return 1;
    }
    long diff = 0;
    for (int y = 0; y < GALAGA_H; y++)
        for (int x = 0; x < GALAGA_W; x++) {
            const uint8_t *p = shot.rgba + ((size_t)y * (size_t)shot.width + (size_t)x) * 4;
            uint32_t want = (uint32_t)p[0] << 16 | (uint32_t)p[1] << 8 | p[2], got = video.screen[y][x] & 0xFFFFFF;
            if (!video.drawn[y][x] && is_star(p)) continue; /* black, or a star */
            if (got != want && diff++ < 6)
                fprintf(stderr, "  pixel %d,%d: %06X, MAME %06X\n", x, y, (unsigned)got, (unsigned)want);
        }
    image_free(&shot);
    printf("frame %s: %ld pixels differ from MAME's snapshot\n", argv[4], diff);
    return diff ? 1 : 0;
}
