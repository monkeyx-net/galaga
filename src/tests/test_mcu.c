/*
 * The C game's customs against the board's MB88 (label rom):
 *   test_mcu <galaga.zip|folder>
 * Runs each chip's program in C (sim/mcu.h) and on emu/mb88.c with the
 * device ROM side by side, an instruction at a time, both with the same
 * inputs, drawn at random: the K and R ports, the interrupt line, the
 * timer's clock. After each step the cycles, the address, the registers,
 * the flags, the RAM and the ports must be the same.
 */
#include <stdio.h>
#include <string.h>

#include "assets/rom.h"
#include "emu/mb88.h"
#include "sim/mcu.h"

#define STEPS 4000000L

static RomSet rom;
static uint32_t seed = 12345;
static uint8_t k_in, r_in[4];
static uint8_t o_emu, o_c;
static int writes_emu, writes_c;

static uint32_t rnd(void)
{
    seed = seed * 1103515245u + 12345u;
    return seed >> 16;
}

static uint8_t emu_k(void *ctx) { (void)ctx; return k_in; }
static uint8_t emu_r(void *ctx, int port) { (void)ctx; return r_in[port]; }
static void emu_o(void *ctx, uint8_t v) { (void)ctx; o_emu = v; writes_emu++; }
static uint8_t c_k(void *ctx) { (void)ctx; return k_in; }
static uint8_t c_r(void *ctx, int port) { (void)ctx; return r_in[port]; }
static void c_o(void *ctx, uint8_t v) { (void)ctx; o_c = v; writes_c++; }

static int same(const MB88 *e, const Mcu *m)
{
    return e->a == m->a && e->x == m->x && e->y == m->y && e->st == m->st && e->zf == m->zf && e->cf == m->cf &&
           e->vf == m->vf && e->sf == m->sf && e->nf == m->nf && e->pio == m->pio && e->th == m->th && e->tl == m->tl &&
           e->sb == m->sb && e->o == m->o && e->p == m->p && !memcmp(e->r, m->r, 4) && !memcmp(e->ram, m->ram, 64) &&
           e->si == m->si && (e->pa << 6 | e->pc) == m->addr && e->irq_pending == m->irq_pending;
}

static int run(const char *name, const uint8_t *code, void (*prog)(Mcu *), int own_r)
{
    MB88 e;
    Mcu m;
    memset(&e, 0, sizeof e);
    memset(&m, 0, sizeof m);
    e.rom = code;
    e.read_k = emu_k;
    e.read_r = own_r ? NULL : emu_r;
    e.write_o = emu_o;
    mb88_reset(&e);
    m.read_k = c_k;
    m.read_r = own_r ? NULL : c_r;
    m.write_o = c_o;
    mcu_reset(&m);
    writes_emu = writes_c = 0;
    static uint8_t seen[0x400];
    memset(seen, 0, sizeof seen);
    int reached = 0;
    for (long i = 0; i < STEPS; i++) {
        uint32_t x = rnd() << 16 | rnd();
        k_in = x & 15;
        for (int p = 0; p < 4; p++) r_in[p] = (uint8_t)(rnd() & 15);
        if ((x >> 4 & 31) == 0) {
            int irq = !e.irq_line;
            mb88_set_irq(&e, irq);
            mcu_set_irq(&m, irq);
        }
        if ((x >> 9 & 255) == 0) {
            mb88_set_tc(&e, 1);
            mb88_set_tc(&e, 0);
            mcu_set_tc(&m, 1);
            mcu_set_tc(&m, 0);
        }
        if ((x >> 17 & 0x3FFF) == 0) {
            mb88_reset(&e);
            mcu_reset(&m);
        }
        if (!seen[m.addr]) seen[m.addr] = 1, reached++;
        int cycles = mb88_step(&e);
        int64_t t = m.t;
        m.budget = m.t + 1;
        prog(&m);
        if (m.t - t != cycles || !same(&e, &m) || writes_emu != writes_c || o_emu != o_c) {
            printf("mcu: the %s's program differs at step %ld: $%03X on the board, $%03X in C (%d, %d cycles)\n",
                   name, i, e.pa << 6 | e.pc, m.addr, cycles, (int)(m.t - t));
            return 1;
        }
    }
    printf("mcu: the %s's program equals the board's for %ld steps, at %d addresses\n", name, STEPS, reached);
    return 0;
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "usage: test_mcu <galaga.zip|folder>\n");
        return 2;
    }
    char err[512];
    if (rom_load(&rom, argv[1], err, sizeof err)) {
        fprintf(stderr, "%s\n", err);
        return 1;
    }
    if (!rom.has_51xx || !rom.has_54xx) {
        printf("mcu: no device ROMs (namco51.zip, namco54.zip) next to %s: skipped\n", argv[1]);
        return 77;
    }
    int bad = run("51xx", rom.mcu51, m51_run, 0);
    bad |= run("54xx", rom.mcu54, m54_run, 1);
    return bad;
}
