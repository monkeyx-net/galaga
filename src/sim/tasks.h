/*
 * main's IRQ ($0038 -> $0237) and its tasks (the table at $0096), line by
 * line (sim/cpu.h), generated from the ROM's code and read; they count
 * their Z80 cycles in z->t.
 */
#ifndef GALAGA_TASKS_H
#define GALAGA_TASKS_H

#include "sim/cpu.h"

/* $0237: the IRQ's start (registers saved, the stars' latches, the watchdog) */
void irq_0237(Cpu *z);
/* $026D: its tasks, then the 51xx read it asks for, its end */
void irq_026D(Cpu *z);
/* a task by its address */
void task_run(Cpu *z, uint16_t addr);

#endif
