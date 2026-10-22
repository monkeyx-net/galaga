/*
 * The Namco WSG (waveform sound generator) as Galaga has it: three voices
 * stepping through 32-sample waveforms of 4 bits (8 in prom-1.1d) at 96
 * kHz. Registers $00-$1F, a nibble each: the waveform of voices 0, 1, 2 at
 * $05, $0A, $0F (low 3 bits); voice 0's frequency at $10-$14 (20 bits, low
 * nibble first) and volume at $15; voice 1's at $16-$19 (bits 4-19) and
 * $1A; voice 2's at $1B-$1E and $1F. Each 96 kHz step a voice's 20-bit
 * accumulator adds its frequency and its top 5 bits pick the sample: (sample
 * - 8) x volume x 48, in MAME's output units (measured: a voice at 0x2000
 * plays 750 Hz). Two steps are averaged into each 48 kHz sample.
 */
#ifndef GALAGA_WSG_H
#define GALAGA_WSG_H

#include <stdint.h>

typedef struct {
    uint8_t wave[8][32];   /* the waveforms (prom-1.1d) */
    uint8_t reg[0x20];
    uint32_t acc[3];
} Wsg;

/* waveforms: prom-1.1d's 256 bytes (low nibbles) */
void wsg_init(Wsg *w, const uint8_t *waveforms);
void wsg_write(Wsg *w, int reg, uint8_t value);
/* Add n samples at 48 kHz to out. */
void wsg_render(Wsg *w, int32_t *out, int n);

#endif
