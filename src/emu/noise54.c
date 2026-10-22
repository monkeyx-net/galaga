#include "emu/noise54.h"

#include <math.h>
#include <string.h>

#define RATE 48000.0
#define PI 3.14159265358979323846

static const struct {
    double f0, q, gain;
} band[2] = { { 181.97, 2.125, 4124.7 }, { 517.13, 1.597, 985.7 } };

void noise54_reset(Noise54 *n) { memset(n, 0, sizeof *n); }

void noise54_write(Noise54 *n, uint8_t o) { n->o = o; }

void noise54_render(Noise54 *n, int32_t *out, int count)
{
    for (int c = 0; c < 2; c++) {
        /* the band-pass by the bilinear transform, peak gain at f0 */
        double w0 = 2 * PI * band[c].f0 / RATE, alpha = sin(w0) / (2 * band[c].q), a0 = 1 + alpha;
        double b0 = alpha * band[c].gain / a0, b2 = -b0, a1 = -2 * cos(w0) / a0, a2 = (1 - alpha) / a0;
        double x = c ? n->o >> 4 : n->o & 15;
        for (int i = 0; i < count; i++) {
            double y = b0 * x + b2 * n->x2[c] - a1 * n->y1[c] - a2 * n->y2[c];
            n->x2[c] = n->x1[c];
            n->x1[c] = x;
            n->y2[c] = n->y1[c];
            n->y1[c] = y;
            out[i] += (int32_t)lrint(y);
        }
    }
}
