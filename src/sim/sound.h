/*
 * sub2's program: the sound driver, run by its NMI twice a frame (lines 64
 * and 192). Main and sub ask for sounds by setting their flags at
 * $9AA0-$9AB6; each sound's header (sub2's $0734) names its tracks and
 * voice; a track's notes (pitch and octave, length in units) play through
 * the WSG with an envelope; the registers are kept at $9A60-$9A72 and
 * copied out to $6805-$681F at the end of each NMI.
 */
#ifndef GALAGA_SOUND_H
#define GALAGA_SOUND_H

#include "sim/game.h"

/* $0066: the NMI (unless one is under way) */
void sound_nmi(Game *g);
/* $00BB: its work */
void sound_work(Game *g);

#endif
