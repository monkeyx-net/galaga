/*
 * main's foreground: from $02D3 (after the power-on tests) the attract mode
 * and the game, a state machine polling what the IRQ's tasks set. Ported
 * line by line as one resumable function: its waits (on the IRQ's timers,
 * on sub, on the 06xx) and its HALTs return with z->pc where it goes on,
 * its calls push their return addresses on the Cpu's stack as the ROM's
 * do (its code pops them at times).
 */
#ifndef GALAGA_FG_H
#define GALAGA_FG_H

#include "sim/cpu.h"

/* Run until the next wait (z->pc 0x02D3 to start) */
void fg_main(Cpu *z);

#endif
