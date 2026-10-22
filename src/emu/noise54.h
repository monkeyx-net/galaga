/*
 * The 54xx's explosions, as MAME 0.289 plays them: the MCU drives two
 * 4-bit levels on its O port (its own noise, already gated), each through
 * a band-pass in the discrete circuit: the low nibble 182 Hz (Q 2.1), the
 * high nibble 517 Hz (Q 1.6), peak gains 4125 and 986 a step in MAME's
 * output units. Fitted to MAME's output with the WSG muted (the board's
 * 54xx outputs are MAME's, instruction for instruction): 86% of its power,
 * every 0.1 s within 3 dB. Generated: nothing from the ROM set.
 */
#ifndef GALAGA_NOISE54_H
#define GALAGA_NOISE54_H

#include <stdint.h>

typedef struct {
    uint8_t o;                 /* the O port */
    double x1[2], x2[2], y1[2], y2[2];
} Noise54;

void noise54_reset(Noise54 *n);
/* The O port's new value. */
void noise54_write(Noise54 *n, uint8_t o);
/* Add n samples at 48 kHz to out. */
void noise54_render(Noise54 *n, int32_t *out, int count);

#endif
