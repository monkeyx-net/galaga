/*
 * The C game against the board, frame by frame (label rom):
 *   test_frame <galaga.zip|folder> [frames]
 * Runs the board to frame HANDOFF (the attract mode), gives the C game its
 * state there (memory, latches, the WSG's registers, the 51xx's RAM and
 * timer, main's registers, stack and where its foreground waits), then
 * runs both with the same inputs (the tests' player) and compares all the
 * memory, the latches and the WSG's registers at the end of each frame.
 * Reports the first frame that differs and stops there.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assets/rom.h"
#include "emu/board.h"
#include "sim/sim.h"
#include "tests/player.h"

#define HANDOFF 900

static RomSet rom;
static Board board;
static Sim sim;
static uint8_t data[GAME_DATA_SIZE];
static uint8_t wsg[0x20]; /* the WSG's registers as written */

static long long irq_from, irq_cycles; /* the board's main IRQ this frame */
static void step_hook(Board *b, int c)
{
    if (c != CPU_MAIN) return;
    const Z80 *z = &b->z80[c];
    long long now = (long long)board_total_cycles(b, c);
    if (z->pc == 0x0038 && z->last_interrupt) irq_from = now - 13;
    if (z->pc == 0x02B8) irq_cycles = now + 10 - irq_from;
}

static void write_hook(Board *b, int c, uint16_t a, uint8_t v)
{
    if (a >= 0x6800 && a <= 0x681F) wsg[a & 0x1F] = v & 15;
}

static uint8_t peek_board(void *ctx, uint16_t a) { return board_peek(ctx, a); }

static int handoff(void)
{
    Game *g = &sim.g;
    sim_init(&sim, data, board.dswa, board.dswb);
    memcpy(g->video, board.video, sizeof g->video);
    memcpy(g->ram, board.ram, sizeof g->ram);
    memcpy(g->misc, board.misc, sizeof g->misc);
    memcpy(g->vlatch, board.vlatch, sizeof g->vlatch);
    memcpy(g->wsg, wsg, sizeof g->wsg);
    /* the 51xx: its loop waits for the next frame */
    const MB88 *m = &board.mcu[0];
    Mcu *c = &sim.io.m51;
    memcpy(c->ram, m->ram, sizeof c->ram);
    c->tl = m->tl, c->th = m->th, c->sb = m->sb, c->o = m->o, c->p = m->p, c->pio = m->pio, c->vf = m->vf;
    memcpy(c->r, m->r, sizeof c->r);
    sim.io.ctrl = board.ctrl;
    /* main: registers, stack, where it goes on */
    /* stopped inside an instruction, the board runs it again from its start */
    const Z80 *z = board.part[CPU_MAIN].active ? &board.part[CPU_MAIN].start : &board.z80[CPU_MAIN];
    Cpu *f = &sim.fg;
    f->a = z->a, f->b = z->b, f->c = z->c, f->d = z->d, f->e = z->e, f->h = z->h, f->l = z->l;
    cpu_set_af(f, (uint16_t)(z->a << 8 | z->f));
    f->a2 = z->a2;
    f->sf2 = z->f2 >> 7 & 1, f->zf2 = z->f2 >> 6 & 1, f->hf2 = z->f2 >> 4 & 1, f->nf2 = z->f2 >> 1 & 1, f->cf2 = z->f2 & 1;
    f->b2 = z->b2, f->c2 = z->c2, f->d2 = z->d2, f->e2 = z->e2, f->h2 = z->h2, f->l2 = z->l2;
    f->ix = (uint16_t)(z->ixh << 8 | z->ixl), f->iy = (uint16_t)(z->iyh << 8 | z->iyl);
    f->iff = z->iff1;
    f->sp = 0;
    for (int a = 0x909E; a >= z->sp; a -= 2) cpu_push(f, (uint16_t)(board_peek(&board, (uint16_t)a) | board_peek(&board, (uint16_t)(a + 1)) << 8));
    uint16_t pc = z->halted ? (uint16_t)(z->pc + 1) : z->pc; /* every instruction is a place to go on from */
    f->pc = pc;
    f->halted = z->halted;
    /* main's time: the vertical blank that ends the board's frame at 0 */
    f->t = (int64_t)((double)(board.local[CPU_MAIN].sec - board.frame_start.sec) * 3072000.0 +
                     (double)(board.local[CPU_MAIN].as - board.frame_start.as) / 325520833333.0);
    sim.vblank = 0;
    sim.irq_main = board.irq_main;
    sim.irq_sub = board.irq_sub;
    printf("frame: the C game takes over at frame %d, main at $%04X (SP %04X)%s\n", HANDOFF, pc, z->sp,
           board.part[CPU_MAIN].active ? ", inside an instruction" : "");
    return 0;
}

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

/* the addresses compared: the WSG and the latches, video RAM, RAM, bar main's stack */
static long compare(void)
{
    static const struct { uint16_t from, to; } ranges[] = {
        { 0x6800, 0x6827 }, { 0x8000, 0x87FF }, { 0x8800, 0x8BFF }, { 0x9000, 0x93FF }, { 0x9800, 0x9BFF }, { 0xA000, 0xA007 },
    };
    long diff = 0;
    /* sub2's NMI under way at the vertical blank ($9A8C): its state, and the
       WSG's registers it copies out at its end, are compared at the next
       frame that has none (the C game runs its NMIs whole) */
    int sound_busy = board_peek(&board, 0x9A8C) != 0;
    for (size_t r = 0; r < sizeof ranges / sizeof ranges[0]; r++)
        for (uint32_t a = ranges[r].from; a <= ranges[r].to; a++) {
            if (sound_busy && ((a >= 0x6800 && a <= 0x681F) || (a >= 0x9A00 && a <= 0x9AFF))) continue;
            if (a >= 0x9080 && a < 0x90A0) continue; /* main's stack */
            if (a >= 0x90E0 && a < 0x9100) continue; /* sub's */
            if (a >= 0x9AE0 && a < 0x9B00) continue; /* sub2's */
            uint8_t b = board_value((uint16_t)a), s = sim_value((uint16_t)a);
            if (b != s && diff++ < 12)
                printf("  $%04X: the C game's %02X, the board's %02X\n", (unsigned)a, s, b);
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
        fprintf(stderr, "test_frame: the board needs the device ROMs (namco51.zip, namco54.zip) next to %s\n", argv[1]);
        return 1;
    }
    memcpy(data, rom.main, ROM_MAIN_SIZE);
    memcpy(data + GAME_SUB, rom.sub, ROM_SUB_SIZE);
    memcpy(data + GAME_SUB2, rom.sub2, ROM_SUB2_SIZE);
    board_init(&board, &rom);
    board.write_hook = write_hook;
    board.step_hook = step_hook;
    while (board.frame < HANDOFF) board_run_frame(&board);
    handoff();
    Player p = { .attract = 0 };
    p.verbose = getenv("TEST_FRAME_VERBOSE") != NULL;
    for (long i = 0; i < frames; i++) {
        uint8_t in0, in1;
        player_step(&p, (long)board.frame, peek_board, &board, &in0, &in1);
        board.in0 = in0;
        board.in1 = in1;
        board_run_frame(&board);
        sim_frame(&sim, in0, in1);
        if (sim.g.unported) {
            printf("frame %lld: the C game reached $%04X, not ported\n", (long long)board.frame, sim.g.unported);
            return 1;
        }
        if (getenv("TEST_FRAME_VERBOSE"))
            fprintf(stderr, "frame %lld: IRQ %lld cycles, C %lld\n", (long long)board.frame, irq_cycles, (long long)sim.irq.t);
        if (0)
            fprintf(stderr, "frame %lld: 99B5 %02X %02X %02X / %02X %02X %02X  9201 %02X/%02X  t %lld budget %lld pc %04X\n",
                    (long long)board.frame, sim_value(0x99B5), sim_value(0x99B6), sim_value(0x99B7), board_value(0x99B5),
                    board_value(0x99B6), board_value(0x99B7), sim_value(0x9201), board_value(0x9201), (long long)sim.fg.t,
                    (long long)sim.fg.budget, sim.fg.pc);
        long d = compare();
        if (d) {
            printf("frame %lld: %ld bytes differ (main at $%04X on the board, $%04X in C)\n", (long long)board.frame, d,
                   board.z80[0].pc, sim.fg.pc);
            return 1;
        }
        if (p.game_over) break;
    }
    printf("frame: the C game equals the board for %lld frames\n", (long long)board.frame - HANDOFF);
    return 0;
}
