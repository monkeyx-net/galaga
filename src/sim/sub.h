/*
 * sub's program: run by its IRQ each frame. The IRQ counts frames and runs
 * the tasks main enables at $9020-$9027 (a byte each: its step to the
 * next, 0 when off): the sprites' double buffer, the enemies' motion
 * through their scripts (twelve slots of 20 bytes at $9100), the
 * player's shots against the enemies, the enemies against the ship.
 */
#ifndef GALAGA_SUB_H
#define GALAGA_SUB_H

#include "sim/game.h"

/* $0513: the IRQ: its start (the frame counts), then its tasks */
void sub_irq(Game *g);
void sub_irq_start(Game *g);
void sub_irq_tasks(Game *g);
/* $05BF: the sprites' registers copied to their second bank */
void sub_sprites(Game *g);
/* $08D3: the motion of the twelve slots; D, E the registers at entry */
void sub_motion(Game *g, uint8_t d, uint8_t e);
/* $06F5: the player's two shots */
void sub_shots(Game *g);
/* $05EE: the enemies against the ship (or the two ships) */
void sub_ship(Game *g);
/* $0ECA: a debugging aid behind DSWA bit 2 (off on every set) */
void sub_debug(Game *g);

#endif
