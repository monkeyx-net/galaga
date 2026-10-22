#include "emu/board.h"

#include <string.h>

/* ---- time ------------------------------------------------------------------------ */

#define AS_PER_SECOND 1000000000000000000LL
#define Z80_APC 325520833333LL      /* attoseconds a cycle at 3.072 MHz (MAME's, truncated) */
#define MCU_APC (12 * Z80_APC - 1)    /* 6 clocks at 1.536 MHz: a hair under 12 Z80 cycles in MAME (1 to 4096 as under fit) */
#define QUANTUM_AS 166666666666666LL /* MAME's slices: 1/6000 s */
#define PIXEL_AS 162760416666LL     /* 6.144 MHz */
#define LINE_AS (PIXEL_AS * 384)
#define FRAME_AS (LINE_AS * BOARD_LINES)
#define SOUND_TIMER_AS 20000000000000000LL /* MAME's sound update, 50 Hz */
#define CLK06_HZ 48000              /* the 06xx's clock */
#define STAR_ORIGIN_POWER_ON 1579   /* the starfield's step at power-on (MAME's) */

static const int64_t apc[BOARD_CPUS] = { Z80_APC, Z80_APC, Z80_APC, MCU_APC, MCU_APC };

static Atime at_make(int64_t sec, int64_t as)
{
    sec += as / AS_PER_SECOND;
    as %= AS_PER_SECOND;
    if (as < 0) {
        as += AS_PER_SECOND;
        sec--;
    }
    return (Atime){ sec, as };
}

static Atime at_add_as(Atime t, int64_t as) { return at_make(t.sec, t.as + as); }
static int at_lt(Atime a, Atime b) { return a.sec < b.sec || (a.sec == b.sec && a.as < b.as); }
static Atime at_max(Atime a, Atime b) { return at_lt(a, b) ? b : a; }
/* a - b in attoseconds (the two are within a second or so of each other) */
static int64_t at_diff(Atime a, Atime b) { return (a.sec - b.sec) * AS_PER_SECOND + (a.as - b.as); }
static const Atime NEVER = { INT64_MAX / 2, 0 };

/* n periods of p attoseconds from t, without overflowing */
static Atime at_mul_add(Atime t, int64_t n, int64_t p)
{
    int64_t per_sec = AS_PER_SECOND / p; /* whole periods a second, roughly */
    if (per_sec < 1) per_sec = 1;
    while (n > per_sec) {
        t = at_add_as(t, per_sec * p);
        n -= per_sec;
    }
    return at_add_as(t, n * p);
}

int64_t board_total_cycles(const Board *b, int cpu)
{
    return b->total[cpu] + (b->executing == cpu ? b->run_cycles - b->icount : 0);
}

double board_cycles(const Board *b, int cpu)
{
    return ((double)b->local[cpu].sec * AS_PER_SECOND + (double)b->local[cpu].as) / (AS_PER_SECOND / 3072000.0);
}

/* ---- timers ---------------------------------------------------------------------- */

enum { T_VBLANK, T_LINE0, T_SUB2_NMI, T_SOUND, T_06XX, T_SYNC0 };
#define SYNC_TIMERS (BOARD_TIMERS - T_SYNC0)

static Atime next_expire(const Board *b)
{
    Atime t = NEVER;
    for (int i = 0; i < BOARD_TIMERS; i++)
        if (b->timers[i].active && at_lt(b->timers[i].expire, t)) t = b->timers[i].expire;
    return t;
}

/* The time the running CPU is at (its access's machine cycle); in a timer's
   callback, the timer's time; else the base. */
static Atime now(const Board *b)
{
    int c = b->executing;
    if (b->in_callback) return b->callback_time;
    if (c < 0) return b->base;
    int64_t done = c < CPU_51XX ? b->z80[c].t - b->t0 : 0;
    return at_add_as(b->local[c], (b->run_cycles - b->icount + done) * apc[c]);
}

/* A timer that becomes the first due, set while a CPU runs, ends the slice
   after the CPU's instruction (MAME's abort of the timeslice). */
static void timer_set(Board *b, int i, Atime at, int64_t period, void (*fire)(Board *, int), int param)
{
    BoardTimer *t = &b->timers[i];
    t->active = 0;
    if (b->executing >= 0 && at_lt(at, next_expire(b)) && !b->abort_slice) {
        b->abort_slice = 1;
        b->abort_t = b->executing < CPU_51XX ? b->z80[b->executing].t : 0;
    }
    t->expire = at;
    t->period = (Atime){ 0, period };
    t->active = 1;
    t->fire = fire;
    t->param = param;
    t->seq = ++b->timer_seq;
}

/* MAME's synchronize(): a timer now, running fire (if any) when the other
   CPUs have caught up. */
static void sync_now(Board *b, void (*fire)(Board *, int), int param)
{
    for (int i = T_SYNC0; i < BOARD_TIMERS; i++)
        if (!b->timers[i].active) {
            timer_set(b, i, now(b), 0, fire, param);
            return;
        }
}

/* Timers due by the base time, in order (equal times: in the order set). */
static void run_timers(Board *b)
{
    for (;;) {
        int due = -1;
        for (int i = 0; i < BOARD_TIMERS; i++) {
            BoardTimer *t = &b->timers[i];
            if (!t->active || at_lt(b->base, t->expire)) continue;
            if (due < 0 || at_lt(t->expire, b->timers[due].expire) ||
                (!at_lt(b->timers[due].expire, t->expire) && t->seq < b->timers[due].seq))
                due = i;
        }
        if (due < 0) return;
        BoardTimer *t = &b->timers[due];
        void (*fire)(Board *, int) = t->fire;
        int param = t->param;
        Atime when = t->expire;
        if (t->period.as) {
            t->expire = at_add_as(t->expire, t->period.as);
            t->seq = ++b->timer_seq;
        } else {
            t->active = 0;
        }
        b->in_callback = 1;
        b->callback_time = when;
        if (fire) fire(b, param);
        b->in_callback = 0;
    }
}

/* ---- interrupt lines ----------------------------------------------------------- */

/* A Z80 stopped inside an instruction sees line changes once it is done */
static void set_nmi(Board *b, int c, int state)
{
    if (b->part[c].active && b->part[c].nmis < 4) b->part[c].nmi[b->part[c].nmis++] = state;
    else z80_set_nmi(&b->z80[c], state);
}

static void set_irq(Board *b, int c, int state)
{
    if (b->part[c].active) b->part[c].irq = state;
    else b->z80[c].irq_line = state;
}

/* ---- the 06xx and the customs ------------------------------------------------- */

#define CHIP_51XX 0x01
#define CHIP_54XX 0x08

static void select_lines(Board *b, int state)
{
    b->select = state;
    mb88_set_irq(&b->mcu[0], state && (b->ctrl & CHIP_51XX));
    mb88_set_irq(&b->mcu[1], state && (b->ctrl & CHIP_54XX));
}

/* the 48 kHz clock's edge n, as a time */
static Atime clk06_edge(int64_t n)
{
    /* n x 1e18 / 48000 = n x 20833333333333 + n / 3 */
    int64_t per_sec = CLK06_HZ;
    return at_make(n / per_sec, (n % per_sec) * 20833333333333LL + (n % per_sec) / 3);
}

static void tick06(Board *b, int unused)
{
    b->tick_edge += b->half_edges;
    if (!b->select) {
        select_lines(b, 1);
        if (!(b->ctrl & 0x10) || !b->first_tick) set_nmi(b, CPU_MAIN, 1);
        b->first_tick = 0;
    } else {
        select_lines(b, 0);
        set_nmi(b, CPU_MAIN, 0);
    }
    timer_set(b, T_06XX, clk06_edge(b->tick_edge), 0, tick06, 0);
}

/* The 06xx's writes take effect once the other CPUs have caught up (MAME
   synchronizes them). */
static void ctrl_sync(Board *b, int v)
{
    b->ctrl = (uint8_t)v;
    select_lines(b, 0);
    set_nmi(b, CPU_MAIN, 0);
    if (!(v & 0x0F)) {
        b->timers[T_06XX].active = 0;
        return;
    }
    /* the clock starts on its next edge */
    Atime t = now(b);
    int64_t edge = (t.sec * CLK06_HZ) + (int64_t)((double)t.as * CLK06_HZ / AS_PER_SECOND);
    while (!at_lt(t, clk06_edge(edge))) edge++;
    int n = v >> 5;
    b->half_edges = n ? (int64_t)1 << (n - 1) : 1;
    b->tick_edge = edge;
    b->first_tick = 1;
    timer_set(b, T_06XX, clk06_edge(edge), 0, tick06, 0);
}

static void data_sync(Board *b, int v)
{
    if (b->ctrl & 0x10) return; /* read mode */
    if (b->ctrl & CHIP_51XX) b->latch51 = (uint8_t)v;
    if (b->ctrl & CHIP_54XX) b->latch54 = (uint8_t)v;
}

/* A read: the selected chips' O ports on the bus, which their latches keep */
static uint8_t data_r(Board *b)
{
    uint8_t v = 0xFF;
    if (!(b->ctrl & 0x10)) return v;
    if (b->ctrl & CHIP_51XX) v &= b->mcu[0].o;
    if (b->ctrl & CHIP_54XX) v &= b->mcu[1].o;
    if (b->ctrl & CHIP_51XX) b->latch51 = v;
    if (b->ctrl & CHIP_54XX) b->latch54 = v;
    return v;
}

static uint8_t mcu51_k(void *ctx)
{
    Board *b = ctx;
    return (uint8_t)((b->ctrl & 0x10 ? 8 : 0) | (b->latch51 & 7));
}

/* R0-R3: the control panel, a nibble each (IN0 low and high, IN1 low and high) */
static uint8_t mcu51_r(void *ctx, int port)
{
    Board *b = ctx;
    uint8_t in = port < 2 ? b->in0 : b->in1;
    return (uint8_t)(port & 1 ? in >> 4 : in & 15);
}

static uint8_t mcu54_k(void *ctx)
{
    Board *b = ctx;
    return b->latch54 >> 4;
}

static uint8_t mcu54_r(void *ctx, int port)
{
    Board *b = ctx;
    return port == 0 ? (b->latch54 & 15) : b->mcu[1].r[port];
}

/* ---- memory ---------------------------------------------------------------------- */

static uint8_t raw_read(Board *b, int cpu, uint16_t a)
{
    if (a < 0x4000) {
        if (cpu == CPU_MAIN) return b->rom->main[a];
        return a < 0x1000 ? (cpu == CPU_SUB ? b->rom->sub : b->rom->sub2)[a] : 0;
    }
    if (a >= 0x6800 && a <= 0x6807) {
        int n = a & 7;
        return (uint8_t)((b->dswb >> n & 1) | (b->dswa >> n & 1) << 1);
    }
    if (a >= 0x7000 && a <= 0x70FF) return data_r(b);
    if (a == 0x7100) return b->ctrl;
    if (a >= 0x8000 && a <= 0x87FF) return b->video[a & 0x7FF];
    if (a >= 0x8800 && a <= 0x8BFF) return b->ram[0][a & 0x3FF];
    if (a >= 0x9000 && a <= 0x93FF) return b->ram[1][a & 0x3FF];
    if (a >= 0x9800 && a <= 0x9BFF) return b->ram[2][a & 0x3FF];
    return 0;
}

uint8_t board_peek(Board *b, uint16_t addr) { return addr >= 0x7000 && addr <= 0x70FF ? 0xFF : raw_read(b, CPU_MAIN, addr); }

/* A Z80's access: replayed (1), or made now (0); one whose machine cycle
   starts once the slice's cycles are spent stops the CPU before it. */
static int z80_access(Board *b, int c, uint8_t *value)
{
    if (b->executing != c) return 0;
    if (b->replay) {
        b->replay--;
        *value = b->part[c].value[b->access++];
        return 1;
    }
    int t = b->z80[c].t;
    if (t - b->t0 >= b->icount_start || (b->abort_t >= 0 && t > b->abort_t)) {
        b->part[c].done = b->access;
        b->part[c].t = t;
        b->part[c].active = 1;
        longjmp(b->stop, 1);
    }
    return 0;
}

static uint8_t mem_read(Board *b, int cpu, uint16_t a)
{
    uint8_t v;
    if (z80_access(b, cpu, &v)) return v;
    v = raw_read(b, cpu, a);
    if (b->executing == cpu && b->access < 16) b->part[cpu].value[b->access] = v;
    b->access++;
    return v;
}

static void reset_lines(Board *b, int unused)
{
    int run = b->misc[3];
    for (int c = CPU_SUB; c < BOARD_CPUS; c++) {
        if (!run && !b->held[c]) {
            b->held[c] = 1;
            if (c < CPU_51XX) b->part[c].active = 0;
        } else if (run && b->held[c]) {
            /* a CPU in reset keeps no time: it starts now */
            b->held[c] = 0;
            b->local[c] = b->base;
            if (c < CPU_51XX) z80_reset(&b->z80[c]);
            else mb88_reset(&b->mcu[c - CPU_51XX]);
        }
    }
}

static void irq_lines(Board *b, int unused)
{
    set_irq(b, CPU_MAIN, b->irq_main);
    set_irq(b, CPU_SUB, b->irq_sub);
}

static void misc_w(Board *b, int n, uint8_t v)
{
    v &= 1;
    uint8_t was = b->misc[n];
    b->misc[n] = v;
    switch (n) {
    case 0:
        if (!v) {
            b->irq_main = 0;
            sync_now(b, irq_lines, 0);
        }
        break;
    case 1:
        if (!v) {
            b->irq_sub = 0;
            sync_now(b, irq_lines, 0);
        }
        break;
    case 2:
        if (v) set_nmi(b, CPU_SUB2, 0);
        break;
    case 3:
        if (v != was) sync_now(b, reset_lines, 0);
        break;
    default: break;
    }
}

static void mem_write(Board *b, int cpu, uint16_t a, uint8_t v)
{
    uint8_t unused;
    if (z80_access(b, cpu, &unused)) return; /* made before the CPU stopped */
    b->access++;
    if (b->write_hook) b->write_hook(b, cpu, a, v);
    if (a >= 0x6800 && a <= 0x681F) b->wsg[a & 0x1F] = v & 15;
    else if (a >= 0x6820 && a <= 0x6827) misc_w(b, a & 7, v);
    else if (a == 0x6830) b->watchdog = 0;
    else if (a >= 0x7000 && a <= 0x70FF) sync_now(b, data_sync, v);
    else if (a == 0x7100) sync_now(b, ctrl_sync, v);
    else if (a >= 0x8000 && a <= 0x87FF) b->video[a & 0x7FF] = v;
    else if (a >= 0x8800 && a <= 0x8BFF) b->ram[0][a & 0x3FF] = v;
    else if (a >= 0x9000 && a <= 0x93FF) b->ram[1][a & 0x3FF] = v;
    else if (a >= 0x9800 && a <= 0x9BFF) b->ram[2][a & 0x3FF] = v;
    else if (a >= 0xA000 && a <= 0xA007) b->vlatch[a & 7] = v & 1;
}

static uint8_t read_main(void *ctx, uint16_t a) { return mem_read(ctx, CPU_MAIN, a); }
static uint8_t read_sub(void *ctx, uint16_t a) { return mem_read(ctx, CPU_SUB, a); }
static uint8_t read_sub2(void *ctx, uint16_t a) { return mem_read(ctx, CPU_SUB2, a); }
static void write_main(void *ctx, uint16_t a, uint8_t v) { mem_write(ctx, CPU_MAIN, a, v); }
static void write_sub(void *ctx, uint16_t a, uint8_t v) { mem_write(ctx, CPU_SUB, a, v); }
static void write_sub2(void *ctx, uint16_t a, uint8_t v) { mem_write(ctx, CPU_SUB2, a, v); }

/* ---- the screen's events --------------------------------------------------------- */

/* The watchdog: the board starts again (RAM keeps its contents), main from
   the vertical blank, the others held in reset by the cleared latch */
static void watchdog_reset(Board *b)
{
    memset(b->misc, 0, sizeof b->misc);
    memset(b->vlatch, 0, sizeof b->vlatch);
    b->irq_main = b->irq_sub = 0;
    b->ctrl = 0;
    b->timers[T_06XX].active = 0;
    select_lines(b, 0);
    b->part[CPU_MAIN].active = 0;
    z80_reset(&b->z80[CPU_MAIN]);
    z80_set_nmi(&b->z80[CPU_MAIN], 0);
    b->z80[CPU_MAIN].irq_line = 0;
    b->local[CPU_MAIN] = b->base;
    reset_lines(b, 0);
    b->watchdog = 0;
}

static void vblank(Board *b, int unused)
{
    b->frame_start = b->timers[T_VBLANK].expire;
    b->frame_done = 1;
    b->star_origin = galaga_star_advance(b->star_origin, b->vlatch);
    if (++b->watchdog >= BOARD_WATCHDOG_FRAMES) watchdog_reset(b);
    if (b->misc[0]) b->irq_main = 1;
    if (b->misc[1]) b->irq_sub = 1;
    sync_now(b, irq_lines, 0);
    /* the 51xx's timer counts frames */
    mb88_set_tc(&b->mcu[0], 1);
    mb88_set_tc(&b->mcu[0], 0);
    timer_set(b, T_VBLANK, at_add_as(b->timers[T_VBLANK].expire, FRAME_AS), 0, vblank, 0);
}

static void line0(Board *b, int unused)
{
    timer_set(b, T_LINE0, at_add_as(b->timers[T_LINE0].expire, FRAME_AS), 0, line0, 0);
}

/* sub2's NMI at lines 64 and 192 */
static void sub2_nmi(Board *b, int line)
{
    if (!b->misc[2]) {
        set_nmi(b, CPU_SUB2, 1);
        set_nmi(b, CPU_SUB2, 0);
    }
    int next = line == 64 ? 192 : 64;
    timer_set(b, T_SUB2_NMI, at_add_as(b->timers[T_SUB2_NMI].expire, (int64_t)(next == 192 ? 128 : 136) * LINE_AS), 0,
              sub2_nmi, next);
}

static void sound_tick(Board *b, int unused) {}

/* ---- power-on and the slices ---------------------------------------------------- */

void board_init(Board *b, const RomSet *rom)
{
    memset(b, 0, sizeof *b);
    b->rom = rom;
    b->in0 = b->in1 = 0xFF;
    b->dswa = DSWA_DEFAULT;
    b->dswb = DSWB_DEFAULT;
    b->executing = -1;
    Z80 *z = b->z80;
    z[0].read = read_main, z[0].write = write_main;
    z[1].read = read_sub, z[1].write = write_sub;
    z[2].read = read_sub2, z[2].write = write_sub2;
    for (int i = 0; i < 3; i++) {
        z[i].ctx = b;
        z80_reset(&z[i]);
    }
    b->mcu[0].rom = rom->mcu51;
    b->mcu[0].read_k = mcu51_k;
    b->mcu[0].read_r = mcu51_r;
    b->mcu[1].rom = rom->mcu54;
    b->mcu[1].read_k = mcu54_k;
    b->mcu[1].read_r = mcu54_r;
    for (int i = 0; i < 2; i++) {
        b->mcu[i].ctx = b;
        mb88_reset(&b->mcu[i]);
    }
    for (int c = CPU_SUB; c < BOARD_CPUS; c++) b->held[c] = 1;
    b->star_origin = STAR_ORIGIN_POWER_ON;
    /* the screen starts at the vertical blank */
    timer_set(b, T_VBLANK, (Atime){ 0, FRAME_AS }, 0, vblank, 0);
    timer_set(b, T_LINE0, (Atime){ 0, 40 * LINE_AS }, 0, line0, 0);
    timer_set(b, T_SUB2_NMI, (Atime){ 0, 104 * LINE_AS }, 0, sub2_nmi, 64);
    timer_set(b, T_SOUND, (Atime){ 0, SOUND_TIMER_AS }, SOUND_TIMER_AS, sound_tick, 0);
}

/* A Z80's instruction, or the rest of one stopped in an earlier slice.
   Returns the cycles it ran in this slice; *stopped is set when it stops
   inside the instruction again. */
static int z80_run(Board *b, int c, int *stopped)
{
    b->icount_start = b->icount;
    b->access = 0;
    b->abort_t = -1;
    *stopped = 0;
    if (b->part[c].active) {
        b->part[c].active = 0;
        b->z80[c] = b->part[c].start;
        b->replay = b->part[c].done;
        b->t0 = b->part[c].t;
    } else {
        if (b->step_hook) b->step_hook(b, c);
        b->part[c].start = b->z80[c];
        b->part[c].nmis = 0;
        b->part[c].irq = -1;
        b->replay = 0;
        b->t0 = 0;
    }
    if (setjmp(b->stop)) {
        *stopped = 1;
        return b->part[c].t - b->t0;
    }
    int cycles = z80_step(&b->z80[c]) - b->t0;
    b->t0 = 0;
    /* the line changes held while it was stopped */
    for (int i = 0; i < b->part[c].nmis; i++) z80_set_nmi(&b->z80[c], b->part[c].nmi[i]);
    if (b->part[c].irq >= 0) b->z80[c].irq_line = b->part[c].irq;
    b->part[c].nmis = 0;
    b->part[c].irq = -1;
    return cycles;
}

static int step(Board *b, int c, int *stopped)
{
    if (c < CPU_51XX) return z80_run(b, c, stopped);
    *stopped = 0;
    if (b->step_hook) b->step_hook(b, c);
    return mb88_step(&b->mcu[c - CPU_51XX]);
}

/* One of MAME's timeslices: every CPU runs to the target in turn; one that
   stops short (its slice aborted, or a part cycle left) brings the target
   back to its time. */
static void timeslice(Board *b)
{
    Atime expire = next_expire(b);
    while (at_lt(b->base, expire)) {
        Atime target = at_add_as(b->base, QUANTUM_AS);
        if (at_lt(expire, target)) target = expire;
        b->target = target;
        for (int c = 0; c < BOARD_CPUS; c++) {
            if (b->target.sec < b->local[c].sec) continue;
            int64_t delta = at_diff(b->target, b->local[c]);
            if (delta < apc[c]) continue;
            if (b->held[c]) continue;
            /* MAME divides by the cycle's attoseconds cut to 32 bits, which
               rounds up where the time is a whole number of cycles less a
               few attoseconds */
            int shift = 0;
            while ((apc[c] >> shift) >= ((int64_t)1 << 32)) shift++;
            int64_t ran = (delta >> shift) / (apc[c] >> shift);
            b->executing = c;
            b->run_cycles = ran;
            b->icount = ran;
            b->abort_slice = 0;
            while (b->icount > 0) {
                int stopped;
                b->icount -= step(b, c, &stopped);
                if (stopped) break;
                if (b->abort_slice) { /* MAME: the cycles left are taken back */
                    b->run_cycles -= b->icount;
                    b->icount = 0;
                }
            }
            ran = b->run_cycles - b->icount;
            b->executing = -1;
            b->total[c] += ran;
            b->local[c] = at_mul_add(b->local[c], ran, apc[c]);
            if (at_lt(b->local[c], b->target)) b->target = at_max(b->local[c], b->base);
        }
        b->base = b->target;
        expire = next_expire(b);
    }
    run_timers(b);
}

void board_run_frame(Board *b)
{
    b->frame_done = 0;
    while (!b->frame_done) timeslice(b);
    b->frame++;
}
