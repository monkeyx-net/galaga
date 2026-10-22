/*
 * main's IRQ tasks, line by line (sim/cpu.h), by their addresses in the
 * table at $0096.
 */
#ifndef GALAGA_TASKS_H
#define GALAGA_TASKS_H

#include "sim/game.h"

void task_1700(Game *g);
void task_17B2(Game *g);
void task_19B2(Game *g);
void task_1A86(Game *g);
void task_1B6B(Game *g);
void task_1D38(Game *g);
void task_1D7C(Game *g);
void task_1DB9(Game *g);
void task_1DD8(Game *g);
void task_1DEC(Game *g);
void task_1EAA(Game *g);
void task_1F0E(Game *g);
void task_1F8F(Game *g);
void task_2000(Game *g);
void task_20F2(Game *g);
void task_21CB(Game *g);
void task_2222(Game *g);
void task_23DD(Game *g);
void task_2916(Game *g);
void task_2A90(Game *g);

#endif
