/*
 * Galaga native build: SDL setup and the fixed 60 Hz loop. See GALAGA_PORT_PLAN.md.
 * The game in C (src/sim/) runs a frame per tick from the assets extracted
 * from the player's set (extracted on the first run when the set is found),
 * its sound streamed; with --emulate the ROM's own code runs on the
 * emulated board instead (src/emu/; it needs the device ROMs too). Without
 * either, or with --test-card, the test card (ROM status, an input tester
 * and the graphics presets).
 *
 *   galaga [--windowed] [--gfx preset] [--scale mode] [--rotate degrees] [--rom path]
 *          [--test-card] [--emulate] [--input script.txt] [--frames N] [--screenshot out.png]
 *   galaga --check-rom <dir|zip>
 *   galaga --dump-rom <dir|zip> [region]
 *   galaga --extract <dir|zip> [<asset dir>]
 *
 * --scale auto|integer|fit|stretch and --rotate 0|90|270 (clockwise, for a
 * landscape device held on its side) lay the picture out (platform/screen.h);
 * galaga.cfg keeps them, F6 and F7 or the operator menu change them.
 * --frames N stops after N updates; with --screenshot the last frame is
 * written as a PNG (use SDL_VIDEODRIVER=dummy to run headless).
 * --input replays scripted controls: lines of "<frame> [coin] [start]
 * [start2] [left] [right] [fire] [up] [down] [menu]", each holding from
 * that frame until the next line ("menu" a press of F2 on its first frame;
 * live controls still quit).
 * --dump-rom prints "REGION:ADDR BYTE" lines (hex) for every byte of each
 * region (MAME's names: maincpu sub sub2 gfx1 gfx2 proms namco 51xx 54xx;
 * the device ROMs where found), or of the one named: the format the MAME
 * probe writes (src/tests/mame/rom_map_probe.lua).
 * --extract writes the graphics, palette, waveforms, font and previews
 * from the ROM set to an asset folder (default: assets/ next to the
 * program, or $GALAGA_ASSETS; assets/extract.h), which the game in C reads
 * instead of it.
 */
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assets/assets.h"
#include "assets/extract.h"
#include "assets/png.h"
#include "assets/rom.h"
#include "assets/savefile.h"
#include "emu/board.h"
#include "gfx/blit.h"
#include "gfx/debugfont.h"
#include "gfx/enhance.h"
#include "gfx/fb.h"
#include "gfx/galaga.h"
#include "sim/sim.h"
#include "platform/audio.h"
#include "platform/input.h"
#include "platform/screen.h"
#include "platform/video.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#define FRAME_HZ 60
/* Cap catch-up after a stall (suspend/resume, SD card hiccup). */
#define MAX_STEPS_PER_PRESENT 4
#define GFX_NOTICE_FRAMES 120
#define SETTINGS_FILE "galaga.cfg"
#define HISCORE_FILE "galaga.hiscore"
#define HISCORE_SAVE_FRAMES 300 /* a changed table is written at most every 5 s */
/* The controls page, shown over the attract mode when nobody has touched the
   controls for CONTROLS_IDLE_FRAMES; it stays CONTROLS_SHOW_FRAMES and comes
   back every CONTROLS_IDLE_FRAMES + CONTROLS_SHOW_FRAMES. Any input hides it. */
#define CONTROLS_IDLE_FRAMES (30 * FRAME_HZ)
#define CONTROLS_SHOW_FRAMES (10 * FRAME_HZ)

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
    ScreenSettings screen; /* the scale mode and rotation (galaga.cfg) */
    int gfx_notice;
    int notice_screen;     /* the notice is the screen's, not the graphics' */
    int coins;
    char rom_path[1024];
    char rom_error[256];
    char assets_path[1024]; /* the extracted assets, or "" */
    RomSet rom;
    enum { SHOW_CARD, SHOW_GAME, SHOW_BOARD } show; /* the test card, the C game, the board */
    Board board;
    Sim sim;
    uint8_t dswa, dswb;  /* the DIP switches (galaga.cfg) */
    long idle;           /* frames of attract mode without input (the controls page) */
    uint8_t hiscore[45]; /* the high-score table as last saved or placed */
    int hiscore_saved;   /* galaga.hiscore read or written */
    int hiscore_placed;  /* put into the game's RAM after its power-on */
    int hiscore_wait;    /* frames the table has differed from the saved one */
    int menu;            /* the operator menu is open: the game waits */
    int menu_row;
    int menu_hold;       /* the screen turned: directions wait for a release */
    uint8_t menu_dswb;   /* DSWB when it opened */
    InputFrame menu_was; /* the controls last tick, for the menu's presses */
    uint8_t data[GAME_DATA_SIZE]; /* the ROMs' tables, from the assets */
    uint8_t waves[256];           /* the WSG's waveforms, from the assets */
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
    centred(t, 200, line, g.gfx_notice > 0 && !g.notice_screen ? YELLOW : WHITE);
    centred(t, 208, "SELECT+L1 / F3, F4 GLOW, F5 SCANLINES", GREY);
    snprintf(line, sizeof line, "SCREEN: %s   F6 SCALE, F7 TURN", screen_describe(&g.screen));
    centred(t, 228, line, g.gfx_notice > 0 && g.notice_screen ? YELLOW : GREY);
    snprintf(line, sizeof line, "FRAME %lu   RENDER %dX", g.clock, t->scale);
    centred(t, 220, line, GREY);
    centred(t, 236, "EXIT: HOLD SELECT+START OR ESC", GREY);
    centred(t, 264, "CONVERTED BY MONKEYX", GREY);
}

/* ---- the game ---------------------------------------------------------------- */

/* The board's memory, the C game's or the emulated board's */
static uint8_t *game_ram(int bank) { return g.show == SHOW_BOARD ? g.board.ram[bank] : g.sim.g.ram[bank]; }
static uint8_t *game_video(void) { return g.show == SHOW_BOARD ? g.board.video : g.sim.g.video; }
/* main's state ($9201): 0 its power-on checks, 1 the attract mode, 3 a game */
static int game_state(void) { return game_ram(1)[0x201]; }

/* Power on, with the switches */
static void power_on(void)
{
    if (g.show == SHOW_BOARD) {
        board_init(&g.board, &g.rom);
        g.board.dswa = g.dswa;
        g.board.dswb = g.dswb;
    } else if (g.show == SHOW_GAME) {
        sim_init(&g.sim, g.data, g.dswa, g.dswb);
        sim_sound(&g.sim, g.waves);
    }
    g.hiscore_placed = 0;
}

/* The DIP switches (MAME's ports DSWA, DSWB), as MAME's defaults unless
   galaga.cfg says otherwise. The file is written with the defaults if
   missing, and again when the operator menu closes. */
#define DIP_ROWS 7

static const struct {
    const char *what, *key;
    int sw, mask, n;            /* DSWA (0) or DSWB (1), its bits, its settings */
    uint8_t values[8];
    const char *tokens[8];      /* in galaga.cfg */
    const char *names[8];       /* in the menu */
} DIPS[DIP_ROWS] = {
    { "DIFFICULTY", "difficulty", 0, 0x03, 4, { 0x03, 0x00, 0x01, 0x02 }, { "easy", "medium", "hard", "hardest" },
      { "EASY", "MEDIUM", "HARD", "HARDEST" } },
    { "SHIPS", "lives", 1, 0xC0, 4, { 0x00, 0x80, 0x40, 0xC0 }, { "2", "3", "4", "5" }, { "2", "3", "4", "5" } },
    { "BONUS SHIP", "bonus", 1, 0x38, 8, { 0x20, 0x18, 0x10, 0x30, 0x38, 0x08, 0x28, 0x00 },
      { "1", "2", "3", "4", "5", "6", "7", "none" }, { 0 } }, /* names by the ships: BONUS */
    { "COINAGE", "coinage", 1, 0x07, 8, { 0x04, 0x02, 0x06, 0x07, 0x01, 0x03, 0x05, 0x00 },
      { "4c1c", "3c1c", "2c1c", "1c1c", "2c3c", "1c2c", "1c3c", "free" },
      { "4 COINS 1 CREDIT", "3 COINS 1 CREDIT", "2 COINS 1 CREDIT", "1 COIN 1 CREDIT", "2 COINS 3 CREDITS",
        "1 COIN 2 CREDITS", "1 COIN 3 CREDITS", "FREE PLAY" } },
    { "DEMO SOUNDS", "demo_sounds", 0, 0x08, 2, { 0x00, 0x08 }, { "on", "off" }, { "ON", "OFF" } },
    { "CABINET", "cabinet", 0, 0x80, 2, { 0x80, 0x00 }, { "upright", "cocktail" }, { "UPRIGHT", "COCKTAIL" } },
    { "FREEZE", "freeze", 0, 0x10, 2, { 0x10, 0x00 }, { "off", "on" }, { "OFF", "ON" } },
};
/* the bonus ships' settings: with 2 to 4 ships, with 5 */
static const char *const BONUS[2][8] = {
    { "20K 60K EVERY 60K", "20K AND 60K ONLY", "20K 70K EVERY 70K", "20K 80K EVERY 80K", "30K AND 80K ONLY",
      "30K 100K EVERY 100K", "30K 120K EVERY 120K", "NONE" },
    { "30K 100K EVERY 100K", "30K AND 150K ONLY", "30K 120K EVERY 120K", "30K 150K EVERY 150K", "30K ONLY",
      "30K AND 100K ONLY", "30K AND 120K ONLY", "NONE" },
};

static int dip_get(int row)
{
    uint8_t v = (DIPS[row].sw ? g.dswb : g.dswa) & DIPS[row].mask;
    for (int i = 0; i < DIPS[row].n; i++)
        if (DIPS[row].values[i] == v) return i;
    return 0;
}

static void dip_set(int row, int i)
{
    uint8_t *sw = DIPS[row].sw ? &g.dswb : &g.dswa;
    *sw = (uint8_t)((*sw & ~DIPS[row].mask) | DIPS[row].values[i]);
}

static const char *dip_name(int row, int i)
{
    if (DIPS[row].names[0]) return DIPS[row].names[i];
    return BONUS[(g.dswb & 0xC0) == 0xC0][i];
}

/* The screen's settings: the layout and the joystick turn together */
static void screen_apply(void)
{
    video_set_screen(&g.screen);
    input_set_rotation(g.screen.rotate);
}

static void settings_save(void)
{
    char text[2048];
    int n = snprintf(text, sizeof text,
                     "# Galaga's DIP switches (MAME's defaults: easy 3 3 1c1c on upright off; F2 in the game)\n");
    for (int r = 0; r < DIP_ROWS && n < (int)sizeof text; r++) {
        n += snprintf(text + n, sizeof text - (size_t)n, "%s %s\n#", DIPS[r].key, DIPS[r].tokens[dip_get(r)]);
        for (int i = 0; i < DIPS[r].n && n < (int)sizeof text; i++)
            n += snprintf(text + n, sizeof text - (size_t)n, " %s", DIPS[r].tokens[i]);
        if (r == 2 && n < (int)sizeof text) /* the bonus ships */
            for (int k = 0; k < 2; k++)
                for (int i = 0; i < 7 && n < (int)sizeof text; i++)
                    n += snprintf(text + n, sizeof text - (size_t)n, "%s%d: %s", i ? ", " : k ? "\n# with 5 ships, " : "\n# with 2-4 ships, ", i + 1, BONUS[k][i]);
        if (n < (int)sizeof text) n += snprintf(text + n, sizeof text - (size_t)n, "\n");
    }
    if (n < (int)sizeof text)
        n += snprintf(text + n, sizeof text - (size_t)n,
                      "# The screen (the port's; F6, F7): scale auto integer fit stretch;\n"
                      "# rotate 0 90 270 (clockwise: a landscape device held on its side)\n"
                      "scale %s\nrotate %d\n", screen_mode_name(g.screen.mode), g.screen.rotate);
    if (n >= (int)sizeof text) n = (int)sizeof text - 1;
    if (save_store(SETTINGS_FILE, text, (size_t)n)) fprintf(stderr, "galaga: cannot save %s\n", save_path(SETTINGS_FILE));
}

static void settings_load(void)
{
    g.dswa = DSWA_DEFAULT;
    g.dswb = DSWB_DEFAULT;
    FILE *f = fopen(save_path(SETTINGS_FILE), "r");
    if (!f) {
        if (g.show != SHOW_CARD) settings_save();
        return;
    }
    char line[256], key[32], value[32];
    while (fgets(line, sizeof line, f)) {
        if (line[0] == '#' || sscanf(line, "%31s %31s", key, value) != 2) continue;
        if (!strcmp(key, "scale") && screen_mode_parse(value) >= 0) g.screen.mode = screen_mode_parse(value);
        if (!strcmp(key, "rotate") && screen_rotate_parse(value) >= 0) g.screen.rotate = screen_rotate_parse(value);
        for (int r = 0; r < DIP_ROWS; r++)
            if (!strcmp(key, DIPS[r].key))
                for (int i = 0; i < DIPS[r].n; i++)
                    if (!strcmp(value, DIPS[r].tokens[i])) dip_set(r, i);
    }
    fclose(f);
}

/* The port's controls as the cabinet's switches (active low): one panel
   drives both players' controls (the game reads player two's on the
   cocktail table, for player two's turns). */
static void panel(const InputFrame *in, uint8_t *in0_out, uint8_t *in1_out)
{
    uint8_t in0 = 0xFF, in1 = 0xFF;
    if (in->move_x < 0) in0 &= (uint8_t)~(IN0_LEFT1 | IN0_LEFT2);
    if (in->move_x > 0) in0 &= (uint8_t)~(IN0_RIGHT1 | IN0_RIGHT2);
    if (in->fire) in1 &= (uint8_t)~(IN1_FIRE1 | IN1_FIRE2);
    if (in->coin) in1 &= (uint8_t)~IN1_COIN1;
    if (in->start) in1 &= (uint8_t)~IN1_START1;
    if (in->start2) in1 &= (uint8_t)~IN1_START2;
    *in0_out = in0;
    *in1_out = in1;
}

/* ---- the high scores ------------------------------------------------------------ */

/* The arcade forgets its table at power-off; the port keeps it. Main's
   power-on fills the table ($8A20: five scores of six digits, the units
   first, $24 blank; $8A3E: their initials, three tiles each) from its ROM,
   and the top of the screen's HIGH SCORE ($83ED-$83F2, the units first)
   too; once both are done (hiscore_fresh), the saved table goes in, its
   first score at the top. A changed table is saved (at most every HISCORE_SAVE_FRAMES). */
#define HISCORE_AT 0x220 /* in the RAM at $8800 */
#define HISCORE_LEN 45
#define HISCORE_TOP 0x3ED /* in video RAM */

/* Main's power-on has just set the table and the top score up (the
   attract mode; the top score the first; the initials its X.X), not
   anything its RAM tests leave */
static int hiscore_fresh(const uint8_t *table)
{
    if (game_state() != 1 || memcmp(game_video() + HISCORE_TOP, table, 6)) return 0;
    for (int i = 0; i < 5; i++)
        if (table[30 + 3 * i + 1] != 0x2A || table[30 + 3 * i] != table[30 + 3 * i + 2]) return 0;
    return 1;
}

static void hiscore_step(void)
{
    uint8_t *table = game_ram(0) + HISCORE_AT;
    if (!g.hiscore_placed) {
        if (!hiscore_fresh(table)) return;
        g.hiscore_placed = 1;
        if (g.hiscore_saved) {
            memcpy(table, g.hiscore, HISCORE_LEN);
            memcpy(game_video() + HISCORE_TOP, table, 6);
        } else {
            memcpy(g.hiscore, table, HISCORE_LEN);
        }
        return;
    }
    if (!memcmp(table, g.hiscore, HISCORE_LEN)) {
        g.hiscore_wait = 0;
        return;
    }
    if (++g.hiscore_wait < HISCORE_SAVE_FRAMES) return;
    memcpy(g.hiscore, table, HISCORE_LEN);
    g.hiscore_saved = 1;
    g.hiscore_wait = 0;
    if (save_store(HISCORE_FILE, g.hiscore, HISCORE_LEN)) fprintf(stderr, "galaga: cannot save %s\n", save_path(HISCORE_FILE));
}

/* ---- the operator menu: the DIP switches, the screen ------------------------------------------- */

/* F2 or Select+R1 opens it over the game, which waits; up/down choose a
   switch, left/right (or fire) change it, F2 or Select+R1 again saves
   galaga.cfg and goes on. The game reads the switches as the board does;
   main hands the 51xx the coinage at power-on, so a new coinage powers the
   game on again. The last two rows are the port's: the screen's rotation
   and scale mode, applied at once. */
#define MENU_ROWS (DIP_ROWS + 2)
#define ROW_ROTATE DIP_ROWS
#define ROW_SCALE (DIP_ROWS + 1)

static const int ROTATIONS[3] = { 0, 90, 270 };
static const char *const ROTATE_NAMES[3] = { "NONE", "90 (TOP AT THE RIGHT)", "270 (TOP AT THE LEFT)" };
static const char *const SCALE_NAMES[SCREEN_MODES] = { "AUTO", "INTEGER", "FIT", "STRETCH" };

static int menu_count(int row) { return row == ROW_ROTATE ? 3 : row == ROW_SCALE ? SCREEN_MODES : DIPS[row].n; }
static const char *menu_what(int row) { return row == ROW_ROTATE ? "ROTATE" : row == ROW_SCALE ? "SCALE" : DIPS[row].what; }

static int menu_get(int row)
{
    if (row == ROW_ROTATE) return g.screen.rotate == 90 ? 1 : g.screen.rotate == 270 ? 2 : 0;
    if (row == ROW_SCALE) return g.screen.mode;
    return dip_get(row);
}

static void menu_set(int row, int i)
{
    if (row == ROW_ROTATE) {
        g.screen.rotate = ROTATIONS[i];
        g.menu_hold = 1; /* the held direction turns too */
        screen_apply();
    } else if (row == ROW_SCALE) {
        g.screen.mode = i;
        screen_apply();
    } else {
        dip_set(row, i);
    }
}

static const char *menu_name(int row, int i)
{
    if (row == ROW_ROTATE) return ROTATE_NAMES[i];
    if (row == ROW_SCALE) return SCALE_NAMES[i];
    return dip_name(row, i);
}

static void menu_step(const InputFrame *in)
{
    const InputFrame *was = &g.menu_was;
    if (g.menu_hold && (in->move_x || in->move_y)) {
        g.menu_was = *in;
        return;
    }
    g.menu_hold = 0;
    if (in->move_y && in->move_y != was->move_y) g.menu_row = (g.menu_row + in->move_y + MENU_ROWS) % MENU_ROWS;
    int dx = in->move_x && in->move_x != was->move_x ? in->move_x : 0;
    if (in->fire && !was->fire) dx = 1;
    if (dx) {
        int n = menu_count(g.menu_row);
        menu_set(g.menu_row, (menu_get(g.menu_row) + dx + n) % n);
    }
    g.menu_was = *in;
}

static void menu_close(void)
{
    settings_save();
    if ((g.dswb & 0x07) != (g.menu_dswb & 0x07)) {
        power_on();
        return;
    }
    if (g.show == SHOW_BOARD) {
        g.board.dswa = g.dswa;
        g.board.dswb = g.dswb;
    } else {
        g.sim.g.dswa = g.dswa;
        g.sim.g.dswb = g.dswb;
    }
}

static void draw_menu(Framebuffer *t)
{
    const int x0 = 4, y0 = 48, w = FB_WIDTH - 8, h = 170;
    fill_rectf(t, x0, y0, w, h, 0x000000);
    fill_rectf(t, x0, y0, w, 1, ORANGE);
    fill_rectf(t, x0, y0 + h - 1, w, 1, ORANGE);
    centred(t, y0 + 8, "GALAGA  -  OPERATOR MENU", YELLOW);
    int y = y0 + 26;
    for (int r = 0; r < MENU_ROWS; r++, y += 11) {
        int on = r == g.menu_row;
        if (r == ROW_ROTATE) y += 4; /* the port's, apart */
        if (on) debug_text(t, x0 + 4, y, ">", YELLOW);
        debug_text(t, x0 + 12, y, menu_what(r), on ? YELLOW : WHITE);
        debug_text(t, x0 + 72, y, menu_name(r, menu_get(r)), on ? YELLOW : GREEN);
    }
    centred(t, y0 + h - 40, "UP/DOWN CHOOSE   LEFT/RIGHT CHANGE", GREY);
    centred(t, y0 + h - 30, "A NEW COINAGE STARTS THE GAME AGAIN", GREY);
    centred(t, y0 + h - 14, "F2 / SELECT+R1: SAVE AND PLAY", CYAN);
}

/* ---- the controls page ---------------------------------------------------------- */

static bool any_input(const InputFrame *in) { return in->move_x || in->fire || in->coin || in->start || in->start2; }

static bool controls_page_up(void)
{
    if (g.show == SHOW_CARD || g.menu || g.idle < CONTROLS_IDLE_FRAMES) return false;
    return (g.idle - CONTROLS_IDLE_FRAMES) % (CONTROLS_IDLE_FRAMES + CONTROLS_SHOW_FRAMES) < CONTROLS_SHOW_FRAMES;
}

static const struct { const char *what, *pad, *keys; } CONTROLS[] = {
    { "LEFT / RIGHT", "STICK, D-PAD", "LEFT RIGHT" },
    { "FIRE", "A, B", "SPACE, CTRL" },
    { "COIN", "SELECT", "5" },
    { "1 PLAYER", "START", "1, ENTER" },
    { "2 PLAYERS", "L1+START", "2" },
    { "GRAPHICS", "SELECT+L1", "F3" },
    { "GLOW, CRT", "", "F4, F5" },
    { "SCALE, ROTATE", "MENU", "F6, F7" },
    { "OPERATOR MENU", "SELECT+R1", "F2" },
    { "EXIT", "SELECT+START", "ESC" },
};

/* Over the game's frame (cosmetic: the next frame is the arcade's again). */
static void draw_controls(Framebuffer *t)
{
    const int x0 = 8, y0 = 52, w = FB_WIDTH - 16, h = 160;
    fill_rectf_alpha(t, x0, y0, w, h, 0x000000, 0.94);
    fill_rectf(t, x0, y0, w, 1, ORANGE);
    fill_rectf(t, x0, y0 + h - 1, w, 1, ORANGE);
    centred(t, y0 + 8, "GALAGA  -  CONTROLS", YELLOW);
    int y = y0 + 22;
    debug_text(t, x0 + 74, y, "PAD", CYAN);
    debug_text(t, x0 + 140, y, "KEYBOARD", CYAN);
    y += 12;
    for (size_t i = 0; i < sizeof CONTROLS / sizeof CONTROLS[0]; i++, y += 9) {
        debug_text(t, x0 + 6, y, CONTROLS[i].what, WHITE);
        debug_text(t, x0 + 74, y, CONTROLS[i].pad, GREEN);
        debug_text(t, x0 + 140, y, CONTROLS[i].keys, GREEN);
    }
    centred(t, y0 + h - 26, "PLAYERS TAKE TURNS", GREY);
    centred(t, y0 + h - 12, "CONVERTED BY MONKEYX", CYAN);
}

/* ---- drawing ------------------------------------------------------------------- */

/* The game's picture (the C game's or the board's) into the arcade framebuffer */
static void draw_game(Framebuffer *t)
{
    GalagaFrame fr;
    if (g.show == SHOW_BOARD) {
        Board *b = &g.board;
        fr = (GalagaFrame){ b->video, { b->ram[0], b->ram[1], b->ram[2] }, b->vlatch, b->star_origin };
    } else {
        Game *m = &g.sim.g;
        fr = (GalagaFrame){ m->video, { m->ram[0], m->ram[1], m->ram[2] }, m->vlatch, g.sim.star_origin };
    }
    galaga_draw(&g.video, &fr);
    for (int y = 0; y < FB_HEIGHT; y++) {
        uint32_t *row = fb_row(t, y);
        for (int x = 0; x < FB_WIDTH; x++) row[x] = 0xFF000000u | g.video.screen[y][x];
    }
}

/* Classic: the arcade frame. HD: drawn at the render scale. FILTER: the arcade
   frame upscaled whole. Glow goes over whichever is shown. */
static void draw(int display_scale)
{
    const GfxSettings *gs = &g.gfx;
    const Scaler scaler = gs->smooth ? SCALER_SMOOTH : SCALER_CRISP;
    const int s = gfx_render_scale(gs, display_scale);
    shown = &fb;
    if (g.show != SHOW_CARD) { /* only pixels: HD upscales them like FILTER */
        draw_game(&fb);
        if (controls_page_up()) draw_controls(&fb);
        if (g.menu) draw_menu(&fb);
        if (g.gfx_notice > 0) {
            char text[80];
            if (g.notice_screen) snprintf(text, sizeof text, "SCREEN: %s", screen_describe(&g.screen));
            else snprintf(text, sizeof text, "GRAPHICS: %s", gfx_describe(&g.gfx));
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

static void step(const InputFrame *in)
{
    static bool coin_was;
    g.clock++;
    g.in = *in;
    if (in->coin && !coin_was) g.coins++;
    coin_was = in->coin;
    if (in->menu && g.show != SHOW_CARD) {
        if (g.menu) menu_close();
        else g.menu_dswb = g.dswb;
        g.menu = !g.menu;
        g.menu_was = *in;
    }
    if (g.menu) { /* the game waits */
        static const int16_t quiet[SIM_AUDIO_SAMPLES];
        menu_step(in);
        audio_stream(quiet, SIM_AUDIO_SAMPLES, SIM_AUDIO_SAMPLES * FRAME_HZ);
    } else if (g.show == SHOW_BOARD) {
        panel(in, &g.board.in0, &g.board.in1);
        board_run_frame(&g.board);
        audio_stream(g.board.audio, BOARD_AUDIO_SAMPLES, BOARD_AUDIO_SAMPLES * FRAME_HZ); /* paced by the loop */
    } else if (g.show == SHOW_GAME) {
        uint8_t in0, in1;
        panel(in, &in0, &in1);
        sim_frame(&g.sim, in0, in1);
        audio_stream(g.sim.audio, SIM_AUDIO_SAMPLES, SIM_AUDIO_SAMPLES * FRAME_HZ);
    }
    if (g.show != SHOW_CARD && !g.menu) {
        hiscore_step();
        g.idle = game_state() == 3 || any_input(in) ? 0 : g.idle + 1;
    }
    if (in->gfx_next || in->gfx_glow || in->gfx_scanlines) {
        if (in->gfx_next) gfx_next_preset(&g.gfx);
        if (in->gfx_glow) g.gfx.glow = !g.gfx.glow;
        if (in->gfx_scanlines) g.gfx.scanlines = !g.gfx.scanlines;
        g.gfx_notice = GFX_NOTICE_FRAMES;
        g.notice_screen = 0;
    }
    if (in->screen_mode || in->screen_rotate) {
        if (in->screen_mode) g.screen.mode = (g.screen.mode + 1) % SCREEN_MODES;
        if (in->screen_rotate) g.screen.rotate = screen_rotate_next(g.screen.rotate);
        screen_apply();
        if (g.show != SHOW_CARD) settings_save();
        g.gfx_notice = GFX_NOTICE_FRAMES;
        g.notice_screen = 1;
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

/* Where --extract writes by default: $GALAGA_ASSETS, else assets/ next to the program */
static void default_assets(const char *argv0, char *dir, size_t len)
{
    if (getenv("GALAGA_ASSETS")) {
        snprintf(dir, len, "%s", getenv("GALAGA_ASSETS"));
        return;
    }
    snprintf(dir, len, "%s", argv0);
    char *slash = strrchr(dir, '/');
#ifdef _WIN32
    char *bslash = strrchr(dir, '\\');
    if (bslash > slash) slash = bslash;
#endif
    if (slash) snprintf(slash, len - (size_t)(slash - dir), "/assets");
    else snprintf(dir, len, "assets");
}

static int extract(const char *argv0, const char *rom_path, const char *out_dir)
{
    static RomSet rom;
    char err[512], dir[1024];
    if (out_dir) snprintf(dir, sizeof dir, "%s", out_dir);
    else default_assets(argv0, dir, sizeof dir);
    if (rom_load(&rom, rom_path, err, sizeof err) || extract_all(&rom, dir, err, sizeof err)) {
        fprintf(stderr, "galaga: %s\n", err);
        return 1;
    }
    printf("galaga: assets extracted to %s\n", dir);
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
/* ---- scripted input ------------------------------------------------------- */

typedef struct {
    long frame;
    InputFrame in;
} ScriptLine;

static ScriptLine script[4096];
static int script_len, script_pos;

static int load_script(const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f) {
        fprintf(stderr, "galaga: cannot open %s\n", path);
        return -1;
    }
    char line[256];
    while (script_len < (int)(sizeof script / sizeof script[0]) && fgets(line, sizeof line, f)) {
        if (line[0] == '#' || line[0] == '\n') continue;
        ScriptLine *l = &script[script_len];
        memset(l, 0, sizeof *l);
        char *tok = strtok(line, " \t\n");
        if (!tok) continue;
        l->frame = strtol(tok, NULL, 10);
        while ((tok = strtok(NULL, " \t\n"))) {
            if (!strcmp(tok, "coin")) l->in.coin = true;
            else if (!strcmp(tok, "start")) l->in.start = true;
            else if (!strcmp(tok, "start2")) l->in.start2 = true;
            else if (!strcmp(tok, "left")) l->in.move_x = -1;
            else if (!strcmp(tok, "right")) l->in.move_x = 1;
            else if (!strcmp(tok, "fire")) l->in.fire = true;
            else if (!strcmp(tok, "up")) l->in.move_y = -1;
            else if (!strcmp(tok, "down")) l->in.move_y = 1;
            else if (!strcmp(tok, "menu")) l->in.menu = true;
            else fprintf(stderr, "galaga: %s: unknown token '%s'\n", path, tok);
        }
        script_len++;
    }
    fclose(f);
    return 0;
}

/* Replace live controls with the script line in force at `frame` (quit stays live). */
static void apply_script(InputFrame *in, long frame)
{
    while (script_pos + 1 < script_len && script[script_pos + 1].frame <= frame) script_pos++;
    bool quit = in->quit;
    if (script[script_pos].frame <= frame) *in = script[script_pos].in;
    else memset(in, 0, sizeof *in);
    in->menu = in->menu && script[script_pos].frame == frame; /* a press, on the line's first frame */
    in->quit = quit;
}

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

    long stepped = frames;
    while (acc >= step_ticks) {
        InputFrame in;
        input_poll(&in);
#ifndef __EMSCRIPTEN__
        if (in.quit) return false; /* a browser tab has nothing to quit to */
#endif
        if (script_len) apply_script(&in, frames + 1);
        step(&in);
        frames++;
        acc -= step_ticks;
        if (max_frames >= 0 && frames >= max_frames) break;
    }
    if (frames == stepped) {
#ifndef __EMSCRIPTEN__
        Uint64 ms = (step_ticks - acc) * 1000 / SDL_GetPerformanceFrequency();
        SDL_Delay(ms > 1 ? (Uint32)ms : 1);
#endif
        return true; /* the last picture stays */
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
    int want_scale = -1, want_rotate = -1;
    const char *rom_arg = NULL;
    if (argc == 3 && !strcmp(argv[1], "--check-rom")) return check_rom(argv[2]);
    if ((argc == 3 || argc == 4) && !strcmp(argv[1], "--dump-rom")) return dump_rom(argv[2], argc == 4 ? argv[3] : NULL);
    if ((argc == 3 || argc == 4) && !strcmp(argv[1], "--extract")) return extract(argv[0], argv[2], argc == 4 ? argv[3] : NULL);
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--windowed")) windowed = true;
        else if (!strcmp(argv[i], "--frames") && i + 1 < argc) max_frames = strtol(argv[++i], NULL, 10);
        else if (!strcmp(argv[i], "--screenshot") && i + 1 < argc) screenshot = argv[++i];
        else if (!strcmp(argv[i], "--rom") && i + 1 < argc) rom_arg = argv[++i];
        else if (!strcmp(argv[i], "--test-card")) want_card = true;
        else if (!strcmp(argv[i], "--emulate")) want_board = true;
        else if (!strcmp(argv[i], "--scale") && i + 1 < argc) {
            if ((want_scale = screen_mode_parse(argv[++i])) < 0) {
                fprintf(stderr, "galaga: unknown --scale '%s' (one of: auto integer fit stretch)\n", argv[i]);
                return 2;
            }
        }
        else if (!strcmp(argv[i], "--rotate") && i + 1 < argc) {
            if ((want_rotate = screen_rotate_parse(argv[++i])) < 0) {
                fprintf(stderr, "galaga: --rotate takes 0, 90 or 270\n");
                return 2;
            }
        }
        else if (!strcmp(argv[i], "--input") && i + 1 < argc) {
            if (load_script(argv[++i])) return 2;
        }
        else if (!strcmp(argv[i], "--gfx") && i + 1 < argc) {
            if (gfx_parse(argv[++i], &g.gfx)) {
                fprintf(stderr, "galaga: unknown --gfx preset '%s' (one of: %s)\n", argv[i], gfx_preset_names());
                return 2;
            }
        } else {
            fprintf(stderr,
                    "usage: %s [--windowed] [--gfx preset] [--scale auto|integer|fit|stretch]\n"
                    "          [--rotate 0|90|270] [--rom path] [--test-card] [--emulate]\n"
                    "          [--input script.txt] [--frames N] [--screenshot out.png]\n"
                    "       %s --check-rom <dir|zip>\n"
                    "       %s --dump-rom <dir|zip> [region]\n"
                    "       %s --extract <dir|zip> [<asset dir>]\n",
                    argv[0], argv[0], argv[0], argv[0]);
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
    if (!g.assets_path[0] && g.rom.loaded && !want_card && !want_board) { /* the first run: extract */
        char err[512];
        default_assets(argv[0], g.assets_path, sizeof g.assets_path);
        if (extract_all(&g.rom, g.assets_path, err, sizeof err)) { /* the program's folder is read-only: the save folder */
            snprintf(g.assets_path, sizeof g.assets_path, "%s", save_path("assets"));
            if (extract_all(&g.rom, g.assets_path, err, sizeof err)) {
                fprintf(stderr, "galaga: cannot extract the assets: %s\n", err);
                g.assets_path[0] = '\0';
            }
        }
        if (g.assets_path[0]) printf("galaga: assets extracted to %s\n", g.assets_path);
    }
    g.show = SHOW_CARD;
    if (want_board && !want_card) {
        if (!g.rom.loaded) fprintf(stderr, "galaga: --emulate needs the ROM set\n");
        else if (!g.rom.has_51xx || !g.rom.has_54xx)
            fprintf(stderr, "galaga: the emulated board needs the device ROMs: namco51.zip and namco54.zip next to %s\n",
                    g.rom_path);
        else {
            g.show = SHOW_BOARD;
            galaga_video_init(&g.video, &g.rom);
        }
    } else if (!want_card && g.assets_path[0]) {
        if (extract_load_tables(g.assets_path, g.data, sizeof g.data) || extract_load_waves(g.assets_path, g.waves) ||
            galaga_video_load(&g.video, g.assets_path))
            fprintf(stderr, "galaga: the assets in %s are incomplete: extract them again (--extract)\n", g.assets_path);
        else
            g.show = SHOW_GAME;
    }
    settings_load(); /* the screen's settings for the test card too; the file is made by the game */
    if (want_scale >= 0) g.screen.mode = want_scale;
    if (want_rotate >= 0) g.screen.rotate = want_rotate;
    screen_apply();
    if (g.show != SHOW_CARD) {
        g.hiscore_saved = !save_load(HISCORE_FILE, g.hiscore, sizeof g.hiscore);
        power_on();
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
