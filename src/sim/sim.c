#include "sim/sim.h"

#include <string.h>

#include "gfx/galaga.h"
#include "sim/progs.h"

/* ---- time: the board's (MAME's), in attoseconds ------------------------------------- */

#define AS_PER_SECOND 1000000000000000000LL
#define Z80_APC 325520833333LL       /* attoseconds a cycle at 3.072 MHz (MAME's, truncated) */
#define QUANTUM_AS 166666666666666LL /* MAME's slices: 1/6000 s */
#define PIXEL_AS 162760416666LL      /* 6.144 MHz */
#define LINE_AS (PIXEL_AS * 384)
#define FRAME_AS (LINE_AS * 264)
#define SOUND_TIMER_AS 20000000000000000LL /* MAME's sound update, 50 Hz */
#define CLK06_HZ 48000               /* the 06xx's clock */
#define MCU_APC (12 * Z80_APC - 1)   /* 6 clocks at 1.536 MHz: a hair under 12 Z80 cycles in MAME */
#define STAR_ORIGIN_POWER_ON 1579    /* the starfield's step at power-on (MAME's) */
#define WATCHDOG_FRAMES 8

typedef SimTime Atime;

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
static int64_t at_diff(Atime a, Atime b) { return (a.sec - b.sec) * AS_PER_SECOND + (a.as - b.as); }
static const Atime NEVER = { INT64_MAX / 2, 0 };

/* n periods of p attoseconds from t, without overflowing */
static Atime at_mul_add(Atime t, int64_t n, int64_t p)
{
    int64_t per_sec = AS_PER_SECOND / p;
    if (per_sec < 1) per_sec = 1;
    while (n > per_sec) {
        t = at_add_as(t, per_sec * p);
        n -= per_sec;
    }
    return at_add_as(t, n * p);
}

/* the 48 kHz clock's edge n, as a time */
static Atime clk06_edge(int64_t n)
{
    int64_t per_sec = CLK06_HZ;
    return at_make(n / per_sec, (n % per_sec) * 20833333333333LL + (n % per_sec) / 3);
}

enum { T_VBLANK, T_LINE0, T_SUB2_NMI, T_SOUND, T_06XX, T_SYNC0 };
enum { E_VBLANK, E_LINE0, E_SUB2_NMI, E_SOUND, E_06XX, E_IRQ_LINES, E_RESET_LINES, E_CTRL, E_DATA };

static void (*const runs[SIM_Z80S])(Cpu *) = { main_run, sub_run, sub2_run };
static void (*const mcu_runs[2])(Mcu *) = { m51_run, m54_run };
static const int64_t apc[SIM_CPUS] = { Z80_APC, Z80_APC, Z80_APC, MCU_APC, MCU_APC };

/* ---- timers ------------------------------------------------------------------------ */

static Atime next_due(const Sim *s)
{
    Atime t = NEVER;
    for (int i = 0; i < SIM_TIMERS; i++)
        if (s->timers[i].active && at_lt(s->timers[i].at, t)) t = s->timers[i].at;
    return t;
}

/* The time now: the running CPU's at the write under way; in a timer's
   event, its time; else the base */
static Atime now(const Sim *s)
{
    if (s->in_event) return s->event_time;
    int c = s->running;
    if (c < 0) return s->base;
    if (c >= SIM_Z80S) return at_mul_add(s->local[c], s->mcu[c - SIM_Z80S].t - s->done[c], MCU_APC);
    const Cpu *z = &s->cpu[c];
    return at_mul_add(s->local[c], z->t_access - s->done[c], Z80_APC);
}

/* A timer; one that becomes the first due while a CPU runs ends its slice
   after its instruction (MAME's abort of the timeslice) */
static void timer_set(Sim *s, int i, Atime at, int64_t period, int what, int param)
{
    s->timers[i].active = 0;
    if (s->running >= 0 && at_lt(at, next_due(s)) && !s->aborted) {
        s->aborted = 1;
        if (s->running < SIM_Z80S) s->cpu[s->running].budget = s->cpu[s->running].t;
        else s->mcu[s->running - SIM_Z80S].budget = s->mcu[s->running - SIM_Z80S].t;
    }
    s->timers[i] = (SimTimer){ at, period, ++s->seq, what, param, 1 };
}

/* MAME's synchronize(): what to do once the other CPUs have caught up */
static void sync_now(Sim *s, int what, int param)
{
    for (int i = T_SYNC0; i < SIM_TIMERS; i++)
        if (!s->timers[i].active) {
            timer_set(s, i, now(s), 0, what, param);
            return;
        }
}

#define CHIP_51XX 0x01
#define CHIP_54XX 0x08

/* ---- the 06xx ---------------------------------------------------------------------------- */

static void select_lines(Sim *s, int state)
{
    s->select = state;
    mcu_set_irq(&s->mcu[0], state && (s->ctrl & CHIP_51XX));
    mcu_set_irq(&s->mcu[1], state && (s->ctrl & CHIP_54XX));
}

static void tick06(Sim *s)
{
    s->tick_edge += s->half_edges;
    if (!s->select) {
        select_lines(s, 1);
        if (!(s->ctrl & 0x10) || !s->first_tick) s->cpu[SIM_MAIN].nmi = 1;
        s->first_tick = 0;
    } else {
        select_lines(s, 0);
    }
    timer_set(s, T_06XX, clk06_edge(s->tick_edge), 0, E_06XX, 0);
}

static void ctrl_sync(Sim *s, uint8_t v)
{
    s->ctrl = v;
    select_lines(s, 0);
    if (!(v & 0x0F)) {
        s->timers[T_06XX].active = 0;
        return;
    }
    /* the clock starts on its next edge */
    Atime t = now(s);
    int64_t edge = (t.sec * CLK06_HZ) + (int64_t)((double)t.as * CLK06_HZ / AS_PER_SECOND);
    while (!at_lt(t, clk06_edge(edge))) edge++;
    int n = v >> 5;
    s->half_edges = n ? (int64_t)1 << (n - 1) : 1;
    s->tick_edge = edge;
    s->first_tick = 1;
    timer_set(s, T_06XX, clk06_edge(edge), 0, E_06XX, 0);
}

static void data_sync(Sim *s, uint8_t v)
{
    if (s->ctrl & 0x10) return; /* read mode */
    if (s->ctrl & CHIP_51XX) s->latch51 = v;
    if (s->ctrl & CHIP_54XX) s->latch54 = v;
}

/* A read: the selected chips' O ports on the bus, which their latches keep */
static uint8_t data_read(Sim *s)
{
    uint8_t v = 0xFF;
    if (!(s->ctrl & 0x10)) return v;
    if (s->ctrl & CHIP_51XX) v &= s->mcu[0].o;
    if (s->ctrl & CHIP_54XX) v &= s->mcu[1].o;
    if (s->ctrl & CHIP_51XX) s->latch51 = v;
    if (s->ctrl & CHIP_54XX) s->latch54 = v;
    return v;
}

/* ---- the sound ---------------------------------------------------------------------------- */

#define SOUND_O54 0x20

/* a write to the WSG or the 54xx's O port, at its sample in the frame */
static void sound_event(Sim *s, Atime at, int reg, uint8_t value)
{
    if (!s->sound || s->sound_event_count == (int)(sizeof s->sound_events / sizeof s->sound_events[0])) return;
    int64_t n = at_diff(at, s->frame_start) / (64 * Z80_APC);
    s->sound_events[s->sound_event_count].sample =
        (int16_t)(n < 0 ? 0 : n >= SIM_AUDIO_SAMPLES ? SIM_AUDIO_SAMPLES - 1 : n);
    s->sound_events[s->sound_event_count].reg = (uint8_t)reg;
    s->sound_events[s->sound_event_count].value = value;
    s->sound_event_count++;
}

/* the frame's sound, from the writes' times in it */
static void make_sound(Sim *s)
{
    if (!s->sound) return;
    static int32_t mix[SIM_AUDIO_SAMPLES];
    memset(mix, 0, sizeof mix);
    int done = 0;
    for (int i = 0; i <= s->sound_event_count; i++) {
        int at = i < s->sound_event_count ? s->sound_events[i].sample : SIM_AUDIO_SAMPLES;
        if (at > done) {
            wsg_render(&s->wsg, mix + done, at - done);
            noise54_render(&s->noise, mix + done, at - done);
            done = at;
        }
        if (i == s->sound_event_count) break;
        if (s->sound_events[i].reg == SOUND_O54) noise54_write(&s->noise, s->sound_events[i].value);
        else wsg_write(&s->wsg, s->sound_events[i].reg, s->sound_events[i].value);
    }
    s->sound_event_count = 0;
    for (int i = 0; i < SIM_AUDIO_SAMPLES; i++)
        s->audio[i] = (int16_t)(mix[i] > 32767 ? 32767 : mix[i] < -32768 ? -32768 : mix[i]);
}

/* the 54xx's O port, at its OUTO's start (one cycle) */
static void m54_o(void *ctx, uint8_t v)
{
    Sim *s = ctx;
    sound_event(s, at_mul_add(s->local[SIM_54XX], s->mcu[1].t - 1 - s->done[SIM_54XX], MCU_APC), SOUND_O54, v);
}

/* the chips' ports: the 51xx's K the mode and the byte's low bits, its R
   the control panel (IN0 low and high, IN1 low and high); the 54xx's K the
   byte's high nibble, its R0 the low */
static uint8_t m51_k(void *ctx)
{
    Sim *s = ctx;
    return (uint8_t)((s->ctrl & 0x10 ? 8 : 0) | (s->latch51 & 7));
}

static uint8_t m51_r(void *ctx, int port)
{
    Sim *s = ctx;
    uint8_t in = port < 2 ? s->g.in0 : s->g.in1;
    return (uint8_t)(port & 1 ? in >> 4 : in & 15);
}

static uint8_t m54_k(void *ctx)
{
    Sim *s = ctx;
    return s->latch54 >> 4;
}

static uint8_t m54_r(void *ctx, int port)
{
    Sim *s = ctx;
    return port == 0 ? (s->latch54 & 15) : s->mcu[1].r[port];
}

/* ---- the latches --------------------------------------------------------------------- */

static void irq_lines(Sim *s)
{
    s->cpu[SIM_MAIN].irq_line = (uint8_t)s->irq_main;
    s->cpu[SIM_SUB].irq_line = (uint8_t)s->irq_sub;
}

static void reset_cpu(Cpu *z)
{
    Game *g = z->g;
    uint16_t rom = z->rom;
    memset(z, 0, sizeof *z);
    z->g = g;
    z->rom = rom;
    /* the registers as the board's Z80 (MAME's) has them: F $40, the index
       registers $FFFF, the others 0 */
    z->f = 0x40;
    z->ix = z->iy = 0xFFFF;
    z->stop_pc = 0xFFFF;
    z->stop_sp = 0xFFFF;
}

static void reset_lines(Sim *s)
{
    int run = s->g.misc[3];
    for (int c = SIM_SUB; c < SIM_CPUS; c++) {
        if (!run && !s->held[c]) {
            s->held[c] = 1;
        } else if (run && s->held[c]) {
            /* a CPU in reset keeps no time: it starts now */
            s->held[c] = 0;
            if (c < SIM_Z80S) reset_cpu(&s->cpu[c]);
            else {
                Mcu *m = &s->mcu[c - SIM_Z80S];
                m->t = 0;
                mcu_reset(m);
            }
            s->local[c] = s->base;
            s->done[c] = 0;
        }
    }
}

/* the writes with an effect beyond the memory (game_wr's bus_write) */
static void bus_write(void *ctx, uint16_t a, uint8_t v)
{
    Sim *s = ctx;
    if (a <= 0x681F) {
        sound_event(s, now(s), a & 0x1F, v & 15);
    } else if (a >= 0x6820 && a <= 0x6827) {
        int n = a & 7;
        if (n == 0 && !(v & 1)) {
            s->irq_main = 0;
            sync_now(s, E_IRQ_LINES, 0);
        } else if (n == 1 && !(v & 1)) {
            s->irq_sub = 0;
            sync_now(s, E_IRQ_LINES, 0);
        } else if (n == 2 && (v & 1)) {
            s->cpu[SIM_SUB2].nmi = 0;
        } else if (n == 3 && (v & 1) != s->misc3) {
            sync_now(s, E_RESET_LINES, 0);
        }
        if (n == 3) s->misc3 = v & 1;
    } else if (a == 0x6830) {
        s->watchdog = 0;
    } else if (a >= 0x7000 && a <= 0x70FF) {
        sync_now(s, E_DATA, v);
    } else if (a == 0x7100) {
        sync_now(s, E_CTRL, v);
    }
}

static uint8_t bus_read(void *ctx, uint16_t a)
{
    Sim *s = ctx;
    if (a >= 0x7000 && a <= 0x70FF) return data_read(s);
    if (a == 0x7100) return s->ctrl;
    return 0;
}

/* ---- the screen's events ---------------------------------------------------------------- */

/* The watchdog: the board starts again (RAM keeps its contents), main from
   the vertical blank, the others held in reset by the cleared latch */
static void watchdog_reset(Sim *s)
{
    memset(s->g.misc, 0, sizeof s->g.misc);
    memset(s->g.vlatch, 0, sizeof s->g.vlatch);
    s->misc3 = 0;
    s->irq_main = s->irq_sub = 0;
    s->ctrl = 0;
    s->timers[T_06XX].active = 0;
    select_lines(s, 0);
    reset_cpu(&s->cpu[SIM_MAIN]);
    s->local[SIM_MAIN] = s->base;
    s->done[SIM_MAIN] = 0;
    irq_lines(s);
    reset_lines(s);
    s->watchdog = 0;
}

static void fire(Sim *s, const SimTimer *t)
{
    switch (t->what) {
    case E_VBLANK:
        make_sound(s);
        s->frame_start = t->at;
        s->frame_done = 1;
        s->star_origin = galaga_star_advance(s->star_origin, s->g.vlatch);
        if (++s->watchdog >= WATCHDOG_FRAMES) watchdog_reset(s);
        if (s->g.misc[0]) s->irq_main = 1;
        if (s->g.misc[1]) s->irq_sub = 1;
        sync_now(s, E_IRQ_LINES, 0);
        /* the 51xx's timer counts frames */
        mcu_set_tc(&s->mcu[0], 1);
        mcu_set_tc(&s->mcu[0], 0);
        timer_set(s, T_VBLANK, at_add_as(s->timers[T_VBLANK].at, FRAME_AS), 0, E_VBLANK, 0);
        break;
    case E_LINE0: timer_set(s, T_LINE0, at_add_as(s->timers[T_LINE0].at, FRAME_AS), 0, E_LINE0, 0); break;
    case E_SUB2_NMI: {
        if (!s->g.misc[2]) s->cpu[SIM_SUB2].nmi = 1;
        int next = t->param == 64 ? 192 : 64;
        timer_set(s, T_SUB2_NMI, at_add_as(s->timers[T_SUB2_NMI].at, (int64_t)(next == 192 ? 128 : 136) * LINE_AS), 0,
                  E_SUB2_NMI, next);
        break;
    }
    case E_SOUND: break;
    case E_06XX: tick06(s); break;
    case E_IRQ_LINES: irq_lines(s); break;
    case E_RESET_LINES: reset_lines(s); break;
    case E_CTRL: ctrl_sync(s, (uint8_t)t->param); break;
    case E_DATA: data_sync(s, (uint8_t)t->param); break;
    default: break;
    }
}

/* Timers due by the base time, in order (equal times: in the order set) */
static void run_timers(Sim *s)
{
    for (;;) {
        int due = -1;
        for (int i = 0; i < SIM_TIMERS; i++) {
            const SimTimer *t = &s->timers[i];
            if (!t->active || at_lt(s->base, t->at)) continue;
            if (due < 0 || at_lt(t->at, s->timers[due].at) ||
                (!at_lt(s->timers[due].at, t->at) && t->seq < s->timers[due].seq))
                due = i;
        }
        if (due < 0) return;
        SimTimer *t = &s->timers[due];
        SimTimer fired = *t;
        if (t->period) {
            t->at = at_add_as(t->at, t->period);
            t->seq = ++s->seq;
        } else {
            t->active = 0;
        }
        s->in_event = 1;
        s->event_time = fired.at;
        fire(s, &fired);
        s->in_event = 0;
    }
}

/* ---- the CPUs ---------------------------------------------------------------------------- */

static void fetch(Cpu *z, int64_t n) { z->r = (uint8_t)((z->r & 0x80) | ((z->r + n) & 0x7F)); }

/* An interrupt (1 the NMI, 2 the IRQ, IM 1), or the one it stopped inside
   taken on: its pushes at the machine cycles of the board's, where its
   slice can end as inside an instruction (cpu_access) */
static void take(Cpu *z, int kind)
{
    static const uint8_t offs[2][2] = { { 5, 8 }, { 7, 10 } };
    if (z->stopped) { /* run again: its accesses so far replayed */
        z->stopped = 0;
        z->rep_now = z->rep_n;
        z->rep_n = 0;
    } else {
        z->rep_now = z->rep_n = 0;
        z->part_pc = z->pc;
        z->t_start = z->t;
        memcpy(z->saved, z, offsetof(Cpu, regs_end));
    }
    z->in_int = (uint8_t)kind;
    z->partial = 1;
    z->acc = 0;
    z->offs = offs[kind - 1];
    fetch(z, 1);
    if (kind == 1) z->iff = 0;
    else z->iff = z->iff2 = 0;
    cpu_push(z, z->pc);
    z->in_int = 0;
    z->partial = 0;
    z->halted = 0;
    z->pc = z->wz = kind == 1 ? 0x0066 : 0x0038;
    z->t = z->t_start + (kind == 1 ? 11 : 13);
}

/* the interrupt a Z80 takes between instructions: an NMI, or its IRQ (not
   in the instruction after an EI) */
static void interrupts(Cpu *z)
{
    if (z->nmi) {
        z->nmi = 0;
        take(z, 1);
    } else if (z->irq_line && z->iff && !z->ei_delay) {
        take(z, 2);
    }
}

/* A CPU its `ran` cycles on: the cycles it ran (more when its last
   instruction ends past them; less when it ended the slice; to its stop
   when it stopped inside an instruction) */
static int64_t run(Sim *s, int c, int64_t ran)
{
    Cpu *z = &s->cpu[c];
    jmp_buf jb;
    int64_t from = s->done[c], end = from + ran;
    s->running = c;
    s->aborted = 0;
    z->jb = &jb;
    z->limit = end;
    if (setjmp(jb)) { /* stopped inside an instruction: the rest in its next slice */
        s->running = -1;
        return z->t_stop - from;
    }
    int64_t at = z->stopped ? z->t_stop : z->t; /* where it is: its stop inside an instruction counts */
    while (at < end && !s->aborted) {
        if (z->stopped && z->in_int) take(z, z->in_int);
        else if (!z->stopped) interrupts(z);
        if (z->halted) { /* NOPs, a fetch each, until an interrupt */
            int64_t steps = (end - z->t + 3) / 4;
            z->t += steps * 4;
            fetch(z, steps);
            break;
        }
        if (z->stopped || z->ei_delay) { /* an instruction to finish, or the one after an EI: then the interrupts */
            z->ei_delay = 0;
            z->budget = z->t + 1;
            runs[c](z);
        } else {
            z->budget = end;
            runs[c](z);
        }
        at = z->t;
    }
    s->running = -1;
    return z->t - from;
}

/* A custom its `ran` cycles on, a whole instruction at a time: the cycles
   it ran */
static int64_t run_mcu(Sim *s, int c, int64_t ran)
{
    Mcu *m = &s->mcu[c - SIM_Z80S];
    int64_t from = m->t;
    s->running = c;
    s->aborted = 0;
    m->budget = from + ran;
    mcu_runs[c - SIM_Z80S](m);
    s->running = -1;
    return m->t - from;
}

/* One of MAME's timeslices: every CPU runs to the target in turn; one that
   ends short brings the target back to its time */
static void timeslice(Sim *s)
{
    Atime due = next_due(s);
    while (at_lt(s->base, due)) {
        Atime target = at_add_as(s->base, QUANTUM_AS);
        if (at_lt(due, target)) target = due;
        s->target = target;
        for (int c = 0; c < SIM_CPUS; c++) {
            if (s->held[c]) continue;
            if (s->target.sec < s->local[c].sec) continue;
            int64_t delta = at_diff(s->target, s->local[c]);
            if (delta < apc[c]) continue;
            /* MAME divides by the cycle's attoseconds cut to 32 bits */
            int shift = 0;
            while ((apc[c] >> shift) >= ((int64_t)1 << 32)) shift++;
            int64_t ran = (delta >> shift) / (apc[c] >> shift);
            ran = c < SIM_Z80S ? run(s, c, ran) : run_mcu(s, c, ran);
            s->done[c] += ran;
            s->local[c] = at_mul_add(s->local[c], ran, apc[c]);
            if (at_lt(s->local[c], s->target)) s->target = at_max(s->local[c], s->base);
        }
        s->base = s->target;
        due = next_due(s);
    }
    run_timers(s);
}

void sim_init(Sim *s, const uint8_t *data, uint8_t dswa, uint8_t dswb)
{
    memset(s, 0, sizeof *s);
    Game *g = &s->g;
    g->data = data;
    g->dswa = dswa;
    g->dswb = dswb;
    g->in0 = g->in1 = 0xFF;
    g->bus_write = bus_write;
    g->bus_read = bus_read;
    g->bus = s;
    static const uint16_t roms[SIM_Z80S] = { 0, GAME_SUB, GAME_SUB2 };
    for (int c = 0; c < SIM_Z80S; c++) {
        s->cpu[c].g = g;
        s->cpu[c].rom = roms[c];
        reset_cpu(&s->cpu[c]);
    }
    for (int c = SIM_SUB; c < SIM_CPUS; c++) s->held[c] = 1;
    for (int i = 0; i < 2; i++) {
        s->mcu[i].ctx = s;
        mcu_reset(&s->mcu[i]);
    }
    s->mcu[0].read_k = m51_k;
    s->mcu[0].read_r = m51_r;
    s->mcu[1].read_k = m54_k;
    s->mcu[1].read_r = m54_r;
    s->mcu[1].write_o = m54_o;
    s->running = -1;
    s->star_origin = STAR_ORIGIN_POWER_ON;
    /* the screen starts at the vertical blank */
    timer_set(s, T_VBLANK, (Atime){ 0, FRAME_AS }, 0, E_VBLANK, 0);
    timer_set(s, T_LINE0, (Atime){ 0, 40 * LINE_AS }, 0, E_LINE0, 0);
    timer_set(s, T_SUB2_NMI, (Atime){ 0, 104 * LINE_AS }, 0, E_SUB2_NMI, 64);
    timer_set(s, T_SOUND, (Atime){ 0, SOUND_TIMER_AS }, SOUND_TIMER_AS, E_SOUND, 0);
}

void sim_sound(Sim *s, const uint8_t *waves)
{
    wsg_init(&s->wsg, waves);
    noise54_reset(&s->noise);
    s->sound = 1;
}

void sim_frame(Sim *s, uint8_t in0, uint8_t in1)
{
    s->g.in0 = in0;
    s->g.in1 = in1;
    s->frame_done = 0;
    while (!s->frame_done) timeslice(s);
    s->frame++;
}
