/*
 * The Galaga board: three Z80s at 3.072 MHz (main, sub, sub2) sharing one
 * memory map, the 51xx and 54xx customs (MB88xx MCUs at 256 k instructions
 * a second) behind the 06xx bus controller, the video RAM and latches.
 *
 * Every CPU sees: its own program ROM at $0000-$3FFF (sub and sub2 have 4
 * KB; past the end reads 0), the DIP switches $6800-$6807 (bit 0 of DSWB,
 * bit 1 of DSWA, a switch per address), the sound (WSG) $6800-$681F, the
 * misc latch $6820-$6827 (bit 0: $6820 main IRQ on / clear, $6821 sub IRQ
 * on / clear, $6822 sub2 NMI off, $6823 the sub CPUs and customs out of
 * reset), the watchdog $6830, the 06xx's data $7000-$70FF and control
 * $7100, the video RAM $8000-$87FF (tile codes, then colours), RAM
 * $8800-$8BFF, $9000-$93FF, $9800-$9BFF (sprites at the top of each), the
 * video latch $A000-$A007 (bit 0: the starfield's speed, sets and enable,
 * flip). Main and sub get an IRQ at the vertical blank (line 224) when on,
 * held until cleared; sub2 an NMI at lines 64 and 192 unless off. Eight
 * vertical blanks without a write to $6830 reset the board.
 *
 * The 06xx: a control byte (bits 0-3 the chips selected, 51xx = 0 and 54xx
 * = 3; bit 4 read; bits 5-7 n) starts a clock of period 64 << n CPU cycles
 * on the next edge of its 48 kHz clock. The select lines (the MCUs'
 * interrupts) are high its first half; main gets an NMI at each rise (in
 * read mode, from the second on). Its handler moves a byte through $7000:
 * written, the selected chips latch it (the 51xx reads its low three bits
 * and the read bit on K, the 54xx its nibbles on R0 and K); read, the
 * selected chip's O port (latched too). Control $10 stops it. The 51xx's
 * R0-R3 are the control panel, its timer counts vertical blanks.
 *
 * Time is kept as MAME keeps it, in attoseconds, and the CPUs run in MAME's
 * slices (1/6000 s, cut short by timers): main, sub, sub2, then the MCUs,
 * each to the slice's end, a CPU that stops short bringing the end back to
 * its time. A write that changes another CPU's lines or the 06xx takes
 * effect once the others have caught up (MAME's synchronize), and stops
 * the writing CPU's slice. A Z80 stops between machine cycles when its
 * cycles run out, the rest of the instruction left for its next slice. So
 * the board equals MAME's run instruction for instruction (plan section 2).
 */
#ifndef GALAGA_BOARD_H
#define GALAGA_BOARD_H

#include <setjmp.h>
#include <stdint.h>

#include "assets/rom.h"
#include "emu/mb88.h"
#include "emu/noise54.h"
#include "emu/wsg.h"
#include "emu/z80.h"
#include "gfx/galaga.h"

#define BOARD_CPUS 5 /* main, sub, sub2, 51xx, 54xx: MAME's order */
enum { CPU_MAIN, CPU_SUB, CPU_SUB2, CPU_51XX, CPU_54XX };

#define BOARD_LINES 264
#define BOARD_CYCLES_PER_LINE 192
#define BOARD_FRAME_CYCLES (BOARD_LINES * BOARD_CYCLES_PER_LINE) /* 50688 */
#define BOARD_VBLANK_LINE 224                                   /* frames start here */
#define BOARD_WATCHDOG_FRAMES 8 /* vertical blanks without a write to $6830 reset the board */
#define BOARD_AUDIO_RATE 48000
#define BOARD_AUDIO_SAMPLES (BOARD_FRAME_CYCLES / 64) /* 792 a frame */

/* Inputs, active low, as MAME's ports */
#define IN0_RIGHT1 0x02
#define IN0_LEFT1 0x08
#define IN0_RIGHT2 0x20
#define IN0_LEFT2 0x80
#define IN1_FIRE1 0x01
#define IN1_FIRE2 0x02
#define IN1_START1 0x04
#define IN1_START2 0x08
#define IN1_COIN1 0x10
#define IN1_COIN2 0x20
#define IN1_SERVICE 0x40
#define IN1_SERVICE_MODE 0x80 /* clear: on */
#define DSWA_DEFAULT 0xF7     /* easy, demo sounds on, upright */
#define DSWB_DEFAULT 0x97     /* 1C/1C, 20K 70K every 70K, 3 ships */

/* MAME's attotime: seconds and attoseconds */
typedef struct {
    int64_t sec, as;
} Atime;

typedef struct Board Board;

typedef struct {
    Atime expire, period; /* period 0: once */
    int active;
    int64_t seq;          /* set in this order (equal times fire in it) */
    void (*fire)(Board *b, int param);
    int param;
} BoardTimer;

#define BOARD_TIMERS 12

struct Board {
    Z80 z80[3];
    MB88 mcu[2];
    const RomSet *rom;
    Atime local[BOARD_CPUS];  /* each CPU's own time */
    int held[BOARD_CPUS];     /* in reset */
    int64_t total[BOARD_CPUS]; /* cycles run */
    Atime base;               /* the scheduler's time: the last slice's end */
    Atime target;             /* the slice's end */
    int executing;            /* the CPU running, or -1 */
    int64_t run_cycles, icount; /* its cycles to run, and left */
    int abort_slice;          /* the running CPU stops after its instruction */
    /* MAME stops a Z80 between machine cycles once its slice's cycles are
       spent: an access that would come later is left for its next slice.
       The instruction is then run again from its start, its accesses so far
       replayed (part[]). */
    struct {
        int active;           /* stopped inside an instruction */
        Z80 start;            /* the CPU when the instruction began */
        uint8_t value[16];    /* what its accesses so far read */
        int done;             /* accesses made */
        int t;                /* the instruction's cycles run */
        int nmi[4], nmis;     /* NMI line changes held meanwhile */
        int irq;              /* the IRQ line held meanwhile, or -1 */
    } part[3];
    int access, replay;       /* the running Z80's accesses in this instruction, and to replay */
    int t0;                   /* its instruction's cycles run in earlier slices */
    int abort_t;              /* the T-state its slice was aborted at, or -1 */
    int64_t icount_start;     /* its cycles left when the instruction began */
    jmp_buf stop;
    BoardTimer timers[BOARD_TIMERS];
    int64_t timer_seq;
    int in_callback;          /* a timer's callback runs: its time is now */
    Atime callback_time;

    uint8_t video[0x800];     /* $8000: tile codes, then colours */
    uint8_t ram[3][0x400];    /* $8800, $9000, $9800 */
    uint8_t misc[8];          /* $6820-$6827, bit 0 */
    uint8_t vlatch[8];        /* $A000-$A007, bit 0 */
    /* the sound: the WSG and the 54xx's noise; the last frame's samples in
       audio, made at its end from the writes' times in it */
    Wsg wsg;
    Noise54 noise;
    int16_t audio[BOARD_AUDIO_SAMPLES];
    struct { int16_t sample; uint8_t reg, value; } sound_events[512]; /* reg $20: the 54xx's O port */
    int sound_event_count;
    uint8_t in0, in1, dswa, dswb;
    int irq_main, irq_sub;    /* the vertical blank's IRQs, held until cleared */
    int watchdog;             /* vertical blanks since $6830 was last written */

    /* the 06xx */
    uint8_t ctrl;
    uint8_t latch51, latch54; /* the last byte written to each chip */
    int select;               /* the select lines are high */
    int first_tick;           /* the next rise is the first */
    int64_t tick_edge;        /* the next tick, as an edge of the 48 kHz clock */
    int64_t half_edges;       /* half the period, in edges */

    int star_origin;          /* the starfield's step (gfx/galaga.c) */
    int64_t frame;            /* frames run */
    Atime frame_start;        /* the frame's vertical blank */
    int frame_done;
    /* optional: called before each instruction of each CPU (traces) */
    void (*step_hook)(Board *b, int cpu);
    /* optional: called on every CPU write */
    void (*write_hook)(Board *b, int cpu, uint16_t addr, uint8_t value);
    void *user;
};

/* Power on. */
void board_init(Board *b, const RomSet *rom);
/* Run one frame: to the next vertical blank. */
void board_run_frame(Board *b);
/* A memory read as the CPUs see it (no side effects). */
uint8_t board_peek(Board *b, uint16_t addr);
/* A CPU's time in CPU cycles since power-on (MCUs: in Z80 cycles too). */
double board_cycles(const Board *b, int cpu);
/* The cycles a CPU has run (MAME's totalcycles), during its instruction too. */
int64_t board_total_cycles(const Board *b, int cpu);

#endif
