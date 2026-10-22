/*
 * The MB88xx core against known results (no ROM needed): each opcode
 * group's result, its ST, Z and C flags, its cycles; the PC's page carry,
 * JPA, CALL / RTS / RTI, the ports, the external interrupt (taken after
 * the instruction under way, 3 cycles, NF following the pin) and the
 * timer's count on its clock input.
 */
#include <stdio.h>
#include <string.h>

#include "emu/mb88.h"

static int failures;
#define CHECK(cond, ...)                                                    \
    do {                                                                    \
        if (!(cond)) {                                                      \
            failures++;                                                     \
            fprintf(stderr, "FAIL %s:%d: ", __FILE__, __LINE__);            \
            fprintf(stderr, __VA_ARGS__);                                   \
            fputc('\n', stderr);                                            \
        }                                                                   \
    } while (0)

static uint8_t rom[MB88_ROM_SIZE];
static MB88 m;
static uint8_t k_in, r_in[4], o_out, p_out, r_out[4];

static uint8_t read_k(void *ctx) { return k_in; }
static uint8_t read_r(void *ctx, int port) { return r_in[port]; }
static void write_r(void *ctx, int port, uint8_t v) { r_out[port] = v; }
static void write_o(void *ctx, uint8_t v) { o_out = v; }
static void write_p(void *ctx, uint8_t v) { p_out = v; }

/* A program at $000 (bytes), the CPU reset */
static void load(const uint8_t *code, int n)
{
    memset(rom, 0, sizeof rom);
    memcpy(rom, code, (size_t)n);
    memset(&m, 0, sizeof m);
    m.rom = rom;
    m.read_k = read_k;
    m.read_r = read_r;
    m.write_r = write_r;
    m.write_o = write_o;
    m.write_p = write_p;
    mb88_reset(&m);
}

/* Run instructions; returns the cycles used. */
static int run(int n)
{
    int cycles = 0;
    while (n--) cycles += mb88_step(&m);
    return cycles;
}

#define PROG(...) do { static const uint8_t code_[] = { __VA_ARGS__ }; load(code_, (int)sizeof code_); } while (0)

static void alu(void)
{
    /* LI 9, AI 8: 17 -> 1, carry, ST clear */
    PROG(0x99, 0x78);
    run(2);
    CHECK(m.a == 1 && m.cf && !m.zf && !m.st, "AI carry: A %X C %d Z %d ST %d", m.a, m.cf, m.zf, m.st);
    /* LI 7, AI 9: 0 with carry, Z set */
    PROG(0x97, 0x79);
    run(2);
    CHECK(m.a == 0 && m.cf && m.zf && !m.st, "AI to zero: A %X C %d Z %d", m.a, m.cf, m.zf);
    /* LXI 1 LYI 2 LI 5 ST (M=5) LI 3 ADC with C clear: 8 */
    PROG(0x59, 0x82, 0x95, 0x1D, 0x93, 0x23, 0x0E);
    run(7);
    CHECK(m.a == 8 && !m.cf && m.st && m.ram[0x12] == 5, "ADC: A %X C %d ST %d M %X", m.a, m.cf, m.st, m.ram[0x12]);
    /* SBC: M - A - C = 5 - 7 = -2 -> E, borrow */
    PROG(0x59, 0x82, 0x95, 0x1D, 0x97, 0x23, 0x1E);
    run(7);
    CHECK(m.a == 0xE && m.cf && !m.st, "SBC: A %X C %d ST %d", m.a, m.cf, m.st);
    /* AND / OR / EOR with M = 0xA */
    PROG(0x9A, 0x1D, 0x96, 0x0F);
    run(4);
    CHECK(m.a == 2 && m.st, "AND: A %X ST %d", m.a, m.st);
    PROG(0x9A, 0x1D, 0x95, 0x0F);
    run(4);
    CHECK(m.a == 0 && m.zf && !m.st, "AND to zero: ST %d Z %d", m.st, m.zf);
    PROG(0x9A, 0x1D, 0x95, 0x1F);
    run(4);
    CHECK(m.a == 0xF, "OR: A %X", m.a);
    PROG(0x9A, 0x1D, 0x9A, 0x2F);
    run(4);
    CHECK(m.a == 0 && m.zf && !m.st, "EOR to zero: A %X ST %d", m.a, m.st);
    /* ROL / ROR through the carry */
    PROG(0x9C, 0x21, 0x0C);
    run(3);
    CHECK(m.a == 9 && m.cf && !m.st, "ROL: A %X C %d ST %d", m.a, m.cf, m.st);
    PROG(0x99, 0x23, 0x1C);
    run(3);
    CHECK(m.a == 4 && m.cf && !m.st, "ROR: A %X C %d ST %d", m.a, m.cf, m.st);
    PROG(0x92, 0x21, 0x1C);
    run(3);
    CHECK(m.a == 9 && !m.cf && m.st, "ROR carry in: A %X C %d", m.a, m.cf);
    /* NEG */
    PROG(0x93, 0x2D);
    run(2);
    CHECK(m.a == 0xD && m.st, "NEG: A %X", m.a);
    /* DAA: A > 9 adds 6 */
    PROG(0x9C, 0x23, 0x10);
    run(3);
    CHECK(m.a == 2 && m.cf, "DAA: A %X C %d", m.a, m.cf);
    /* compares: CI (n - A), CYI (n - Y), C (M - A) */
    PROG(0x95, 0xB5);
    run(2);
    CHECK(m.zf && !m.st && !m.cf, "CI equal: Z %d ST %d C %d", m.zf, m.st, m.cf);
    PROG(0x97, 0xB5);
    run(2);
    CHECK(!m.zf && m.st && m.cf, "CI less: Z %d ST %d C %d", m.zf, m.st, m.cf);
    PROG(0x84, 0xA4);
    run(2);
    CHECK(m.zf && !m.st, "CYI equal: Z %d ST %d", m.zf, m.st);
    PROG(0x93, 0x1D, 0x94, 0x2E);
    run(4);
    CHECK(m.cf && !m.zf && m.st, "C: M 3 - A 4: C %d Z %d ST %d", m.cf, m.zf, m.st);
}

static void memory(void)
{
    /* STIC / STDC move Y; ICY's ST clears when Y wraps */
    PROG(0x8F, 0x97, 0x0A);
    run(3);
    CHECK(m.ram[0x0F] == 7 && m.y == 0 && m.zf && !m.st, "STIC wraps: Y %X Z %d ST %d", m.y, m.zf, m.st);
    PROG(0x80, 0x97, 0x1A);
    run(3);
    CHECK(m.ram[0x00] == 7 && m.y == 0xF && !m.st, "STDC borrows: Y %X ST %d", m.y, m.st);
    PROG(0x81, 0x18);
    run(2);
    CHECK(m.y == 0 && m.st, "DCY to 0: ST %d", m.st);
    /* ICM / DCM */
    PROG(0x9F, 0x1D, 0x09);
    run(3);
    CHECK(m.ram[0] == 0 && !m.st, "ICM wraps: M %X ST %d", m.ram[0], m.st);
    PROG(0x90, 0x1D, 0x19);
    run(3);
    CHECK(m.ram[0] == 0xF && !m.st, "DCM borrows: M %X ST %d", m.ram[0], m.st);
    /* X, XX, XD, XYD, LXI's three bits */
    PROG(0x95, 0x1D, 0x93, 0x0B);
    run(4);
    CHECK(m.a == 5 && m.ram[0] == 3, "X: A %X M %X", m.a, m.ram[0]);
    PROG(0x5F, 0x96, 0x1B);
    run(3);
    CHECK(m.x == 6 && m.a == 7, "XX / LXI 7: X %X A %X", m.x, m.a);
    PROG(0x9C, 0x53);
    run(2);
    CHECK(m.ram[3] == 0xC && m.a == 0 && m.zf, "XD 3: M %X A %X", m.ram[3], m.a);
    PROG(0x89, 0x56);
    run(2);
    CHECK(m.ram[6] == 9 && m.y == 0, "XYD 6: M %X Y %X", m.ram[6], m.y);
    /* bit set, reset and test; TBA */
    PROG(0x90, 0x1D, 0x32, 0x3A);
    run(4);
    CHECK(m.ram[0] == 4 && !m.st, "SBIT 2 / TBIT 2: M %X ST %d", m.ram[0], m.st);
    PROG(0x9F, 0x1D, 0x35, 0x39);
    run(4);
    CHECK(m.ram[0] == 0xD && m.st, "RBIT 1 / TBIT 1: M %X ST %d", m.ram[0], m.st);
    PROG(0x98, 0x4F);
    run(2);
    CHECK(!m.st, "TBA 3: ST %d", m.st);
    /* X:Y picks the nibble */
    PROG(0x5B, 0x85, 0x9E, 0x1D);
    run(4);
    CHECK(m.ram[0x35] == 0xE, "M at X 3 Y 5: %X", m.ram[0x35]);
}

static void flow(void)
{
    /* JMP is taken with ST set; skipped (setting ST) with it clear */
    PROG(0xC5, 0, 0, 0, 0, 0x91);
    run(2);
    CHECK(m.a == 1, "JMP taken: A %X", m.a);
    PROG(0x97, 0x79, 0xC5, 0x92, 0, 0x91);
    run(4);
    CHECK(m.a == 2 && m.st, "JMP not taken: A %X ST %d", m.a, m.st);
    /* the PC counts on into the next page */
    memset(rom, 0, sizeof rom);
    load((const uint8_t[]){ 0x68, 0x3F }, 2); /* JPL $03F */
    rom[0x03F] = 0x00;
    rom[0x040] = 0x94;
    run(3);
    CHECK(m.a == 4 && m.pa == 1, "page carry: A %X PA %X", m.a, m.pa);
    /* JPA: page from the operand, PC = A x 4 */
    load((const uint8_t[]){ 0x92, 0x3D, 0x03 }, 3);
    rom[0x0C8] = 0x9B;
    int c = run(2);
    CHECK(m.pa == 3 && m.pc == 8 && c == 3, "JPA: PA %X PC %02X cycles %d", m.pa, m.pc, c);
    run(1);
    CHECK(m.a == 0xB, "JPA target: A %X", m.a);
    /* CALL / RTS; CALL is skipped with ST clear */
    load((const uint8_t[]){ 0x61, 0x10, 0x97 }, 3);
    rom[0x110] = 0x95;
    rom[0x111] = 0x2C;
    c = run(1);
    CHECK(m.pa == 4 && m.pc == 0x10 && c == 2, "CALL: PA %X PC %02X cycles %d", m.pa, m.pc, c);
    run(3);
    CHECK(m.a == 7 && m.pa == 0 && m.pc == 3, "RTS: A %X PA %X PC %02X", m.a, m.pa, m.pc);
    load((const uint8_t[]){ 0x97, 0x79, 0x61, 0x10, 0x92 }, 5);
    run(4);
    CHECK(m.a == 2 && m.si == 0, "CALL skipped: A %X SI %d", m.a, m.si);
}

static void ports(void)
{
    k_in = 0xA;
    r_in[2] = 0x6;
    PROG(0x12, 0x82, 0x13);
    run(1);
    CHECK(m.a == 0xA, "INK: %X", m.a);
    run(2);
    CHECK(m.a == 6, "IN R2: %X", m.a);
    /* OUTO: CF picks the nibble */
    PROG(0x95, 0x23, 0x01, 0x93, 0x21, 0x01);
    run(6);
    CHECK(o_out == 0x35, "OUTO: %02X", o_out);
    PROG(0x9C, 0x02, 0x81, 0x9D, 0x03);
    run(5);
    CHECK(p_out == 0xC && r_out[1] == 0xD, "OUTP %X, OUT R1 %X", p_out, r_out[1]);
    /* SETR / RSTR: R(Y / 4) bit Y & 3 */
    PROG(0x86, 0x20);
    run(2);
    CHECK(r_out[1] == 4, "SETR Y 6: R1 %X", r_out[1]);
    PROG(0x86, 0x20, 0x22);
    run(3);
    CHECK(r_out[1] == 0, "RSTR: R1 %X", r_out[1]);
}

static void interrupts(void)
{
    /* EN 4, then the pin rises: one more instruction, then $002 in 3 cycles */
    load((const uint8_t[]){ 0x3E, 0x04, 0x91, 0x92, 0x93 }, 5);
    run(1);
    mb88_set_irq(&m, 1);
    CHECK(m.nf, "NF follows the pin");
    int c = run(1);
    CHECK(m.a == 1 && m.pa == 0 && m.pc == 2 && c == 4 && m.st, "interrupt after LI 1: A %X PC %02X cycles %d", m.a,
          m.pc, c);
    /* RTI comes back with the flags: JPL $010; the handler at $002 sets C */
    load((const uint8_t[]){ 0x68, 0x10, 0x21, 0x3C }, 4);
    rom[0x010] = 0x3E, rom[0x011] = 0x04, rom[0x012] = 0x23, rom[0x013] = 0x94;
    run(2); /* JPL, EN */
    mb88_set_irq(&m, 1);
    run(1); /* RSTC, then the interrupt */
    CHECK(m.pc == 2 && !m.cf, "interrupt after RSTC: PC %02X", m.pc);
    run(2); /* SETC, RTI */
    CHECK(m.pc == 0x13 && !m.cf, "RTI: PC %02X C %d", m.pc, m.cf);
    mb88_set_irq(&m, 0);
    CHECK(!m.nf, "NF clears with the pin");
    /* disabled: no interrupt on the edge */
    load((const uint8_t[]){ 0x91, 0x92 }, 2);
    mb88_set_irq(&m, 1);
    run(2);
    CHECK(m.a == 2 && m.pc == 2 && m.pa == 0, "disabled: A %X", m.a);
    /* TSTI: ST clear while the pin is high */
    load((const uint8_t[]){ 0x25 }, 1);
    mb88_set_irq(&m, 1);
    run(1);
    CHECK(!m.st, "TSTI with the pin high: ST %d", m.st);
}

static void timer(void)
{
    /* EN $20: the timer counts falling edges of its clock input */
    load((const uint8_t[]){ 0x3E, 0x20, 0x16 }, 3);
    run(1);
    for (int i = 0; i < 17; i++) {
        mb88_set_tc(&m, 1);
        mb88_set_tc(&m, 0);
    }
    run(1);
    CHECK(m.a == 1 && m.th == 1, "timer after 17 counts: TL %X TH %X", m.a, m.th);
    load((const uint8_t[]){ 0x16 }, 1);
    mb88_set_tc(&m, 1);
    mb88_set_tc(&m, 0);
    run(1);
    CHECK(m.a == 0, "timer off: TL %X", m.a);
}

int main(void)
{
    alu();
    memory();
    flow();
    ports();
    interrupts();
    timer();
    if (failures) {
        fprintf(stderr, "%d failure(s)\n", failures);
        return 1;
    }
    puts("mb88: all checks passed");
    return 0;
}
