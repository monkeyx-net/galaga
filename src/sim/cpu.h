/*
 * Registers and flags for routines ported line by line, where a ROM's code
 * carries values in registers and flags from one block to the next (a
 * register left over from an earlier block feeding a division, a carry
 * feeding a rotate). Only the flags the programs test are kept: carry,
 * zero and sign; each operation sets them as the Z80 does.
 */
#ifndef GALAGA_CPU_H
#define GALAGA_CPU_H

#include "sim/game.h"

typedef struct {
    Game *g;
    uint8_t a, b, c, d, e, h, l;
    uint8_t cf, zf, sf;  /* carry, zero, sign */
    uint16_t ix, iy;
    uint16_t rom;        /* where the CPU's ROM sits in g->data (0, GAME_SUB, GAME_SUB2) */
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

static inline void cpu_zs(Cpu *z, uint8_t v)
{
    z->zf = v == 0;
    z->sf = v >> 7;
}

/* the accumulator's operations */
static inline void cpu_add(Cpu *z, uint8_t v)
{
    unsigned r = (unsigned)z->a + v;
    z->cf = r > 0xFF;
    z->a = (uint8_t)r;
    cpu_zs(z, z->a);
}
static inline void cpu_adc(Cpu *z, uint8_t v)
{
    unsigned r = (unsigned)z->a + v + z->cf;
    z->cf = r > 0xFF;
    z->a = (uint8_t)r;
    cpu_zs(z, z->a);
}
static inline void cpu_cp(Cpu *z, uint8_t v)
{
    z->cf = z->a < v;
    cpu_zs(z, (uint8_t)(z->a - v));
}
static inline void cpu_sub(Cpu *z, uint8_t v)
{
    cpu_cp(z, v);
    z->a = (uint8_t)(z->a - v);
}
static inline void cpu_and(Cpu *z, uint8_t v)
{
    z->a &= v;
    z->cf = 0;
    cpu_zs(z, z->a);
}
static inline void cpu_or(Cpu *z, uint8_t v)
{
    z->a |= v;
    z->cf = 0;
    cpu_zs(z, z->a);
}
static inline void cpu_xor(Cpu *z, uint8_t v)
{
    z->a ^= v;
    z->cf = 0;
    cpu_zs(z, z->a);
}
static inline void cpu_neg(Cpu *z)
{
    z->cf = z->a != 0;
    z->a = (uint8_t)-z->a;
    cpu_zs(z, z->a);
}
/* INC r, DEC r: the carry kept */
static inline uint8_t cpu_inc(Cpu *z, uint8_t v)
{
    v++;
    cpu_zs(z, v);
    return v;
}
static inline uint8_t cpu_dec(Cpu *z, uint8_t v)
{
    v--;
    cpu_zs(z, v);
    return v;
}
/* RLCA, RRCA, RLA, RRA: the carry only */
static inline void cpu_rlca(Cpu *z)
{
    z->cf = z->a >> 7;
    z->a = (uint8_t)(z->a << 1 | z->cf);
}
static inline void cpu_rrca(Cpu *z)
{
    z->cf = z->a & 1;
    z->a = (uint8_t)(z->a >> 1 | z->cf << 7);
}
static inline void cpu_rla(Cpu *z)
{
    uint8_t c = z->a >> 7;
    z->a = (uint8_t)(z->a << 1 | z->cf);
    z->cf = c;
}
static inline void cpu_rra(Cpu *z)
{
    uint8_t c = z->a & 1;
    z->a = (uint8_t)(z->a >> 1 | z->cf << 7);
    z->cf = c;
}
/* the CB rotates and shifts of a value: carry, zero, sign */
static inline uint8_t cpu_rrc(Cpu *z, uint8_t v)
{
    z->cf = v & 1;
    v = (uint8_t)(v >> 1 | z->cf << 7);
    cpu_zs(z, v);
    return v;
}
static inline uint8_t cpu_rlc(Cpu *z, uint8_t v)
{
    z->cf = v >> 7;
    v = (uint8_t)(v << 1 | z->cf);
    cpu_zs(z, v);
    return v;
}
static inline uint8_t cpu_rl(Cpu *z, uint8_t v)
{
    uint8_t c = v >> 7;
    v = (uint8_t)(v << 1 | z->cf);
    z->cf = c;
    cpu_zs(z, v);
    return v;
}
static inline uint8_t cpu_rr(Cpu *z, uint8_t v)
{
    uint8_t c = v & 1;
    v = (uint8_t)(v >> 1 | z->cf << 7);
    z->cf = c;
    cpu_zs(z, v);
    return v;
}
static inline uint8_t cpu_sla(Cpu *z, uint8_t v)
{
    z->cf = v >> 7;
    v = (uint8_t)(v << 1);
    cpu_zs(z, v);
    return v;
}
static inline uint8_t cpu_sra(Cpu *z, uint8_t v)
{
    z->cf = v & 1;
    v = (uint8_t)(v >> 1 | (v & 0x80));
    cpu_zs(z, v);
    return v;
}
static inline uint8_t cpu_srl(Cpu *z, uint8_t v)
{
    z->cf = v & 1;
    v = (uint8_t)(v >> 1);
    cpu_zs(z, v);
    return v;
}
/* BIT n: the zero flag */
static inline void cpu_bit(Cpu *z, int n, uint8_t v) { z->zf = !(v >> n & 1); }

/* 16 bits: ADD HL (the carry), ADC HL and SBC HL (carry, zero, sign) */
static inline uint16_t cpu_add16(Cpu *z, uint16_t a, uint16_t b)
{
    uint32_t r = (uint32_t)a + b;
    z->cf = r > 0xFFFF;
    return (uint16_t)r;
}
static inline uint16_t cpu_adc16(Cpu *z, uint16_t a, uint16_t b)
{
    uint32_t r = (uint32_t)a + b + z->cf;
    z->cf = r > 0xFFFF;
    z->zf = (uint16_t)r == 0;
    z->sf = (uint8_t)(r >> 15 & 1);
    return (uint16_t)r;
}
static inline uint16_t cpu_sbc16(Cpu *z, uint16_t a, uint16_t b)
{
    uint32_t r = (uint32_t)a - b - z->cf;
    z->cf = (uint32_t)a < (uint32_t)b + z->cf;
    z->zf = (uint16_t)r == 0;
    z->sf = (uint8_t)(r >> 15 & 1);
    return (uint16_t)r;
}

/* RST $10 (main's and sub's): HL += A, A the new L */
static inline void cpu_rst10(Cpu *z)
{
    cpu_add(z, z->l);
    z->l = z->a;
    if (z->cf) z->h++;
}
/* RST $08: HL += 2A */
static inline void cpu_rst08(Cpu *z)
{
    cpu_add(z, z->a);
    if (z->cf) z->h++;
    cpu_rst10(z);
}

#endif
