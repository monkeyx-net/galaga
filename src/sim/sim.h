/*
 * The C game: the three programs and the customs over the shared state, a
 * frame at a time in the order the board runs them: at the vertical blank
 * the 51xx's loop, main's IRQ (its tasks, then the 51xx's three bytes and
 * a 54xx command if one is asked for) and sub's; main's foreground on to
 * the next vertical blank, by its cycles (main's code counts them, as the
 * board's Z80 takes them), so it stops where the board's would; sub2's two
 * NMIs (lines 64 and 192).
 */
#ifndef GALAGA_SIM_H
#define GALAGA_SIM_H

#include "sim/cpu.h"
#include "sim/game.h"
#include "sim/io.h"

typedef struct {
    Game g;
    Io io;
    Cpu fg;          /* main's foreground (resumable); its t is main's time in cycles */
    Cpu irq;         /* main's IRQ's registers */
    int irq_main;    /* main's IRQ line, held from the vertical blank until its handler takes it */
    int irq_sub;
    int64_t vblank;  /* the next vertical blank on main's time */
    long frame;
} Sim;

/* The state after the power-on tests, at $02D3 (the 51xx set up as the
   tests leave it); `data` the programs' bytes (GAME_DATA_SIZE). */
void sim_init(Sim *s, const uint8_t *data, uint8_t dswa, uint8_t dswb);
/* One frame, with the control panel (active low) as the board reads it */
void sim_frame(Sim *s, uint8_t in0, uint8_t in1);

#endif
