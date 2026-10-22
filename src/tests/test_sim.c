/*
 * The C game's routines against the ROMs' on the emulated board (label rom):
 *   test_sim <galaga.zip|folder>
 * Plays a scripted run on the board (the attract mode, coins, a one- and a
 * two-player game) and checks every ported routine each time its CPU runs
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
 * unported code are counted apart. The ROMs' bytes the routines read must
 * all be among the tables the extractor writes.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assets/extract.h"
#include "assets/rom.h"
#include "emu/board.h"
#include "sim/game.h"
#include "sim/sound.h"
#include "sim/sub.h"

#define FRAMES 30000

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
};
#define ROUTINES ((int)(sizeof routines / sizeof routines[0]))

/* the handshakes: an address a CPU waits on for another to change it */
static const struct { int cpu; uint16_t addr; } syncs[] = { { CPU_SUB, 0x92D6 } };
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
    Game model;                 /* the snapshot, then the C routine's result */
    uint8_t written[0x10000];   /* the CPU wrote the address (1), and its value */
    uint8_t value[0x10000];
    int interrupted, raced;
    int sync_seen[SYNCS];       /* a handshake read, and its last value */
    uint8_t sync_value[SYNCS];
    uint8_t snapshot[0x10000]; /* the memory at entry, by address */
} Check;

#define DEPTH 6
static Check checks[3][DEPTH]; /* each CPU's checks under way, outermost first */
static int depth[3];
static int verbose; /* every difference of the first failures (TEST_SIM_VERBOSE) */

static uint8_t board_value(uint16_t a)
{
    if (a >= 0x6800 && a <= 0x681F) return wsg[a & 0x1F];
    if (a >= 0x6820 && a <= 0x6827) return board.misc[a & 7];
    if (a >= 0xA000 && a <= 0xA007) return board.vlatch[a & 7];
    return board_peek(&board, a);
}

/* the addresses a check compares: memory, latches, the WSG */
static int compared(uint32_t a)
{
    return (a >= 0x6800 && a <= 0x6827) || (a >= 0x8000 && a <= 0x87FF) || (a >= 0x8800 && a <= 0x8BFF) ||
           (a >= 0x9000 && a <= 0x93FF) || (a >= 0x9800 && a <= 0x9BFF) || (a >= 0xA000 && a <= 0xA007);
}

/* the compared addresses, listed once */
static uint16_t addrs[0x1800];
static int naddrs;

static void list_addrs(void)
{
    for (uint32_t a = 0; a < 0x10000; a++)
        if (compared(a)) addrs[naddrs++] = (uint16_t)a;
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
    load_model(&k->model);
    for (int i = 0; i < naddrs; i++) k->snapshot[addrs[i]] = board_value(addrs[i]);
    for (int i = 0; i < naddrs; i++) k->written[addrs[i]] = 0;
    k->interrupted = k->raced = 0;
    memset(k->sync_seen, 0, sizeof k->sync_seen);
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
    for (int i = 0; i < naddrs; i++) {
        uint32_t a = addrs[i];
        if (a < k->sp && a >= (uint32_t)(k->sp - 0x40)) continue; /* the CPU's stack */
        if (!k->written[a] && sync_index(c, (uint16_t)a) >= 0) continue; /* another CPU's handshake */
        uint8_t want = k->written[a] ? k->value[a] : k->snapshot[a];
        uint8_t got = model_value(&k->model, (uint16_t)a);
        if (a >= 0x6800 && a <= 0x681F) want &= 15;
        if ((a >= 0x6820 && a <= 0x6827) || (a >= 0xA000 && a <= 0xA007)) want &= 1;
        if (got != want) {
            if ((!bad || verbose) && r->failed < 5)
                fprintf(stderr, "%s ($%04X) at frame %lld: $%04X is %02X, the ROM's %02X (was %02X)\n", r->name,
                        r->addr, (long long)board.frame, (unsigned)a, got, want, k->snapshot[a]);
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

static void step_hook(Board *b, int c)
{
    if (c >= CPU_51XX) return;
    const Z80 *z = &b->z80[c];
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
    for (int i = 0; i < depth[c]; i++) {
        checks[c][i].written[a] = 1;
        checks[c][i].value[a] = v;
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
        if (!k->written[a] && v != k->snapshot[a]) k->raced = 1;
    }
}

/* ---- the run ------------------------------------------------------------------ */

typedef struct {
    long frame;
    uint8_t in0, in1;
} Step;
static Step steps[8192];
static int nsteps;

static void script(void)
{
    static const char *const moves[10] = { "left,fire", "right", "fire", "left", "right,fire",
                                           "fire", "left", "right", "fire,left", "fire,right" };
    struct { long f; const char *w; } fixed[] = { { 1050, "coin" }, { 1060, "" }, { 1150, "start" }, { 1160, "" } };
    for (int i = 0; i < 4; i++) {
        steps[nsteps].frame = fixed[i].f;
        steps[nsteps].in0 = 0xFF;
        steps[nsteps].in1 = (uint8_t)~(!strcmp(fixed[i].w, "coin") ? IN1_COIN1 : !strcmp(fixed[i].w, "start") ? IN1_START1 : 0);
        nsteps++;
    }
    long f = 1400;
    for (int k = 0; f < FRAMES && nsteps < 8190; k++) {
        const char *m = moves[k % 10];
        long hold = 15 + (k * 7) % 25;
        uint8_t in0 = 0xFF, in1 = 0xFF;
        if (strstr(m, "left")) in0 &= (uint8_t)~IN0_LEFT1;
        if (strstr(m, "right")) in0 &= (uint8_t)~IN0_RIGHT1;
        if (strstr(m, "fire")) in1 &= (uint8_t)~IN1_FIRE1;
        if (f >= 15000 && f < 15100) { /* two coins and a two-player start */
            in1 = (uint8_t)~(f < 15010 || (f >= 15030 && f < 15040) ? IN1_COIN1 : 0);
            in0 = 0xFF;
            if (f >= 15060) in1 = (uint8_t)~IN1_START2;
        }
        steps[nsteps++] = (Step){ f, in0, in1 };
        steps[nsteps++] = (Step){ f + hold, 0xFF, 0xFF };
        f += hold + 5;
    }
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
    script();
    list_addrs();
    board_init(&board, &rom);
    board.step_hook = step_hook;
    board.write_hook = write_hook;
    board.read_hook = read_hook;
    while (board.frame < FRAMES) {
        board_run_frame(&board);
        for (int i = 0; i < nsteps; i++)
            if (steps[i].frame == board.frame - 1) {
                board.in0 = steps[i].in0;
                board.in1 = steps[i].in1;
            }
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
    /* the game reads them from tables.txt: none outside it */
    for (unsigned a = 0; a < GAME_DATA_SIZE; a++)
        if (data_read[a] && !extract_is_table(a)) {
            fprintf(stderr, "the routines read $%04X (data), not among the extracted tables\n", a);
            failed = 1;
            break;
        }
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
