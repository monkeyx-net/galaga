#include "sim/sim.h"

#include <string.h>

#include "sim/fg.h"
#include "sim/sound.h"
#include "sim/sub.h"
#include "sim/tasks.h"

/* main's cycles: a frame (384 x 264 pixels, a Z80 cycle each two), the
   IRQ's acknowledge (IM 1) and its JP $0237 at $0038 */
#define FRAME_CYCLES 50688
#define IRQ_ENTRY (13 + 10)
/* main's NMI ($0066) on a byte of a transfer: on, last, last when it then
   starts the 54xx's command */
#define NMI_BYTE 59
#define NMI_LAST 129
#define NMI_LAST_54XX 191

void sim_init(Sim *s, const uint8_t *data, uint8_t dswa, uint8_t dswb)
{
    memset(s, 0, sizeof *s);
    Game *g = &s->g;
    g->data = data;
    g->dswa = dswa;
    g->dswb = dswb;
    g->in0 = g->in1 = 0xFF;
    g->io = &s->io;
    io_reset(&s->io);
    s->fg.g = g;
    s->fg.pc = 0x02D3;
    s->fg.iff = 1;
    s->irq.g = g;
}

/* the cycles of main's NMIs on an n-byte transfer */
static int64_t nmis(int n, int then_54xx)
{
    if (n <= 0) return 0;
    return (int64_t)(n - 1) * NMI_BYTE + (then_54xx ? NMI_LAST_54XX : NMI_LAST);
}

/* main's IRQ's tasks, then the 51xx's three bytes it asks for and the
   54xx's command main's NMI sends after them when $9AB9 asks; their
   cycles on main's time */
static void main_irq_end(Sim *s)
{
    Game *g = &s->g;
    irq_026D(&s->irq);
    s->fg.t += s->irq.t;
    if (!(RD(0x6804) & 2)) return; /* frozen: no transfer */
    io_transfer(&s->io, g, 0x71, 0x99B5, 3);
    if (RD(0x9AB9)) {
        WR(0x9AB9, 0);
        io_transfer(&s->io, g, 0xA8, 0x0092, 4);
        s->fg.t += nmis(3, 1) + nmis(4, 0);
    } else {
        s->fg.t += nmis(3, 0);
    }
}

void sim_frame(Sim *s, uint8_t in0, uint8_t in1)
{
    Game *g = &s->g;
    Cpu *fg = &s->fg;
    g->in0 = in0;
    g->in1 = in1;
    io_frame(&s->io, in0, in1);
    if (g->misc[0]) s->irq_main = 1;
    if (g->misc[1]) s->irq_sub = 1;
    int64_t vblank = s->vblank;
    s->vblank += FRAME_CYCLES;
    if (fg->halted && fg->t < vblank) /* a HALT runs NOPs until the interrupt */
        fg->t += (vblank - fg->t + 3) / 4 * 4;
    for (;;) {
        /* the two IRQs at once on the board: main's start (the stars from
           the frame count), sub's (the frame count), main's tasks, sub's */
        int main_now = s->irq_main && fg->iff, sub_now = s->irq_sub;
        if (main_now) {
            s->irq_main = 0;
            fg->halted = 0;
            s->irq.t = IRQ_ENTRY;
            s->irq.sp = 0;
            irq_0237(&s->irq);
        }
        s->irq_sub = 0;
        if (sub_now) sub_irq_start(g);
        if (main_now) main_irq_end(s);
        if (sub_now) sub_irq_tasks(g);
        /* the foreground on to the next vertical blank */
        fg->budget = s->vblank;
        fg->resume = 0;
        if (!fg->halted) fg_main(fg);
        if (!fg->resume) break;
    }
    if (!g->misc[2]) { /* sub2's NMIs, lines 64 and 192 */
        sound_nmi(g);
        sound_nmi(g);
    }
    s->frame++;
}
