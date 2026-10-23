/*
 * The C game: the three programs (sim/progs.h) and the customs' two
 * (sim/mcu.h) over the shared state, run by a scheduler like the board's
 * (MAME's), its time in attoseconds as MAME keeps it: in slices of 1/6000 s
 * or to the next timer, main, sub, sub2, the 51xx and the 54xx in turn,
 * each on to the slice's end by its cycles (a Z80 stopped between machine
 * cycles there, as MAME's is; one that ends short brings the slice's end
 * back to it);
 * the board's timers (the vertical blank, sub2's NMIs at lines 64 and 192,
 * line 0, the sound update, the 06xx's clock, which interrupts the chips it
 * selects); writes to the latches and the 06xx end the writer's slice
 * there and take effect once the others have caught up.
 */
#ifndef GALAGA_SIM_H
#define GALAGA_SIM_H

#include "sim/cpu.h"
#include "sim/game.h"
#include "sim/mcu.h"

enum { SIM_MAIN, SIM_SUB, SIM_SUB2, SIM_Z80S, SIM_51XX = SIM_Z80S, SIM_54XX, SIM_CPUS };

typedef struct {
    int64_t sec, as;
} SimTime;

typedef struct {
    SimTime at;
    int64_t period;  /* attoseconds, or 0: once */
    int64_t seq;     /* set in this order (equal times fire in it) */
    int what, param;
    int active;
} SimTimer;

#define SIM_TIMERS 16

typedef struct Sim {
    Game g;
    Cpu cpu[SIM_Z80S];
    Mcu mcu[2];              /* the 51xx, the 54xx */
    SimTime local[SIM_CPUS]; /* each CPU's time */
    int64_t done[SIM_CPUS];  /* its cycles counted into it (a stop inside an instruction's too) */
    int held[SIM_CPUS];      /* in reset */
    SimTime base;            /* the scheduler's time: the last slice's end */
    SimTime target;          /* the slice's end */
    int running;             /* the CPU running, or -1 */
    int aborted;             /* a timer set while it ran ended its slice */
    SimTimer timers[SIM_TIMERS];
    int64_t seq;
    int in_event;            /* a timer's event: its time is now */
    SimTime event_time;
    int frame_done;
    long frame;
    int watchdog;            /* vertical blanks since $6830 was last written */
    int irq_main, irq_sub;   /* the IRQ lines the vertical blank sets */
    uint8_t misc3;           /* $6823 as last written */
    /* the 06xx */
    uint8_t ctrl;            /* its control register ($7100) */
    uint8_t latch51, latch54; /* the bytes on the chips' inputs */
    int select;              /* the select lines are high */
    int first_tick;          /* the next rise is the first */
    int64_t tick_edge;       /* the next tick, as an edge of the 48 kHz clock */
    int64_t half_edges;      /* half the period, in edges */
    int star_origin;         /* the starfield's step (gfx/galaga.c) */
} Sim;

/* Power on; `data` the programs' bytes (GAME_DATA_SIZE). */
void sim_init(Sim *s, const uint8_t *data, uint8_t dswa, uint8_t dswb);
/* One frame, to the next vertical blank, with the control panel (active
   low) as the 51xx reads it in the frame */
void sim_frame(Sim *s, uint8_t in0, uint8_t in1);

#endif
