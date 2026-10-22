/*
 * Fujitsu MB88xx 4-bit microcontroller core (the MB8843 in the 51xx and
 * the MB8844 in the 54xx): 1 KB of program in 16 pages of 64 bytes (JMP
 * stays in its page; the PC counts on into the next), 64 nibbles of RAM
 * addressed by X:Y, the accumulator A, the status flag ST that conditional
 * jumps and calls test, the zero and carry flags, a 4-level stack, the
 * 8-bit timer TH:TL, the 4-bit serial register, the external interrupt
 * (taken after the instruction under way, in 3 cycles, to $002), and the
 * ports: K (4-bit input), O (8-bit output, a nibble at a time), P (4-bit
 * output) and R0-R3 (4-bit input/output). One instruction cycle is 6
 * clocks; two-byte instructions take two. The serial unit is not modelled
 * (the 51xx starts it, which sets SF, but neither program reads it).
 * Written from the MB88xx instruction set; checked against known results
 * (test mb88) and MAME's traces of the 51xx and 54xx (mame_emu). The ports
 * go through the board's callbacks.
 */
#ifndef GALAGA_MB88_H
#define GALAGA_MB88_H

#include <stdint.h>

#define MB88_ROM_SIZE 0x400
#define MB88_RAM_SIZE 0x40

/* EN / DIS bits */
#define MB88_PIO_IRQ 0x04      /* the external interrupt */
#define MB88_PIO_TIMER 0x20    /* the timer counts */
#define MB88_PIO_TIMER_IRQ 0x40 /* (unused here) */

typedef struct MB88 {
    const uint8_t *rom;      /* MB88_ROM_SIZE bytes */
    uint8_t ram[MB88_RAM_SIZE];
    uint8_t pc;              /* within the page, 0-63 */
    uint8_t pa;              /* the page, 0-15 */
    uint16_t stack[4];       /* PC | PA << 6 | flags << 10 */
    uint8_t si;              /* the stack's index */
    uint8_t a, x, y;
    uint8_t st, zf, cf;      /* the status, zero and carry flags (0 or 1) */
    uint8_t vf, sf, nf;      /* timer overflow, serial full, the interrupt pin */
    uint8_t pio;             /* the EN / DIS bits */
    uint8_t th, tl;          /* the timer */
    uint8_t sb;              /* the serial register */
    uint8_t o;               /* the O port's latch */
    uint8_t p;               /* the P port's latch */
    uint8_t r[4];            /* the R ports' output latches */
    int irq_line;            /* the external interrupt input, 1 = asserted */
    int irq_pending;         /* an edge was seen while enabled */
    int tc_line;             /* the timer's clock input */
    int64_t cycles;          /* instruction cycles executed */
    int last_interrupt;      /* the last step took an interrupt (traces) */
    void *ctx;
    uint8_t (*read_k)(void *ctx);
    uint8_t (*read_r)(void *ctx, int port);            /* R0-R3 as the pins read */
    void (*write_r)(void *ctx, int port, uint8_t v);
    void (*write_o)(void *ctx, uint8_t v);
    void (*write_p)(void *ctx, uint8_t v);
} MB88;

void mb88_reset(MB88 *m);
/* Execute one instruction (and an interrupt after it); returns the cycles used. */
int mb88_step(MB88 *m);
/* The external interrupt input. */
void mb88_set_irq(MB88 *m, int state);
/* The timer's clock input: the timer counts on a falling edge. */
void mb88_set_tc(MB88 *m, int state);
/* MAME's CURFLAGS: ST bit 0, ZF 1, CF 2, VF 3, SF 4, NF 5. */
int mb88_flags(const MB88 *m);

#endif
