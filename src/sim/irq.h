/*
 * main's IRQ: each frame it sets the stars' latches from $99BE (bar a
 * frozen game), feeds the watchdog, runs the tasks set at $9000-$901F (a
 * byte each: its step to the next, 0 when off) through the pointers at
 * $0096, then asks the 51xx for its three bytes ($99B5-$99B7; the board's
 * NMIs move them, the C game's 51xx hands them over).
 */
#ifndef GALAGA_IRQ_H
#define GALAGA_IRQ_H

#include "sim/game.h"

/* $0237: the IRQ, bar the 51xx read it starts */
void irq_main(Game *g);

/* the tasks */
void task_sprites(Game *g);    /* $0828: the sprites' second bank (main's half) */
void task_difficulty(Game *g); /* $0857: the enemies' rates from the rank */
void task_blink(Game *g);      /* $0935: 1UP / 2UP blinking */
void task_credits(Game *g);    /* $0977: the credits, the clock, a game starting */

#endif
