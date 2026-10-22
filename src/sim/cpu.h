/*
 * Registers and flags for routines ported line by line, where a ROM's code
 * carries values in registers and flags from one block to the next (a
 * register left over from an earlier block feeding a division, a carry
 * feeding a rotate). The flags kept are the ones the programs test or
 * DAA reads: carry, zero, sign, half carry, subtract; each operation sets
 * them as the Z80 does.
 */
#ifndef GALAGA_CPU_H
#define GALAGA_CPU_H

#include "sim/game.h"

typedef struct {
    Game *g;
    uint8_t a, b, c, d, e, h, l;
    uint8_t cf, zf, sf, hf, nf; /* carry, zero, sign, half carry, subtract */
    uint8_t pf;                 /* P/V, as the block instructions leave it (BC not 0) */
    uint8_t iff;                /* interrupts enabled (EI / DI) */
    uint16_t pc;                /* a resumable routine: where it goes on, */
    uint8_t resume;             /* and at once (1: an EI let the IRQ in) or at its next frame (0) */
    uint8_t halted;             /* stopped at a HALT until the next interrupt */
    int64_t t, budget;          /* the Z80 cycles run, and where a resumable routine stops (the next vertical blank) */
    uint16_t ix, iy;
    uint8_t a2, cf2, zf2, sf2, hf2, nf2; /* AF' */
    uint8_t b2, c2, d2, e2, h2, l2;      /* BC', DE', HL' */
    uint16_t sp, stack[32];     /* the stack: PUSH / POP and the return addresses of a resumable routine */
    uint16_t rom;               /* where the CPU's ROM sits in g->data (0, GAME_SUB, GAME_SUB2) */
} Cpu;

#define HL(z) ((uint16_t)((z)->h << 8 | (z)->l))
#define DE(z) ((uint16_t)((z)->d << 8 | (z)->e))
#define BC(z) ((uint16_t)((z)->b << 8 | (z)->c))
#define SET_HL(z, v) cpu_pair(&(z)->h, &(z)->l, (unsigned)(v))
#define SET_DE(z, v) cpu_pair(&(z)->d, &(z)->e, (unsigned)(v))
#define SET_BC(z, v) cpu_pair(&(z)->b, &(z)->c, (unsigned)(v))
static inline void cpu_pair(uint8_t *hi, uint8_t *lo, unsigned v)
{
    *hi = (uint8_t)(v >> 8);
    *lo = (uint8_t)v;
}
static inline void cpu_ex_de_hl(Cpu *z)
{
    uint8_t d = z->d, e = z->e;
    z->d = z->h, z->e = z->l;
    z->h = d, z->l = e;
}

static inline void cpu_ex_af(Cpu *z)
{
    uint8_t t;
#define SWAP(x, y) (t = (x), (x) = (y), (y) = t)
    SWAP(z->a, z->a2);
    SWAP(z->cf, z->cf2);
    SWAP(z->zf, z->zf2);
    SWAP(z->sf, z->sf2);
    SWAP(z->hf, z->hf2);
    SWAP(z->nf, z->nf2);
}
static inline void cpu_exx(Cpu *z)
{
    uint8_t t;
    SWAP(z->b, z->b2);
    SWAP(z->c, z->c2);
    SWAP(z->d, z->d2);
    SWAP(z->e, z->e2);
    SWAP(z->h, z->h2);
    SWAP(z->l, z->l2);
#undef SWAP
}
/* PUSH / POP of a pair (the values only: the stack is the Cpu's own) */
static inline void cpu_push(Cpu *z, uint16_t v) { z->stack[z->sp++ & 31] = v; }
static inline uint16_t cpu_pop(Cpu *z) { return z->stack[--z->sp & 31]; }
#define AF(z) ((uint16_t)((z)->a << 8 | (z)->sf << 7 | (z)->zf << 6 | (z)->hf << 4 | (z)->nf << 1 | (z)->cf))
static inline void cpu_set_af(Cpu *z, uint16_t v)
{
    z->a = (uint8_t)(v >> 8);
    z->sf = v >> 7 & 1, z->zf = v >> 6 & 1, z->hf = v >> 4 & 1, z->nf = v >> 1 & 1, z->cf = v & 1;
}

/* A byte as the CPU reads it: its ROM below $1000 (main's below $4000), the board above */
static inline uint8_t cpu_rd(const Cpu *z, uint16_t a)
{
    if (a < (z->rom ? 0x1000 : 0x4000)) return game_data(z->g, z->rom + a);
    if (a < 0x4000) return 0;
    return game_rd(z->g, a);
}
static inline void cpu_wr(Cpu *z, uint16_t a, uint8_t v) { game_wr(z->g, a, v); }
static inline uint8_t cpu_ix(const Cpu *z, int d) { return cpu_rd(z, (uint16_t)(z->ix + d)); }
static inline void cpu_set_ix(Cpu *z, int d, uint8_t v) { cpu_wr(z, (uint16_t)(z->ix + d), v); }
static inline uint8_t cpu_iy(const Cpu *z, int d) { return cpu_rd(z, (uint16_t)(z->iy + d)); }
static inline void cpu_set_iy(Cpu *z, int d, uint8_t v) { cpu_wr(z, (uint16_t)(z->iy + d), v); }

static inline void cpu_zs(Cpu *z, uint8_t v)
{
    z->zf = v == 0;
    z->sf = v >> 7;
}

/* the accumulator's operations */
static inline void cpu_add(Cpu *z, uint8_t v)
{
    unsigned r = (unsigned)z->a + v;
    z->hf = ((z->a ^ v ^ r) >> 4) & 1;
    z->nf = 0;
    z->cf = r > 0xFF;
    z->a = (uint8_t)r;
    cpu_zs(z, z->a);
}
static inline void cpu_adc(Cpu *z, uint8_t v)
{
    unsigned r = (unsigned)z->a + v + z->cf;
    z->hf = ((z->a ^ v ^ r) >> 4) & 1;
    z->nf = 0;
    z->cf = r > 0xFF;
    z->a = (uint8_t)r;
    cpu_zs(z, z->a);
}
static inline void cpu_cp(Cpu *z, uint8_t v)
{
    unsigned r = (unsigned)z->a - v;
    z->hf = ((z->a ^ v ^ r) >> 4) & 1;
    z->nf = 1;
    z->cf = z->a < v;
    cpu_zs(z, (uint8_t)r);
}
static inline void cpu_sub(Cpu *z, uint8_t v)
{
    cpu_cp(z, v);
    z->a = (uint8_t)(z->a - v);
}
static inline void cpu_sbc(Cpu *z, uint8_t v)
{
    unsigned r = (unsigned)z->a - v - z->cf;
    z->hf = ((z->a ^ v ^ r) >> 4) & 1;
    z->nf = 1;
    z->cf = (unsigned)z->a < (unsigned)v + z->cf;
    z->a = (uint8_t)r;
    cpu_zs(z, z->a);
}
static inline void cpu_and(Cpu *z, uint8_t v)
{
    z->a &= v;
    z->cf = z->nf = 0;
    z->hf = 1;
    cpu_zs(z, z->a);
}
static inline void cpu_or(Cpu *z, uint8_t v)
{
    z->a |= v;
    z->cf = z->nf = z->hf = 0;
    cpu_zs(z, z->a);
}
static inline void cpu_xor(Cpu *z, uint8_t v)
{
    z->a ^= v;
    z->cf = z->nf = z->hf = 0;
    cpu_zs(z, z->a);
}
static inline void cpu_neg(Cpu *z)
{
    uint8_t v = z->a;
    z->a = 0;
    cpu_sub(z, v);
}
static inline void cpu_cpl(Cpu *z)
{
    z->a = (uint8_t)~z->a;
    z->hf = z->nf = 1;
}
static inline void cpu_scf(Cpu *z)
{
    z->cf = 1;
    z->hf = z->nf = 0;
}
static inline void cpu_ccf(Cpu *z)
{
    z->hf = z->cf;
    z->cf = !z->cf;
    z->nf = 0;
}
static inline void cpu_daa(Cpu *z)
{
    uint8_t a = z->a, diff = 0, cf = 0;
    if (z->cf || a > 0x99) {
        diff |= 0x60;
        cf = 1;
    }
    if (z->hf || (a & 0x0F) > 9) diff |= 0x06;
    if (z->nf) {
        z->hf = z->hf && (a & 0x0F) < 6;
        z->a = (uint8_t)(a - diff);
    } else {
        z->hf = (a & 0x0F) > 9;
        z->a = (uint8_t)(a + diff);
    }
    z->cf = cf;
    cpu_zs(z, z->a);
}
/* INC r, DEC r: the carry kept */
static inline uint8_t cpu_inc(Cpu *z, uint8_t v)
{
    v++;
    z->hf = (v & 0x0F) == 0;
    z->nf = 0;
    cpu_zs(z, v);
    return v;
}
static inline uint8_t cpu_dec(Cpu *z, uint8_t v)
{
    v--;
    z->hf = (v & 0x0F) == 0x0F;
    z->nf = 1;
    cpu_zs(z, v);
    return v;
}
/* RLCA, RRCA, RLA, RRA: the carry (H, N cleared) */
static inline void cpu_rlca(Cpu *z)
{
    z->cf = z->a >> 7;
    z->a = (uint8_t)(z->a << 1 | z->cf);
    z->hf = z->nf = 0;
}
static inline void cpu_rrca(Cpu *z)
{
    z->cf = z->a & 1;
    z->a = (uint8_t)(z->a >> 1 | z->cf << 7);
    z->hf = z->nf = 0;
}
static inline void cpu_rla(Cpu *z)
{
    uint8_t c = z->a >> 7;
    z->a = (uint8_t)(z->a << 1 | z->cf);
    z->cf = c;
    z->hf = z->nf = 0;
}
static inline void cpu_rra(Cpu *z)
{
    uint8_t c = z->a & 1;
    z->a = (uint8_t)(z->a >> 1 | z->cf << 7);
    z->cf = c;
    z->hf = z->nf = 0;
}
/* the CB rotates and shifts of a value: carry, zero, sign (H, N cleared) */
static inline uint8_t cpu_rot(Cpu *z, uint8_t v, uint8_t c)
{
    z->cf = c;
    z->hf = z->nf = 0;
    cpu_zs(z, v);
    return v;
}
static inline uint8_t cpu_rrc(Cpu *z, uint8_t v) { return cpu_rot(z, (uint8_t)(v >> 1 | v << 7), v & 1); }
static inline uint8_t cpu_rlc(Cpu *z, uint8_t v) { return cpu_rot(z, (uint8_t)(v << 1 | v >> 7), v >> 7); }
static inline uint8_t cpu_rl(Cpu *z, uint8_t v) { return cpu_rot(z, (uint8_t)(v << 1 | z->cf), v >> 7); }
static inline uint8_t cpu_rr(Cpu *z, uint8_t v) { return cpu_rot(z, (uint8_t)(v >> 1 | z->cf << 7), v & 1); }
static inline uint8_t cpu_sla(Cpu *z, uint8_t v) { return cpu_rot(z, (uint8_t)(v << 1), v >> 7); }
static inline uint8_t cpu_sra(Cpu *z, uint8_t v) { return cpu_rot(z, (uint8_t)(v >> 1 | (v & 0x80)), v & 1); }
static inline uint8_t cpu_srl(Cpu *z, uint8_t v) { return cpu_rot(z, (uint8_t)(v >> 1), v & 1); }
/* BIT n: the zero flag (H set, N cleared) */
static inline void cpu_bit(Cpu *z, int n, uint8_t v)
{
    z->zf = !(v >> n & 1);
    z->hf = 1;
    z->nf = 0;
}
/* RLD, RRD: A's low nibble and (HL)'s */
static inline void cpu_rld(Cpu *z)
{
    uint8_t m = cpu_rd(z, HL(z));
    cpu_wr(z, HL(z), (uint8_t)(m << 4 | (z->a & 0x0F)));
    z->a = (uint8_t)((z->a & 0xF0) | m >> 4);
    z->hf = z->nf = 0;
    cpu_zs(z, z->a);
}
static inline void cpu_rrd(Cpu *z)
{
    uint8_t m = cpu_rd(z, HL(z));
    cpu_wr(z, HL(z), (uint8_t)(z->a << 4 | m >> 4));
    z->a = (uint8_t)((z->a & 0xF0) | (m & 0x0F));
    z->hf = z->nf = 0;
    cpu_zs(z, z->a);
}

/* 16 bits: ADD HL (the carry), ADC HL and SBC HL (carry, zero, sign) */
static inline uint16_t cpu_add16(Cpu *z, uint16_t a, uint16_t b)
{
    uint32_t r = (uint32_t)a + b;
    z->hf = ((a ^ b ^ r) >> 12) & 1;
    z->nf = 0;
    z->cf = r > 0xFFFF;
    return (uint16_t)r;
}
static inline uint16_t cpu_adc16(Cpu *z, uint16_t a, uint16_t b)
{
    uint32_t r = (uint32_t)a + b + z->cf;
    z->hf = ((a ^ b ^ r) >> 12) & 1;
    z->nf = 0;
    z->cf = r > 0xFFFF;
    z->zf = (uint16_t)r == 0;
    z->sf = (uint8_t)(r >> 15 & 1);
    return (uint16_t)r;
}
static inline uint16_t cpu_sbc16(Cpu *z, uint16_t a, uint16_t b)
{
    uint32_t r = (uint32_t)a - b - z->cf;
    z->hf = ((a ^ b ^ r) >> 12) & 1;
    z->nf = 1;
    z->cf = (uint32_t)a < (uint32_t)b + z->cf;
    z->zf = (uint16_t)r == 0;
    z->sf = (uint8_t)(r >> 15 & 1);
    return (uint16_t)r;
}

/* the block instructions */
static inline void cpu_ldi(Cpu *z)
{
    cpu_wr(z, DE(z), cpu_rd(z, HL(z)));
    SET_HL(z, HL(z) + 1);
    SET_DE(z, DE(z) + 1);
    SET_BC(z, BC(z) - 1);
    z->hf = z->nf = 0;
    z->pf = BC(z) != 0;
}
static inline void cpu_ldd(Cpu *z)
{
    cpu_wr(z, DE(z), cpu_rd(z, HL(z)));
    SET_HL(z, HL(z) - 1);
    SET_DE(z, DE(z) - 1);
    SET_BC(z, BC(z) - 1);
    z->hf = z->nf = 0;
    z->pf = BC(z) != 0;
}
/* LDIR, LDDR, CPIR whole: 21 cycles a step, 16 the last */
static inline void cpu_ldir(Cpu *z)
{
    for (;;) {
        cpu_ldi(z);
        if (!z->pf) break;
        z->t += 21;
    }
    z->t += 16;
}
static inline void cpu_lddr(Cpu *z)
{
    for (;;) {
        cpu_ldd(z);
        if (!z->pf) break;
        z->t += 21;
    }
    z->t += 16;
}
static inline void cpu_cpi(Cpu *z)
{
    uint8_t v = cpu_rd(z, HL(z)), r = (uint8_t)(z->a - v);
    z->hf = ((z->a ^ v ^ r) >> 4) & 1;
    z->nf = 1;
    cpu_zs(z, r);
    SET_HL(z, HL(z) + 1);
    SET_BC(z, BC(z) - 1);
    z->pf = BC(z) != 0;
}
static inline void cpu_cpir(Cpu *z)
{
    for (;;) {
        cpu_cpi(z);
        if (!z->pf || z->zf) break;
        z->t += 21;
    }
    z->t += 16;
}

/* $0010 (main's and sub's, after its RST): HL += A, A the new L, with the
   cycles of its ADD A,L / LD L,A / RET NC / INC H / RET */
static inline void cpu_add_hl_a(Cpu *z)
{
    cpu_add(z, z->l);
    z->l = z->a;
    if (z->cf) {
        z->h++;
        z->t += 4 + 4 + 5 + 4 + 10;
    } else {
        z->t += 4 + 4 + 11;
    }
}
/* RST $10 */
static inline void cpu_rst10(Cpu *z)
{
    z->t += 11;
    cpu_add_hl_a(z);
}
/* RST $08: HL += 2A ($0008: ADD A,A / JR NC,$0010 / INC H / JP $0010) */
static inline void cpu_rst08(Cpu *z)
{
    z->t += 11;
    cpu_add(z, z->a);
    if (z->cf) {
        z->h++;
        z->t += 4 + 7 + 4 + 10;
    } else {
        z->t += 4 + 12;
    }
    cpu_add_hl_a(z);
}
/* a word as the CPU reads it */
static inline uint16_t cpu_rd16(const Cpu *z, uint16_t a)
{
    return (uint16_t)(cpu_rd(z, a) | cpu_rd(z, (uint16_t)(a + 1)) << 8);
}

/* main's $1061 and sub's $0EAE: HL / A (17 steps of ADC A / ADC HL), HL
   the quotient, A the remainder */
static inline void cpu_divide(Cpu *z)
{
    uint8_t c = z->a, a = 0;
    int cy = 0;
    uint16_t hl = HL(z);
    for (int i = 0; i < 17; i++) {
        unsigned t = (unsigned)a * 2 + (unsigned)cy;
        a = (uint8_t)t;
        if (t > 0xFF) {
            a = (uint8_t)(a - c);
            cy = 1;
        } else if (a < c) {
            cy = 0;
        } else {
            a = (uint8_t)(a - c);
            cy = 1;
        }
        uint32_t n = (uint32_t)hl * 2 + (uint32_t)cy;
        hl = (uint16_t)n;
        cy = (int)(n >> 16);
    }
    SET_HL(z, hl);
    z->a = a;
    z->cf = (uint8_t)cy;
    z->zf = hl == 0;
    z->sf = (uint8_t)(hl >> 15);
}

/* main's $104E and sub's $0E9B: HL * A, A left 0 */
static inline void cpu_multiply(Cpu *z)
{
    SET_HL(z, HL(z) * z->a);
    z->a = 0;
    z->cf = z->nf = 0;
    z->zf = z->hf = 1;
    z->sf = 0;
}

#endif
