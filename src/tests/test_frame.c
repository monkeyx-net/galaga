/*
 * The C game against the board, frame by frame (label rom):
 *   test_frame <galaga.zip|folder> [frames]
 * Powers both on and runs them with the same inputs (the tests' player,
 * from the board's memory), comparing at the end of each frame all the
 * memory, the latches and the WSG's registers. Reports the first frame
 * that differs and stops there. A game's end ends the run; with
 * TEST_FRAME_ON set the player goes on to the next game (and the board's
 * 51xx can go astray at a game's end, plan section 2: the C game's too).
 * TEST_FRAME_TRACE=<frame>[:<from>-<to>] prints, from that frame on, both
 * sides' writes to the latches, the 06xx and the given addresses (hex), at
 * their times (seconds since power-on).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assets/rom.h"
#include "emu/board.h"
#include "sim/sim.h"
#include "tests/player.h"

#define AS_PER_SECOND 1000000000000000000LL
#define Z80_APC 325520833333LL

static RomSet rom;
static Board board;
static Sim sim;
static uint8_t data[GAME_DATA_SIZE];
static uint8_t wsg[0x20]; /* the board's WSG registers as written */
static const char *const names[] = { "main", "sub", "sub2" };

static long trace_from = -1;
static unsigned watch_from = 1, watch_to = 0;

static int traced(uint16_t a)
{
    return (a >= 0x6820 && a <= 0x6827) || (a >= 0x7000 && a <= 0x7100) || (a >= watch_from && a <= watch_to);
}

/* a CPU's time, `cycles` on from its own */
static void trace(const char *side, int c, int64_t sec, int64_t as, int64_t cycles, uint16_t a, uint8_t v)
{
    as += cycles * Z80_APC;
    sec += as / AS_PER_SECOND;
    as %= AS_PER_SECOND;
    fprintf(stderr, "%lld.%018lld %-5s %-4s $%04X=%02X\n", (long long)sec, (long long)as, side, names[c], a, v);
}

static void write_hook(Board *b, int c, uint16_t a, uint8_t v)
{
    if (a >= 0x6800 && a <= 0x681F) wsg[a & 0x1F] = v & 15;
    if (trace_from >= 0 && b->frame >= trace_from && c < 3 && traced(a))
        trace("board", c, b->local[c].sec, b->local[c].as, b->run_cycles - b->icount + b->z80[c].t - b->t0, a, v);
}

static void sim_write(void *user, uint16_t a, uint8_t v)
{
    Sim *s = user;
    int c = s->running;
    if (board.frame >= trace_from && c >= 0 && c < SIM_Z80S && traced(a))
        trace("C", c, s->local[c].sec, s->local[c].as, s->cpu[c].t_access - s->done[c], a, v);
}

static uint8_t peek_board(void *ctx, uint16_t a) { return board_peek(ctx, a); }

static uint8_t sim_value(uint16_t a)
{
    if (a >= 0x6800 && a <= 0x681F) return sim.g.wsg[a & 0x1F];
    if (a >= 0x6820 && a <= 0x6827) return sim.g.misc[a & 7];
    if (a >= 0xA000 && a <= 0xA007) return sim.g.vlatch[a & 7];
    return game_rd(&sim.g, a);
}

static uint8_t board_value(uint16_t a)
{
    if (a >= 0x6800 && a <= 0x681F) return wsg[a & 0x1F];
    if (a >= 0x6820 && a <= 0x6827) return board.misc[a & 7];
    if (a >= 0xA000 && a <= 0xA007) return board.vlatch[a & 7];
    return board_peek(&board, a);
}

/* the WSG and the latches, video RAM, RAM */
static long compare(void)
{
    static const struct { uint16_t from, to; } ranges[] = {
        { 0x6800, 0x6827 }, { 0x8000, 0x87FF }, { 0x8800, 0x8BFF }, { 0x9000, 0x93FF }, { 0x9800, 0x9BFF }, { 0xA000, 0xA007 },
    };
    long diff = 0;
    for (size_t r = 0; r < sizeof ranges / sizeof ranges[0]; r++)
        for (uint32_t a = ranges[r].from; a <= ranges[r].to; a++) {
            uint8_t b = board_value((uint16_t)a), s = sim_value((uint16_t)a);
            if (b != s && diff++ < 12) printf("  $%04X: the C game's %02X, the board's %02X\n", (unsigned)a, s, b);
        }
    return diff;
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "usage: test_frame <galaga.zip|folder> [frames]\n");
        return 2;
    }
    long frames = argc > 2 ? atol(argv[2]) : 20000;
    char err[512];
    if (rom_load(&rom, argv[1], err, sizeof err)) {
        fprintf(stderr, "%s\n", err);
        return 1;
    }
    if (!rom.has_51xx || !rom.has_54xx) {
        printf("frame: the board needs the device ROMs (namco51.zip, namco54.zip) next to %s: skipped\n", argv[1]);
        return 77;
    }
    memcpy(data, rom.main, ROM_MAIN_SIZE);
    memcpy(data + GAME_SUB, rom.sub, ROM_SUB_SIZE);
    memcpy(data + GAME_SUB2, rom.sub2, ROM_SUB2_SIZE);
    board_init(&board, &rom);
    board.write_hook = write_hook;
    sim_init(&sim, data, board.dswa, board.dswb);
    const char *tr = getenv("TEST_FRAME_TRACE");
    if (tr) {
        trace_from = atol(tr);
        const char *range = strchr(tr, ':');
        if (range && sscanf(range + 1, "%x-%x", &watch_from, &watch_to) == 1) watch_to = watch_from;
        sim.g.on_write = sim_write;
        sim.g.user = &sim;
    }
    Player p = { .attract = 6200 };
    p.verbose = getenv("TEST_FRAME_VERBOSE") != NULL;
    uint8_t in0 = 0xFF, in1 = 0xFF;
    int games = 0;
    for (long i = 0; i < frames; i++) {
        board.in0 = in0;
        board.in1 = in1;
        board_run_frame(&board);
        sim_frame(&sim, in0, in1);
        if (sim.g.unported) {
            printf("frame %lld: the C game reached $%04X, not ported\n", (long long)board.frame, sim.g.unported);
            return 1;
        }
        long d = compare();
        if (d) {
            printf("frame %lld: %ld bytes differ\n", (long long)board.frame, d);
            for (int c = 0; c < 3; c++)
                printf("  %s at $%04X on the board, $%04X in C\n", names[c], board.z80[c].pc, sim.cpu[c].pc);
            return 1;
        }
        player_step(&p, (long)board.frame, peek_board, &board, &in0, &in1);
        if (p.game_over) {
            games++;
            if (!getenv("TEST_FRAME_ON")) break;
            p.game_over = 0;
        }
    }
    printf("frame: the C game equals the board for %lld frames, %d games\n", (long long)board.frame, games);
    return 0;
}
