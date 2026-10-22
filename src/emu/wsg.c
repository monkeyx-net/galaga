#include "emu/wsg.h"

#include <string.h>

void wsg_init(Wsg *w, const uint8_t *waveforms)
{
    memset(w, 0, sizeof *w);
    for (int i = 0; i < 256; i++) w->wave[i >> 5][i & 31] = waveforms[i] & 15;
}

void wsg_write(Wsg *w, int reg, uint8_t value) { w->reg[reg & 0x1F] = value & 15; }

static uint32_t frequency(const Wsg *w, int voice)
{
    const uint8_t *r = w->reg;
    if (voice == 0) return (uint32_t)(r[0x10] | r[0x11] << 4 | r[0x12] << 8 | r[0x13] << 12 | r[0x14] << 16);
    int base = voice == 1 ? 0x16 : 0x1B;
    return (uint32_t)(r[base] << 4 | r[base + 1] << 8 | r[base + 2] << 12 | r[base + 3] << 16);
}

void wsg_render(Wsg *w, int32_t *out, int n)
{
    static const int wave_reg[3] = { 0x05, 0x0A, 0x0F }, volume_reg[3] = { 0x15, 0x1A, 0x1F };
    for (int v = 0; v < 3; v++) {
        int volume = w->reg[volume_reg[v]];
        uint32_t f = frequency(w, v);
        const uint8_t *wave = w->wave[w->reg[wave_reg[v]] & 7];
        uint32_t acc = w->acc[v];
        for (int i = 0; i < n; i++) {
            int sum = 0;
            for (int k = 0; k < 2; k++) {
                acc = (acc + f) & 0xFFFFF;
                sum += wave[acc >> 15] - 8;
            }
            out[i] += sum * volume * 24; /* the two steps' mean, x 48 */
        }
        w->acc[v] = acc;
    }
}
