/*
 * Galaga native build: SDL setup and the fixed 60 Hz loop. See GALAGA_PORT_PLAN.md.
 * The ROM's own code runs on the emulated board (src/emu/), a frame per
 * tick, its sound streamed, when the set and its device ROMs are found; else,
 * or with --test-card, the test card (ROM status, an input tester and the
 * graphics presets). The game in C follows (plan section 3).
 *
 *   galaga [--windowed] [--gfx preset] [--rom path] [--test-card] [--emulate]
 *          [--frames N] [--screenshot out.png]
 *   galaga --check-rom <dir|zip>
 *   galaga --dump-rom <dir|zip> [region]
 *
 * --frames N stops after N updates; with --screenshot the last frame is
 * written as a PNG (use SDL_VIDEODRIVER=dummy to run headless).
 * --dump-rom prints "REGION:ADDR BYTE" lines (hex) for every byte of each
 * region (MAME's names: maincpu sub sub2 gfx1 gfx2 proms namco 51xx 54xx;
 * the device ROMs where found), or of the one named: the format the MAME
 * probe writes (src/tests/mame/rom_map_probe.lua).
 */
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assets/assets.h"
#include "assets/png.h"
#include "assets/rom.h"
#include "emu/board.h"
#include "gfx/blit.h"
#include "gfx/debugfont.h"
#include "gfx/enhance.h"
#include "gfx/fb.h"
#include "gfx/galaga.h"
#include "platform/audio.h"
#include "platform/input.h"
#include "platform/video.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#define FRAME_HZ 60
/* Cap catch-up after a stall (suspend/resume, SD card hiccup). */
#define MAX_STEPS_PER_PRESENT 4
#define GFX_NOTICE_FRAMES 120

#define WHITE 0xFFFFFFu
#define GREY 0x606060u
#define YELLOW 0xFFFF44u
#define CYAN 0x44FFFFu
#define GREEN 0x44FF44u
#define RED 0xFF4444u
#define ORANGE 0xFF8800u

static Framebuffer fb;     /* arcade resolution */
static Framebuffer fb_big; /* enhanced modes: the frame at the render scale */
static Framebuffer *shown; /* the frame last drawn */

typedef struct {
    unsigned long clock;
    InputFrame in;
    GfxSettings gfx;
    int gfx_notice;
    int coins;
    char rom_path[1024];
    char rom_error[256];
    char assets_path[1024]; /* the extracted assets, or "" */
    RomSet rom;
    int test_card;          /* the test card, not the board */
    Board board;
    GalagaVideo video;
} Port;

static Port g;

/* ---- test card ------------------------------------------------------------ */

static void centred(Framebuffer *t, int y, const char *text, uint32_t rgb)
{
    debug_text(t, (FB_WIDTH - debug_text_width(text)) / 2.0, y, text, rgb);
}

static void lamp(Framebuffer *t, int x, int y, const char *label, int on)
{
    fill_rectf(t, x, y, 5, 5, on ? GREEN : GREY);
    debug_text(t, x + 8, y, label, on ? WHITE : GREY);
}

static void draw_scene(Framebuffer *t)
{
    char line[160];
    fb_clear(t, 0xFF000000u);
    fill_rectf(t, 4, 15, FB_WIDTH - 8, 2, ORANGE);
    fill_rectf(t, 4, FB_HEIGHT - 11, FB_WIDTH - 8, 2, ORANGE);
    fill_rectf(t, 4, 15, 2, FB_HEIGHT - 24, ORANGE);
    fill_rectf(t, FB_WIDTH - 6, 15, 2, FB_HEIGHT - 24, ORANGE);
    centred(t, 4, "GALAGA - NATIVE PORT - TEST CARD", YELLOW);

    if (g.rom.loaded) {
        snprintf(line, sizeof line, "ROM: %.46s", g.rom_path);
        centred(t, 26, line, GREEN);
        centred(t, 34, "MAME GALAGA SET OK", GREEN);
        snprintf(line, sizeof line, "FIRST BYTES $%02X $%02X $%02X", g.rom.main[0], g.rom.main[1], g.rom.main[2]);
        centred(t, 42, line, GREEN);
        int both = g.rom.has_51xx && g.rom.has_54xx;
        snprintf(line, sizeof line, "DEVICE ROMS: 51XX %s, 54XX %s", g.rom.has_51xx ? "OK" : "MISSING",
                 g.rom.has_54xx ? "OK" : "MISSING");
        centred(t, 50, line, both ? GREEN : YELLOW);
    } else {
        centred(t, 26, g.rom_error[0] ? "ROM SET INCOMPLETE OR WRONG" : "NO ROM SET FOUND", RED);
        centred(t, 34, "PUT GALAGA.ZIP NEXT TO THE PROGRAM,", RED);
        centred(t, 42, "OR USE --ROM", RED);
    }
    if (g.assets_path[0]) {
        snprintf(line, sizeof line, "ASSETS: %.44s", g.assets_path);
        centred(t, 58, line, GREEN);
    } else {
        centred(t, 58, "NO ASSETS EXTRACTED YET", YELLOW);
    }

    const InputFrame *in = &g.in;
    snprintf(line, sizeof line, "CONTROLS%s", in->pads ? "  (PAD)" : "");
    debug_text(t, 16, 82, line, CYAN);
    lamp(t, 16, 94, "LEFT", in->move_x < 0);
    lamp(t, 64, 94, "RIGHT", in->move_x > 0);
    lamp(t, 116, 94, "FIRE", in->fire);
    lamp(t, 16, 108, "COIN", in->coin);
    lamp(t, 64, 108, "START", in->start);
    lamp(t, 116, 108, "2P START", in->start2);
    snprintf(line, sizeof line, "COINS %d", g.coins);
    debug_text(t, 16, 120, line, WHITE);

    /* something moving, to judge pacing and the enhanced modes */
    double phase = (double)(g.clock % 240) / 240.0;
    double bx = 16 + 180 * (phase < 0.5 ? phase * 2 : 2 - phase * 2);
    fill_rectf(t, 16, 156, 192, 1, GREY);
    fill_rectf(t, bx, 144, 12, 12, YELLOW);
    fill_rectf(t, bx + 3, 147, 6, 6, RED);
    for (int i = 0; i < 8; i++) {
        static const uint32_t bars[8] = { 0xFFFFFF, 0xFFFF00, 0x00FFFF, 0x00FF00, 0xFF00FF, 0xFF0000, 0x0000FF, 0x000000 };
        fill_rectf(t, 16 + i * 24, 166, 24, 20, bars[i]);
    }

    snprintf(line, sizeof line, "GRAPHICS: %s", gfx_describe(&g.gfx));
    centred(t, 200, line, g.gfx_notice > 0 ? YELLOW : WHITE);
    centred(t, 208, "SELECT+L1 / F3, F4 GLOW, F5 SCANLINES", GREY);
    snprintf(line, sizeof line, "FRAME %lu   RENDER %dX", g.clock, t->scale);
    centred(t, 220, line, GREY);
    centred(t, 236, "EXIT: HOLD SELECT+START OR ESC", GREY);
    centred(t, 264, "CONVERTED BY MONKEYX", GREY);
}

/* Classic: the arcade frame. HD: drawn at the render scale. FILTER: the arcade
   frame upscaled whole. Glow goes over whichever is shown. */
/* The board's picture into the arcade framebuffer */
static void draw_board(Framebuffer *t)
{
    Board *b = &g.board;
    GalagaFrame fr = { b->video, { b->ram[0], b->ram[1], b->ram[2] }, b->vlatch, b->star_origin };
    galaga_draw(&g.video, &fr);
    for (int y = 0; y < FB_HEIGHT; y++) {
        uint32_t *row = fb_row(t, y);
        for (int x = 0; x < FB_WIDTH; x++) row[x] = 0xFF000000u | g.video.screen[y][x];
    }
}

static void draw(int display_scale)
{
    const GfxSettings *gs = &g.gfx;
    const Scaler scaler = gs->smooth ? SCALER_SMOOTH : SCALER_CRISP;
    const int s = gfx_render_scale(gs, display_scale);
    shown = &fb;
    if (!g.test_card) { /* only pixels: HD upscales them like FILTER */
        draw_board(&fb);
        if (g.gfx_notice > 0) {
            char text[80];
            snprintf(text, sizeof text, "GRAPHICS: %s", gfx_describe(&g.gfx));
            fill_rectf(&fb, 0, FB_HEIGHT - 9, FB_WIDTH, 9, 0x000000);
            centred(&fb, FB_HEIGHT - 7, text, YELLOW);
        }
        if (gs->mode != GFX_CLASSIC && !fb_init(&fb_big, s) && !enhance_filter_frame(&fb, &fb_big, scaler))
            shown = &fb_big;
        if (gs->glow) enhance_glow(shown);
        return;
    }
    if (gs->mode == GFX_HD && !fb_init(&fb_big, s) && !enhance_prepare(s, scaler)) {
        draw_scene(&fb_big);
        shown = &fb_big;
    } else {
        draw_scene(&fb);
        if (gs->mode == GFX_FILTER && !fb_init(&fb_big, s) && !enhance_filter_frame(&fb, &fb_big, scaler))
            shown = &fb_big;
    }
    if (gs->glow) enhance_glow(shown);
}

/* The panel to the board's switches (active low) */
static void board_inputs(const InputFrame *in)
{
    uint8_t in0 = 0xFF, in1 = 0xFF;
    if (in->move_x < 0) in0 &= (uint8_t)~IN0_LEFT1;
    if (in->move_x > 0) in0 &= (uint8_t)~IN0_RIGHT1;
    if (in->fire) in1 &= (uint8_t)~IN1_FIRE1;
    if (in->coin) in1 &= (uint8_t)~IN1_COIN1;
    if (in->start) in1 &= (uint8_t)~IN1_START1;
    if (in->start2) in1 &= (uint8_t)~IN1_START2;
    g.board.in0 = in0;
    g.board.in1 = in1;
}

static void step(const InputFrame *in)
{
    static bool coin_was;
    g.clock++;
    g.in = *in;
    if (in->coin && !coin_was) g.coins++;
    coin_was = in->coin;
    if (!g.test_card) {
        board_inputs(in);
        board_run_frame(&g.board);
        audio_stream(g.board.audio, BOARD_AUDIO_SAMPLES, BOARD_AUDIO_SAMPLES * FRAME_HZ); /* paced by the loop */
    }
    if (in->gfx_next || in->gfx_glow || in->gfx_scanlines) {
        if (in->gfx_next) gfx_next_preset(&g.gfx);
        if (in->gfx_glow) g.gfx.glow = !g.gfx.glow;
        if (in->gfx_scanlines) g.gfx.scanlines = !g.gfx.scanlines;
        g.gfx_notice = GFX_NOTICE_FRAMES;
    }
    if (g.gfx_notice > 0) g.gfx_notice--;
}

/* ---- tools ---------------------------------------------------------------- */

static int check_rom(const char *path)
{
    static RomSet rom;
    char err[512];
    if (rom_load(&rom, path, err, sizeof err)) {
        fprintf(stderr, "galaga: %s\n", err);
        return 1;
    }
    printf("galaga: %s: MAME galaga set OK\n", path);
    printf("galaga: device ROMs (for --emulate): 51xx %s, 54xx %s\n", rom.has_51xx ? "OK" : "missing (namco51.zip)",
           rom.has_54xx ? "OK" : "missing (namco54.zip)");
    return 0;
}

static int dump_rom(const char *path, const char *only)
{
    static RomSet rom;
    char err[512];
    if (rom_load(&rom, path, err, sizeof err)) {
        fprintf(stderr, "galaga: %s\n", err);
        return 1;
    }
    int shown_any = 0;
    for (int r = 0; r < ROM_REGIONS; r++) {
        if (only && strcmp(only, rom_region_name(r))) continue;
        unsigned size;
        const uint8_t *p = rom_region(&rom, r, &size);
        if (!p) {
            fprintf(stderr, "galaga: no %s ROM found\n", rom_region_name(r));
            if (only) return 1;
            continue;
        }
        for (unsigned i = 0; i < size; i++) printf("%s:%04X %02X\n", rom_region_name(r), i, p[i]);
        shown_any = 1;
    }
    if (!shown_any) {
        fprintf(stderr, "galaga: no region '%s'\n", only ? only : "");
        return 1;
    }
    return 0;
}

static int save_screenshot(const char *path)
{
    static uint8_t rgba[FB_HEIGHT * FB_MAX_SCALE * FB_WIDTH * FB_MAX_SCALE * 4];
    const Framebuffer *f = shown ? shown : &fb;
    for (int y = 0; y < f->h; y++)
        for (int x = 0; x < f->w; x++) {
            uint32_t p = fb_row(f, y)[x];
            uint8_t *o = rgba + ((long)y * f->w + x) * 4;
            o[0] = (uint8_t)(p >> 16);
            o[1] = (uint8_t)(p >> 8);
            o[2] = (uint8_t)p;
            o[3] = 0xFF;
        }
    if (png_write(path, rgba, f->w, f->h)) {
        fprintf(stderr, "galaga: cannot write %s\n", path);
        return -1;
    }
    return 0;
}

/* ---- loop ----------------------------------------------------------------- */

/* Loop state, static so the browser can run one pass per animation frame. */
static long max_frames = -1;
static const char *screenshot;
static Uint64 step_ticks, last, acc;
static long frames;
static int rc;

/* Step for the time elapsed, then draw and present; false = stop. */
static bool run_once(void)
{
    Uint64 now = SDL_GetPerformanceCounter();
    acc += now - last;
    last = now;
    if (acc > step_ticks * MAX_STEPS_PER_PRESENT)
        acc = step_ticks * MAX_STEPS_PER_PRESENT;
    /* Headless runs step once per loop so the frame count is exact. */
    if (max_frames >= 0 && acc < step_ticks) acc = step_ticks;

    while (acc >= step_ticks) {
        InputFrame in;
        input_poll(&in);
#ifndef __EMSCRIPTEN__
        if (in.quit) return false; /* a browser tab has nothing to quit to */
#endif
        step(&in);
        frames++;
        acc -= step_ticks;
        if (max_frames >= 0 && frames >= max_frames) break;
    }
    if (max_frames < 0 || frames >= max_frames) draw(video_display_scale());
    if (max_frames < 0) video_present(shown, g.gfx.scanlines); /* blocks on vsync */
    if (max_frames >= 0 && frames >= max_frames) {
        if (screenshot && save_screenshot(screenshot)) rc = 1;
        return false;
    }
    return true;
}

#ifdef __EMSCRIPTEN__
static void web_frame(void) { run_once(); }
#endif

int main(int argc, char **argv)
{
    bool windowed = false, want_card = false, want_board = false;
    const char *rom_arg = NULL;
    if (argc == 3 && !strcmp(argv[1], "--check-rom")) return check_rom(argv[2]);
    if ((argc == 3 || argc == 4) && !strcmp(argv[1], "--dump-rom")) return dump_rom(argv[2], argc == 4 ? argv[3] : NULL);
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--windowed")) windowed = true;
        else if (!strcmp(argv[i], "--frames") && i + 1 < argc) max_frames = strtol(argv[++i], NULL, 10);
        else if (!strcmp(argv[i], "--screenshot") && i + 1 < argc) screenshot = argv[++i];
        else if (!strcmp(argv[i], "--rom") && i + 1 < argc) rom_arg = argv[++i];
        else if (!strcmp(argv[i], "--test-card")) want_card = true;
        else if (!strcmp(argv[i], "--emulate")) want_board = true;
        else if (!strcmp(argv[i], "--gfx") && i + 1 < argc) {
            if (gfx_parse(argv[++i], &g.gfx)) {
                fprintf(stderr, "galaga: unknown --gfx preset '%s' (one of: %s)\n", argv[i], gfx_preset_names());
                return 2;
            }
        } else {
            fprintf(stderr,
                    "usage: %s [--windowed] [--gfx preset] [--rom path] [--test-card] [--emulate]\n"
                    "          [--frames N] [--screenshot out.png]\n"
                    "       %s --check-rom <dir|zip>\n"
                    "       %s --dump-rom <dir|zip> [region]\n",
                    argv[0], argv[0], argv[0]);
            return 2;
        }
    }

    if (fb_init(&fb, 1)) {
        fprintf(stderr, "galaga: out of memory\n");
        return 1;
    }
    if (rom_arg) snprintf(g.rom_path, sizeof g.rom_path, "%s", rom_arg);
    else rom_locate(argv[0], g.rom_path, sizeof g.rom_path);
    if (g.rom_path[0] && rom_load(&g.rom, g.rom_path, g.rom_error, sizeof g.rom_error))
        fprintf(stderr, "galaga: %s\n", g.rom_error);
    if (assets_find(argv[0], "manifest.txt", g.assets_path, sizeof g.assets_path)) g.assets_path[0] = '\0';
    g.test_card = 1;
    if (!want_card && g.rom.loaded) {
        if (g.rom.has_51xx && g.rom.has_54xx) {
            g.test_card = 0;
            galaga_video_init(&g.video, &g.rom);
            board_init(&g.board, &g.rom);
        } else {
            fprintf(stderr, "galaga: the emulated board needs the device ROMs: namco51.zip and namco54.zip next to %s\n",
                    g.rom_path);
        }
    } else if (want_board && !g.rom.loaded) {
        fprintf(stderr, "galaga: --emulate needs the ROM set\n");
    }

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_EVENTS) != 0) {
        SDL_Log("SDL_Init: %s", SDL_GetError());
        return 1;
    }
#ifdef __EMSCRIPTEN__
    windowed = true; /* the page's canvas */
#endif
    if (!video_init(windowed)) {
        SDL_Quit();
        return 1;
    }
    input_init();
    if (max_frames < 0) audio_init(argv[0]); /* headless runs stay silent */

    step_ticks = SDL_GetPerformanceFrequency() / FRAME_HZ;
    last = SDL_GetPerformanceCounter();
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(web_frame, 0, 1); /* never returns */
#endif
    while (run_once()) {}

    audio_shutdown();
    input_shutdown();
    video_shutdown();
    SDL_Quit();
    enhance_clear_images();
    fb_free(&fb_big);
    fb_free(&fb);
    return rc;
}
