/*
 * The C game's 06xx customs against the board's (label rom):
 *   test_io <galaga.zip|folder>
 * Runs the board (its 51xx the MB8843 program itself) with a script of
 * coins, starts and moves, and follows main's transfers: the bytes main
 * writes to the 51xx go to sim/io.c's 51xx as well, and each byte main
 * reads from it must be the one the C 51xx hands out. The C 51xx runs its
 * loop at each frame's start, with that frame's control panel, from the
 * frame main lets the customs out of reset. With the default switches,
 * then with other coinages (DSWB bits 0-2, free play among them), which
 * main hands the 51xx at power-on.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assets/rom.h"
#include "emu/board.h"
#include "sim/io.h"

#define FRAMES 30000

static RomSet rom;
static Board board;
static Io io;
static int running;      /* the customs out of reset */
static uint8_t ctrl;     /* main's last control write */
static uint8_t pending;  /* the byte the C 51xx put out at the last rise */
static long reads, writes, bad;

static void write_hook(Board *b, int c, uint16_t a, uint8_t v)
{
    if (c != CPU_MAIN) return;
    if (a == 0x6823 && (v & 1) && !running) {
        running = 1;
        io_reset(&io);
    }
    if (a == 0x7100) {
        ctrl = v;
        if ((v & 0x11) == 0x11 && running) pending = io_51xx_read(&io.m51); /* the first rise */
        return;
    }
    if (a >= 0x7000 && a <= 0x70FF && !(ctrl & 0x10) && (ctrl & 1) && running) {
        io_51xx_write(&io.m51, v);
        writes++;
    }
}

static void read_hook(Board *b, int c, uint16_t a, uint8_t v)
{
    if (c != CPU_MAIN || a < 0x7000 || a > 0x70FF || (ctrl & 0x11) != 0x11 || !running) return;
    if (v != pending && bad++ < 10)
        fprintf(stderr, "frame %lld: main read %02X from the 51xx, the C 51xx's %02X\n", (long long)b->frame, v, pending);
    reads++;
    pending = io_51xx_read(&io.m51);
}

/* the inputs: frame -> what is held (active high here) */
static uint8_t script_in0(long f)
{
    uint8_t v = 0;
    if (f > 1400 && f % 300 < 40) v |= IN0_LEFT1;
    if (f > 1400 && f % 300 >= 150 && f % 300 < 200) v |= IN0_RIGHT1;
    if (f > 1400 && f % 700 >= 300 && f % 700 < 330) v |= IN0_LEFT2 | IN0_RIGHT2;
    return v;
}

static uint8_t script_in1(long f)
{
    static const struct { long from, to; uint8_t v; } held[] = {
        { 1000, 1003, IN1_COIN1 },  { 1020, 1030, IN1_COIN1 },  { 1050, 1100, IN1_COIN1 },
        { 1200, 1201, IN1_COIN2 },  { 1300, 1310, IN1_START1 }, { 3000, 3020, IN1_COIN1 },
        { 3040, 3041, IN1_COIN1 },  { 3100, 3110, IN1_START2 }, { 3500, 3510, IN1_SERVICE },
        { 8000, 8010, IN1_COIN2 },  { 8020, 8030, IN1_COIN2 },  { 8040, 8050, IN1_COIN2 },
        { 9000, 9030, IN1_START1 }, { 12000, 12005, IN1_COIN1 | IN1_START1 },
    };
    uint8_t v = 0;
    for (size_t i = 0; i < sizeof held / sizeof held[0]; i++)
        if (f >= held[i].from && f < held[i].to) v |= held[i].v;
    if (f > 1400 && f % 13 < 3) v |= IN1_FIRE1;
    if (f > 1400 && f % 17 < 2) v |= IN1_FIRE2;
    return v;
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "usage: test_io <galaga.zip|folder>\n");
        return 2;
    }
    char err[512];
    if (rom_load(&rom, argv[1], err, sizeof err)) {
        fprintf(stderr, "%s\n", err);
        return 1;
    }
    if (!rom.has_51xx || !rom.has_54xx) {
        fprintf(stderr, "test_io: the board needs the device ROMs (namco51.zip, namco54.zip) next to %s\n", argv[1]);
        return 1;
    }
    static const int coinages[] = { -1, 0, 1, 2, 3, 5 };
    for (size_t i = 0; i < sizeof coinages / sizeof coinages[0]; i++) {
        board_init(&board, &rom);
        if (coinages[i] >= 0) board.dswb = (uint8_t)((board.dswb & ~7) | coinages[i]);
        board.write_hook = write_hook;
        board.read_hook = read_hook;
        running = 0;
        while (board.frame < (coinages[i] < 0 ? FRAMES : 12000)) {
            if (running) io_frame(&io, board.in0, board.in1);
            board_run_frame(&board);
            long f = (long)board.frame;
            board.in0 = (uint8_t)~script_in0(f);
            board.in1 = (uint8_t)~script_in1(f);
        }
    }
    printf("io: %ld bytes written to the 51xx, %ld read, %ld differ\n", writes, reads, bad);
    if (bad || !reads || !writes) return 1;
    puts("io: the C 51xx hands out what the board's does");
    return 0;
}
