#include "emu/mb88.h"

#include <string.h>

#define M (m->ram[(m->x & 3) << 4 | m->y]) /* the RAM nibble X:Y points at */

void mb88_reset(MB88 *m)
{
    const uint8_t *rom = m->rom;
    void *ctx = m->ctx;
    uint8_t (*read_k)(void *) = m->read_k;
    uint8_t (*read_r)(void *, int) = m->read_r;
    void (*write_r)(void *, int, uint8_t) = m->write_r;
    void (*write_o)(void *, uint8_t) = m->write_o;
    void (*write_p)(void *, uint8_t) = m->write_p;
    uint8_t ram[MB88_RAM_SIZE];
    memcpy(ram, m->ram, sizeof ram); /* RAM keeps its contents */
    int irq_line = m->irq_line, tc_line = m->tc_line;
    memset(m, 0, sizeof *m);
    m->rom = rom;
    m->ctx = ctx;
    m->read_k = read_k;
    m->read_r = read_r;
    m->write_r = write_r;
    m->write_o = write_o;
    m->write_p = write_p;
    memcpy(m->ram, ram, sizeof ram);
    m->irq_line = irq_line;
    m->tc_line = tc_line;
    m->st = 1;
}

int mb88_flags(const MB88 *m)
{
    return m->st | m->zf << 1 | m->cf << 2 | m->vf << 3 | m->sf << 4 | m->nf << 5;
}

void mb88_set_irq(MB88 *m, int state)
{
    /* NF follows the pin; the interrupt is taken on its rising edge, when
       enabled, after the instruction under way */
    if (state && !m->irq_line && (m->pio & MB88_PIO_IRQ)) m->irq_pending = 1;
    m->irq_line = state;
    m->nf = (uint8_t)(state != 0);
}

void mb88_set_tc(MB88 *m, int state)
{
    if (!state && m->tc_line && (m->pio & MB88_PIO_TIMER)) {
        m->tl = (m->tl + 1) & 15;
        if (!m->tl) {
            m->th = (m->th + 1) & 15;
            if (!m->th) m->vf = 1;
        }
    }
    m->tc_line = state;
}

/* The PC counts on into the next page */
static uint8_t fetch(MB88 *m)
{
    uint8_t v = m->rom[m->pa << 6 | m->pc];
    if (++m->pc == 0x40) {
        m->pc = 0;
        m->pa = (m->pa + 1) & 0x0F;
    }
    return v;
}

static void push(MB88 *m)
{
    m->stack[m->si] = (uint16_t)(m->pc | m->pa << 6 | (m->st | m->zf << 1 | m->cf << 2) << 10);
    m->si = (m->si + 1) & 3;
}

/* RTI restores the flags as well */
static void pop(MB88 *m, int flags)
{
    m->si = (m->si - 1) & 3;
    uint16_t v = m->stack[m->si];
    m->pc = v & 0x3F;
    m->pa = (v >> 6) & 0x0F;
    if (flags) {
        m->st = (v >> 10) & 1;
        m->zf = (v >> 11) & 1;
        m->cf = (v >> 12) & 1;
    }
}

static uint8_t read_r(MB88 *m, int port) { return m->read_r ? m->read_r(m->ctx, port) & 15 : m->r[port]; }

static void write_r(MB88 *m, int port, uint8_t v)
{
    m->r[port] = v & 15;
    if (m->write_r) m->write_r(m->ctx, port, v & 15);
}

/* A result in A: ZF from it */
static void set_a(MB88 *m, int v)
{
    m->a = (uint8_t)(v & 15);
    m->zf = m->a == 0;
}

int mb88_step(MB88 *m)
{
    m->last_interrupt = 0;
    uint8_t op = fetch(m);
    int cycles = 1;
    int v;
    switch (op) {
    case 0x00: /* NOP */ m->st = 1; break;
    case 0x01: /* OUTO: a nibble of O, CF picks the half */
        m->o = m->cf ? (uint8_t)((m->o & 0x0F) | m->a << 4) : (uint8_t)((m->o & 0xF0) | m->a);
        if (m->write_o) m->write_o(m->ctx, m->o);
        m->st = 1;
        break;
    case 0x02: /* OUTP */
        m->p = m->a;
        if (m->write_p) m->write_p(m->ctx, m->a);
        m->st = 1;
        break;
    case 0x03: /* OUT: R(Y) */ write_r(m, m->y & 3, m->a); m->st = 1; break;
    case 0x04: /* TAY */ m->y = m->a; m->st = 1; break;
    case 0x05: /* TATH */ m->th = m->a; m->st = 1; break;
    case 0x06: /* TATL */ m->tl = m->a; m->st = 1; break;
    case 0x07: /* TAS */ m->sb = m->a; m->st = 1; break;
    case 0x08: /* ICY */
        m->y = (m->y + 1) & 15;
        m->zf = m->y == 0;
        m->st = !m->zf;
        break;
    case 0x09: /* ICM */
        v = (M + 1) & 15;
        M = (uint8_t)v;
        m->zf = v == 0;
        m->st = !m->zf;
        break;
    case 0x0A: /* STIC */
        M = m->a;
        m->y = (m->y + 1) & 15;
        m->zf = m->y == 0;
        m->st = !m->zf;
        break;
    case 0x0B: /* X */
        v = M;
        M = m->a;
        set_a(m, v);
        m->st = 1;
        break;
    case 0x0C: /* ROL */
        v = m->a << 1 | m->cf;
        m->cf = (v >> 4) & 1;
        set_a(m, v);
        m->st = !m->cf;
        break;
    case 0x0D: /* L */ set_a(m, M); m->st = 1; break;
    case 0x0E: /* ADC */
        v = m->a + M + m->cf;
        m->cf = v > 15;
        set_a(m, v);
        m->st = !m->cf;
        break;
    case 0x0F: /* AND */ set_a(m, m->a & M); m->st = !m->zf; break;
    case 0x10: /* DAA */
        if (m->cf || m->a > 9) {
            v = m->a + 6;
            if (v > 15) m->cf = 1;
            m->a = (uint8_t)(v & 15);
        }
        m->st = !m->cf;
        break;
    case 0x11: /* DAS */
        if (m->cf || m->a > 9) {
            v = m->a + 10;
            if (v > 15) m->cf = 1;
            m->a = (uint8_t)(v & 15);
        }
        m->st = !m->cf;
        break;
    case 0x12: /* INK */ set_a(m, m->read_k ? m->read_k(m->ctx) : 0); m->st = 1; break;
    case 0x13: /* IN: R(Y) */ set_a(m, read_r(m, m->y & 3)); m->st = 1; break;
    case 0x14: /* TYA */ set_a(m, m->y); m->st = 1; break;
    case 0x15: /* TTHA */ set_a(m, m->th); m->st = 1; break;
    case 0x16: /* TTLA */ set_a(m, m->tl); m->st = 1; break;
    case 0x17: /* TSA */ set_a(m, m->sb); m->st = 1; break;
    case 0x18: /* DCY */
        m->y = (m->y - 1) & 15;
        m->st = m->y != 15;
        break;
    case 0x19: /* DCM */
        v = (M - 1) & 15;
        M = (uint8_t)v;
        m->zf = v == 0;
        m->st = v != 15;
        break;
    case 0x1A: /* STDC */
        M = m->a;
        m->y = (m->y - 1) & 15;
        m->zf = m->y == 0;
        m->st = m->y != 15;
        break;
    case 0x1B: /* XX */
        v = m->x;
        m->x = m->a;
        set_a(m, v);
        m->st = 1;
        break;
    case 0x1C: /* ROR */
        v = m->a >> 1 | m->cf << 3;
        m->cf = m->a & 1;
        set_a(m, v);
        m->st = !m->cf;
        break;
    case 0x1D: /* ST */ M = m->a; m->st = 1; break;
    case 0x1E: /* SBC */
        v = M - m->a - m->cf;
        m->cf = v < 0;
        set_a(m, v);
        m->st = !m->cf;
        break;
    case 0x1F: /* OR */ set_a(m, m->a | M); m->st = !m->zf; break;
    case 0x20: /* SETR: R(Y/4) bit Y&3 */
        write_r(m, (m->y >> 2) & 3, (uint8_t)(m->r[(m->y >> 2) & 3] | 1 << (m->y & 3)));
        m->st = 1;
        break;
    case 0x21: /* SETC */ m->cf = 1; m->st = 1; break;
    case 0x22: /* RSTR */
        write_r(m, (m->y >> 2) & 3, (uint8_t)(m->r[(m->y >> 2) & 3] & ~(1 << (m->y & 3))));
        m->st = 1;
        break;
    case 0x23: /* RSTC */ m->cf = 0; m->st = 1; break;
    case 0x24: /* TSTR */ m->st = !(read_r(m, (m->y >> 2) & 3) >> (m->y & 3) & 1); break;
    case 0x25: /* TSTI */ m->st = !m->irq_line; break;
    case 0x26: /* TSTV */ m->st = !m->vf; m->vf = 0; break;
    case 0x27: /* TSTS */ m->st = !m->sf; m->sf = 0; break;
    case 0x28: /* TSTC */ m->st = !m->cf; break;
    case 0x29: /* TSTZ */ m->st = !m->zf; break;
    case 0x2A: /* STS */ M = m->sb; m->zf = m->sb == 0; m->st = 1; break;
    case 0x2B: /* LS */ m->sb = M; m->zf = m->sb == 0; m->st = 1; break;
    case 0x2C: /* RTS */ pop(m, 0); m->st = 1; break;
    case 0x2D: /* NEG */ set_a(m, -m->a); m->st = !m->zf; break;
    case 0x2E: /* C: M - A */
        v = M - m->a;
        m->cf = v < 0;
        m->zf = v == 0;
        m->st = !m->zf;
        break;
    case 0x2F: /* EOR */ set_a(m, m->a ^ M); m->st = !m->zf; break;
    case 0x30: case 0x31: case 0x32: case 0x33: /* SBIT */
        M = (uint8_t)(M | 1 << (op & 3));
        m->st = 1;
        break;
    case 0x34: case 0x35: case 0x36: case 0x37: /* RBIT */
        M = (uint8_t)(M & ~(1 << (op & 3)));
        m->st = 1;
        break;
    case 0x38: case 0x39: case 0x3A: case 0x3B: /* TBIT */ m->st = !(M >> (op & 3) & 1); break;
    case 0x3C: /* RTI */ pop(m, 1); break;
    case 0x3D: /* JPA: the page, PC = A x 4 */
        v = fetch(m);
        cycles = 2;
        m->pa = (uint8_t)(v & 0x0F);
        m->pc = (uint8_t)(m->a << 2);
        m->st = 1;
        break;
    case 0x3E: /* EN */ cycles = 2; m->pio |= fetch(m); m->st = 1; break;
    case 0x3F: /* DIS */ cycles = 2; m->pio &= (uint8_t)~fetch(m); m->st = 1; break;
    case 0x40: case 0x41: case 0x42: case 0x43: /* SETD: R0 bit */
        write_r(m, 0, (uint8_t)(m->r[0] | 1 << (op & 3)));
        m->st = 1;
        break;
    case 0x44: case 0x45: case 0x46: case 0x47: /* RSTD */
        write_r(m, 0, (uint8_t)(m->r[0] & ~(1 << (op & 3))));
        m->st = 1;
        break;
    case 0x48: case 0x49: case 0x4A: case 0x4B: /* TSTD: R2 bit */ m->st = !(read_r(m, 2) >> (op & 3) & 1); break;
    case 0x4C: case 0x4D: case 0x4E: case 0x4F: /* TBA */ m->st = !(m->a >> (op & 3) & 1); break;
    case 0x50: case 0x51: case 0x52: case 0x53: /* XD: A with RAM 0:n */
        v = m->ram[op & 3];
        m->ram[op & 3] = m->a;
        set_a(m, v);
        m->st = 1;
        break;
    case 0x54: case 0x55: case 0x56: case 0x57: /* XYD: Y with RAM 0:4+n */
        v = m->ram[4 + (op & 3)];
        m->ram[4 + (op & 3)] = m->y;
        m->y = (uint8_t)v;
        m->zf = m->y == 0;
        m->st = 1;
        break;
    case 0x58: case 0x59: case 0x5A: case 0x5B: case 0x5C: case 0x5D: case 0x5E: case 0x5F: /* LXI */
        m->x = op & 7;
        m->zf = m->x == 0;
        m->st = 1;
        break;
    case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: case 0x67: /* CALL */
        v = fetch(m);
        cycles = 2;
        if (m->st) {
            push(m);
            m->pa = (uint8_t)((op & 7) << 2 | v >> 6);
            m->pc = (uint8_t)(v & 0x3F);
        }
        m->st = 1;
        break;
    case 0x68: case 0x69: case 0x6A: case 0x6B: case 0x6C: case 0x6D: case 0x6E: case 0x6F: /* JPL */
        v = fetch(m);
        cycles = 2;
        if (m->st) {
            m->pa = (uint8_t)((op & 7) << 2 | v >> 6);
            m->pc = (uint8_t)(v & 0x3F);
        }
        m->st = 1;
        break;
    default:
        switch (op >> 4) {
        case 0x7: /* AI */
            v = m->a + (op & 15);
            m->cf = v > 15;
            set_a(m, v);
            m->st = !m->cf;
            break;
        case 0x8: /* LYI */
            m->y = op & 15;
            m->zf = m->y == 0;
            m->st = 1;
            break;
        case 0x9: /* LI */ set_a(m, op & 15); m->st = 1; break;
        case 0xA: /* CYI: n - Y */
            v = (op & 15) - m->y;
            m->cf = v < 0;
            m->zf = v == 0;
            m->st = !m->zf;
            break;
        case 0xB: /* CI: n - A */
            v = (op & 15) - m->a;
            m->cf = v < 0;
            m->zf = v == 0;
            m->st = !m->zf;
            break;
        default: /* JMP within the page */
            if (m->st) m->pc = op & 0x3F;
            m->st = 1;
            break;
        }
        break;
    }
    if (m->irq_pending) { /* to $002, in 3 cycles */
        m->irq_pending = 0;
        push(m);
        m->pa = 0;
        m->pc = 2;
        m->st = 1;
        m->last_interrupt = 1;
        cycles += 3;
    }
    m->cycles += cycles;
    return cycles;
}
