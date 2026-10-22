/*
 * The emulated board against MAME (label mame):
 *   test_emu <galaga.zip|folder> <script> <capture dir> <trace dir>
 *            <trace from> <trace to> <frame>...
 * Runs the scripted game the probes ran (the same PROBE_SCRIPT string, e.g.
 * "1050:coin 1060: 1150:start 1160:") from power-on and checks it against
 * MAME:
 *   - MAME's instruction traces of frames <trace from>-<trace to>
 *     (trace_probe.lua, <trace dir>/<cpu>.tr for maincpu, sub, sub2, 51xx,
 *     54xx): every instruction at the same cycle with the same registers.
 *     The 51xx is only reported: where MAME's 06xx lets it see its select
 *     line change in read mode is not modelled to the instruction (plan
 *     section 2); what it hands the Z80s is.
 *   - at each given frame, what capture_probe.lua dumped (capture
 *     dir/frame_<n>.txt) and MAME's snapshot (capture dir/<n>.png): the
 *     video RAM, all RAM, the latches, and the picture pixel for pixel,
 *     the stars included.
 *   - the sound, against MAME's recording of the run (capture dir/game.wav,
 *     48 kHz), loosely, as the 54xx's noise is modelled from MAME's output
 *     and MAME smooths the WSG differently: in 1/10 s windows where MAME is
 *     not quiet, at least 90% have the board's level within 3 dB.
 * MAME's frame callback, where the probes dump, runs at the vertical blank,
 * as the board's frames end. Inputs a MAME probe sets in its frame callback
 * reach the game a frame later. DUMP_DIR=dir writes the board's pictures.
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assets/png.h"
#include "assets/rom.h"
#include "emu/board.h"
#include "gfx/galaga.h"

static RomSet rom;
static Board board;
static GalagaVideo video;

/* the probe's script: from each frame on, these switches are held */
typedef struct {
    long frame;
    uint8_t in0, in1; /* the bits pressed */
} Step;
static Step steps[4096];
static int nsteps;

static int parse_script(const char *text)
{
    const char *p = text;
    while (*p) {
        while (*p == ' ') p++;
        if (!*p) break;
        if (nsteps == (int)(sizeof steps / sizeof steps[0])) return -1;
        char *end;
        Step *s = &steps[nsteps++];
        memset(s, 0, sizeof *s);
        s->frame = strtol(p, &end, 10);
        if (end == p || *end != ':') return -1;
        p = end + 1;
        while (*p && *p != ' ') {
            size_t n = strcspn(p, ", ");
            static const struct { const char *name; int port; uint8_t bit; } names[] = {
                { "coin", 1, IN1_COIN1 }, { "start", 1, IN1_START1 }, { "start2", 1, IN1_START2 },
                { "left", 0, IN0_LEFT1 }, { "right", 0, IN0_RIGHT1 }, { "fire", 1, IN1_FIRE1 },
            };
            int ok = 0;
            for (size_t i = 0; i < sizeof names / sizeof names[0]; i++)
                if (strlen(names[i].name) == n && !strncmp(p, names[i].name, n)) {
                    *(names[i].port ? &s->in1 : &s->in0) |= names[i].bit;
                    ok = 1;
                }
            if (!ok) return -1;
            p += n;
            if (*p == ',') p++;
        }
    }
    return 0;
}

static void inputs(Board *b)
{
    long f = (long)b->frame - 1; /* the probe's frame whose inputs apply now */
    for (int i = 0; i < nsteps; i++)
        if (steps[i].frame == f) {
            b->in0 = (uint8_t)~steps[i].in0;
            b->in1 = (uint8_t)~steps[i].in1;
        }
}

/* ---- traces ---------------------------------------------------------------- */

static const char *const cpu_names[BOARD_CPUS] = { "maincpu", "sub", "sub2", "51xx", "54xx" };

typedef struct {
    int64_t cycles;
    uint16_t reg[8]; /* Z80: AF BC DE HL SP IX IY PC; MB88: A X Y flags PA PC */
} Line;

typedef struct {
    Line *l;
    long n, cap;
} Trace;

static Trace mame[BOARD_CPUS], ours[BOARD_CPUS];
static int traced[BOARD_CPUS];
static long trace_from, trace_to;

static void trace_add(Trace *t, const Line *l)
{
    if (t->n == t->cap) {
        t->cap = t->cap ? t->cap * 2 : 1 << 16;
        t->l = realloc(t->l, (size_t)t->cap * sizeof *t->l);
    }
    t->l[t->n++] = *l;
}

/* MAME's trace: "cycles REG... PC: disassembly" */
static int load_trace(Trace *t, const char *path, int mcu)
{
    FILE *f = fopen(path, "r");
    if (!f) return -1;
    static char text[512];
    int nregs = mcu ? 6 : 8;
    while (fgets(text, sizeof text, f)) {
        char *p = text, *end;
        Line l;
        memset(&l, 0, sizeof l);
        l.cycles = strtoll(p, &end, 10);
        if (end == p || *end != ' ') continue;
        p = end;
        int i;
        for (i = 0; i < nregs; i++) {
            unsigned long v = strtoul(p, &end, 16);
            if (end == p) break;
            l.reg[i] = (uint16_t)v;
            p = end;
            if (*p == ':') break;
        }
        if (i < nregs - 1) continue;
        if (mcu) l.reg[3] &= 0x2F; /* SF: the serial unit is not modelled */
        trace_add(t, &l);
    }
    fclose(f);
    return 0;
}

static void step_hook(Board *b, int c)
{
    if (!traced[c] || b->frame < trace_from || b->frame >= trace_to) return;
    Line l;
    memset(&l, 0, sizeof l);
    l.cycles = board_total_cycles(b, c);
    if (c < CPU_51XX) {
        const Z80 *z = &b->z80[c];
        if (z->nmi_pending || (z->irq_line && z->iff1 && !z->ei_delay)) return; /* MAME's trace has no line for it */
        uint16_t r[8] = { (uint16_t)(z->a << 8 | z->f), (uint16_t)(z->b << 8 | z->c), (uint16_t)(z->d << 8 | z->e),
                          (uint16_t)(z->h << 8 | z->l), z->sp, (uint16_t)(z->ixh << 8 | z->ixl),
                          (uint16_t)(z->iyh << 8 | z->iyl), z->pc };
        memcpy(l.reg, r, sizeof r);
    } else {
        const MB88 *m = &b->mcu[c - CPU_51XX];
        uint16_t r[6] = { m->a, m->x, m->y, (uint16_t)(mb88_flags(m) & 0x2F), m->pa, (uint16_t)(m->pa << 6 | m->pc) };
        memcpy(l.reg, r, sizeof r);
    }
    trace_add(&ours[c], &l);
}

/* Both traces from MAME's first line on; 0 if equal. */
static int compare_trace(int c)
{
    Trace *a = &mame[c], *b = &ours[c];
    long j = 0;
    while (j < b->n && b->l[j].cycles < a->l[0].cycles) j++;
    long n = a->n < b->n - j ? a->n : b->n - j;
    if (n < a->n * 9 / 10) {
        printf("%s: our trace covers %ld of MAME's %ld lines\n", cpu_names[c], n, a->n);
        return 1;
    }
    int nregs = c < CPU_51XX ? 8 : 6;
    for (long i = 0; i < n; i++) {
        const Line *x = &a->l[i], *y = &b->l[j + i];
        if (x->cycles != y->cycles || memcmp(x->reg, y->reg, (size_t)nregs * sizeof x->reg[0])) {
            printf("%s: instruction %ld of %ld differs: MAME cycle %lld PC %04X, ours cycle %lld PC %04X\n", cpu_names[c], i, n,
                   (long long)x->cycles, x->reg[nregs - 1], (long long)y->cycles, y->reg[nregs - 1]);
            return 1;
        }
    }
    printf("%s: %ld instructions equal MAME's trace\n", cpu_names[c], n);
    return 0;
}

/* ---- sound ----------------------------------------------------------------- */

static int16_t *ours_audio;
static long ours_n, ours_cap;

static void keep_audio(void)
{
    if (ours_n + BOARD_AUDIO_SAMPLES > ours_cap) {
        ours_cap = ours_cap ? ours_cap * 2 : 1 << 20;
        ours_audio = realloc(ours_audio, (size_t)ours_cap * sizeof *ours_audio);
    }
    memcpy(ours_audio + ours_n, board.audio, sizeof board.audio);
    ours_n += BOARD_AUDIO_SAMPLES;
}

/* a 16-bit mono WAV's samples */
static int16_t *load_wav(const char *path, long *n)
{
    FILE *f = fopen(path, "rb");
    if (!f) return NULL;
    uint8_t h[44];
    if (fread(h, 1, 44, f) != 44 || memcmp(h, "RIFF", 4) || memcmp(h + 8, "WAVE", 4)) {
        fclose(f);
        return NULL;
    }
    int channels = h[22] | h[23] << 8, bits = h[34] | h[35] << 8;
    fseek(f, 0, SEEK_END);
    long bytes = ftell(f) - 44;
    fseek(f, 44, SEEK_SET);
    if (channels != 1 || bits != 16 || bytes <= 0) {
        fclose(f);
        return NULL;
    }
    int16_t *s = malloc((size_t)bytes);
    *n = (long)fread(s, 2, (size_t)bytes / 2, f);
    fclose(f);
    return s;
}

static double rms(const int16_t *s, long n)
{
    double sum = 0;
    for (long i = 0; i < n; i++) sum += (double)s[i] * s[i];
    return sqrt(sum / (double)n);
}

static int compare_sound(const char *path)
{
    long n;
    int16_t *mame_audio = load_wav(path, &n);
    if (!mame_audio) {
        printf("sound: no recording (%s)\n", path);
        return 1;
    }
    const long w = BOARD_AUDIO_RATE / 10;
    long windows = 0, close = 0;
    for (long at = w; at + w <= n && at + w <= ours_n; at += w) { /* the first window: MAME's filters settling */
        double m = rms(mame_audio + at, w), o = rms(ours_audio + at, w);
        if (m < 100) continue;
        windows++;
        double db = 20 * log10((o + 1) / (m + 1));
        if (fabs(db) <= 3) close++;
        else if (windows - close <= 5) printf("  sound at %.1f s: %.1f dB from MAME's level (%.0f, MAME %.0f)\n", (double)at / BOARD_AUDIO_RATE, db, o, m);
    }
    free(mame_audio);
    printf("sound: %ld of %ld windows within 3 dB of MAME's level\n", close, windows);
    return !windows || close * 10 < windows * 9;
}

/* ---- captures -------------------------------------------------------------- */

static int hex(char c)
{
    return c >= '0' && c <= '9' ? c - '0' : c >= 'A' && c <= 'F' ? c - 'A' + 10 : c >= 'a' && c <= 'f' ? c - 'a' + 10 : -1;
}

static int hex_bytes(const char *p, uint8_t *out, int n)
{
    for (int i = 0; i < n; i++) {
        int hi = hex(p[2 * i]), lo = hex(p[2 * i + 1]);
        if (hi < 0 || lo < 0) return -1;
        out[i] = (uint8_t)(hi << 4 | lo);
    }
    return 0;
}

/* frame_<n>.txt against the board; returns the bytes that differ, or -1 */
static long compare_memory(const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f) return -1;
    static char text[8192];
    static uint8_t vram[0x800], ram[3][0x400];
    int have = 0;
    long diff = 0;
    while (fgets(text, sizeof text, f)) {
        if (!strncmp(text, "video ", 6)) have |= !hex_bytes(text + 6, vram, 0x800);
        else if (!strncmp(text, "ram1 ", 5)) have |= !hex_bytes(text + 5, ram[0], 0x400) << 1;
        else if (!strncmp(text, "ram2 ", 5)) have |= !hex_bytes(text + 5, ram[1], 0x400) << 2;
        else if (!strncmp(text, "ram3 ", 5)) have |= !hex_bytes(text + 5, ram[2], 0x400) << 3;
        else if (!strncmp(text, "latch ", 6) && strlen(text) >= 6 + 32) {
            for (int i = 0; i < 16; i++) { /* $6820-$6827, $A000-$A007; "--": never written, 0 */
                int v = hex(text[6 + 2 * i]) < 0 ? 0 : hex(text[6 + 2 * i + 1]) & 1;
                int mine = i < 8 ? board.misc[i] : board.vlatch[i - 8];
                if (v != mine) {
                    if (!diff) printf("  latch $%04X: %d, MAME %d\n", i < 8 ? 0x6820 + i : 0xA000 + i - 8, mine, v);
                    diff++;
                }
            }
            have |= 16;
        }
    }
    fclose(f);
    if (have != 31) return -1;
    for (int i = 0; i < 0x800; i++)
        if (vram[i] != board.video[i] && diff++ < 4) printf("  $%04X: %02X, MAME %02X\n", 0x8000 + i, board.video[i], vram[i]);
    for (int k = 0; k < 3; k++)
        for (int i = 0; i < 0x400; i++)
            if (ram[k][i] != board.ram[k][i] && diff++ < 4)
                printf("  $%04X: %02X, MAME %02X\n", 0x8800 + k * 0x800 + i, board.ram[k][i], ram[k][i]);
    return diff;
}

/* the board's picture against MAME's snapshot; returns the pixels that differ, or -1 */
static long compare_picture(const char *path, long frame)
{
    GalagaFrame fr = { board.video, { board.ram[0], board.ram[1], board.ram[2] }, board.vlatch, board.star_origin };
    galaga_draw(&video, &fr);
    const char *dump = getenv("DUMP_DIR");
    if (dump && *dump) {
        static uint8_t rgba[GALAGA_W * GALAGA_H * 4];
        for (int y = 0; y < GALAGA_H; y++)
            for (int x = 0; x < GALAGA_W; x++) {
                uint32_t p = video.screen[y][x];
                uint8_t *o = rgba + ((size_t)y * GALAGA_W + (size_t)x) * 4;
                o[0] = (uint8_t)(p >> 16), o[1] = (uint8_t)(p >> 8), o[2] = (uint8_t)p, o[3] = 0xFF;
            }
        char out[1100];
        snprintf(out, sizeof out, "%s/board_%ld.png", dump, frame);
        png_write(out, rgba, GALAGA_W, GALAGA_H);
    }
    Image shot;
    const char *err;
    if (png_load(path, &shot, &err)) return -1;
    if (shot.width != GALAGA_W || shot.height != GALAGA_H) {
        image_free(&shot);
        return -1;
    }
    long diff = 0;
    for (int y = 0; y < GALAGA_H; y++)
        for (int x = 0; x < GALAGA_W; x++) {
            const uint8_t *p = shot.rgba + ((size_t)y * GALAGA_W + (size_t)x) * 4;
            uint32_t want = (uint32_t)p[0] << 16 | (uint32_t)p[1] << 8 | p[2];
            if ((video.screen[y][x] & 0xFFFFFF) != want && diff++ < 4)
                printf("  pixel %d,%d: %06X, MAME %06X\n", x, y, (unsigned)video.screen[y][x], (unsigned)want);
        }
    image_free(&shot);
    return diff;
}

int main(int argc, char **argv)
{
    if (argc < 7) {
        fprintf(stderr, "usage: test_emu <rom> <script> <capture dir> <trace dir> <trace from> <trace to> <frame>...\n");
        return 2;
    }
    char err[512];
    if (rom_load(&rom, argv[1], err, sizeof err)) {
        fprintf(stderr, "%s\n", err);
        return 1;
    }
    if (!rom.has_51xx || !rom.has_54xx) {
        fprintf(stderr, "test_emu: the device ROMs (namco51.zip, namco54.zip) are needed next to %s\n", argv[1]);
        return 1;
    }
    if (parse_script(argv[2])) {
        fprintf(stderr, "test_emu: bad script\n");
        return 2;
    }
    trace_from = atol(argv[5]);
    trace_to = atol(argv[6]);
    int failures = 0;
    if (trace_to > trace_from)
        for (int c = 0; c < BOARD_CPUS; c++) {
            char path[1100];
            snprintf(path, sizeof path, "%s/%s.tr", argv[4], cpu_names[c]);
            if (!load_trace(&mame[c], path, c >= CPU_51XX) && mame[c].n) traced[c] = 1;
        }
    galaga_video_init(&video, &rom);
    board_init(&board, &rom);
    board.step_hook = step_hook;
    for (int i = 7; i < argc; i++) {
        long frame = atol(argv[i]);
        while (board.frame < frame) {
            board_run_frame(&board);
            keep_audio();
            inputs(&board);
        }
        char path[1100];
        snprintf(path, sizeof path, "%s/frame_%ld.txt", argv[3], frame);
        long bytes = compare_memory(path);
        snprintf(path, sizeof path, "%s/%ld.png", argv[3], frame);
        long pixels = compare_picture(path, frame);
        if (bytes < 0 || pixels < 0) {
            printf("frame %ld: no capture\n", frame);
            failures++;
            continue;
        }
        printf("frame %ld: %ld bytes and %ld pixels differ\n", frame, bytes, pixels);
        if (bytes || pixels) failures++;
    }
    while (board.frame < trace_to) {
        board_run_frame(&board);
        keep_audio();
        inputs(&board);
    }
    char wav[1100];
    snprintf(wav, sizeof wav, "%s/game.wav", argv[3]);
    if (compare_sound(wav)) failures++;
    for (int c = 0; c < BOARD_CPUS; c++)
        if (traced[c] && compare_trace(c) && c != CPU_51XX) failures++;
    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    puts("emu: the board equals MAME");
    return 0;
}
