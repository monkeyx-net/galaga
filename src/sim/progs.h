/*
 * The three programs in C: each the whole of its Z80 code (from its reset,
 * its interrupts' entries and the jump tables, all it reaches), generated
 * line by line from the ROM's code (build/re/tools/z80prog.py, not in the
 * repository) on sim/cpu.h, then read. A program runs from z->pc until its
 * cycles (z->t, counted as the Z80 takes them) reach z->budget, or it
 * HALTs; every instruction is a place to go on from, so a scheduler
 * (sim.c) can run the three by turns as the board's does and take their
 * interrupts between instructions. CALL, RET, PUSH and POP go through
 * memory, as the ROM's code expects.
 */
#ifndef GALAGA_PROGS_H
#define GALAGA_PROGS_H

#include "sim/cpu.h"

void main_run(Cpu *z); /* main: reset at $0000, IRQ $0038, NMI $0066 */
void sub_run(Cpu *z);  /* sub: reset $0000, IRQ $0038 */
void sub2_run(Cpu *z); /* sub2: reset $0000, NMI $0066 */

#endif
