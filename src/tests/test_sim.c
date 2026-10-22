/*
 * The C game's routines against the ROMs' on the emulated board (label rom):
 *   test_sim <galaga.zip|folder>
 * Plays a run on the board (an attract cycle, then one- and two-player
 * games played by a simple player) and checks every ported routine each time its CPU runs
 * it: when the CPU enters the routine (by a CALL or RST, or an interrupt
 * for a handler), a model loaded from the board's memory, latches and the
 * WSG's registers runs the C routine with the CPU's registers; when the
 * ROM's routine returns, every address that CPU wrote must hold in the
 * model what the CPU last wrote there, every other address what it held
 * at entry (bar the CPU's stack below the entry SP), and the routine's
 * output registers must be the CPU's. Routines called inside others are
 * checked too. Checks
 * are skipped where an interrupt broke into the routine, or where the CPU
 * read something another CPU had changed since the entry (the model reads
 * the old value), bar the handshakes a CPU waits on (the model reads the
 * value the CPU left its wait on); calls a C routine reports reaching
 * unported code are counted apart. For the RE work:
 * TEST_SIM_VERBOSE (all differences, the player's progress),
 * TEST_SIM_SCRIPT=<file> (the inputs as a probe's script, up to the first
 * power-on), TEST_SIM_COVER=<dir> (instructions executed by PC),
 * TEST_SIM_DATA (the ROM ranges read).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assets/rom.h"
#include "emu/board.h"
#include "sim/game.h"
#include "sim/irq.h"
#include "sim/sound.h"
#include "sim/sub.h"
#include "sim/tasks.h"

#define FRAMES 40000
#define ATTRACT 6200 /* one attract cycle, the demo included */

enum { OUT_A = 1, OUT_B = 2, OUT_C = 4, OUT_D = 8, OUT_E = 16, OUT_H = 32, OUT_L = 64 };

typedef void (*Adapter)(Game *g, Regs *r);
typedef struct {
    int cpu;
    uint16_t addr;
    const char *name;
    Adapter fn;
    unsigned outputs;
    int interrupt; /* entered by an interrupt (a handler) */
    long checked, interrupted, raced, unported, failed;
} Routine;

/* ---- the routines, through their registers ----------------------------------- */

static void a_sound_nmi(Game *g, Regs *r) { sound_nmi(g); }
static void a_sound_work(Game *g, Regs *r) { sound_work(g); }
static void a_sub_irq(Game *g, Regs *r) { sub_irq(g); }
static void a_sub_sprites(Game *g, Regs *r) { sub_sprites(g); }
static void a_sub_motion(Game *g, Regs *r) { sub_motion(g, r->d, r->e); }
static void a_sub_shots(Game *g, Regs *r) { sub_shots(g); }
static void a_sub_ship(Game *g, Regs *r) { sub_ship(g); }
static void a_sub_debug(Game *g, Regs *r) { sub_debug(g); }
static void a_irq_main(Game *g, Regs *r) { irq_main(g); }
static void a_task_sprites(Game *g, Regs *r) { task_sprites(g); }
static void a_task_difficulty(Game *g, Regs *r) { task_difficulty(g); }
static void a_task_blink(Game *g, Regs *r) { task_blink(g); }
static void a_task_credits(Game *g, Regs *r) { task_credits(g); }
static void a_task_17B2(Game *g, Regs *r) { task_17B2(g); }
static void a_task_1700(Game *g, Regs *r) { task_1700(g); }
static void a_task_1A86(Game *g, Regs *r) { task_1A86(g); }
static void a_task_2916(Game *g, Regs *r) { task_2916(g); }
static void a_task_1DEC(Game *g, Regs *r) { task_1DEC(g); }
static void a_task_2A90(Game *g, Regs *r) { task_2A90(g); }
static void a_task_1DB9(Game *g, Regs *r) { task_1DB9(g); }
static void a_task_23DD(Game *g, Regs *r) { task_23DD(g); }
static void a_task_1EAA(Game *g, Regs *r) { task_1EAA(g); }
static void a_task_1D38(Game *g, Regs *r) { task_1D38(g); }
static void a_task_1B6B(Game *g, Regs *r) { task_1B6B(g); }
static void a_task_19B2(Game *g, Regs *r) { task_19B2(g); }
static void a_task_1D7C(Game *g, Regs *r) { task_1D7C(g); }
static void a_task_1F8F(Game *g, Regs *r) { task_1F8F(g); }
static void a_task_1F0E(Game *g, Regs *r) { task_1F0E(g); }
static void a_task_1DD8(Game *g, Regs *r) { task_1DD8(g); }
static void a_task_2222(Game *g, Regs *r) { task_2222(g); }
static void a_task_21CB(Game *g, Regs *r) { task_21CB(g); }
static void a_task_20F2(Game *g, Regs *r) { task_20F2(g); }
static void a_task_2000(Game *g, Regs *r) { task_2000(g); }

#define R(cpu, addr, name, fn, outputs, interrupt) { cpu, addr, name, fn, outputs, interrupt, 0, 0, 0, 0, 0 }
static Routine routines[] = {
    R(CPU_SUB2, 0x0066, "sound NMI", a_sound_nmi, 0, 1),
    R(CPU_SUB2, 0x00BB, "sound", a_sound_work, 0, 0),
    R(CPU_SUB, 0x0038, "sub IRQ", a_sub_irq, 0, 1),
    R(CPU_SUB, 0x05BF, "sprite banks", a_sub_sprites, 0, 0),
    R(CPU_SUB, 0x08D3, "motion", a_sub_motion, 0, 0),
    R(CPU_SUB, 0x06F5, "shots", a_sub_shots, 0, 0),
    R(CPU_SUB, 0x05EE, "ship hits", a_sub_ship, 0, 0),
    R(CPU_SUB, 0x0ECA, "debug switch", a_sub_debug, 0, 0),
    R(CPU_MAIN, 0x0038, "main IRQ", a_irq_main, 0, 1),
    R(CPU_MAIN, 0x0828, "sprite banks (main)", a_task_sprites, 0, 0),
    R(CPU_MAIN, 0x0857, "difficulty", a_task_difficulty, 0, 0),
    R(CPU_MAIN, 0x0935, "1UP blink", a_task_blink, 0, 0),
    R(CPU_MAIN, 0x0977, "credits", a_task_credits, 0, 0),
    R(CPU_MAIN, 0x17B2, "task $17B2", a_task_17B2, 0, 0),
    R(CPU_MAIN, 0x1700, "task $1700", a_task_1700, 0, 0),
    R(CPU_MAIN, 0x1A86, "task $1A86", a_task_1A86, 0, 0),
    R(CPU_MAIN, 0x2916, "task $2916", a_task_2916, 0, 0),
    R(CPU_MAIN, 0x1DEC, "task $1DEC", a_task_1DEC, 0, 0),
    R(CPU_MAIN, 0x2A90, "task $2A90", a_task_2A90, 0, 0),
    R(CPU_MAIN, 0x1DB9, "task $1DB9", a_task_1DB9, 0, 0),
    R(CPU_MAIN, 0x23DD, "task $23DD", a_task_23DD, 0, 0),
    R(CPU_MAIN, 0x1EAA, "task $1EAA", a_task_1EAA, 0, 0),
    R(CPU_MAIN, 0x1D38, "task $1D38", a_task_1D38, 0, 0),
    R(CPU_MAIN, 0x1B6B, "task $1B6B", a_task_1B6B, 0, 0),
    R(CPU_MAIN, 0x19B2, "task $19B2", a_task_19B2, 0, 0),
    R(CPU_MAIN, 0x1D7C, "task $1D7C", a_task_1D7C, 0, 0),
    R(CPU_MAIN, 0x1F8F, "task $1F8F", a_task_1F8F, 0, 0),
    R(CPU_MAIN, 0x1F0E, "task $1F0E", a_task_1F0E, 0, 0),
    R(CPU_MAIN, 0x1DD8, "task $1DD8", a_task_1DD8, 0, 0),
    R(CPU_MAIN, 0x2222, "task $2222", a_task_2222, 0, 0),
    R(CPU_MAIN, 0x21CB, "task $21CB", a_task_21CB, 0, 0),
    R(CPU_MAIN, 0x20F2, "task $20F2", a_task_20F2, 0, 0),
    R(CPU_MAIN, 0x2000, "task $2000", a_task_2000, 0, 0),
};
#define ROUTINES ((int)(sizeof routines / sizeof routines[0]))

/* the handshakes: an address a CPU waits on for another to change it */
static const struct { int cpu; uint16_t addr; } syncs[] = {
    { CPU_SUB, 0x92D6 }, { CPU_MAIN, 0x92D7 }, { CPU_MAIN, 0x9287 }, { CPU_MAIN, 0x92AE },
};
#define SYNCS ((int)(sizeof syncs / sizeof syncs[0]))

static int sync_index(int c, uint16_t a)
{
    for (int i = 0; i < SYNCS; i++)
        if (syncs[i].cpu == c && syncs[i].addr == a) return i;
    return -1;
}

/* ---- the board and the model ------------------------------------------------- */

static RomSet rom;
static Board board;
static uint8_t data[GAME_DATA_SIZE];
static uint8_t data_read[GAME_DATA_SIZE]; /* the ROM bytes the C routines read */
static uint8_t wsg[0x20];       /* the WSG's registers as written so far */

typedef struct {
    Routine *r;
    uint16_t sp, ret;
    Regs regs;
    Game model;                 /* the memory at entry, then the C routine's result */
    uint32_t gen;               /* this check's mark in wgen */
    uint32_t wgen[0x10000];     /* the CPU wrote the address (gen), its value, the addresses in order */
    uint8_t value[0x10000];
    uint16_t wlist[0x2000];
    int nw;
    int interrupted, raced;
    uint8_t rval[16];           /* the values main's LD A,R read, in order */
    int nr, ir;
    int sync_seen[SYNCS];       /* a handshake read, and its last value */
    uint8_t sync_value[SYNCS];
} Check;

#define WRITTEN(k, a) ((k)->wgen[a] == (k)->gen)

/* the C routine's writes: the addresses, the values before the first */
static uint32_t mgen = 1, mmark[0x10000];
static uint16_t mlist[0x2000];
static uint8_t mold[0x2000];
static int nm;

#define DEPTH 6
static Check checks[3][DEPTH]; /* each CPU's checks under way, outermost first */
static int depth[3];
static int verbose; /* every difference of the first failures (TEST_SIM_VERBOSE) */

/* the addresses a check compares: memory, latches, the WSG */
static int compared(uint32_t a)
{
    return (a >= 0x6800 && a <= 0x6827) || (a >= 0x8000 && a <= 0x87FF) || (a >= 0x8800 && a <= 0x8BFF) ||
           (a >= 0x9000 && a <= 0x93FF) || (a >= 0x9800 && a <= 0x9BFF) || (a >= 0xA000 && a <= 0xA007);
}

static uint8_t model_value(const Game *g, uint16_t a)
{
    if (a >= 0x6800 && a <= 0x681F) return g->wsg[a & 0x1F];
    if (a >= 0x6820 && a <= 0x6827) return g->misc[a & 7];
    if (a >= 0xA000 && a <= 0xA007) return g->vlatch[a & 7];
    return game_rd(g, a);
}

static void load_model(Game *g)
{
    memset(g, 0, sizeof *g);
    memcpy(g->video, board.video, sizeof g->video);
    memcpy(g->ram, board.ram, sizeof g->ram);
    memcpy(g->misc, board.misc, sizeof g->misc);
    memcpy(g->vlatch, board.vlatch, sizeof g->vlatch);
    memcpy(g->wsg, wsg, sizeof g->wsg);
    g->in0 = board.in0;
    g->in1 = board.in1;
    g->dswa = board.dswa;
    g->dswb = board.dswb;
    g->data = data;
    g->data_read = data_read;
}

static Regs cpu_regs(const Z80 *z)
{
    Regs r = { z->a, z->f, z->b, z->c, z->d, z->e, z->h, z->l, (uint16_t)(z->ixh << 8 | z->ixl),
               (uint16_t)(z->iyh << 8 | z->iyl) };
    return r;
}

static void begin(int c, Routine *r)
{
    if (depth[c] == DEPTH) return;
    Check *k = &checks[c][depth[c]++];
    const Z80 *z = &board.z80[c];
    k->r = r;
    k->sp = z->sp;
    k->ret = (uint16_t)(board_peek(&board, z->sp) | board_peek(&board, (uint16_t)(z->sp + 1)) << 8);
    k->regs = cpu_regs(z);
    static uint32_t gens;
    load_model(&k->model);
    k->gen = ++gens;
    k->nw = 0;
    k->interrupted = k->raced = 0;
    memset(k->sync_seen, 0, sizeof k->sync_seen);
    k->nr = k->ir = 0;
}

/* the C routine's writes, before each */
static void model_write(void *user, uint16_t a, uint8_t v)
{
    Check *k = user;
    if (mmark[a] == mgen || !compared(a) || nm == (int)(sizeof mlist / sizeof mlist[0])) return;
    mmark[a] = mgen;
    mlist[nm] = a;
    mold[nm++] = model_value(&k->model, a);
}

/* the model's LD A,R: the board's values, in order */
static uint8_t model_r(void *user)
{
    Check *k = user;
    return k->ir < k->nr ? k->rval[k->ir++] : 0;
}

static void finish(int c)
{
    Check *k = &checks[c][--depth[c]];
    Routine *r = k->r;
    k->r = NULL;
    if (k->interrupted) {
        r->interrupted++;
        return;
    }
    if (k->raced) {
        r->raced++;
        return;
    }
    Regs regs = k->regs;
    for (int i = 0; i < SYNCS; i++)
        if (k->sync_seen[i]) game_wr(&k->model, syncs[i].addr, k->sync_value[i]);
    k->model.r_hook = model_r;
    k->model.user = k;
    k->model.on_write = model_write;
    mgen++;
    nm = 0;
    r->fn(&k->model, &regs);
    if (k->model.unported) {
        r->unported++;
        return;
    }
    r->checked++;
    int bad = 0;
    if (k->model.stalled) {
        if (r->failed < 5)
            fprintf(stderr, "%s ($%04X) at frame %lld: waits on $%04X\n", r->name, r->addr, (long long)board.frame,
                    k->model.stalled);
        bad++;
    }
    /* what either wrote: the CPU's last value, or else what was there */
    for (int i = 0; i < k->nw + nm; i++) {
        uint16_t a = i < k->nw ? k->wlist[i] : mlist[i - k->nw];
        if (i >= k->nw && WRITTEN(k, a)) continue;
        if (a < k->sp && a >= (uint32_t)(k->sp - 0x40)) continue; /* the CPU's stack */
        if (!WRITTEN(k, a) && sync_index(c, a) >= 0) continue; /* another CPU's handshake */
        uint8_t was = i < k->nw ? 0 : mold[i - k->nw];
        uint8_t want = WRITTEN(k, a) ? k->value[a] : was;
        uint8_t got = model_value(&k->model, a);
        if (a >= 0x6800 && a <= 0x681F) want &= 15;
        if ((a >= 0x6820 && a <= 0x6827) || (a >= 0xA000 && a <= 0xA007)) want &= 1;
        if (got != want) {
            if ((!bad || verbose) && r->failed < 5)
                fprintf(stderr, "%s ($%04X) at frame %lld: $%04X is %02X, the ROM's %02X\n", r->name, r->addr,
                        (long long)board.frame, (unsigned)a, got, want);
            bad++;
        }
    }
    const Z80 *z = &board.z80[c];
    static const struct { unsigned bit; const char *name; } outs[] = {
        { OUT_A, "A" }, { OUT_B, "B" }, { OUT_C, "C" }, { OUT_D, "D" }, { OUT_E, "E" }, { OUT_H, "H" }, { OUT_L, "L" },
    };
    uint8_t mine[7] = { regs.a, regs.b, regs.c, regs.d, regs.e, regs.h, regs.l };
    uint8_t cpu[7] = { z->a, z->b, z->c, z->d, z->e, z->h, z->l };
    for (int i = 0; i < 7; i++)
        if ((r->outputs & outs[i].bit) && mine[i] != cpu[i]) {
            if (!bad && r->failed < 5)
                fprintf(stderr, "%s ($%04X) at frame %lld: %s is %02X, the ROM's %02X\n", r->name, r->addr,
                        (long long)board.frame, outs[i].name, mine[i], cpu[i]);
            bad++;
        }
    if (bad) r->failed++;
}

/* Is the CPU entering a routine by a CALL or RST (the return address on the
   stack follows one), or by the interrupt it just took? */
static int called(int c, int interrupt)
{
    const Z80 *z = &board.z80[c];
    if (interrupt) return z->last_interrupt;
    uint16_t ret = (uint16_t)(board_peek(&board, z->sp) | board_peek(&board, (uint16_t)(z->sp + 1)) << 8);
    const uint8_t *prog = c == CPU_MAIN ? rom.main : c == CPU_SUB ? rom.sub : rom.sub2;
    unsigned size = c == CPU_MAIN ? ROM_MAIN_SIZE : ROM_SUB_SIZE;
    if (ret >= 3 && ret <= size) {
        uint8_t op = prog[ret - 3];
        if (op == 0xCD || (op & 0xC7) == 0xC4) return 1; /* CALL, CALL cc */
    }
    if (ret >= 1 && ret <= size && (prog[ret - 1] & 0xC7) == 0xC7) return 1; /* RST */
    return 0;
}

static uint32_t *cover[3]; /* TEST_SIM_COVER: instructions executed by PC, per CPU */

static void step_hook(Board *b, int c)
{
    if (c >= CPU_51XX) return;
    const Z80 *z = &b->z80[c];
    if (cover[c]) cover[c][z->pc]++;
    if (c == CPU_MAIN && (z->pc == 0x1003 || z->pc == 0x100F)) /* after $1000's LD A,R */
        for (int i = 0; i < depth[c]; i++)
            if (checks[c][i].nr < 16) checks[c][i].rval[checks[c][i].nr++] = z->a;
    while (depth[c] && z->pc == checks[c][depth[c] - 1].ret && z->sp == (uint16_t)(checks[c][depth[c] - 1].sp + 2))
        finish(c);
    if (z->nmi_pending || (z->irq_line && z->iff1 && !z->ei_delay))
        for (int i = 0; i < depth[c]; i++) checks[c][i].interrupted = 1;
    for (int i = 0; i < ROUTINES; i++)
        if (routines[i].cpu == c && routines[i].addr == z->pc && called(c, routines[i].interrupt)) {
            begin(c, &routines[i]);
            return;
        }
}

static void write_hook(Board *b, int c, uint16_t a, uint8_t v)
{
    if (a >= 0x6800 && a <= 0x681F) wsg[a & 0x1F] = v & 15;
    if (c >= CPU_51XX) return;
    if (!compared(a)) return;
    for (int i = 0; i < depth[c]; i++) {
        Check *k = &checks[c][i];
        if (!WRITTEN(k, a) && k->nw < (int)(sizeof k->wlist / sizeof k->wlist[0])) {
            k->wgen[a] = k->gen;
            k->wlist[k->nw++] = a;
        }
        k->value[a] = v;
    }
}

/* another CPU's change the checked CPU then reads: the model cannot see it */
static void read_hook(Board *b, int c, uint16_t a, uint8_t v)
{
    if (c >= CPU_51XX || !compared(a) || (a >= 0x6800 && a <= 0x6807)) return; /* the switches */
    int s = sync_index(c, a);
    if (s >= 0) {
        for (int i = 0; i < depth[c]; i++) {
            checks[c][i].sync_seen[s] = 1;
            checks[c][i].sync_value[s] = v;
        }
        return;
    }
    for (int i = 0; i < depth[c]; i++) {
        Check *k = &checks[c][i];
        if (v != (WRITTEN(k, a) ? k->value[a] : model_value(&k->model, a))) k->raced = 1;
    }
}

/* ---- the run ------------------------------------------------------------------ */

/* The player: one attract cycle untouched, then games one after another
   (one player, then two, by turns): coins and a start once the attract
   mode is on, and in a game the ship steered under the nearest enemy,
   firing. Its inputs reach the game a frame later, as a probe's would.
   After each game the board is powered on again: at a game's end the
   board's 51xx can see a select edge an instruction earlier than MAME's
   (plan section 2) and hand main its credit bytes out of order, which
   leaves the game asking for a start that never comes. */
static int games;      /* games started */
static int in_game, power_cycle;
static FILE *script_out;
static long next_coin; /* the frame of the next game's first coin, or 0 */

static void player(void)
{
    long f = board.frame;
    uint8_t in0 = 0xFF, in1 = 0xFF, state = board_peek(&board, 0x9201);
    static uint8_t was;
    if (verbose && state != was) fprintf(stderr, "frame %ld: state %d (stage %d)\n", f, state, board_peek(&board, 0x9821));
    was = state;
    if (state == 3) in_game = 1;
    if (in_game && state == 1) { /* the game is over */
        in_game = 0;
        power_cycle = 1;
        goto out;
    }
    if (!games && f < ATTRACT) goto out;
    if (state == 1 && !next_coin) next_coin = f + 60; /* the attract mode: a game next */
    if (next_coin) {
        int two = games & 1;
        long t = f - next_coin;
        if ((t >= 0 && t < 8) || (two && t >= 30 && t < 38)) in1 &= (uint8_t)~IN1_COIN1;
        if (t >= 100 && t < 108) in1 &= (uint8_t)~(two ? IN1_START2 : IN1_START1);
        if (t >= 108) {
            next_coin = 0;
            games++;
        }
        goto out;
    }
    /* only while the ship is in play: a button held through the game's end
       reads as credits (the 51xx still sends the switches) */
    if (state != 3 || !board_peek(&board, 0x9014)) goto out;
    {
        /* the sprites from $8800 (states: $80 free, 4 and 5 exploding); $60-$67
           the ship and its shots, $68-$77 the bombs. X grows to the right, Y
           (nine bits) down to the ship's row at $129. */
        int ship = board_peek(&board, 0x9362), aim = -1, dist = 256, threat = -1, near = 256;
        for (int l = 0; l < 0x78; l += 2) {
            if (l >= 0x60 && l < 0x68) continue;
            uint8_t st = board_peek(&board, (uint16_t)(0x8800 + l));
            int x = board_peek(&board, (uint16_t)(0x9300 + l));
            int y = (board_peek(&board, (uint16_t)(0x9B01 + l)) & 1) << 8 | board_peek(&board, (uint16_t)(0x9301 + l));
            if (st == 0x80 || (st & 0xFE) == 4 || !x) continue;
            int d = x > ship ? x - ship : ship - x;
            if (y >= 0xF0 && y < 0x138 && d < 0x18 && d < near) { /* about to reach the ship's row */
                near = d;
                threat = x;
            }
            if (l < 0x60 && d < dist) {
                dist = d;
                aim = x;
            }
        }
        if (threat >= 0) { /* away from it, towards the middle at the edges */
            if ((threat >= ship && ship > 0x30) || ship > 0xC0) in0 &= (uint8_t)~IN0_LEFT1;
            else in0 &= (uint8_t)~IN0_RIGHT1;
        } else if (aim >= 0 && aim + 2 < ship) {
            in0 &= (uint8_t)~IN0_LEFT1;
        } else if (aim >= 0 && aim > ship + 2) {
            in0 &= (uint8_t)~IN0_RIGHT1;
        }
        if ((f & 3) < 2) in1 &= (uint8_t)~IN1_FIRE1;
        if (verbose && f % 100 == 0) {
            int alive = 0;
            for (int l = 0; l < 0x60; l += 2) alive += board_peek(&board, (uint16_t)(0x8800 + l)) != 0x80;
            fprintf(stderr, "  %ld: ship %02X aim %d threat %d alive %d hits %d lives %d\n", f, ship, aim, threat, alive,
                    board_peek(&board, 0x9844) | board_peek(&board, 0x9845) << 8, board_peek(&board, 0x9820));
        }
    }
out:
    /* TEST_SIM_SCRIPT: the inputs as a MAME probe's script (frame:inputs) */
    if (script_out && (in0 != board.in0 || in1 != board.in1)) {
        fprintf(script_out, "%ld:", f - 1);
        const char *sep = "";
        if (!(in1 & IN1_COIN1)) fprintf(script_out, "%scoin", sep), sep = ",";
        if (!(in1 & IN1_START1)) fprintf(script_out, "%sstart", sep), sep = ",";
        if (!(in1 & IN1_START2)) fprintf(script_out, "%sstart2", sep), sep = ",";
        if (!(in0 & IN0_LEFT1)) fprintf(script_out, "%sleft", sep), sep = ",";
        if (!(in0 & IN0_RIGHT1)) fprintf(script_out, "%sright", sep), sep = ",";
        if (!(in1 & IN1_FIRE1)) fprintf(script_out, "%sfire", sep);
        fputc(' ', script_out);
    }
    board.in0 = in0;
    board.in1 = in1;
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "usage: test_sim <galaga.zip|folder>\n");
        return 2;
    }
    char err[512];
    if (rom_load(&rom, argv[1], err, sizeof err)) {
        fprintf(stderr, "%s\n", err);
        return 1;
    }
    if (!rom.has_51xx || !rom.has_54xx) {
        fprintf(stderr, "test_sim: the board needs the device ROMs (namco51.zip, namco54.zip) next to %s\n", argv[1]);
        return 1;
    }
    memcpy(data, rom.main, ROM_MAIN_SIZE);
    memcpy(data + GAME_SUB, rom.sub, ROM_SUB_SIZE);
    memcpy(data + GAME_SUB2, rom.sub2, ROM_SUB2_SIZE);
    verbose = getenv("TEST_SIM_VERBOSE") != NULL;
    if (getenv("TEST_SIM_COVER"))
        for (int c = 0; c < 3; c++) cover[c] = calloc(0x10000, sizeof *cover[c]);
    if (getenv("TEST_SIM_SCRIPT")) script_out = fopen(getenv("TEST_SIM_SCRIPT"), "w");
    board_init(&board, &rom);
    board.step_hook = step_hook;
    board.write_hook = write_hook;
    board.read_hook = read_hook;
    for (long frames = 0; frames < FRAMES; frames++) {
        board_run_frame(&board);
        player();
        if (power_cycle) {
            power_cycle = 0;
            board_init(&board, &rom);
            board.step_hook = step_hook;
            board.write_hook = write_hook;
            board.read_hook = read_hook;
            memset(depth, 0, sizeof depth);
            memset(wsg, 0, sizeof wsg);
        }
    }
    /* coverage for the RE listings: "<pc> <count>" lines, <dir>/main.pcs, sub.pcs, sub2.pcs */
    if (cover[0])
        for (int c = 0; c < 3; c++) {
            char path[1024];
            snprintf(path, sizeof path, "%s/%s.pcs", getenv("TEST_SIM_COVER"), c == 0 ? "main" : c == 1 ? "sub" : "sub2");
            FILE *f = fopen(path, "w");
            if (!f) continue;
            for (int a = 0; a < 0x10000; a++)
                if (cover[c][a]) fprintf(f, "%04X %u\n", a, cover[c][a]);
            fclose(f);
        }
    /* the ROMs' bytes the routines read, as ranges (for the extractor's tables) */
    if (getenv("TEST_SIM_DATA"))
        for (unsigned a = 0; a < GAME_DATA_SIZE; a++)
            if (data_read[a] && (!a || !data_read[a - 1])) {
                unsigned e = a;
                while (e + 1 < GAME_DATA_SIZE && data_read[e + 1]) e++;
                printf("read %s $%04X-$%04X\n", a < GAME_SUB ? "main" : a < GAME_SUB2 ? "sub" : "sub2",
                       a < GAME_SUB ? a : a < GAME_SUB2 ? a - GAME_SUB : a - GAME_SUB2,
                       e < GAME_SUB ? e : e < GAME_SUB2 ? e - GAME_SUB : e - GAME_SUB2);
            }
    int failed = 0;
    for (int i = 0; i < ROUTINES; i++) {
        Routine *r = &routines[i];
        printf("%-24s $%04X (%s): %ld checked, %ld failed, %ld interrupted, %ld raced, %ld unported\n", r->name, r->addr,
               r->cpu == CPU_MAIN ? "main" : r->cpu == CPU_SUB ? "sub" : "sub2", r->checked, r->failed, r->interrupted,
               r->raced, r->unported);
        if (r->failed || !r->checked) failed = 1;
    }
    if (failed) return 1;
    puts("sim: every ported routine equals the ROM's");
    return 0;
}
