/*
 * The 06xx's customs in C: the 51xx (coins, credits, the control panel) and
 * the 54xx (the noise commands), each its MB88 program whole, generated
 * instruction by instruction from the chip's code (build/re/tools/
 * mb88prog.py, not in the repository), then read. A program runs from
 * m->addr until its cycles (m->t, one or two an instruction, three more for
 * an interrupt) reach m->budget, its interrupt taken after the instruction
 * under way, as the board's MB88 (emu/mb88.c) does; the scheduler (sim.c)
 * runs the two by turns with the Z80s, as the board's does, which their
 * cycles shape. Their ports go through the callbacks.
 */
#ifndef GALAGA_MCU_H
#define GALAGA_MCU_H

#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* EN / DIS bits */
#define MCU_PIO_IRQ 0x04   /* the external interrupt */
#define MCU_PIO_TIMER 0x20 /* the timer counts */

typedef struct Mcu {
    uint8_t ram[64];
    uint16_t stack[4];          /* the address | the flags (ST, ZF, CF) << 10 */
    uint8_t si;                 /* the stack's index */
    uint8_t a, x, y;
    uint8_t st, zf, cf;         /* the status, zero and carry flags */
    uint8_t vf, sf, nf;         /* timer overflow, serial full, the interrupt pin */
    uint8_t pio;                /* the EN / DIS bits */
    uint8_t th, tl, sb;         /* the timer, the serial register */
    uint8_t o, p;               /* the O and P ports' latches */
    uint8_t r[4];               /* the R ports' latches */
    uint8_t irq_line, irq_pending, tc_line;
    uint16_t addr;              /* where it goes on: the page << 6 | the PC */
    int64_t t, budget;          /* its cycles run, and where it stops */
    void *ctx;
    uint8_t (*read_k)(void *ctx);
    uint8_t (*read_r)(void *ctx, int port); /* or none: the latches */
    void (*write_o)(void *ctx, uint8_t v);  /* or none */
} Mcu;

void m51_run(Mcu *m);
void m54_run(Mcu *m);

/* The reset: the RAM and the input lines keep their state */
static inline void mcu_reset(Mcu *m)
{
    Mcu k = *m;
    memset(m, 0, offsetof(Mcu, ctx));
    memcpy(m->ram, k.ram, sizeof m->ram);
    m->irq_line = k.irq_line;
    m->tc_line = k.tc_line;
    m->t = k.t;
    m->budget = k.budget;
    m->st = 1;
}

/* The external interrupt: NF follows the pin; it is taken on its rising
   edge, when enabled, after the instruction under way */
static inline void mcu_set_irq(Mcu *m, int state)
{
    if (state && !m->irq_line && (m->pio & MCU_PIO_IRQ)) m->irq_pending = 1;
    m->irq_line = (uint8_t)(state != 0);
    m->nf = (uint8_t)(state != 0);
}

/* The timer's clock input: it counts on a falling edge */
static inline void mcu_set_tc(Mcu *m, int state)
{
    if (!state && m->tc_line && (m->pio & MCU_PIO_TIMER)) {
        m->tl = (m->tl + 1) & 15;
        if (!m->tl) {
            m->th = (m->th + 1) & 15;
            if (!m->th) m->vf = 1;
        }
    }
    m->tc_line = (uint8_t)(state != 0);
}

static inline void mcu_set_a(Mcu *m, int v)
{
    m->a = (uint8_t)(v & 15);
    m->zf = m->a == 0;
}

static inline uint8_t mcu_read_r(Mcu *m, int port) { return m->read_r ? m->read_r(m->ctx, port) & 15 : m->r[port]; }

/* OUTO: a nibble of O, CF picks the half */
static inline void mcu_outo(Mcu *m)
{
    m->o = m->cf ? (uint8_t)((m->o & 0x0F) | m->a << 4) : (uint8_t)((m->o & 0xF0) | m->a);
    if (m->write_o) m->write_o(m->ctx, m->o);
}

static inline void mcu_push(Mcu *m, unsigned addr)
{
    m->stack[m->si] = (uint16_t)(addr | (m->st | m->zf << 1 | m->cf << 2) << 10);
    m->si = (m->si + 1) & 3;
}

/* RTI restores the flags as well */
static inline unsigned mcu_pop(Mcu *m, int flags)
{
    m->si = (m->si - 1) & 3;
    unsigned v = m->stack[m->si];
    if (flags) {
        m->st = (v >> 10) & 1;
        m->zf = (v >> 11) & 1;
        m->cf = (v >> 12) & 1;
    }
    return v & 0x3FF;
}

/* The interrupt, after an instruction: to $002, in 3 cycles */
static inline unsigned mcu_interrupt(Mcu *m, unsigned addr)
{
    m->irq_pending = 0;
    mcu_push(m, addr);
    m->st = 1;
    m->t += 3;
    return 0x002;
}

#endif
