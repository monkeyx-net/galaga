#include "sim/tasks.h"

#include "sim/routines.h"

static void t_0827(Cpu *z);
static void t_0828(Cpu *z);
static void t_0857(Cpu *z);
static void t_0935(Cpu *z);
static void t_0977(Cpu *z);
static void t_1700(Cpu *z);
static void t_17B2(Cpu *z);
static void t_19B2(Cpu *z);
static void t_1A86(Cpu *z);
static void t_1B6B(Cpu *z);
static void t_1D38(Cpu *z);
static void t_1D7C(Cpu *z);
static void t_1DB9(Cpu *z);
static void t_1DD8(Cpu *z);
static void t_1DEC(Cpu *z);
static void t_1EAA(Cpu *z);
static void t_1F0E(Cpu *z);
static void t_1F8F(Cpu *z);
static void t_2000(Cpu *z);
static void t_20F2(Cpu *z);
static void t_21CB(Cpu *z);
static void t_2222(Cpu *z);
static void t_23DD(Cpu *z);
static void t_2916(Cpu *z);
static void t_2A90(Cpu *z);
static void task(Cpu *z);

void irq_0237(Cpu *z)
{
    z->t += 11; cpu_push(z, AF(z));
    z->t += 4; cpu_ex_af(z);
    z->t += 11; cpu_push(z, AF(z));
    z->t += 11; cpu_push(z, BC(z));
    z->t += 11; cpu_push(z, DE(z));
    z->t += 11; cpu_push(z, HL(z));
    z->t += 15; cpu_push(z, z->ix);
    z->t += 15; cpu_push(z, z->iy);
    z->t += 13; z->a = cpu_rd(z, 0x6804);
    z->t += 4; z->d = z->a;
    z->t += 13; z->a = cpu_rd(z, 0x92A0);
    z->t += 7; cpu_and(z, 0x1C);
    z->t += 4; z->c = z->a;
    z->t += 4; cpu_rrca(z);
    z->t += 4; cpu_xor(z, z->c);
    z->t += 7; cpu_and(z, 0x18);
    z->t += 4; z->c = z->a;
    z->t += 13; z->a = cpu_rd(z, 0x99BE);
    z->t += 8; cpu_bit(z, 1, z->d);
    if (!z->zf) { z->t += 12; goto x0259; } z->t += 7;
    z->t += 7; z->a = 0x07;
x0259:
    z->t += 7; cpu_and(z, 0x07);
    z->t += 4; cpu_or(z, z->c);
    z->t += 7; z->b = 0x05;
    z->t += 10; SET_HL(z, 0xA000);
x0261:
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 4; cpu_rrca(z);
    if (--z->b) { z->t += 13; goto x0261; } z->t += 8;
    z->t += 13; cpu_wr(z, 0x6830, z->a);
    z->t += 4; cpu_xor(z, z->a);
    z->t += 13; cpu_wr(z, 0x6820, z->a);
    return; /* on in irq_026D: sub's IRQ starts meanwhile */
}

void irq_026D(Cpu *z)
{
    z->t += 8; cpu_bit(z, 1, z->d);
    z->t += 10; if (z->zf) { goto x02A8; }
    z->t += 4; z->c = z->a;
x0273:
    z->t += 10; SET_HL(z, 0x9000);
    z->t += 4; z->a = z->c;
    z->t += 4; cpu_add(z, z->l);
    z->t += 4; z->l = z->a;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; cpu_and(z, z->a);
    if (!z->zf) { z->t += 12; goto x0280; } z->t += 7;
    z->t += 4; z->c = cpu_inc(z, z->c);
    z->t += 12; goto x0273;
x0280:
    z->t += 4; z->b = z->a;
    z->t += 10; SET_HL(z, 0x0096);
    z->t += 4; z->a = z->c;
    z->t += 8; z->a = cpu_sla(z, z->a);
    z->t += 4; cpu_add(z, z->l);
    z->t += 4; z->l = z->a;
    z->t += 7; z->e = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; z->d = cpu_rd(z, HL(z));
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 11; cpu_push(z, BC(z));
    z->t += 17; task(z); /* CALL $003B: JP (HL), a task */
    z->t += 10; SET_BC(z, cpu_pop(z));
    z->t += 4; z->a = z->b;
    z->t += 4; cpu_add(z, z->c);
    z->t += 4; z->c = z->a;
    z->t += 7; cpu_and(z, 0xE0);
    if (z->zf) { z->t += 12; goto x0273; } z->t += 7;
    z->t += 10; SET_HL(z, 0x7000);
    z->t += 10; SET_DE(z, 0x99B5);
    z->t += 10; SET_BC(z, 0x0003);
    z->t += 4; cpu_exx(z);
    z->t += 7; z->a = 0x71;
    z->t += 13; /* LD ($7100),A: the 51xx's three bytes (sim.c) */
x02A8:
    z->t += 7; z->a = 0x01;
    z->t += 13; cpu_wr(z, 0x6820, z->a);
    z->t += 14; z->iy = (uint16_t)(cpu_pop(z));
    z->t += 14; z->ix = (uint16_t)(cpu_pop(z));
    z->t += 10; SET_HL(z, cpu_pop(z));
    z->t += 10; SET_DE(z, cpu_pop(z));
    z->t += 10; SET_BC(z, cpu_pop(z));
    z->t += 10; cpu_set_af(z, cpu_pop(z));
    z->t += 4; cpu_ex_af(z);
    z->t += 10; cpu_set_af(z, cpu_pop(z));
    z->t += 4; z->iff = 1; /* EI */
    z->t += 10; return;
}

static void t_0827(Cpu *z)
{
    z->t += 10; return;
}

static void t_0828(Cpu *z)
{
    z->t += 7; z->a = 0x01;
    z->t += 13; cpu_wr(z, 0x92D6, z->a);
    z->t += 10; SET_HL(z, 0x8B40);
    z->t += 10; SET_DE(z, 0x8BC0);
    z->t += 10; SET_BC(z, 0x0040);
    cpu_ldir(z);
    z->t += 10; SET_HL(z, 0x9340);
    z->t += 10; SET_DE(z, 0x93C0);
    z->t += 7; z->c = 0x40;
    cpu_ldir(z);
    z->t += 10; SET_HL(z, 0x9B40);
    z->t += 10; SET_DE(z, 0x9BC0);
    z->t += 7; z->c = 0x40;
    cpu_ldir(z);
    z->t += 4; cpu_xor(z, z->a);
    z->t += 13; cpu_wr(z, 0x92D6, z->a);
x0850:
    z->t += 13; z->a = cpu_rd(z, 0x92D7);
    z->t += 4; z->a = cpu_dec(z, z->a);
    if (z->zf) { z->t += 12; goto x0850; } z->t += 7;
    z->t += 10; return;
}

static void t_0857(Cpu *z)
{
    z->t += 13; z->a = cpu_rd(z, 0x92AE);
    z->t += 4; z->b = z->a;
    z->t += 7; cpu_cp(z, 0x3C);
    if (!z->cf) { z->t += 12; goto x0865; } z->t += 7;
    z->t += 13; z->a = cpu_rd(z, 0x99C5);
    z->t += 13; cpu_wr(z, 0x99C4, z->a);
x0865:
    z->t += 13; z->a = cpu_rd(z, 0x92A7);
    z->t += 4; z->c = z->a;
    z->t += 13; z->a = cpu_rd(z, 0x99C0);
    z->t += 10; SET_HL(z, 0x0909);
    z->t += 17; m_08BE(z);
    z->t += 13; cpu_wr(z, 0x92C8, z->a);
    z->t += 13; z->a = cpu_rd(z, 0x92AA);
    z->t += 4; cpu_and(z, z->a);
    if (z->zf) { z->t += 12; goto x0888; } z->t += 7;
    z->t += 10; SET_HL(z, 0x92C4);
    z->t += 7; z->a = 0x02;
    z->t += 7; z->b = 0x03;
    z->t += 11; m_0018(z);
    z->t += 4; cpu_xor(z, z->a);
    z->t += 13; cpu_wr(z, 0x9AA0, z->a);
    z->t += 10; return;
x0888:
    z->t += 13; z->a = cpu_rd(z, 0x99C1);
    z->t += 10; SET_HL(z, 0x0929);
    z->t += 17; m_08BE(z);
    z->t += 13; cpu_wr(z, 0x92C4, z->a);
    z->t += 13; z->a = cpu_rd(z, 0x99C2);
    z->t += 10; SET_HL(z, 0x08CD);
    z->t += 17; m_08AD(z);
    z->t += 13; cpu_wr(z, 0x92C5, z->a);
    z->t += 13; z->a = cpu_rd(z, 0x99C3);
    z->t += 10; SET_HL(z, 0x08EB);
    z->t += 17; m_08AD(z);
    z->t += 13; cpu_wr(z, 0x92C6, z->a);
    z->t += 10; return;
}

static void t_0935(Cpu *z)
{
    z->t += 13; z->a = cpu_rd(z, 0x92A0);
    z->t += 4; cpu_rlca(z);
    z->t += 4; cpu_rlca(z);
    z->t += 4; cpu_rlca(z);
    z->t += 4; cpu_rlca(z);
    z->t += 4; z->c = z->a;
    z->t += 13; z->a = cpu_rd(z, 0x9201);
    z->t += 7; cpu_cp(z, 0x03);
    if (!z->zf) { z->t += 11; return; } z->t += 5;
    z->t += 13; z->a = cpu_rd(z, 0x9840);
    z->t += 4; z->b = z->a;
    z->t += 4; cpu_cpl(z);
    z->t += 4; cpu_and(z, z->c);
    z->t += 10; SET_HL(z, 0x096E);
    z->t += 10; SET_DE(z, 0x83D9);
    z->t += 17; m_095F(z);
    z->t += 13; z->a = cpu_rd(z, 0x99B3);
    z->t += 4; cpu_and(z, z->a);
    if (z->zf) { z->t += 11; return; } z->t += 5;
    z->t += 4; z->a = z->b;
    z->t += 4; cpu_and(z, z->c);
    z->t += 10; SET_HL(z, 0x0971);
    z->t += 10; SET_DE(z, 0x83C4);
    z->t += 11; cpu_push(z, BC(z));
    z->t += 7; cpu_and(z, 0x01);
    if (z->zf) { z->t += 12; goto x0967; } z->t += 7;
    z->t += 10; SET_HL(z, 0x0974);
x0967:
    z->t += 10; SET_BC(z, 0x0003);
    cpu_ldir(z);
    z->t += 10; SET_BC(z, cpu_pop(z));
    z->t += 10; return;
}

static void t_0977(Cpu *z)
{
    goto x0977;
x02D3:
    z->t += 10; z->sp = 0x90A0; /* SP */
    z->t += 4; cpu_xor(z, z->a);
    z->t += 10; SET_HL(z, 0x92AC);
    z->t += 7; z->b = 0x04;
    z->t += 11; m_0018(z);
    z->t += 10; SET_HL(z, 0x9AA0);
    z->t += 7; z->b = 0x20;
    z->t += 11; m_0018(z);
    z->t += 13; cpu_wr(z, 0xA007, z->a);
    z->t += 13; cpu_wr(z, 0x9215, z->a);
    z->t += 13; cpu_wr(z, 0x99B9, z->a);
    z->t += 4; z->a = cpu_dec(z, z->a);
    z->t += 10; SET_HL(z, 0x92CA);
    z->t += 7; z->b = 0x10;
    z->t += 11; m_0018(z);
    z->t += 7; z->a = 0x01;
    z->t += 13; cpu_wr(z, 0x6820, z->a);
    z->t += 10; SET_HL(z, 0x83C0);
    z->t += 7; z->b = 0x40;
    z->t += 7; z->a = 0x24;
    z->t += 11; m_0018(z);
    z->t += 7; z->h = 0x80;
    z->t += 7; z->b = 0x40;
    z->t += 11; m_0018(z);
    z->t += 10; SET_HL(z, 0x8400);
    z->t += 7; z->b = 0x40;
    z->t += 7; z->a = 0x03;
    z->t += 11; m_0018(z);
    z->t += 17; m_0160(z);
    z->t += 10; SET_DE(z, 0x8A20);
    z->t += 7; z->a = 0x05;
    z->t += 7; z->b = 0x00;
x0317:
    z->t += 10; SET_HL(z, 0x02B9);
    z->t += 7; z->c = 0x06;
    cpu_ldir(z);
    z->t += 4; z->a = cpu_dec(z, z->a);
    if (!z->zf) { z->t += 12; goto x0317; } z->t += 7;
    z->t += 10; SET_HL(z, 0x02BF);
    z->t += 7; z->a = 0x2A;
    z->t += 7; z->b = 0x05;
    z->t += 7; z->c = 0xFF;
x032A:
    z->t += 16; cpu_ldi(z);
    z->t += 6; SET_HL(z, HL(z) - 1);
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 4; z->e = cpu_inc(z, z->e);
    z->t += 16; cpu_ldi(z);
    if (--z->b) { z->t += 13; goto x032A; } z->t += 8;
    z->t += 7; z->a = 0x01;
    z->t += 13; cpu_wr(z, 0x9201, z->a);
    z->t += 10; SET_HL(z, 0xA005);
    z->t += 10; cpu_wr(z, HL(z), 0x00);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 17; m_003C(z);
    z->t += 17; m_00D6(z);
    z->t += 17; m_1242(z);
    z->t += 11; m_0028(z);
    z->t += 7; z->a = 0x20;
    z->t += 13; cpu_wr(z, 0x901E, z->a);
    z->t += 13; z->a = cpu_rd(z, 0x99B5);
    z->t += 13; cpu_wr(z, 0x99B8, z->a);
    z->t += 4; cpu_xor(z, z->a);
    z->t += 13; cpu_wr(z, 0x901E, z->a);
    z->t += 13; cpu_wr(z, 0x9020, z->a);
    z->t += 4; cpu_xor(z, z->a);
    z->t += 13; cpu_wr(z, 0xA007, z->a);
    z->t += 13; cpu_wr(z, 0x9215, z->a);
    z->t += 13; cpu_wr(z, 0x9012, z->a);
    z->t += 7; z->b = 0x80;
    z->t += 10; SET_HL(z, 0x9200);
    z->t += 11; m_0018(z);
    z->t += 7; z->a = 0x06;
    z->t += 13; cpu_wr(z, 0x99BE, z->a);
    z->t += 11; m_0028(z);
    z->t += 17; m_003C(z);
    z->t += 17; m_1242(z);
    z->t += 13; z->a = cpu_rd(z, 0x99B8);
    z->t += 4; cpu_and(z, z->a);
    z->t += 7; z->a = 0x01;
    if (z->zf) { z->t += 12; goto x0380; } z->t += 7;
    z->t += 7; z->a = 0x02;
x0380:
    z->t += 13; cpu_wr(z, 0x9201, z->a);
    if (!z->zf) { z->t += 12; goto x039D; } z->t += 7;
    z->t += 4; cpu_xor(z, z->a);
    z->t += 13; cpu_wr(z, 0x9203, z->a);
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 13; cpu_wr(z, 0x9002, z->a);
x038D:
    z->t += 13; z->a = cpu_rd(z, 0x9201);
    z->t += 4; z->a = cpu_dec(z, z->a);
    if (z->zf) { z->t += 12; goto x038D; } z->t += 7;
    z->t += 17; m_1242(z);
    z->t += 17; m_0160(z);
    z->t += 11; m_0028(z);
    z->t += 17; m_003C(z);
x039D:
    z->t += 4; cpu_xor(z, z->a);
    z->t += 13; cpu_wr(z, 0x920B, z->a);
    z->t += 7; z->c = 0x13;
    z->t += 11; m_0030(z);
    z->t += 7; z->c = 0x01;
    z->t += 11; m_0030(z);
    z->t += 10; SET_HL(z, 0x0452);
    z->t += 16; game_wr16(z->g, 0x9280, HL(z));
    z->t += 13; z->a = cpu_rd(z, 0x9980);
    z->t += 7; cpu_cp(z, 0xFF);
    if (z->zf) { z->t += 12; goto x03D8; } z->t += 7;
    z->t += 4; z->e = z->a;
    z->t += 7; z->c = 0x1B;
    z->t += 17; m_043D(z);
    z->t += 13; z->a = cpu_rd(z, 0x9981);
    z->t += 7; cpu_cp(z, 0xFF);
    if (z->zf) { z->t += 12; goto x03D8; } z->t += 7;
    z->t += 7; cpu_and(z, 0x7F);
    z->t += 4; z->e = z->a;
    z->t += 7; z->c = 0x1C;
    z->t += 17; m_043D(z);
    z->t += 13; z->a = cpu_rd(z, 0x9981);
    z->t += 8; cpu_bit(z, 7, z->a);
    if (!z->zf) { z->t += 12; goto x03D8; } z->t += 7;
    z->t += 7; cpu_and(z, 0x7F);
    z->t += 4; z->e = z->a;
    z->t += 7; z->c = 0x1D;
    z->t += 17; m_043D(z);
x03D8:
    z->t += 13; z->a = cpu_rd(z, 0x9201);
    z->t += 7; cpu_cp(z, 0x02);
    if (z->zf) { z->t += 12; goto x03D8; } z->t += 7;
    z->t += 13; cpu_wr(z, 0x9AB7, z->a);
    z->t += 17; m_0160(z);
    z->t += 17; m_003C(z);
    z->t += 10; SET_HL(z, 0xA005);
    z->t += 10; cpu_wr(z, HL(z), 0x00);
    z->t += 10; cpu_wr(z, HL(z), 0x01);
    z->t += 10; SET_HL(z, 0x9820);
    z->t += 4; cpu_xor(z, z->a);
    z->t += 7; z->b = 0xA0;
    z->t += 11; m_0018(z);
    z->t += 13; cpu_wr(z, 0x9AB7, z->a);
    z->t += 13; cpu_wr(z, 0x99B9, z->a);
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 13; cpu_wr(z, 0x9AAB, z->a);
    z->t += 13; cpu_wr(z, 0x9012, z->a);
    z->t += 13; cpu_wr(z, 0x98F2, z->a);
    z->t += 17; m_0466(z);
    z->t += 17; m_127B(z);
    z->t += 7; z->c = 0x04;
    z->t += 11; m_0030(z);
    z->t += 10; SET_HL(z, 0x92AF);
    z->t += 10; cpu_wr(z, HL(z), 0x08);
x0414:
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; cpu_and(z, z->a);
    if (!z->zf) { z->t += 12; goto x0414; } z->t += 7;
    z->t += 10; SET_HL(z, 0x9290);
    z->t += 7; z->b = 0x10;
    z->t += 11; m_0018(z);
    z->t += 7; z->b = 0x30;
    z->t += 10; SET_HL(z, 0x98B0);
    z->t += 11; m_0018(z);
    z->t += 10; SET_HL(z, 0x83B0);
    z->t += 7; z->c = 0x0B;
    z->t += 17; m_13B3(z);
    z->t += 7; z->a = 0x01;
    z->t += 13; cpu_wr(z, 0x9880, z->a);
    z->t += 13; z->a = cpu_rd(z, 0x9980);
    z->t += 13; cpu_wr(z, 0x983E, z->a);
    z->t += 13; cpu_wr(z, 0x987E, z->a);
    z->t += 10; goto x060F;
x045E:
    z->t += 17; m_0728(z);
    z->t += 17; m_080B(z);
    z->t += 12; goto x045E;
x060F:
    z->t += 17; m_0185(z);
    z->t += 13; z->a = cpu_rd(z, 0x9840);
    z->t += 7; cpu_add(z, 0x04);
    z->t += 4; z->c = z->a;
    z->t += 10; SET_HL(z, 0x826E);
    z->t += 17; m_13B3(z);
    z->t += 17; m_133D(z);
    z->t += 13; z->a = cpu_rd(z, 0x92AE);
    z->t += 7; cpu_add(z, 0x1E);
    z->t += 7; cpu_cp(z, 0x78);
    if (z->cf) { z->t += 12; goto x062C; } z->t += 7;
    z->t += 7; z->a = 0x78;
x062C:
    z->t += 13; cpu_wr(z, 0x92AE, z->a);
    z->t += 17; m_1331(z);
    z->t += 7; z->a = 0x01;
    z->t += 13; cpu_wr(z, 0x9015, z->a);
    z->t += 13; cpu_wr(z, 0x9025, z->a);
    z->t += 13; cpu_wr(z, 0x9842, z->a);
    z->t += 7; z->c = 0x0B;
    z->t += 10; SET_HL(z, 0x83B0);
    z->t += 17; m_13B3(z);
    z->t += 7; z->c = 0x0B;
    z->t += 10; SET_HL(z, 0x83AE);
    z->t += 17; m_13B3(z);
    z->t += 10; goto x045E;
x0977:
    z->t += 13; z->a = cpu_rd(z, 0x99B5);
    z->t += 7; cpu_cp(z, 0xBB);
    z->t += 10; if (z->zf) { goto x336C; }
    z->t += 13; z->a = cpu_rd(z, 0x9201);
    z->t += 7; cpu_cp(z, 0x03);
    if (!z->zf) { z->t += 12; goto x099F; } z->t += 7;
    z->t += 10; SET_HL(z, 0x99E9);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_add(z, 0x01);
    z->t += 4; cpu_daa(z);
    z->t += 7; cpu_cp(z, 0x60);
    if (!z->zf) { z->t += 12; goto x0992; } z->t += 7;
    z->t += 4; cpu_xor(z, z->a);
x0992:
    z->t += 7; z->b = 0x04;
    z->t += 4; cpu_ccf(z);
x0995:
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 4; z->l = cpu_dec(z, z->l);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_adc(z, 0x00);
    z->t += 4; cpu_daa(z);
    if (--z->b) { z->t += 13; goto x0995; } z->t += 8;
    z->t += 12; goto x09E1;
x099F:
    z->t += 13; z->a = cpu_rd(z, 0x99B8);
    z->t += 7; cpu_cp(z, 0xA0);
    z->t += 10; SET_DE(z, 0x803C);
    if (z->zf) { z->t += 12; goto x09D9; } z->t += 7;
    z->t += 13; z->a = cpu_rd(z, 0x99B5);
    z->t += 10; SET_HL(z, 0x09CF);
    z->t += 10; SET_BC(z, 0x0006);
    cpu_lddr(z);
    z->t += 4; z->e = cpu_dec(z, z->e);
    z->t += 4; z->c = z->a;
    z->t += 4; cpu_rlca(z);
    z->t += 4; cpu_rlca(z);
    z->t += 4; cpu_rlca(z);
    z->t += 4; cpu_rlca(z);
    z->t += 7; cpu_and(z, 0x0F);
    if (z->zf) { z->t += 12; goto x09C0; } z->t += 7;
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 4; z->e = cpu_dec(z, z->e);
x09C0:
    z->t += 4; z->a = z->c;
    z->t += 7; cpu_and(z, 0x0F);
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 4; z->e = cpu_dec(z, z->e);
    z->t += 7; z->a = 0x24;
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 12; goto x09E1;
x09D9:
    z->t += 10; SET_HL(z, 0x09D8);
    z->t += 10; SET_BC(z, 0x0009);
    cpu_lddr(z);
x09E1:
    z->t += 13; z->a = cpu_rd(z, 0x9201);
    z->t += 4; cpu_and(z, z->a);
    if (z->zf) { z->t += 11; return; } z->t += 5;
    z->t += 4; z->a = cpu_dec(z, z->a);
    if (!z->zf) { z->t += 12; goto x09FF; } z->t += 7;
    z->t += 13; z->a = cpu_rd(z, 0x99B5);
    z->t += 4; cpu_and(z, z->a);
    if (z->zf) { z->t += 12; goto x09FF; } z->t += 7;
    z->t += 7; z->a = 0x02;
    z->t += 13; cpu_wr(z, 0x9201, z->a);
    z->t += 4; cpu_xor(z, z->a);
    z->t += 10; SET_HL(z, 0x9AA0);
    z->t += 7; z->b = 0x08;
    z->t += 11; m_0018(z);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 7; z->b = 0x0F;
    z->t += 11; m_0018(z);
x09FF:
    z->t += 13; z->a = cpu_rd(z, 0x99B5);
    z->t += 4; z->c = z->a;
    z->t += 13; z->a = cpu_rd(z, 0x99B8);
    z->t += 4; z->b = z->a;
    z->t += 4; cpu_sub(z, z->c);
    if (z->zf) { z->t += 11; return; } z->t += 5;
    if (z->cf) { z->t += 12; goto x0A1A; } z->t += 7;
    z->t += 4; cpu_daa(z);
    z->t += 4; z->a = cpu_dec(z, z->a);
    z->t += 13; cpu_wr(z, 0x99B3, z->a);
    z->t += 4; z->a = z->c;
    z->t += 13; cpu_wr(z, 0x99B8, z->a);
    z->t += 7; z->a = 0x03;
    z->t += 13; cpu_wr(z, 0x9201, z->a);
    z->t += 10; return;
x0A1A:
    z->t += 4; z->a = z->c;
    z->t += 13; cpu_wr(z, 0x99B8, z->a);
    z->t += 7; cpu_cp(z, 0xA0);
    if (z->zf) { z->t += 11; return; } z->t += 5;
    z->t += 4; cpu_sub(z, z->b);
    z->t += 4; cpu_daa(z);
    z->t += 13; cpu_wr(z, 0x9A79, z->a);
    z->t += 10; return;
x336C:
    z->t += 4; cpu_xor(z, z->a);
    z->t += 13; cpu_wr(z, 0x6823, z->a);
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 13; cpu_wr(z, 0x6822, z->a);
    z->t += 7; z->a = 0x10;
    z->t += 13; cpu_wr(z, 0x7100, z->a);
    z->t += 7; z->a = 0xFF;
    z->t += 13; cpu_wr(z, 0x7000, z->a);
    z->t += 4; z->iff = 0; /* DI */
    z->t += 13; cpu_wr(z, 0x6830, z->a);
    z->t += 7; z->b = 0x0A;
x3384:
    z->t += 4; cpu_exx(z);
    z->t += 10; SET_DE(z, 0x8000);
    z->t += 10; SET_HL(z, 0x0000);
    z->t += 10; SET_BC(z, 0x0400);
x338E:
    z->t += 4; z->a = z->l;
    z->t += 4; cpu_xor(z, z->h);
    z->t += 4; cpu_cpl(z);
    z->t += 4; cpu_add(z, z->a);
    z->t += 4; cpu_add(z, z->a);
    z->t += 15; SET_HL(z, cpu_adc16(z, HL(z), HL(z)));
    z->t += 4; z->a = z->l;
    z->t += 13; cpu_wr(z, 0x6830, z->a);
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 6; SET_DE(z, DE(z) + 1);
    z->t += 6; SET_BC(z, BC(z) - 1);
    z->t += 4; z->a = z->b;
    z->t += 4; cpu_or(z, z->c);
    if (!z->zf) { z->t += 12; goto x338E; } z->t += 7;
    z->t += 10; SET_DE(z, 0x8000);
    z->t += 10; SET_HL(z, 0x0000);
    z->t += 10; SET_BC(z, 0x0400);
x33A9:
    z->t += 4; z->a = z->l;
    z->t += 4; cpu_xor(z, z->h);
    z->t += 4; cpu_cpl(z);
    z->t += 4; cpu_add(z, z->a);
    z->t += 4; cpu_add(z, z->a);
    z->t += 15; SET_HL(z, cpu_adc16(z, HL(z), HL(z)));
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 4; cpu_xor(z, z->l);
    z->t += 10; if (!z->zf) { goto x34CA; }
    z->t += 6; SET_DE(z, DE(z) + 1);
    z->t += 13; cpu_wr(z, 0x6830, z->a);
    z->t += 6; SET_BC(z, BC(z) - 1);
    z->t += 4; z->a = z->b;
    z->t += 4; cpu_or(z, z->c);
    if (!z->zf) { z->t += 12; goto x33A9; } z->t += 7;
    z->t += 10; SET_DE(z, 0x8000);
    z->t += 10; SET_HL(z, 0x5555);
    z->t += 10; SET_BC(z, 0x0400);
x33C7:
    z->t += 4; z->a = z->l;
    z->t += 4; cpu_xor(z, z->h);
    z->t += 4; cpu_cpl(z);
    z->t += 4; cpu_add(z, z->a);
    z->t += 4; cpu_add(z, z->a);
    z->t += 15; SET_HL(z, cpu_adc16(z, HL(z), HL(z)));
    z->t += 4; z->a = z->l;
    z->t += 13; cpu_wr(z, 0x6830, z->a);
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 6; SET_DE(z, DE(z) + 1);
    z->t += 6; SET_BC(z, BC(z) - 1);
    z->t += 4; z->a = z->b;
    z->t += 4; cpu_or(z, z->c);
    if (!z->zf) { z->t += 12; goto x33C7; } z->t += 7;
    z->t += 10; SET_DE(z, 0x8000);
    z->t += 10; SET_HL(z, 0x5555);
    z->t += 10; SET_BC(z, 0x0400);
x33E2:
    z->t += 4; z->a = z->l;
    z->t += 4; cpu_xor(z, z->h);
    z->t += 4; cpu_cpl(z);
    z->t += 4; cpu_add(z, z->a);
    z->t += 4; cpu_add(z, z->a);
    z->t += 15; SET_HL(z, cpu_adc16(z, HL(z), HL(z)));
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 4; cpu_xor(z, z->l);
    z->t += 10; if (!z->zf) { goto x34CA; }
    z->t += 6; SET_DE(z, DE(z) + 1);
    z->t += 13; cpu_wr(z, 0x6830, z->a);
    z->t += 6; SET_BC(z, BC(z) - 1);
    z->t += 4; z->a = z->b;
    z->t += 4; cpu_or(z, z->c);
    if (!z->zf) { z->t += 12; goto x33E2; } z->t += 7;
    z->t += 10; SET_DE(z, 0x8000);
    z->t += 10; SET_HL(z, 0xAAAA);
    z->t += 10; SET_BC(z, 0x0400);
x3400:
    z->t += 4; z->a = z->l;
    z->t += 4; cpu_xor(z, z->h);
    z->t += 4; cpu_cpl(z);
    z->t += 4; cpu_add(z, z->a);
    z->t += 4; cpu_add(z, z->a);
    z->t += 15; SET_HL(z, cpu_adc16(z, HL(z), HL(z)));
    z->t += 4; z->a = z->l;
    z->t += 13; cpu_wr(z, 0x6830, z->a);
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 6; SET_DE(z, DE(z) + 1);
    z->t += 6; SET_BC(z, BC(z) - 1);
    z->t += 4; z->a = z->b;
    z->t += 4; cpu_or(z, z->c);
    if (!z->zf) { z->t += 12; goto x3400; } z->t += 7;
    z->t += 10; SET_DE(z, 0x8000);
    z->t += 10; SET_HL(z, 0xAAAA);
    z->t += 10; SET_BC(z, 0x0400);
x341B:
    z->t += 4; z->a = z->l;
    z->t += 4; cpu_xor(z, z->h);
    z->t += 4; cpu_cpl(z);
    z->t += 4; cpu_add(z, z->a);
    z->t += 4; cpu_add(z, z->a);
    z->t += 15; SET_HL(z, cpu_adc16(z, HL(z), HL(z)));
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 4; cpu_xor(z, z->l);
    z->t += 10; if (!z->zf) { goto x34CA; }
    z->t += 6; SET_DE(z, DE(z) + 1);
    z->t += 13; cpu_wr(z, 0x6830, z->a);
    z->t += 6; SET_BC(z, BC(z) - 1);
    z->t += 4; z->a = z->b;
    z->t += 4; cpu_or(z, z->c);
    if (!z->zf) { z->t += 12; goto x341B; } z->t += 7;
    z->t += 4; cpu_exx(z);
    z->t += 4; z->b = cpu_dec(z, z->b);
    z->t += 10; if (!z->zf) { goto x3384; }
    z->t += 10; z->sp = 0x8400; /* SP */
    z->t += 10; SET_DE(z, 0x8400);
    z->t += 17; m_3489(z);
    z->t += 10; SET_DE(z, 0x8800);
    z->t += 17; m_3489(z);
    z->t += 10; SET_DE(z, 0x9000);
    z->t += 17; m_3489(z);
    z->t += 10; SET_HL(z, 0x99E0);
    z->t += 10; SET_DE(z, 0x9000);
    z->t += 10; SET_BC(z, 0x0020);
    cpu_ldir(z);
    z->t += 10; SET_DE(z, 0x9800);
    z->t += 17; m_3489(z);
    z->t += 10; SET_HL(z, 0x9000);
    z->t += 10; SET_DE(z, 0x99E0);
    z->t += 10; SET_BC(z, 0x0020);
    cpu_ldir(z);
    z->t += 10; z->sp = 0x8B00; /* SP */
    z->t += 10; SET_DE(z, 0x8000);
    z->t += 17; m_3489(z);
    z->t += 17; m_3962(z);
    z->t += 10; SET_HL(z, 0x3B8B);
    z->t += 17; m_331B(z);
    z->t += 13; cpu_wr(z, 0x6830, z->a);
    z->t += 17; m_3A46(z);
    z->t += 7; z->a = 0x07;
    z->t += 13; cpu_wr(z, 0x9020, z->a);
    z->t += 17; m_397C(z);
    z->t += 10; goto x355A;
x34CA:
    z->t += 4; z->b = z->a;
    z->t += 4; z->a = z->d;
    z->t += 4; cpu_rra(z);
    z->t += 4; cpu_rra(z);
    z->t += 7; cpu_and(z, 0x07);
    z->t += 7; cpu_cp(z, 0x04);
    if (z->cf) { z->t += 12; goto x34D5; } z->t += 7;
    z->t += 4; z->a = cpu_dec(z, z->a);
x34D5:
    z->t += 7; cpu_cp(z, 0x05);
    if (z->cf) { z->t += 12; goto x34DA; } z->t += 7;
    z->t += 4; z->a = cpu_dec(z, z->a);
x34DA:
    z->t += 4; z->e = z->a;
    z->t += 4; z->a = z->b;
    z->t += 7; z->d = 0x15;
    z->t += 7; cpu_and(z, 0x0F);
    if (!z->zf) { z->t += 12; goto x34E4; } z->t += 7;
    z->t += 7; z->d = 0x11;
x34E4:
    z->t += 13; cpu_wr(z, 0x6830, z->a);
    z->t += 4; cpu_exx(z);
    z->t += 10; SET_HL(z, 0x8000);
    z->t += 10; SET_DE(z, 0x8001);
    z->t += 10; SET_BC(z, 0x0400);
    z->t += 10; cpu_wr(z, HL(z), 0x24);
    cpu_ldir(z);
    z->t += 10; cpu_wr(z, HL(z), 0x00);
    z->t += 10; SET_BC(z, 0x03FF);
    cpu_ldir(z);
    z->t += 13; cpu_wr(z, 0x6830, z->a);
    z->t += 4; cpu_exx(z);
    z->t += 10; SET_HL(z, 0x82E2);
    z->t += 10; cpu_wr(z, HL(z), 0x1B);
    z->t += 7; z->a = 0xE0;
    z->t += 4; z->h = cpu_dec(z, z->h);
    cpu_rst10(z);
    z->t += 10; cpu_wr(z, HL(z), 0x0A);
    z->t += 7; z->a = 0xE0;
    z->t += 4; z->h = cpu_dec(z, z->h);
    cpu_rst10(z);
    z->t += 10; cpu_wr(z, HL(z), 0x16);
    z->t += 7; z->a = 0xA0;
    z->t += 4; z->h = cpu_dec(z, z->h);
    cpu_rst10(z);
    z->t += 7; cpu_wr(z, HL(z), z->e);
    z->t += 7; z->a = 0xE0;
    z->t += 4; z->h = cpu_dec(z, z->h);
    cpu_rst10(z);
    z->t += 7; cpu_wr(z, HL(z), z->d);
    z->t += 10; SET_HL(z, 0x9380);
    z->t += 7; z->b = 0x80;
x3520:
    z->t += 10; cpu_wr(z, HL(z), 0xF1);
    z->t += 6; SET_HL(z, HL(z) + 1);
    if (--z->b) { z->t += 13; goto x3520; } z->t += 8;
x3525:
    z->t += 13; cpu_wr(z, 0x6830, z->a);
    z->t += 10; goto x3525;
x353F:
    z->t += 10; SET_HL(z, 0x3B95);
    z->t += 17; m_331B(z);
    z->t += 10; SET_DE(z, 0x8244);
    z->t += 10; SET_HL(z, 0x9102);
    z->t += 4; cpu_xor(z, z->a);
    z->t += 18; cpu_rld(z);
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 11; m_0020(z);
    z->t += 4; cpu_xor(z, z->a);
    z->t += 18; cpu_rld(z);
    z->t += 7; cpu_wr(z, DE(z), z->a);
x3554:
    z->t += 13; cpu_wr(z, 0x6830, z->a);
    z->t += 10; goto x3554;
x355A:
    z->t += 10; SET_HL(z, 0x9100);
    z->t += 10; cpu_wr(z, HL(z), 0x00);
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 10; cpu_wr(z, HL(z), 0x00);
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 10; cpu_wr(z, HL(z), 0x01);
    z->t += 4; cpu_xor(z, z->a);
    z->t += 13; cpu_wr(z, 0x9270, z->a);
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 13; cpu_wr(z, 0x6823, z->a);
    z->t += 10; SET_DE(z, 0x0000);
    z->t += 7; z->c = 0x00;
    z->t += 17; m_352B(z);
    z->t += 11; cpu_wr(z, HL(z), cpu_inc(z, cpu_rd(z, HL(z))));
    z->t += 7; z->c = 0x00;
    z->t += 17; m_352B(z);
    z->t += 11; cpu_wr(z, HL(z), cpu_inc(z, cpu_rd(z, HL(z))));
    z->t += 7; z->c = 0x00;
    z->t += 17; m_352B(z);
    z->t += 11; cpu_wr(z, HL(z), cpu_inc(z, cpu_rd(z, HL(z))));
    z->t += 7; z->c = 0x00;
    z->t += 17; m_352B(z);
    z->t += 10; cpu_wr(z, HL(z), 0xFF);
x3589:
    z->t += 13; z->a = cpu_rd(z, 0x9100);
    z->t += 13; cpu_wr(z, 0x6830, z->a);
    z->t += 4; cpu_and(z, z->a);
    if (z->zf) { z->t += 12; goto x3589; } z->t += 7;
    z->t += 4; z->a = cpu_inc(z, z->a);
    if (z->zf) { z->t += 12; goto x359C; } z->t += 7;
    z->t += 4; z->a = cpu_dec(z, z->a);
    z->t += 13; cpu_wr(z, 0x9102, z->a);
    z->t += 10; goto x353F;
x359C:
    z->t += 13; z->a = cpu_rd(z, 0x9101);
    z->t += 13; cpu_wr(z, 0x6830, z->a);
    z->t += 4; cpu_and(z, z->a);
    if (z->zf) { z->t += 12; goto x359C; } z->t += 7;
    z->t += 4; z->a = cpu_inc(z, z->a);
    if (z->zf) { z->t += 12; goto x35BF; } z->t += 7;
    z->t += 4; z->a = cpu_dec(z, z->a);
    z->t += 13; cpu_wr(z, 0x9102, z->a);
    z->t += 10; goto x353F;
x35BF:
    z->t += 10; SET_HL(z, 0x3B95);
    z->t += 17; m_331B(z);
    z->t += 17; m_37FE(z);
    z->t += 10; SET_HL(z, 0x9100);
    z->t += 7; z->b = 0x03;
x35CD:
    z->t += 10; cpu_wr(z, HL(z), 0x00);
    z->t += 6; SET_HL(z, HL(z) + 1);
    if (--z->b) { z->t += 13; goto x35CD; } z->t += 8;
    z->t += 7; z->a = 0x20;
    z->t += 13; cpu_wr(z, 0x9000, z->a);
    z->t += 10; SET_HL(z, 0x35AF);
    z->t += 10; SET_DE(z, 0x7000);
    z->t += 10; SET_BC(z, 0x0004);
    z->t += 4; cpu_exx(z);
    z->t += 7; z->a = 0xA1;
    z->t += 13; cpu_wr(z, 0x7100, z->a);
    z->t += 13; cpu_wr(z, 0x6830, z->a);
    z->t += 17; m_37F6(z);
    z->t += 4; cpu_xor(z, z->a);
    z->t += 13; cpu_wr(z, 0x6830, z->a);
    z->t += 13; cpu_wr(z, 0x92A0, z->a);
x35F3:
    z->t += 13; z->a = cpu_rd(z, 0x92A0);
    z->t += 7; cpu_cp(z, 0x02);
    if (!z->zf) { z->t += 12; goto x35F3; } z->t += 7;
    z->t += 10; SET_HL(z, 0x35B3);
    z->t += 10; SET_DE(z, 0x7000);
    z->t += 10; SET_BC(z, 0x000C);
    z->t += 4; cpu_exx(z);
    z->t += 7; z->a = 0xA8;
    z->t += 13; cpu_wr(z, 0x7100, z->a);
    z->t += 13; cpu_wr(z, 0x6830, z->a);
    z->t += 17; m_37F6(z);
    z->t += 13; cpu_wr(z, 0x6830, z->a);
    z->t += 8; /* im 1 */
    z->t += 10; SET_HL(z, 0x6820);
    z->t += 10; cpu_wr(z, HL(z), 0x00);
    z->t += 10; cpu_wr(z, HL(z), 0x01);
    z->t += 4; z->iff = 1; /* EI */
    z->t += 17; m_39FC(z);
    z->t += 4; cpu_xor(z, z->a);
    z->t += 13; cpu_wr(z, 0x92A0, z->a);
x3623:
    z->t += 13; z->a = cpu_rd(z, 0x92A0);
    z->t += 7; cpu_and(z, 0x08);
    if (z->zf) { z->t += 12; goto x3623; } z->t += 7;
x362A:
    z->t += 13; z->a = cpu_rd(z, 0x92A0);
    z->t += 4; z->c = z->a;
x362E:
    z->t += 13; z->a = cpu_rd(z, 0x92A0);
    z->t += 4; cpu_cp(z, z->c);
    if (z->zf) { z->t += 12; goto x362E; } z->t += 7;
    z->t += 10; SET_HL(z, 0x9116);
    z->t += 10; SET_DE(z, 0x9117);
    z->t += 10; SET_BC(z, 0x0007);
    cpu_lddr(z);
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 10; SET_DE(z, 0x99B5);
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 8; cpu_bit(z, 7, z->a);
    z->t += 10; if (!z->zf) { goto x36C4; }
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; cpu_or(z, cpu_rd(z, HL(z)));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 4; cpu_cpl(z);
    z->t += 7; cpu_and(z, cpu_rd(z, HL(z)));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; cpu_and(z, cpu_rd(z, HL(z)));
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 4; z->b = z->a;
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 6; SET_DE(z, DE(z) + 1);
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; cpu_or(z, cpu_rd(z, HL(z)));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 4; cpu_cpl(z);
    z->t += 7; cpu_and(z, cpu_rd(z, HL(z)));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; cpu_and(z, cpu_rd(z, HL(z)));
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 4; z->l = z->a;
    z->t += 4; z->h = z->b;
    z->t += 7; z->b = 0x10;
x3663:
    z->t += 11; SET_HL(z, cpu_add16(z, HL(z), HL(z)));
    if (z->cf) { z->t += 17; m_39E0(z); } else z->t += 10;
    if (--z->b) { z->t += 13; goto x3663; } z->t += 8;
    z->t += 17; m_37FE(z);
    z->t += 16; SET_HL(z, cpu_rd16(z, 0x9272));
    z->t += 4; z->a = z->h;
    z->t += 4; cpu_or(z, z->l);
    if (z->zf) { z->t += 12; goto x367C; } z->t += 7;
    z->t += 6; SET_HL(z, HL(z) - 1);
    z->t += 16; game_wr16(z->g, 0x9272, HL(z));
    z->t += 4; z->a = z->h;
    z->t += 4; cpu_or(z, z->l);
    if (z->zf) { z->t += 17; m_39C5(z); } else z->t += 10;
x367C:
    z->t += 13; z->a = cpu_rd(z, 0x9110);
    z->t += 4; cpu_rra(z);
    if (!z->cf) { z->t += 12; goto x3689; } z->t += 7;
    z->t += 4; cpu_xor(z, z->a);
    z->t += 13; cpu_wr(z, 0x9271, z->a);
    z->t += 10; goto x362A;
x3689:
    z->t += 13; z->a = cpu_rd(z, 0x9117);
    z->t += 7; cpu_and(z, 0x0F);
    z->t += 10; if (z->zf) { goto x362A; }
    z->t += 4; z->c = z->a;
    z->t += 10; SET_HL(z, 0x378C);
    z->t += 10; SET_DE(z, 0x9271);
    z->t += 7; z->a = cpu_rd(z, DE(z));
    cpu_rst10(z);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; cpu_cp(z, z->c);
    if (z->zf) { z->t += 12; goto x36A3; } z->t += 7;
    z->t += 4; cpu_xor(z, z->a);
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 10; goto x362A;
x36A3:
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 11; cpu_wr(z, HL(z), cpu_inc(z, cpu_rd(z, HL(z))));
    z->t += 6; SET_DE(z, DE(z) + 1);
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 10; if (!z->zf) { goto x362A; }
    z->t += 17; m_3962(z);
    z->t += 17; m_397C(z);
    z->t += 10; SET_DE(z, 0x37A2);
    z->t += 10; SET_HL(z, 0x8042);
    z->t += 7; z->b = 0x1C;
x36B9:
    z->t += 17; m_3770(z);
    if (--z->b) { z->t += 13; goto x36B9; } z->t += 8;
x36BE:
    z->t += 13; z->a = cpu_rd(z, 0x99B5);
    z->t += 4; cpu_add(z, z->a);
    if (!z->cf) { z->t += 12; goto x36BE; } z->t += 7;
x36C4:
    z->t += 4; cpu_xor(z, z->a);
    z->t += 13; cpu_wr(z, 0x92A0, z->a);
x36C8:
    z->t += 13; z->a = cpu_rd(z, 0x92A0);
    z->t += 7; cpu_cp(z, 0x08);
    if (z->cf) { z->t += 12; goto x36C8; } z->t += 7;
    z->t += 13; z->a = cpu_rd(z, 0x99B5);
    z->t += 4; cpu_add(z, z->a);
    z->t += 10; if (!z->cf) { goto x362A; }
    z->t += 17; m_397C(z);
    z->t += 10; SET_HL(z, 0x8000);
    z->t += 7; z->b = 0x10;
x36DE:
    z->t += 10; cpu_wr(z, HL(z), 0x28);
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 10; cpu_wr(z, HL(z), 0x27);
    z->t += 6; SET_HL(z, HL(z) + 1);
    if (--z->b) { z->t += 13; goto x36DE; } z->t += 8;
    z->t += 7; z->b = 0x10;
x36E8:
    z->t += 10; cpu_wr(z, HL(z), 0x2D);
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 10; cpu_wr(z, HL(z), 0x2B);
    z->t += 6; SET_HL(z, HL(z) + 1);
    if (--z->b) { z->t += 13; goto x36E8; } z->t += 8;
    z->t += 7; z->b = 0x10;
x36F2:
    z->t += 10; cpu_wr(z, HL(z), 0x28);
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 10; cpu_wr(z, HL(z), 0x2D);
    z->t += 6; SET_HL(z, HL(z) + 1);
    if (--z->b) { z->t += 13; goto x36F2; } z->t += 8;
    z->t += 7; z->b = 0x10;
x36FC:
    z->t += 10; cpu_wr(z, HL(z), 0x27);
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 10; cpu_wr(z, HL(z), 0x2B);
    z->t += 6; SET_HL(z, HL(z) + 1);
    if (--z->b) { z->t += 13; goto x36FC; } z->t += 8;
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 10; SET_HL(z, 0x8040);
    z->t += 10; SET_BC(z, 0x0340);
    cpu_ldir(z);
    z->t += 10; SET_HL(z, 0x8000);
    z->t += 10; SET_BC(z, 0x0040);
    cpu_ldir(z);
    z->t += 4; cpu_xor(z, z->a);
    z->t += 13; cpu_wr(z, 0x92A0, z->a);
x3719:
    z->t += 13; z->a = cpu_rd(z, 0x92A0);
    z->t += 4; cpu_add(z, z->a);
    if (!z->cf) { z->t += 12; goto x3719; } z->t += 7;
x371F:
    z->t += 13; z->a = cpu_rd(z, 0x99B5);
    z->t += 4; cpu_add(z, z->a);
    if (!z->cf) { z->t += 12; goto x371F; } z->t += 7;
    z->t += 4; z->iff = 0; /* DI */
    z->t += 17; m_37F6(z);
    z->t += 7; z->a = 0xFE;
    z->t += 13; cpu_wr(z, 0x92A0, z->a);
x372E:
    z->t += 13; z->a = cpu_rd(z, 0x92A0);
    z->t += 4; cpu_and(z, z->a);
    if (!z->zf) { z->t += 12; goto x372E; } z->t += 7;
    z->t += 13; cpu_wr(z, 0x6830, z->a);
x3737:
    z->t += 10; SET_HL(z, 0x9280);
    z->t += 10; SET_DE(z, 0x7000);
    z->t += 10; SET_BC(z, 0x0008);
    z->t += 4; cpu_exx(z);
    z->t += 7; z->a = 0xE1;
    z->t += 13; cpu_wr(z, 0x7100, z->a);
    z->t += 17; m_37F6(z);
    z->t += 10; SET_HL(z, 0x7000);
    z->t += 10; SET_DE(z, 0x9288);
    z->t += 10; SET_BC(z, 0x0003);
    z->t += 4; cpu_exx(z);
    z->t += 7; z->a = 0xB1;
    z->t += 13; cpu_wr(z, 0x7100, z->a);
    z->t += 17; m_37F6(z);
    z->t += 13; z->a = cpu_rd(z, 0x9288);
    z->t += 7; cpu_cp(z, 0xA1);
    if (!z->cf) { z->t += 12; goto x3737; } z->t += 7;
    z->t += 7; cpu_and(z, 0x0F);
    z->t += 7; cpu_cp(z, 0x0A);
    if (!z->cf) { z->t += 12; goto x3737; } z->t += 7;
    z->t += 4; z->iff = 1; /* EI */
    z->t += 4; cpu_xor(z, z->a);
    z->t += 13; cpu_wr(z, 0x8210, z->a);
    z->t += 10; goto x02D3;
}

static void t_1700(Cpu *z)
{
    z->t += 20; SET_DE(z, cpu_rd16(z, 0x9282));
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 4; cpu_rlca(z);
    z->t += 4; cpu_rlca(z);
    z->t += 4; cpu_rlca(z);
    z->t += 7; cpu_and(z, 0x07);
    z->t += 10; SET_HL(z, 0x1713);
    cpu_rst08(z);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; z->h = cpu_rd(z, HL(z));
    z->t += 4; z->l = z->a;
    z->t += 4; switch (HL(z)) { case 0x171F: goto x171F; case 0x172D: goto x172D; case 0x1734: goto x1734; case 0x1766: goto x1766; default: game_unported(z->g, HL(z)); return; }
x171F:
    z->t += 13; z->a = cpu_rd(z, 0x92A0);
    z->t += 7; cpu_and(z, 0x0F);
    if (!z->zf) { z->t += 11; return; } z->t += 5;
    z->t += 10; SET_HL(z, 0x9207);
    z->t += 11; cpu_wr(z, HL(z), cpu_dec(z, cpu_rd(z, HL(z))));
    if (!z->zf) { z->t += 11; return; } z->t += 5;
    z->t += 10; goto x1766;
x172D:
    z->t += 17; m_1F19(z);
    z->t += 20; SET_DE(z, cpu_rd16(z, 0x9282));
x1734:
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 10; SET_HL(z, 0x9827);
    z->t += 7; z->e = cpu_rd(z, HL(z));
    z->t += 8; cpu_bit(z, 0, z->a);
    if (!z->zf) { z->t += 12; goto x1741; } z->t += 7;
    z->t += 7; cpu_and(z, 0x0A);
    z->t += 12; goto x1755;
x1741:
    z->t += 13; z->a = cpu_rd(z, 0x9209);
    z->t += 4; z->l = z->a;
    z->t += 7; z->h = 0x93;
    z->t += 13; z->a = cpu_rd(z, 0x9362);
    z->t += 7; cpu_sub(z, cpu_rd(z, HL(z)));
    z->t += 7; z->a = 0x0A;
    if (z->zf) { z->t += 12; goto x1755; } z->t += 7;
    z->t += 7; z->a = 0x08;
    if (z->cf) { z->t += 12; goto x1755; } z->t += 7;
    z->t += 7; z->a = 0x02;
x1755:
    z->t += 17; m_1F9C(z);
    z->t += 13; z->a = cpu_rd(z, 0x92A0);
    z->t += 7; cpu_and(z, 0x03);
    if (!z->zf) { z->t += 11; return; } z->t += 5;
    z->t += 10; SET_HL(z, 0x9207);
    z->t += 11; cpu_wr(z, HL(z), cpu_dec(z, cpu_rd(z, HL(z))));
    if (!z->zf) { z->t += 11; return; } z->t += 5;
    z->t += 17; m_1F19(z);
x1766:
    z->t += 20; SET_DE(z, cpu_rd16(z, 0x9282));
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 7; cpu_and(z, 0xC0);
    z->t += 7; cpu_cp(z, 0x80);
    if (!z->zf) { z->t += 12; goto x1772; } z->t += 7;
    z->t += 6; SET_DE(z, DE(z) + 1);
x1772:
    z->t += 6; SET_DE(z, DE(z) + 1);
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 20; game_wr16(z->g, 0x9282, DE(z));
    z->t += 4; cpu_rlca(z);
    z->t += 4; cpu_rlca(z);
    z->t += 4; cpu_rlca(z);
    z->t += 7; cpu_and(z, 0x07);
    z->t += 10; SET_HL(z, 0x1786);
    cpu_rst08(z);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; z->h = cpu_rd(z, HL(z));
    z->t += 4; z->l = z->a;
    z->t += 4; switch (HL(z)) { case 0x1794: goto x1794; case 0x179C: goto x179C; case 0x17A1: goto x17A1; case 0x17A8: goto x17A8; case 0x17AE: goto x17AE; default: game_unported(z->g, HL(z)); return; }
x1794:
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 4; cpu_rlca(z);
    z->t += 7; cpu_and(z, 0x7E);
    z->t += 13; cpu_wr(z, 0x9209, z->a);
    z->t += 10; return;
x179C:
    z->t += 4; cpu_xor(z, z->a);
    z->t += 13; cpu_wr(z, 0x9003, z->a);
    z->t += 10; return;
x17A1:
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 7; cpu_and(z, 0x1F);
x17A4:
    z->t += 13; cpu_wr(z, 0x9207, z->a);
    z->t += 10; return;
x17A8:
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 7; cpu_and(z, 0x1F);
    z->t += 4; z->c = z->a;
    z->t += 11; m_0030(z);
    z->t += 10; return;
x17AE:
    z->t += 6; SET_DE(z, DE(z) + 1);
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 12; goto x17A4;
}

static void t_17B2(Cpu *z)
{
    z->t += 13; z->a = cpu_rd(z, 0x9201);
    z->t += 4; z->a = cpu_dec(z, z->a);
    if (!z->zf) { z->t += 11; return; } z->t += 5;
    z->t += 13; z->a = cpu_rd(z, 0x9203);
    z->t += 10; SET_HL(z, 0x17C3);
    cpu_rst08(z);
    z->t += 7; z->e = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; z->d = cpu_rd(z, HL(z));
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 4; switch (HL(z)) { case 0x17E1: goto x17E1; case 0x17F5: goto x17F5; case 0x1808: goto x1808; case 0x1840: goto x1840; case 0x1852: goto x1852; case 0x18AC: goto x18AC; case 0x18D1: goto x18D1; case 0x18D9: goto x18D9; case 0x1940: goto x1940; case 0x1948: goto x1948; case 0x1984: goto x1984; default: game_unported(z->g, HL(z)); return; }
x17E1:
    z->t += 13; z->a = cpu_rd(z, 0x92AF);
    z->t += 4; cpu_and(z, z->a);
    if (z->zf) { z->t += 12; goto x17EC; } z->t += 7;
    z->t += 4; z->a = cpu_dec(z, z->a);
    z->t += 10; if (z->zf) { goto x19A7; }
    z->t += 10; return;
x17EC:
    z->t += 17; m_3214(z);
    z->t += 7; z->a = 0x0A;
    z->t += 13; cpu_wr(z, 0x92AF, z->a);
    z->t += 10; return;
x17F5:
    z->t += 13; z->a = cpu_rd(z, 0x92A0);
    z->t += 7; cpu_and(z, 0x1F);
    z->t += 7; cpu_cp(z, 0x1F);
    if (!z->zf) { z->t += 11; return; } z->t += 5;
    z->t += 7; z->a = 0x01;
    z->t += 13; cpu_wr(z, 0x9005, z->a);
    z->t += 7; z->c = 0x02;
    z->t += 11; m_0030(z);
    z->t += 10; goto x19A7;
x1808:
    z->t += 17; m_134C(z);
    z->t += 10; SET_HL(z, 0x181F);
    z->t += 16; game_wr16(z->g, 0x9282, HL(z));
    z->t += 7; z->a = 0x01;
    z->t += 13; cpu_wr(z, 0x9003, z->a);
    z->t += 13; cpu_wr(z, 0x9015, z->a);
    z->t += 13; cpu_wr(z, 0x9025, z->a);
    z->t += 10; goto x19A7;
x1840:
    z->t += 11; m_0028(z);
    z->t += 17; m_1242(z);
    z->t += 4; cpu_xor(z, z->a);
    z->t += 13; cpu_wr(z, 0x9010, z->a);
    z->t += 13; cpu_wr(z, 0x920B, z->a);
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 13; cpu_wr(z, 0x9002, z->a);
    z->t += 10; goto x19A7;
x1852:
    z->t += 4; cpu_xor(z, z->a);
    z->t += 13; cpu_wr(z, 0x982B, z->a);
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 13; cpu_wr(z, 0x9AB7, z->a);
    z->t += 13; cpu_wr(z, 0x9821, z->a);
    z->t += 13; cpu_wr(z, 0x9003, z->a);
    z->t += 13; cpu_wr(z, 0x9015, z->a);
    z->t += 13; cpu_wr(z, 0x9825, z->a);
    z->t += 10; SET_HL(z, 0x1887);
    z->t += 16; game_wr16(z->g, 0x9282, HL(z));
    z->t += 17; m_01C5(z);
    z->t += 17; m_134C(z);
    z->t += 7; z->a = 0x01;
    z->t += 13; cpu_wr(z, 0x920B, z->a);
    z->t += 13; cpu_wr(z, 0x9842, z->a);
    z->t += 13; cpu_wr(z, 0x982C, z->a);
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 13; cpu_wr(z, 0x99C4, z->a);
    z->t += 13; cpu_wr(z, 0x99C5, z->a);
    z->t += 10; goto x19A7;
x18AC:
    z->t += 13; z->a = cpu_rd(z, 0x92AE);
    z->t += 4; cpu_and(z, z->a);
    if (z->zf) { z->t += 12; goto x18BB; } z->t += 7;
    z->t += 4; z->a = cpu_dec(z, z->a);
    z->t += 10; if (z->zf) { goto x19A7; }
    z->t += 7; cpu_cp(z, 0x05);
    if (z->zf) { z->t += 12; goto x18C6; } z->t += 7;
    z->t += 10; return;
x18BB:
    z->t += 7; z->a = 0x34;
    z->t += 13; cpu_wr(z, 0x9234, z->a);
    z->t += 7; z->a = 0x09;
    z->t += 13; cpu_wr(z, 0x92AE, z->a);
    z->t += 10; return;
x18C6:
    z->t += 4; cpu_xor(z, z->a);
    z->t += 13; cpu_wr(z, 0x9362, z->a);
    z->t += 7; z->c = 0x13;
    z->t += 11; m_0030(z);
    z->t += 7; z->c = 0x14;
    z->t += 11; m_0030(z);
    z->t += 10; return;
x18D1:
    z->t += 13; z->a = cpu_rd(z, 0x9003);
    z->t += 4; cpu_and(z, z->a);
    z->t += 10; if (z->zf) { goto x19A7; }
    z->t += 10; return;
x18D9:
    z->t += 7; z->b = 0x07;
x18DB:
    z->t += 17; m_129E(z);
    if (--z->b) { z->t += 13; goto x18DB; } z->t += 8;
    z->t += 4; cpu_xor(z, z->a);
    z->t += 13; cpu_wr(z, 0x9820, z->a);
    z->t += 13; cpu_wr(z, 0x9005, z->a);
    z->t += 17; m_134C(z);
    z->t += 10; SET_HL(z, 0xFF0D);
    z->t += 16; game_wr16(z->g, 0x92C5, HL(z));
    z->t += 16; game_wr16(z->g, 0x92C4, HL(z));
    z->t += 16; game_wr16(z->g, 0x92C1, HL(z));
    z->t += 16; game_wr16(z->g, 0x92C0, HL(z));
    z->t += 10; SET_HL(z, 0x1928);
    z->t += 16; game_wr16(z->g, 0x9282, HL(z));
    z->t += 4; cpu_xor(z, z->a);
    z->t += 7; z->b = 0x10;
    z->t += 10; SET_HL(z, 0x92CA);
    z->t += 11; m_0018(z);
    z->t += 13; cpu_wr(z, 0x9827, z->a);
    z->t += 13; cpu_wr(z, 0x920B, z->a);
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 13; cpu_wr(z, 0x982B, z->a);
    z->t += 13; cpu_wr(z, 0x9010, z->a);
    z->t += 13; cpu_wr(z, 0x900B, z->a);
    z->t += 13; cpu_wr(z, 0x9003, z->a);
    z->t += 13; z->a = cpu_rd(z, 0x6803);
    z->t += 4; cpu_rrca(z);
    z->t += 7; cpu_and(z, 0x01);
    z->t += 13; cpu_wr(z, 0x9AB7, z->a);
    z->t += 17; m_127B(z);
    z->t += 10; goto x19A7;
x1940:
    z->t += 17; m_0160(z);
    z->t += 17; m_003C(z);
    z->t += 12; goto x19A7;
x1948:
    z->t += 10; SET_HL(z, 0x195C);
    z->t += 16; game_wr16(z->g, 0x9280, HL(z));
    z->t += 4; cpu_xor(z, z->a);
    z->t += 13; cpu_wr(z, 0x9205, z->a);
    z->t += 13; cpu_wr(z, 0x92A8, z->a);
    z->t += 7; z->a = 0x02;
    z->t += 13; cpu_wr(z, 0x92AE, z->a);
    z->t += 12; goto x19A7;
x1984:
    z->t += 13; z->a = cpu_rd(z, 0x92AE);
    z->t += 4; cpu_and(z, z->a);
    if (!z->zf) { z->t += 11; return; } z->t += 5;
    z->t += 7; z->a = 0x02;
    z->t += 13; cpu_wr(z, 0x92AE, z->a);
    z->t += 13; z->a = cpu_rd(z, 0x9205);
    z->t += 7; cpu_cp(z, 0x05);
    if (z->zf) { z->t += 12; goto x19A7; } z->t += 7;
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 13; cpu_wr(z, 0x9205, z->a);
    z->t += 7; cpu_add(z, 0x0D);
    z->t += 4; z->c = z->a;
    z->t += 11; m_0030(z);
    z->t += 13; z->a = cpu_rd(z, 0x9205);
    z->t += 7; cpu_cp(z, 0x03);
    if (z->cf) { z->t += 11; return; } z->t += 5;
    z->t += 17; m_129E(z);
    z->t += 10; return;
x19A7:
    z->t += 10; SET_HL(z, 0x9203);
    z->t += 11; cpu_wr(z, HL(z), cpu_inc(z, cpu_rd(z, HL(z))));
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_cp(z, 0x0F);
    if (!z->zf) { z->t += 11; return; } z->t += 5;
    z->t += 10; cpu_wr(z, HL(z), 0x00);
    z->t += 10; return;
}

static void t_19B2(Cpu *z)
{
    z->t += 13; z->a = cpu_rd(z, 0x928E);
    z->t += 4; cpu_and(z, z->a);
    if (!z->zf) { z->t += 12; goto x19D2; } z->t += 7;
    z->t += 10; SET_HL(z, 0x92AD);
    z->t += 7; cpu_or(z, cpu_rd(z, HL(z)));
    if (z->zf) { z->t += 12; goto x19E6; } z->t += 7;
    z->t += 7; cpu_cp(z, 0x04);
    if (!z->zf) { z->t += 12; goto x19C7; } z->t += 7;
    z->t += 4; z->a = cpu_dec(z, z->a);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 13; cpu_wr(z, 0x9AA9, z->a);
x19C7:
    z->t += 13; z->a = cpu_rd(z, 0x9829);
    z->t += 7; cpu_add(z, 0x0D);
    z->t += 4; z->l = z->a;
    z->t += 7; z->h = 0x91;
    z->t += 10; cpu_wr(z, HL(z), 0x04);
    z->t += 10; return;
x19D2:
    z->t += 7; z->c = 0x0A;
    z->t += 11; m_0030(z);
    z->t += 7; z->a = 0x06;
    z->t += 13; cpu_wr(z, 0x92AD, z->a);
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 13; cpu_wr(z, 0x8B63, z->a);
    z->t += 4; cpu_xor(z, z->a);
    z->t += 13; cpu_wr(z, 0x928B, z->a);
    z->t += 13; cpu_wr(z, 0x928E, z->a);
    z->t += 10; return;
x19E6:
    z->t += 13; z->a = cpu_rd(z, 0x82D1);
    z->t += 7; cpu_cp(z, 0x24);
    if (z->zf) { z->t += 12; goto x1A16; } z->t += 7;
    z->t += 10; SET_HL(z, 0x9362);
    z->t += 13; z->a = cpu_rd(z, 0x9828);
    z->t += 7; cpu_and(z, 0x07);
    z->t += 4; z->e = z->a;
    z->t += 4; z->d = z->h;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 10; cpu_wr(z, HL(z), 0x00);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 4; z->e = cpu_inc(z, z->e);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 7; z->h = 0x9B;
    z->t += 4; z->d = z->h;
    z->t += 16; cpu_ldd(z);
    z->t += 16; cpu_ldi(z);
    z->t += 7; z->h = 0x8B;
    z->t += 4; z->l = z->e;
    z->t += 10; cpu_wr(z, HL(z), 0x07);
    z->t += 4; z->l = cpu_dec(z, z->l);
    z->t += 10; cpu_wr(z, HL(z), 0x07);
    z->t += 7; z->c = 0x0B;
    z->t += 10; SET_HL(z, 0x83B1);
    z->t += 17; m_13B3(z);
x1A16:
    z->t += 13; z->a = cpu_rd(z, 0x9828);
    z->t += 4; z->l = z->a;
    z->t += 7; cpu_and(z, 0x07);
    z->t += 4; z->e = z->a;
    z->t += 7; z->h = 0x88;
    z->t += 13; z->a = cpu_rd(z, 0x9215);
    z->t += 4; z->c = z->a;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_cp(z, 0x09);
    if (!z->zf) { z->t += 12; goto x1A45; } z->t += 7;
    z->t += 7; z->h = 0x93;
    z->t += 4; z->d = z->h;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 4; z->e = cpu_inc(z, z->e);
    z->t += 7; z->a = 0x10;
    z->t += 8; cpu_bit(z, 0, z->c);
    if (z->zf) { z->t += 12; goto x1A37; } z->t += 7;
    z->t += 8; cpu_neg(z);
x1A37:
    z->t += 4; z->b = z->a;
    z->t += 7; cpu_add(z, cpu_rd(z, HL(z)));
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 4; cpu_rra(z);
    z->t += 4; cpu_xor(z, z->b);
    z->t += 4; cpu_rlca(z);
    z->t += 7; cpu_and(z, 0x01);
    z->t += 7; z->h = 0x9B;
    z->t += 4; z->d = z->h;
    z->t += 7; cpu_xor(z, cpu_rd(z, HL(z)));
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 10; return;
x1A45:
    z->t += 10; SET_HL(z, 0x928B);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; cpu_and(z, z->a);
    if (!z->zf) { z->t += 12; goto x1A51; } z->t += 7;
    z->t += 7; z->d = 0x8B;
    z->t += 7; z->a = 0x06;
    z->t += 7; cpu_wr(z, DE(z), z->a);
x1A51:
    z->t += 11; cpu_wr(z, HL(z), cpu_inc(z, cpu_rd(z, HL(z))));
    z->t += 7; cpu_cp(z, 0x24);
    if (z->zf) { z->t += 12; goto x1A70; } z->t += 7;
    z->t += 7; z->b = 0x01;
    z->t += 8; cpu_bit(z, 0, z->c);
    if (!z->zf) { z->t += 12; goto x1A5E; } z->t += 7;
    z->t += 4; z->b = cpu_dec(z, z->b);
    z->t += 4; z->b = cpu_dec(z, z->b);
x1A5E:
    z->t += 4; z->l = z->e;
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 7; z->h = 0x93;
    z->t += 4; z->a = z->b;
    z->t += 7; cpu_add(z, cpu_rd(z, HL(z)));
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 4; cpu_rra(z);
    z->t += 4; cpu_xor(z, z->b);
    z->t += 4; cpu_rlca(z);
    if (!z->cf) { z->t += 11; return; } z->t += 5;
    z->t += 7; z->h = 0x9B;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_xor(z, 0x01);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 10; return;
x1A70:
    z->t += 4; cpu_xor(z, z->a);
    z->t += 13; cpu_wr(z, 0x9011, z->a);
    z->t += 13; cpu_wr(z, 0x9AA9, z->a);
    z->t += 7; z->d = 0x88;
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 13; cpu_wr(z, 0x9828, z->a);
    z->t += 13; cpu_wr(z, 0x99B9, z->a);
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 13; cpu_wr(z, 0x9213, z->a);
    z->t += 10; return;
}

static void t_1A86(Cpu *z)
{
    z->t += 13; z->a = cpu_rd(z, 0x99CA);
    z->t += 4; z->c = z->a;
    z->t += 13; z->a = cpu_rd(z, 0x92A7);
    z->t += 4; cpu_cp(z, z->c);
    if (!z->cf) { z->t += 11; return; } z->t += 5;
    z->t += 13; z->a = cpu_rd(z, 0x9841);
    z->t += 4; cpu_and(z, z->a);
    if (!z->zf) { z->t += 12; goto x1ADB; } z->t += 7;
    z->t += 10; SET_HL(z, 0x8807);
    z->t += 10; SET_BC(z, 0x14FF);
    z->t += 7; z->a = 0x01;
x1A9D:
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 16; cpu_cpi(z);
    if (z->zf) { z->t += 12; goto x1AB1; } z->t += 7;
    if (--z->b) { z->t += 13; goto x1A9D; } z->t += 8;
    z->t += 10; SET_HL(z, 0x883F);
    z->t += 7; z->b = 0x10;
x1AA9:
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 16; cpu_cpi(z);
    if (z->zf) { z->t += 12; goto x1AB1; } z->t += 7;
    if (--z->b) { z->t += 13; goto x1AA9; } z->t += 8;
    z->t += 10; return;
x1AB1:
    z->t += 7; z->a = 0xC0;
    z->t += 13; cpu_wr(z, 0x9841, z->a);
    z->t += 4; z->l = cpu_dec(z, z->l);
    z->t += 4; z->e = z->l;
    z->t += 7; z->d = 0x8B;
    z->t += 4; z->e = cpu_inc(z, z->e);
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 4; z->e = cpu_dec(z, z->e);
    z->t += 4; z->c = z->a;
    z->t += 13; z->a = cpu_rd(z, 0x9821);
    z->t += 8; z->a = cpu_srl(z, z->a);
    z->t += 8; z->a = cpu_srl(z, z->a);
    z->t += 4; z->l = z->a;
    z->t += 7; z->h = 0x00;
    z->t += 7; z->a = 0x03;
    z->t += 17; m_1061(z);
    z->t += 7; cpu_add(z, 0x04);
    z->t += 10; SET_HL(z, 0x982D);
    z->t += 7; cpu_wr(z, HL(z), z->e);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 7; cpu_wr(z, HL(z), z->c);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 13; cpu_wr(z, 0x9AB2, z->a);
    z->t += 10; return;
x1ADB:
    z->t += 4; z->a = cpu_inc(z, z->a);
    if (z->zf) { z->t += 12; goto x1AFA; } z->t += 7;
    z->t += 13; cpu_wr(z, 0x9841, z->a);
    z->t += 4; cpu_ex_af(z);
    z->t += 10; SET_HL(z, 0x982D);
    z->t += 7; z->e = cpu_rd(z, HL(z));
    z->t += 7; z->d = 0x88;
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 4; z->a = cpu_dec(z, z->a);
    z->t += 10; if (!z->zf) { goto x1B5A; }
    z->t += 7; z->d = 0x8B;
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 4; cpu_ex_af(z);
    z->t += 8; cpu_bit(z, 4, z->a);
    if (z->zf) { z->t += 12; goto x1AF6; } z->t += 7;
    z->t += 4; z->l = cpu_inc(z, z->l);
x1AF6:
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; z->e = cpu_inc(z, z->e);
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 10; return;
x1AFA:
    z->t += 13; z->a = cpu_rd(z, 0x9015);
    z->t += 4; cpu_and(z, z->a);
    if (!z->zf) { z->t += 12; goto x1B06; } z->t += 7;
    z->t += 7; z->a = 0xE0;
    z->t += 13; cpu_wr(z, 0x9841, z->a);
    z->t += 10; return;
x1B06:
    z->t += 13; z->a = cpu_rd(z, 0x982D);
    z->t += 4; z->l = z->a;
    z->t += 7; z->h = 0x88;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; z->a = cpu_dec(z, z->a);
    if (!z->zf) { z->t += 12; goto x1B5A; } z->t += 7;
    z->t += 7; z->h = 0x92;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 8; cpu_bit(z, 7, z->a);
    if (!z->zf) { z->t += 12; goto x1B5A; } z->t += 7;
    z->t += 13; z->a = cpu_rd(z, 0x982F);
    z->t += 7; cpu_sub(z, 0x04);
    z->t += 10; SET_HL(z, 0x1B5F);
    cpu_rst08(z);
    z->t += 10; SET_DE(z, 0x99B0);
    z->t += 7; z->a = 0x03;
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 4; z->e = cpu_inc(z, z->e);
    z->t += 16; cpu_ldi(z);
    z->t += 16; cpu_ldi(z);
    z->t += 13; z->a = cpu_rd(z, 0x982F);
    z->t += 7; cpu_sub(z, 0x04);
    z->t += 7; cpu_and(z, 0x0F);
    z->t += 4; z->c = z->a;
    z->t += 10; SET_HL(z, 0x1B65);
    cpu_rst08(z);
    z->t += 7; z->e = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; z->d = cpu_rd(z, HL(z));
    z->t += 7; z->h = 0x8B;
    z->t += 13; z->a = cpu_rd(z, 0x982D);
    z->t += 4; z->l = z->a;
    z->t += 4; z->a = z->c;
    z->t += 4; cpu_rlca(z);
    z->t += 4; cpu_rlca(z);
    z->t += 4; cpu_rlca(z);
    z->t += 7; cpu_add(z, 0x56);
    z->t += 7; z->c = cpu_rd(z, HL(z));
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 4; z->a = z->c;
    z->t += 7; cpu_and(z, 0xF8);
    z->t += 4; z->c = z->a;
    z->t += 13; z->a = cpu_rd(z, 0x982E);
    z->t += 7; cpu_and(z, 0x07);
    z->t += 4; cpu_or(z, z->c);
    z->t += 13; cpu_wr(z, 0x982E, z->a);
    z->t += 7; z->h = 0x88;
    z->t += 17; m_1083(z);
x1B5A:
    z->t += 4; cpu_xor(z, z->a);
    z->t += 13; cpu_wr(z, 0x9004, z->a);
    z->t += 10; return;
}

static void t_1B6B(Cpu *z)
{
    z->t += 13; z->a = cpu_rd(z, 0x920B);
    z->t += 4; cpu_and(z, z->a);
    if (z->zf) { z->t += 12; goto x1B7B; } z->t += 7;
    z->t += 13; z->a = cpu_rd(z, 0x9015);
    z->t += 4; z->c = z->a;
    z->t += 13; z->a = cpu_rd(z, 0x901D);
    z->t += 4; cpu_cpl(z);
    z->t += 4; cpu_and(z, z->c);
    if (z->zf) { z->t += 11; return; } z->t += 5;
x1B7B:
    z->t += 7; z->b = 0x04;
    z->t += 10; SET_HL(z, 0x92CA);
x1B80:
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; z->a = cpu_inc(z, z->a);
    if (!z->zf) { z->t += 12; goto x1B91; } z->t += 7;
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 4; z->l = cpu_inc(z, z->l);
    if (--z->b) { z->t += 13; goto x1B80; } z->t += 8;
    z->t += 13; z->a = cpu_rd(z, 0x92A0);
    z->t += 7; cpu_and(z, 0x0F);
    if (z->zf) { z->t += 12; goto x1BAE; } z->t += 7;
    z->t += 10; return;
x1B91:
    z->t += 10; cpu_wr(z, HL(z), 0xFF);
    z->t += 4; z->a = cpu_dec(z, z->a);
    z->t += 7; z->d = 0x88;
    z->t += 4; z->e = z->a;
    z->t += 8; z->e = (uint8_t)(z->e & 0x7F);
    z->t += 4; cpu_ex_af(z);
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 4; z->a = cpu_dec(z, z->a);
    if (!z->zf) { z->t += 11; return; } z->t += 5;
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 7; z->e = cpu_rd(z, HL(z));
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 7; z->d = cpu_rd(z, HL(z));
    z->t += 4; cpu_ex_af(z);
    z->t += 4; z->l = z->a;
    z->t += 7; z->h = 0x88;
    z->t += 17; m_1079(z);
    z->t += 7; z->a = 0x01;
    z->t += 13; cpu_wr(z, 0x9AB3, z->a);
    z->t += 10; return;
x1BAE:
    z->t += 10; SET_HL(z, 0x92C0);
    z->t += 7; z->b = 0x03;
x1BB3:
    z->t += 11; cpu_wr(z, HL(z), cpu_dec(z, cpu_rd(z, HL(z))));
    if (z->zf) { z->t += 12; goto x1BBA; } z->t += 7;
    z->t += 4; z->l = cpu_inc(z, z->l);
    if (--z->b) { z->t += 13; goto x1BB3; } z->t += 8;
    z->t += 10; return;
x1BBA:
    z->t += 13; z->a = cpu_rd(z, 0x99C4);
    z->t += 4; z->c = z->a;
    z->t += 13; z->a = cpu_rd(z, 0x9287);
    z->t += 4; cpu_cp(z, z->c);
    if (z->cf) { z->t += 12; goto x1BC6; } z->t += 7;
    z->t += 11; cpu_wr(z, HL(z), cpu_inc(z, cpu_rd(z, HL(z))));
    z->t += 10; return;
x1BC6:
    z->t += 8; z->l = (uint8_t)(z->l | 0x04);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 8; z->l = (uint8_t)(z->l & 0xFB);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 4; z->a = z->b;
    z->t += 4; z->a = cpu_dec(z, z->a);
    z->t += 10; SET_HL(z, 0x1BD7);
    cpu_rst08(z);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; z->h = cpu_rd(z, HL(z));
    z->t += 4; z->l = z->a;
    z->t += 4; switch (HL(z)) { case 0x1BDD: goto x1BDD; case 0x1BFD: goto x1BFD; case 0x1C07: goto x1C07; default: game_unported(z->g, HL(z)); return; }
x1BDD:
    z->t += 7; z->b = 0x14;
    z->t += 10; SET_HL(z, 0x8808);
    z->t += 10; SET_DE(z, 0x034F);
x1BE5:
    z->t += 13; z->a = cpu_rd(z, 0x982D);
    z->t += 4; z->c = z->a;
x1BE9:
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; z->a = cpu_dec(z, z->a);
    if (!z->zf) { z->t += 12; goto x1BF1; } z->t += 7;
    z->t += 4; z->a = z->c;
    z->t += 4; cpu_cp(z, z->l);
    if (!z->zf) { z->t += 12; goto x1BF6; } z->t += 7;
x1BF1:
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 4; z->l = cpu_inc(z, z->l);
    if (--z->b) { z->t += 13; goto x1BE9; } z->t += 8;
    z->t += 10; return;
x1BF6:
    z->t += 13; cpu_wr(z, 0x9AB3, z->a);
    z->t += 17; m_1083(z);
    z->t += 10; return;
x1BFD:
    z->t += 7; z->b = 0x10;
    z->t += 10; SET_HL(z, 0x8840);
    z->t += 10; SET_DE(z, 0x03A9);
    z->t += 12; goto x1BE5;
x1C07:
    z->t += 13; z->a = cpu_rd(z, 0x982B);
    z->t += 4; cpu_and(z, z->a);
    if (!z->zf) { z->t += 12; goto x1C36; } z->t += 7;
    z->t += 10; SET_HL(z, 0x982C);
    z->t += 11; cpu_wr(z, HL(z), cpu_inc(z, cpu_rd(z, HL(z))));
    z->t += 12; cpu_bit(z, 0, cpu_rd(z, HL(z)));
    if (!z->zf) { z->t += 12; goto x1C36; } z->t += 7;
    z->t += 11; z->ix = (uint16_t)((z->ix & 0xFF00) | (uint8_t)(0x02));
    z->t += 14; z->iy = (uint16_t)(0x0454);
    z->t += 10; SET_DE(z, 0x8830);
    z->t += 7; z->b = 0x04;
x1C21:
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 4; z->a = cpu_dec(z, z->a);
    if (z->zf) { z->t += 12; goto x1C2A; } z->t += 7;
    z->t += 4; z->e = cpu_inc(z, z->e);
    z->t += 4; z->e = cpu_inc(z, z->e);
    if (--z->b) { z->t += 13; goto x1C21; } z->t += 8;
    z->t += 10; return;
x1C2A:
    z->t += 7; z->a = 0x01;
    z->t += 13; cpu_wr(z, 0x982B, z->a);
    z->t += 4; z->a = z->e;
    z->t += 13; cpu_wr(z, 0x9828, z->a);
    z->t += 10; goto x1CB4;
x1C36:
    z->t += 10; SET_HL(z, 0x1D32);
    z->t += 7; z->d = 0x88;
    z->t += 10; SET_BC(z, 0x0600);
x1C3E:
    z->t += 7; z->e = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 13; z->a = cpu_rd(z, 0x982D);
    z->t += 4; cpu_cp(z, z->e);
    if (z->zf) { z->t += 12; goto x1C4A; } z->t += 7;
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 4; z->a = cpu_dec(z, z->a);
    z->t += 7; cpu_sub(z, 0x01);
x1C4A:
    z->t += 8; z->c = cpu_rl(z, z->c);
    if (--z->b) { z->t += 13; goto x1C3E; } z->t += 8;
    z->t += 11; z->ix = (uint16_t)((z->ix & 0xFF00) | (uint8_t)(0x00));
    z->t += 7; z->b = 0x04;
    z->t += 8; z->ix = (uint16_t)((z->ix & 0x00FF) | (uint8_t)(z->c) << 8);
x1C55:
    z->t += 4; z->a = z->c;
    z->t += 7; cpu_and(z, 0x07);
    z->t += 7; cpu_cp(z, 0x04);
    if (z->zf) { z->t += 12; goto x1C61; } z->t += 7;
    z->t += 7; cpu_cp(z, 0x03);
    if (!z->cf) { z->t += 17; if (m_1C93(z, 0x1C61)) return; } else z->t += 10;
x1C61:
    z->t += 8; z->c = cpu_rr(z, z->c);
    if (--z->b) { z->t += 13; goto x1C55; } z->t += 8;
    z->t += 8; z->ix = (uint16_t)((z->ix & 0xFF00) | (uint8_t)(cpu_inc(z, (uint8_t)z->ix)));
    z->t += 8; z->c = (uint8_t)(z->ix >> 8);
    z->t += 7; z->b = 0x04;
x1C6B:
    z->t += 4; z->a = z->c;
    z->t += 7; cpu_and(z, 0x07);
    if (!z->zf) { z->t += 17; if (m_1C93(z, 0x1C71)) return; } else z->t += 10;
    z->t += 8; z->c = cpu_rr(z, z->c);
    if (--z->b) { z->t += 13; goto x1C6B; } z->t += 8;
    z->t += 8; z->ix = (uint16_t)((z->ix & 0xFF00) | (uint8_t)(cpu_inc(z, (uint8_t)z->ix)));
    z->t += 10; SET_DE(z, 0x8830);
    z->t += 7; z->b = 0x04;
x1C7C:
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 4; z->a = cpu_dec(z, z->a);
    if (z->zf) { z->t += 12; goto x1CA6; } z->t += 7;
    z->t += 4; z->e = cpu_inc(z, z->e);
    z->t += 4; z->e = cpu_inc(z, z->e);
    if (--z->b) { z->t += 13; goto x1C7C; } z->t += 8;
    z->t += 10; SET_HL(z, 0x8800);
    z->t += 7; z->b = 0x04;
x1C89:
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; z->a = cpu_dec(z, z->a);
    z->t += 10; if (z->zf) { goto x1D2B; }
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 4; z->l = cpu_inc(z, z->l);
    if (--z->b) { z->t += 13; goto x1C89; } z->t += 8;
    z->t += 10; return;
x1CA6:
    z->t += 14; z->iy = (uint16_t)(0x0411);
    z->t += 13; z->a = cpu_rd(z, 0x920B);
    z->t += 4; cpu_and(z, z->a);
    if (!z->zf) { z->t += 12; goto x1CB4; } z->t += 7;
    z->t += 14; z->iy = (uint16_t)(0x00F1);
x1CB4:
    z->t += 4; z->a = z->e;
    z->t += 4; cpu_rrca(z);
    z->t += 4; cpu_rrca(z);
    z->t += 4; z->a = z->e;
    z->t += 4; cpu_rla(z);
    z->t += 4; cpu_rrca(z);
    z->t += 13; cpu_wr(z, 0x92CA, z->a);
    z->t += 4; cpu_ex_af(z);
    z->t += 20; game_wr16(z->g, 0x92CB, z->iy);
    z->t += 4; z->b = cpu_inc(z, z->b);
    z->t += 4; z->a = z->e;
    z->t += 7; cpu_and(z, 0x07);
    z->t += 10; SET_HL(z, 0x9830);
    cpu_rst10(z);
    z->t += 8; z->a = (uint8_t)z->ix;
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 10; SET_HL(z, 0x1D03);
    cpu_rst08(z);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 4; z->e = cpu_inc(z, z->e);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 8; z->a = (uint8_t)z->ix;
    z->t += 7; cpu_cp(z, 0x02);
    if (z->zf) { z->t += 12; goto x1CE9; } z->t += 7;
    z->t += 10; SET_DE(z, 0x92CD);
    z->t += 4; z->a = cpu_dec(z, z->a);
    if (z->zf) { z->t += 12; goto x1CE6; } z->t += 7;
    z->t += 17; m_1D09(z);
x1CE6:
    z->t += 17; m_1D09(z);
x1CE9:
    z->t += 13; z->a = cpu_rd(z, 0x92CA);
    z->t += 7; cpu_and(z, 0x07);
    z->t += 4; z->l = z->a;
    z->t += 7; z->h = 0x88;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; z->a = cpu_dec(z, z->a);
    if (!z->zf) { z->t += 11; return; } z->t += 5;
    z->t += 4; z->c = z->l;
    z->t += 10; SET_HL(z, 0x92CA);
x1CF8:
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; z->a = cpu_inc(z, z->a);
    if (!z->zf) { z->t += 12; goto x1CF8; } z->t += 7;
    z->t += 4; cpu_ex_af(z);
    z->t += 4; z->a = z->c;
    z->t += 12; goto x1D1C;
x1D1C:
    z->t += 4; cpu_rla(z);
    z->t += 4; cpu_rrca(z);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 4; cpu_ex_af(z);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 8; z->a = (uint8_t)z->iy;
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 8; z->a = (uint8_t)(z->iy >> 8);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 10; return;
x1D2B:
    z->t += 10; SET_DE(z, 0x0444);
    z->t += 17; m_1083(z);
    z->t += 10; return;
}

static void t_1D38(Cpu *z)
{
    z->t += 10; SET_HL(z, 0x99B4);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_and(z, 0x7F);
    z->t += 7; cpu_sub(z, 0x7E);
    if (z->zf) { z->t += 12; goto x1D78; } z->t += 7;
    z->t += 7; z->c = cpu_rd(z, HL(z));
    z->t += 11; cpu_wr(z, HL(z), cpu_inc(z, cpu_rd(z, HL(z))));
    z->t += 13; z->a = cpu_rd(z, 0x9215);
    z->t += 8; z->c = cpu_rlc(z, z->c);
    z->t += 4; cpu_xor(z, z->c);
    z->t += 4; cpu_rrca(z);
    z->t += 7; z->a = 0x01;
    if (z->cf) { z->t += 12; goto x1D51; } z->t += 7;
    z->t += 8; cpu_neg(z);
x1D51:
    z->t += 4; z->c = z->a;
    z->t += 10; SET_HL(z, 0x9814);
    z->t += 7; z->b = 0x06;
x1D57:
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; cpu_add(z, z->c);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 4; cpu_rra(z);
    z->t += 4; cpu_xor(z, z->c);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 4; cpu_rlca(z);
    if (!z->cf) { z->t += 12; goto x1D64; } z->t += 7;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_xor(z, 0x01);
    z->t += 7; cpu_wr(z, HL(z), z->a);
x1D64:
    z->t += 4; z->l = cpu_inc(z, z->l);
    if (--z->b) { z->t += 13; goto x1D57; } z->t += 8;
    z->t += 13; z->a = cpu_rd(z, 0x92A0);
    z->t += 7; cpu_and(z, 0xFC);
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 11; cpu_push(z, AF(z));
    z->t += 17; m_23E0(z);
    z->t += 10; cpu_set_af(z, cpu_pop(z));
    z->t += 7; cpu_add(z, 0x02);
    z->t += 17; m_23E0(z);
    z->t += 10; return;
x1D78:
    z->t += 13; cpu_wr(z, 0x900E, z->a);
    z->t += 10; return;
}

static void t_1D7C(Cpu *z)
{
    z->t += 13; z->a = cpu_rd(z, 0x9215);
    z->t += 4; z->b = z->a;
    z->t += 10; SET_HL(z, 0x99B9);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 4; cpu_and(z, z->a);
    if (z->zf) { z->t += 12; goto x1DAE; } z->t += 7;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; cpu_and(z, z->a);
    z->t += 7; z->a = 0xFD;
    if (!z->zf) { z->t += 12; goto x1DA1; } z->t += 7;
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 7; cpu_cp(z, cpu_rd(z, HL(z)));
    if (z->zf) { z->t += 12; goto x1D95; } z->t += 7;
    z->t += 11; cpu_wr(z, HL(z), cpu_inc(z, cpu_rd(z, HL(z))));
x1D95:
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 7; cpu_add(z, cpu_rd(z, HL(z)));
    z->t += 4; z->c = z->a;
    z->t += 7; cpu_and(z, 0x3F);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 4; z->a = z->c;
    z->t += 4; cpu_rlca(z);
    z->t += 4; cpu_rlca(z);
    z->t += 7; cpu_and(z, 0x03);
x1DA1:
    z->t += 8; cpu_bit(z, 0, z->b);
    if (!z->zf) { z->t += 12; goto x1DA7; } z->t += 7;
    z->t += 8; cpu_neg(z);
x1DA7:
    z->t += 4; z->a = cpu_dec(z, z->a);
    z->t += 7; cpu_and(z, 0x07);
x1DAA:
    z->t += 13; cpu_wr(z, 0x99BE, z->a);
    z->t += 10; return;
x1DAE:
    z->t += 4; cpu_xor(z, z->a);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 7; z->a = 0x07;
    z->t += 12; goto x1DAA;
}

static void t_1DB9(Cpu *z)
{
    z->t += 10; SET_HL(z, 0x9200);
    z->t += 7; z->b = 0x30;
x1DBE:
    z->t += 12; cpu_bit(z, 7, cpu_rd(z, HL(z)));
    if (!z->zf) { z->t += 12; goto x1DC7; } z->t += 7;
    z->t += 4; z->l = cpu_inc(z, z->l);
x1DC3:
    z->t += 4; z->l = cpu_inc(z, z->l);
    if (--z->b) { z->t += 13; goto x1DBE; } z->t += 8;
    z->t += 10; return;
x1DC7:
    z->t += 15; cpu_wr(z, HL(z), (uint8_t)(cpu_rd(z, HL(z)) & 0x7F));
    z->t += 7; z->h = 0x88;
    z->t += 10; cpu_wr(z, HL(z), 0x04);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 10; cpu_wr(z, HL(z), 0x40);
    z->t += 7; z->h = 0x8B;
    z->t += 10; cpu_wr(z, HL(z), 0x0A);
    z->t += 7; z->h = 0x92;
    z->t += 12; goto x1DC3;
}

static void t_1DD8(Cpu *z)
{
    z->t += 13; z->a = cpu_rd(z, 0x92A2);
    z->t += 7; cpu_and(z, 0x01);
    if (!z->zf) { z->t += 11; return; } z->t += 5;
    z->t += 10; SET_HL(z, 0x92AC);
    z->t += 7; z->b = 0x04;
x1DE3:
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; cpu_and(z, z->a);
    if (z->zf) { z->t += 12; goto x1DE8; } z->t += 7;
    z->t += 11; cpu_wr(z, HL(z), cpu_dec(z, cpu_rd(z, HL(z))));
x1DE8:
    z->t += 4; z->l = cpu_inc(z, z->l);
    if (--z->b) { z->t += 13; goto x1DE3; } z->t += 8;
    z->t += 10; return;
}

static void t_1DEC(Cpu *z)
{
    z->t += 13; z->a = cpu_rd(z, 0x92A0);
    z->t += 7; cpu_and(z, 0x03);
    if (!z->zf) { z->t += 11; return; } z->t += 5;
    z->t += 10; SET_HL(z, 0x920F);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; z->e = z->a;
    z->t += 7; z->d = 0xFF;
    z->t += 8; cpu_bit(z, 7, z->a);
    if (!z->zf) { z->t += 12; goto x1E02; } z->t += 7;
    z->t += 4; z->d = cpu_inc(z, z->d);
    z->t += 4; z->d = cpu_inc(z, z->d);
    z->t += 11; cpu_wr(z, HL(z), cpu_inc(z, cpu_rd(z, HL(z))));
    z->t += 12; goto x1E03;
x1E02:
    z->t += 11; cpu_wr(z, HL(z), cpu_dec(z, cpu_rd(z, HL(z))));
x1E03:
    z->t += 7; cpu_cp(z, 0x1F);
    if (!z->zf) { z->t += 12; goto x1E09; } z->t += 7;
    z->t += 15; cpu_wr(z, HL(z), (uint8_t)(cpu_rd(z, HL(z)) | 0x80));
x1E09:
    z->t += 7; cpu_cp(z, 0x81);
    if (!z->zf) { z->t += 12; goto x1E0F; } z->t += 7;
    z->t += 15; cpu_wr(z, HL(z), (uint8_t)(cpu_rd(z, HL(z)) & 0x7F));
x1E0F:
    z->t += 7; z->c = cpu_rd(z, HL(z));
    z->t += 7; cpu_and(z, 0x07);
    z->t += 4; z->a = z->d;
    z->t += 13; cpu_wr(z, 0x9211, z->a);
    z->t += 4; z->a = z->e;
    if (!z->zf) { z->t += 12; goto x1E29; } z->t += 7;
    z->t += 10; SET_HL(z, 0x1E6A);
    z->t += 4; z->a = z->c;
    z->t += 7; cpu_and(z, 0x18);
    cpu_rst08(z);
    z->t += 4; z->a = z->e;
    z->t += 10; SET_DE(z, 0x9920);
    z->t += 10; SET_BC(z, 0x0010);
    cpu_ldir(z);
x1E29:
    z->t += 10; SET_HL(z, 0x9215);
    z->t += 4; cpu_rlca(z);
    z->t += 7; cpu_xor(z, cpu_rd(z, HL(z)));
    z->t += 4; cpu_rrca(z);
    z->t += 10; SET_HL(z, 0x9920);
    z->t += 10; SET_DE(z, 0x9900);
    if (!z->cf) { z->t += 12; goto x1E3C; } z->t += 7;
    z->t += 10; SET_BC(z, 0x01FF);
    z->t += 12; goto x1E3F;
x1E3C:
    z->t += 10; SET_BC(z, 0xFF01);
x1E3F:
    z->t += 11; z->ix = (uint16_t)((z->ix & 0xFF00) | (uint8_t)(0x05));
    z->t += 17; m_1E49(z);
    z->t += 4; z->b = z->c;
    z->t += 11; z->ix = (uint16_t)((z->ix & 0xFF00) | (uint8_t)(0x0B));
x1E49:
    z->t += 15; cpu_wr(z, HL(z), cpu_rrc(z, cpu_rd(z, HL(z))));
    if (!z->cf) { z->t += 12; goto x1E62; } z->t += 7;
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 4; cpu_add(z, z->b);
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 7; z->d = 0x98;
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 4; cpu_add(z, z->b);
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 4; cpu_rra(z);
    z->t += 4; cpu_xor(z, z->b);
    z->t += 4; cpu_rlca(z);
    if (!z->cf) { z->t += 12; goto x1E60; } z->t += 7;
    z->t += 4; z->e = cpu_inc(z, z->e);
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 7; cpu_xor(z, 0x01);
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 4; z->e = cpu_dec(z, z->e);
x1E60:
    z->t += 7; z->d = 0x99;
x1E62:
    z->t += 4; z->e = cpu_inc(z, z->e);
    z->t += 4; z->e = cpu_inc(z, z->e);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 8; z->ix = (uint16_t)((z->ix & 0xFF00) | (uint8_t)(cpu_dec(z, (uint8_t)z->ix)));
    if (!z->zf) { z->t += 12; goto x1E49; } z->t += 7;
    z->t += 10; return;
}

static void t_1EAA(Cpu *z)
{
    z->t += 13; z->a = cpu_rd(z, 0x92A0);
    z->t += 7; cpu_and(z, 0x01);
    z->t += 7; cpu_add(z, 0x02);
    z->t += 4; z->b = z->a;
    z->t += 13; z->a = cpu_rd(z, 0x9215);
    z->t += 4; cpu_and(z, z->a);
    z->t += 4; z->a = z->b;
    if (z->zf) { z->t += 12; goto x1EBB; } z->t += 7;
    z->t += 8; cpu_neg(z);
x1EBB:
    z->t += 8; z->ix = (uint16_t)((z->ix & 0x00FF) | (uint8_t)(z->a) << 8);
    z->t += 7; z->l = 0x68;
    z->t += 10; SET_DE(z, 0x92B0);
    z->t += 11; z->ix = (uint16_t)((z->ix & 0xFF00) | (uint8_t)(0x08));
x1EC5:
    z->t += 7; z->h = 0x8B;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_cp(z, 0x30);
    if (!z->zf) { z->t += 12; goto x1F05; } z->t += 7;
    z->t += 7; z->h = 0x93;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; cpu_and(z, z->a);
    if (z->zf) { z->t += 12; goto x1F05; } z->t += 7;
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 7; z->b = cpu_rd(z, HL(z));
    z->t += 4; z->a = z->b;
    z->t += 7; cpu_and(z, 0x7E);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 7; cpu_add(z, cpu_rd(z, HL(z)));
    z->t += 4; z->c = z->a;
    z->t += 7; cpu_and(z, 0x1F);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 4; z->a = z->c;
    z->t += 4; cpu_rlca(z);
    z->t += 4; cpu_rlca(z);
    z->t += 4; cpu_rlca(z);
    z->t += 7; cpu_and(z, 0x07);
    z->t += 8; cpu_bit(z, 7, z->b);
    if (z->zf) { z->t += 12; goto x1EEA; } z->t += 7;
    z->t += 8; cpu_neg(z);
x1EEA:
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 7; cpu_add(z, cpu_rd(z, HL(z)));
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 8; cpu_add(z, (uint8_t)(z->ix >> 8));
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 4; cpu_rra(z);
    z->t += 8; cpu_xor(z, (uint8_t)(z->ix >> 8));
    z->t += 4; cpu_rlca(z);
    if (!z->cf) { z->t += 12; goto x1EFF; } z->t += 7;
    z->t += 7; z->h = 0x9B;
    z->t += 15; cpu_wr(z, HL(z), cpu_rrc(z, cpu_rd(z, HL(z))));
    z->t += 4; cpu_ccf(z);
    z->t += 15; cpu_wr(z, HL(z), cpu_rl(z, cpu_rd(z, HL(z))));
x1EFF:
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 8; z->ix = (uint16_t)((z->ix & 0xFF00) | (uint8_t)(cpu_dec(z, (uint8_t)z->ix)));
    if (!z->zf) { z->t += 12; goto x1EC5; } z->t += 7;
    z->t += 10; return;
x1F05:
    z->t += 7; z->h = 0x88;
    z->t += 10; cpu_wr(z, HL(z), 0x80);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 4; z->e = cpu_inc(z, z->e);
    z->t += 4; z->e = cpu_inc(z, z->e);
    z->t += 12; goto x1EFF;
}

static void t_1F0E(Cpu *z)
{
    z->t += 13; z->a = cpu_rd(z, 0x9215);
    z->t += 7; cpu_add(z, 0xB6);
    z->t += 4; z->l = z->a;
    z->t += 7; z->h = 0x99;
    z->t += 12; cpu_bit(z, 4, cpu_rd(z, HL(z)));
    if (!z->zf) { z->t += 11; return; } z->t += 5;
    z->t += 10; SET_HL(z, 0x9364);
    z->t += 10; SET_DE(z, 0x92A4);
    z->t += 4; cpu_xor(z, z->a);
    z->t += 7; cpu_cp(z, cpu_rd(z, HL(z)));
    if (z->zf) { z->t += 12; goto x1F28; } z->t += 7;
    z->t += 7; z->l = 0x66;
    z->t += 4; z->e = cpu_inc(z, z->e);
    z->t += 7; cpu_cp(z, cpu_rd(z, HL(z)));
    if (!z->zf) { z->t += 11; return; } z->t += 5;
x1F28:
    z->t += 11; cpu_push(z, DE(z));
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 10; SET_HL(z, 0x9B63);
    z->t += 4; z->d = z->h;
    z->t += 4; z->e = cpu_inc(z, z->e);
    z->t += 12; cpu_bit(z, 2, cpu_rd(z, HL(z)));
    if (z->zf) { z->t += 12; goto x1F35; } z->t += 7;
    z->t += 10; SET_DE(z, cpu_pop(z));
    z->t += 10; return;
x1F35:
    z->t += 16; cpu_ldd(z);
    z->t += 7; z->h = 0x93;
    z->t += 4; z->d = z->h;
    z->t += 16; cpu_ldi(z);
    z->t += 16; cpu_ldd(z);
    z->t += 7; z->h = 0x9B;
    z->t += 4; z->d = z->h;
    z->t += 7; z->b = cpu_rd(z, HL(z));
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 13; z->a = cpu_rd(z, 0x9827);
    z->t += 7; cpu_and(z, 0x01);
    z->t += 4; cpu_rlca(z);
    z->t += 4; cpu_rlca(z);
    z->t += 4; cpu_rlca(z);
    z->t += 4; cpu_or(z, z->b);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 7; z->d = 0x8B;
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 4; z->h = z->d;
    z->t += 7; cpu_and(z, 0x07);
    z->t += 7; z->c = 0x30;
    z->t += 7; cpu_cp(z, 0x05);
    if (!z->cf) { z->t += 12; goto x1F60; } z->t += 7;
    z->t += 4; z->c = cpu_inc(z, z->c);
    z->t += 7; cpu_cp(z, 0x02);
    if (!z->cf) { z->t += 12; goto x1F60; } z->t += 7;
    z->t += 4; z->c = cpu_inc(z, z->c);
    z->t += 4; z->c = cpu_inc(z, z->c);
x1F60:
    z->t += 7; cpu_wr(z, HL(z), z->c);
    z->t += 7; cpu_cp(z, 0x04);
    if (z->cf) { z->t += 12; goto x1F68; } z->t += 7;
    z->t += 4; cpu_cpl(z);
    z->t += 7; cpu_add(z, 0x47);
x1F68:
    z->t += 8; z->a = cpu_sla(z, z->a);
    z->t += 4; z->c = z->a;
    z->t += 4; z->a = z->b;
    z->t += 4; cpu_rrca(z);
    z->t += 4; cpu_rrca(z);
    z->t += 4; cpu_rrca(z);
    z->t += 7; cpu_and(z, 0x60);
    z->t += 4; z->b = z->a;
    z->t += 13; z->a = cpu_rd(z, 0x9215);
    z->t += 4; cpu_and(z, z->a);
    z->t += 4; z->a = z->b;
    if (!z->zf) { z->t += 12; goto x1F7B; } z->t += 7;
    z->t += 7; cpu_xor(z, 0x60);
x1F7B:
    z->t += 4; cpu_or(z, z->c);
    z->t += 10; SET_DE(z, cpu_pop(z));
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 7; z->h = 0x88;
    z->t += 10; cpu_wr(z, HL(z), 0x06);
    z->t += 7; z->a = 0x01;
    z->t += 13; cpu_wr(z, 0x9AAF, z->a);
    z->t += 16; SET_HL(z, cpu_rd16(z, 0x9846));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 16; game_wr16(z->g, 0x9846, HL(z));
    z->t += 10; return;
}

static void t_1F8F(Cpu *z)
{
    z->t += 13; z->a = cpu_rd(z, 0x9827);
    z->t += 4; z->e = z->a;
    z->t += 13; z->a = cpu_rd(z, 0x9215);
    z->t += 7; cpu_add(z, 0xB6);
    z->t += 4; z->l = z->a;
    z->t += 7; z->h = 0x99;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_and(z, 0x0A);
    z->t += 7; cpu_cp(z, 0x0A);
    if (z->zf) { z->t += 12; goto x1FD9; } z->t += 7;
    z->t += 10; SET_HL(z, 0x9215);
    z->t += 12; cpu_bit(z, 0, cpu_rd(z, HL(z)));
    if (z->zf) { z->t += 12; goto x1FAB; } z->t += 7;
    z->t += 7; cpu_xor(z, 0x0A);
x1FAB:
    z->t += 10; SET_HL(z, 0x92A3);
    z->t += 4; z->b = z->a;
    z->t += 7; z->c = 0x01;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_xor(z, 0x01);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    if (!z->zf) { z->t += 12; goto x1FB8; } z->t += 7;
    z->t += 4; z->c = cpu_inc(z, z->c);
x1FB8:
    z->t += 10; SET_HL(z, 0x9362);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; cpu_and(z, z->a);
    if (z->zf) { z->t += 11; return; } z->t += 5;
    z->t += 8; cpu_bit(z, 1, z->b);
    if (!z->zf) { z->t += 12; goto x1FD1; } z->t += 7;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_cp(z, 0xD1);
    if (z->cf) { z->t += 12; goto x1FCA; } z->t += 7;
    z->t += 8; cpu_bit(z, 0, z->e);
    if (!z->zf) { z->t += 11; return; } z->t += 5;
x1FCA:
    z->t += 7; cpu_cp(z, 0xE1);
    if (!z->cf) { z->t += 11; return; } z->t += 5;
    z->t += 4; cpu_add(z, z->c);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 12; goto x1FDE;
x1FD1:
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_cp(z, 0x12);
    if (z->cf) { z->t += 11; return; } z->t += 5;
    z->t += 4; cpu_sub(z, z->c);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 12; goto x1FDE;
x1FD9:
    z->t += 4; cpu_xor(z, z->a);
    z->t += 13; cpu_wr(z, 0x92A3, z->a);
    z->t += 10; return;
x1FDE:
    z->t += 8; cpu_bit(z, 0, z->e);
    if (z->zf) { z->t += 11; return; } z->t += 5;
    z->t += 7; cpu_add(z, 0x0F);
    z->t += 13; cpu_wr(z, 0x9360, z->a);
    z->t += 10; return;
}

static void t_2000(Cpu *z)
{
    z->t += 13; z->a = cpu_rd(z, 0x9828);
    z->t += 4; z->l = z->a;
    z->t += 7; z->h = 0x88;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; cpu_and(z, z->a);
    z->t += 10; if (!z->zf) { goto x20BF; }
    z->t += 13; z->a = cpu_rd(z, 0x928B);
    z->t += 4; cpu_and(z, z->a);
    z->t += 10; if (z->zf) { goto x20C7; }
    z->t += 4; z->a = cpu_dec(z, z->a);
    z->t += 10; if (z->zf) { goto x20D1; }
    z->t += 7; z->h = 0x93;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_cp(z, 0x80);
    if (z->zf) { z->t += 12; goto x2026; } z->t += 7;
    z->t += 10; if (!z->sf) { goto x2023; }
    z->t += 11; cpu_wr(z, HL(z), cpu_inc(z, cpu_rd(z, HL(z))));
    z->t += 12; goto x205E;
x2023:
    z->t += 11; cpu_wr(z, HL(z), cpu_dec(z, cpu_rd(z, HL(z))));
    z->t += 12; goto x205E;
x2026:
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 13; z->a = cpu_rd(z, 0x9215);
    z->t += 4; cpu_and(z, z->a);
    if (!z->zf) { z->t += 12; goto x204C; } z->t += 7;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_cp(z, 0x29);
    if (!z->zf) { z->t += 12; goto x2041; } z->t += 7;
    z->t += 7; z->h = 0x9B;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; z->h = 0x93;
    z->t += 4; z->a = cpu_dec(z, z->a);
    if (!z->zf) { z->t += 12; goto x2041; } z->t += 7;
x203A:
    z->t += 7; z->a = 0x03;
    z->t += 13; cpu_wr(z, 0x928B, z->a);
    z->t += 12; goto x205E;
x2041:
    z->t += 11; cpu_wr(z, HL(z), cpu_inc(z, cpu_rd(z, HL(z))));
    if (!z->zf) { z->t += 12; goto x205E; } z->t += 7;
x2044:
    z->t += 7; z->h = 0x9B;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_xor(z, 0x01);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 12; goto x205E;
x204C:
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_cp(z, 0x37);
    if (!z->zf) { z->t += 12; goto x2059; } z->t += 7;
    z->t += 7; z->h = 0x9B;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; z->h = 0x93;
    z->t += 4; cpu_and(z, z->a);
    if (z->zf) { z->t += 12; goto x203A; } z->t += 7;
x2059:
    z->t += 11; cpu_wr(z, HL(z), cpu_dec(z, cpu_rd(z, HL(z))));
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; z->a = cpu_inc(z, z->a);
    if (z->zf) { z->t += 12; goto x2044; } z->t += 7;
x205E:
    z->t += 10; SET_HL(z, 0x8B62);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_sub(z, 0x06);
    z->t += 4; z->c = z->a;
    z->t += 7; z->h = 0x93;
    if (!z->zf) { z->t += 12; goto x2075; } z->t += 7;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_cp(z, 0x71);
    if (z->zf) { z->t += 12; goto x2075; } z->t += 7;
    z->t += 10; if (!z->sf) { goto x2073; }
    z->t += 11; cpu_wr(z, HL(z), cpu_inc(z, cpu_rd(z, HL(z))));
    z->t += 10; return;
x2073:
    z->t += 11; cpu_wr(z, HL(z), cpu_dec(z, cpu_rd(z, HL(z))));
    z->t += 10; return;
x2075:
    z->t += 13; z->a = cpu_rd(z, 0x928B);
    z->t += 7; cpu_cp(z, 0x03);
    if (!z->zf) { z->t += 11; return; } z->t += 5;
    z->t += 13; z->a = cpu_rd(z, 0x9828);
    z->t += 4; z->l = z->a;
    z->t += 10; cpu_wr(z, HL(z), 0x00);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 4; z->c = cpu_dec(z, z->c);
    z->t += 4; z->c = cpu_inc(z, z->c);
    if (z->zf) { z->t += 12; goto x208F; } z->t += 7;
    z->t += 10; SET_DE(z, 0x9363);
    z->t += 4; cpu_xor(z, z->a);
    z->t += 13; cpu_wr(z, 0x982B, z->a);
    z->t += 12; goto x2097;
x208F:
    z->t += 7; z->a = 0x01;
    z->t += 13; cpu_wr(z, 0x9827, z->a);
    z->t += 10; SET_DE(z, 0x9361);
x2097:
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 7; z->h = 0x9B;
    z->t += 4; z->d = z->h;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 4; z->l = cpu_dec(z, z->l);
    z->t += 7; z->h = 0x88;
    z->t += 10; cpu_wr(z, HL(z), 0x80);
    z->t += 7; z->h = 0x8B;
    z->t += 4; z->l = z->e;
    z->t += 4; z->l = cpu_dec(z, z->l);
    z->t += 10; cpu_wr(z, HL(z), 0x06);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 10; cpu_wr(z, HL(z), 0x09);
    z->t += 4; z->l = cpu_dec(z, z->l);
    z->t += 7; z->h = 0x93;
    z->t += 10; cpu_wr(z, HL(z), 0x80);
    z->t += 7; z->a = 0x01;
    z->t += 13; cpu_wr(z, 0x9014, z->a);
    z->t += 13; cpu_wr(z, 0x9015, z->a);
    z->t += 13; cpu_wr(z, 0x9025, z->a);
    z->t += 13; cpu_wr(z, 0x99B9, z->a);
x20BF:
    z->t += 4; cpu_xor(z, z->a);
    z->t += 13; cpu_wr(z, 0x901D, z->a);
    z->t += 13; cpu_wr(z, 0x9AB1, z->a);
    z->t += 10; return;
x20C7:
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 13; cpu_wr(z, 0x928B, z->a);
    z->t += 7; z->a = 0x02;
    z->t += 13; cpu_wr(z, 0x92AD, z->a);
    z->t += 10; return;
x20D1:
    z->t += 7; z->h = 0x9B;
    z->t += 13; z->a = cpu_rd(z, 0x92AD);
    z->t += 4; z->e = z->a;
    z->t += 13; z->a = cpu_rd(z, 0x9287);
    z->t += 4; cpu_or(z, z->e);
    z->t += 13; cpu_wr(z, 0x928D, z->a);
    z->t += 17; m_2188(z);
    z->t += 4; z->b = cpu_dec(z, z->b);
    if (!z->zf) { z->t += 11; return; } z->t += 5;
    z->t += 13; cpu_wr(z, 0x9014, z->a);
    z->t += 13; cpu_wr(z, 0x9015, z->a);
    z->t += 13; cpu_wr(z, 0x9025, z->a);
    z->t += 7; z->a = 0x02;
    z->t += 13; cpu_wr(z, 0x928B, z->a);
    z->t += 10; return;
}

static void t_20F2(Cpu *z)
{
    z->t += 10; SET_HL(z, 0x9B62);
    z->t += 17; m_2188(z);
    z->t += 8; cpu_bit(z, 0, z->b);
    if (!z->zf) { z->t += 12; goto x2151; } z->t += 7;
    z->t += 13; z->a = cpu_rd(z, 0x928B);
    z->t += 8; cpu_bit(z, 7, z->a);
    if (!z->zf) { z->t += 12; goto x215D; } z->t += 7;
    z->t += 13; z->a = cpu_rd(z, 0x928D);
    z->t += 4; cpu_and(z, z->a);
    if (z->zf) { z->t += 11; return; } z->t += 5;
    z->t += 7; z->h = 0x93;
    z->t += 13; z->a = cpu_rd(z, 0x9828);
    z->t += 4; z->e = z->a;
    z->t += 4; z->d = z->h;
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 7; cpu_cp(z, cpu_rd(z, HL(z)));
    if (z->zf) { z->t += 12; goto x211A; } z->t += 7;
    z->t += 10; if (!z->sf) { goto x2119; }
    z->t += 11; cpu_wr(z, HL(z), cpu_dec(z, cpu_rd(z, HL(z))));
    z->t += 12; goto x211A;
x2119:
    z->t += 11; cpu_wr(z, HL(z), cpu_inc(z, cpu_rd(z, HL(z))));
x211A:
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 13; z->a = cpu_rd(z, 0x9215);
    z->t += 4; cpu_and(z, z->a);
    if (z->zf) { z->t += 12; goto x212C; } z->t += 7;
    z->t += 11; cpu_wr(z, HL(z), cpu_inc(z, cpu_rd(z, HL(z))));
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_cp(z, 0x7A);
    if (z->zf) { z->t += 12; goto x214C; } z->t += 7;
    z->t += 7; cpu_cp(z, 0x80);
    if (z->zf) { z->t += 12; goto x2141; } z->t += 7;
    z->t += 10; return;
x212C:
    z->t += 11; cpu_wr(z, HL(z), cpu_dec(z, cpu_rd(z, HL(z))));
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; z->a = cpu_inc(z, z->a);
    if (!z->zf) { z->t += 12; goto x2139; } z->t += 7;
    z->t += 7; z->h = 0x9B;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_xor(z, 0x01);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 7; z->h = 0x93;
x2139:
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_cp(z, 0xE6);
    if (z->zf) { z->t += 12; goto x214C; } z->t += 7;
    z->t += 7; cpu_cp(z, 0xE0);
    if (!z->zf) { z->t += 11; return; } z->t += 5;
x2141:
    z->t += 4; cpu_xor(z, z->a);
    z->t += 13; cpu_wr(z, 0x928D, z->a);
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 7; z->a = 0x07;
    z->t += 13; cpu_wr(z, 0x8B63, z->a);
    z->t += 10; return;
x214C:
    z->t += 4; cpu_xor(z, z->a);
    z->t += 13; cpu_wr(z, 0x9015, z->a);
    z->t += 10; return;
x2151:
    z->t += 13; z->a = cpu_rd(z, 0x9015);
    z->t += 4; cpu_and(z, z->a);
    if (!z->zf) { z->t += 12; goto x215D; } z->t += 7;
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 13; cpu_wr(z, 0x920D, z->a);
    z->t += 12; goto x217F;
x215D:
    z->t += 7; z->h = 0x93;
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 13; z->a = cpu_rd(z, 0x9215);
    z->t += 4; cpu_and(z, z->a);
    if (z->zf) { z->t += 12; goto x216D; } z->t += 7;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_cp(z, 0x37);
    if (z->zf) { z->t += 12; goto x217D; } z->t += 7;
    z->t += 11; cpu_wr(z, HL(z), cpu_dec(z, cpu_rd(z, HL(z))));
    z->t += 10; return;
x216D:
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_cp(z, 0x29);
    if (z->zf) { z->t += 12; goto x217D; } z->t += 7;
    z->t += 11; cpu_wr(z, HL(z), cpu_inc(z, cpu_rd(z, HL(z))));
    if (!z->zf) { z->t += 11; return; } z->t += 5;
    z->t += 7; z->h = 0x9B;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_xor(z, 0x01);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 7; z->h = 0x93;
    z->t += 10; return;
x217D:
    z->t += 4; z->b = cpu_dec(z, z->b);
    if (!z->zf) { z->t += 11; return; } z->t += 5;
x217F:
    z->t += 4; cpu_xor(z, z->a);
    z->t += 13; cpu_wr(z, 0x901C, z->a);
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 13; cpu_wr(z, 0x9025, z->a);
    z->t += 10; return;
}

static void t_21CB(Cpu *z)
{
    z->t += 10; SET_HL(z, 0x9828);
    z->t += 7; z->e = cpu_rd(z, HL(z));
    z->t += 7; z->d = 0x88;
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 7; cpu_cp(z, 0x09);
    if (!z->zf) { z->t += 12; goto x221A; } z->t += 7;
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 8; z->ix = (uint16_t)((z->ix & 0xFF00) | (uint8_t)(z->a));
    z->t += 11; z->ix = (uint16_t)((z->ix & 0x00FF) | (uint8_t)(0x91) << 8);
    z->t += 19; z->a = cpu_ix(z, 10);
    z->t += 4; cpu_and(z, z->a);
    if (!z->zf) { z->t += 11; return; } z->t += 5;
    z->t += 7; z->a = 0x0C;
    z->t += 20; cpu_bit(z, 0, cpu_ix(z, 5));
    if (z->zf) { z->t += 12; goto x21EC; } z->t += 7;
    z->t += 8; cpu_neg(z);
x21EC:
    z->t += 19; cpu_set_ix(z, 12, z->a);
    z->t += 19; z->a = cpu_ix(z, 5);
    z->t += 4; cpu_rrca(z);
    z->t += 19; z->a = cpu_ix(z, 4);
    z->t += 4; cpu_rra(z);
    z->t += 7; cpu_sub(z, 0x78);
    z->t += 7; cpu_cp(z, 0x10);
    if (!z->cf) { z->t += 11; return; } z->t += 5;
    z->t += 13; z->a = cpu_rd(z, 0x99C6);
    z->t += 13; cpu_wr(z, 0x982A, z->a);
    z->t += 4; cpu_xor(z, z->a);
    z->t += 19; cpu_set_ix(z, 12, z->a);
    z->t += 13; cpu_wr(z, 0x9019, z->a);
    z->t += 13; cpu_wr(z, 0x928B, z->a);
    z->t += 13; cpu_wr(z, 0x920D, z->a);
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 13; cpu_wr(z, 0x9018, z->a);
    z->t += 13; cpu_wr(z, 0x928C, z->a);
    z->t += 13; cpu_wr(z, 0x928D, z->a);
    z->t += 10; return;
x221A:
    z->t += 4; cpu_xor(z, z->a);
    z->t += 13; cpu_wr(z, 0x9019, z->a);
    z->t += 13; cpu_wr(z, 0x982B, z->a);
    z->t += 10; return;
}

static void t_2222(Cpu *z)
{
    z->t += 13; z->a = cpu_rd(z, 0x92A0);
    z->t += 4; z->c = z->a;
    z->t += 7; cpu_and(z, 0x03);
    if (!z->zf) { z->t += 12; goto x2257; } z->t += 7;
    z->t += 13; z->a = cpu_rd(z, 0x928A);
    z->t += 8; cpu_neg(z);
    z->t += 7; cpu_sub(z, 0x18);
    z->t += 7; z->h = 0x21;
    z->t += 4; cpu_rlca(z);
    z->t += 8; z->h = cpu_rl(z, z->h);
    z->t += 4; cpu_rlca(z);
    z->t += 8; z->h = cpu_rl(z, z->h);
    z->t += 7; cpu_and(z, 0xE0);
    z->t += 7; cpu_add(z, 0x15);
    z->t += 4; z->l = z->a;
    z->t += 4; z->a = z->c;
    z->t += 4; cpu_rrca(z);
    z->t += 4; cpu_rrca(z);
    z->t += 7; cpu_and(z, 0x03);
    if (!z->zf) { z->t += 12; goto x2246; } z->t += 7;
    z->t += 4; z->a = cpu_inc(z, z->a);
x2246:
    z->t += 7; cpu_add(z, 0x17);
    z->t += 10; SET_DE(z, 0x0016);
    z->t += 7; z->c = 0x06;
x224D:
    z->t += 7; z->b = 0x0A;
x224F:
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 4; z->l = cpu_inc(z, z->l);
    if (--z->b) { z->t += 13; goto x224F; } z->t += 8;
    z->t += 11; SET_HL(z, cpu_add16(z, HL(z), DE(z)));
    z->t += 4; z->c = cpu_dec(z, z->c);
    if (!z->zf) { z->t += 12; goto x224D; } z->t += 7;
x2257:
    z->t += 10; SET_HL(z, 0x928B);
    z->t += 12; cpu_bit(z, 7, cpu_rd(z, HL(z)));
    if (!z->zf) { z->t += 12; goto x226A; } z->t += 7;
    z->t += 13; z->a = cpu_rd(z, 0x9828);
    z->t += 4; z->e = z->a;
    z->t += 7; z->d = 0x88;
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 7; cpu_cp(z, 0x09);
    z->t += 10; if (!z->zf) { goto x2327; }
x226A:
    z->t += 10; SET_HL(z, 0x928C);
    z->t += 11; cpu_wr(z, HL(z), cpu_dec(z, cpu_rd(z, HL(z))));
    z->t += 10; if (!z->zf) { goto x233D; }
    z->t += 13; z->a = cpu_rd(z, 0x982A);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 10; SET_HL(z, 0x928B);
    z->t += 12; cpu_bit(z, 7, cpu_rd(z, HL(z)));
    if (!z->zf) { z->t += 12; goto x22AB; } z->t += 7;
    z->t += 13; cpu_wr(z, 0x9AA5, z->a);
    z->t += 13; z->a = cpu_rd(z, 0x9829);
    z->t += 7; cpu_add(z, 0x0D);
    z->t += 4; z->e = z->a;
    z->t += 7; z->d = 0x91;
    z->t += 7; z->a = 0xFF;
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 11; cpu_wr(z, HL(z), cpu_inc(z, cpu_rd(z, HL(z))));
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_and(z, 0x0F);
    z->t += 7; cpu_cp(z, 0x0B);
    if (z->zf) { z->t += 12; goto x22D2; } z->t += 7;
    z->t += 12; cpu_bit(z, 6, cpu_rd(z, HL(z)));
    if (!z->zf) { z->t += 12; goto x22C1; } z->t += 7;
    z->t += 11; cpu_push(z, AF(z));
    z->t += 4; z->c = z->a;
    z->t += 4; cpu_rlca(z);
    z->t += 4; cpu_add(z, z->c);
    z->t += 10; SET_HL(z, 0x239B);
    cpu_rst08(z);
    z->t += 10; cpu_set_af(z, cpu_pop(z));
    z->t += 17; m_238A(z);
    z->t += 7; z->b = 0x06;
x22A4:
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 11; m_0020(z);
    if (--z->b) { z->t += 13; goto x22A4; } z->t += 8;
    z->t += 10; return;
x22AB:
    z->t += 11; cpu_wr(z, HL(z), cpu_inc(z, cpu_rd(z, HL(z))));
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_and(z, 0x0F);
    z->t += 7; cpu_cp(z, 0x0B);
    if (!z->zf) { z->t += 12; goto x22C5; } z->t += 7;
    z->t += 4; cpu_xor(z, z->a);
    z->t += 13; cpu_wr(z, 0x9018, z->a);
    z->t += 13; cpu_wr(z, 0x9AA5, z->a);
    z->t += 13; cpu_wr(z, 0x9AA6, z->a);
    z->t += 13; cpu_wr(z, 0x982B, z->a);
    z->t += 10; return;
x22C1:
    z->t += 8; cpu_neg(z);
    z->t += 7; cpu_add(z, 0x0B);
x22C5:
    z->t += 17; m_238A(z);
    z->t += 7; z->b = 0x06;
    z->t += 7; z->c = 0x24;
x22CC:
    z->t += 4; z->a = z->c;
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 11; m_0020(z);
    if (--z->b) { z->t += 13; goto x22CC; } z->t += 8;
    z->t += 10; return;
x22D2:
    z->t += 12; cpu_bit(z, 6, cpu_rd(z, HL(z)));
    if (z->zf) { z->t += 12; goto x231C; } z->t += 7;
    z->t += 13; z->a = cpu_rd(z, 0x920D);
    z->t += 4; cpu_and(z, z->a);
    if (z->zf) { z->t += 12; goto x22E3; } z->t += 7;
    z->t += 12; cpu_bit(z, 5, cpu_rd(z, HL(z)));
    if (!z->zf) { z->t += 12; goto x22E3; } z->t += 7;
    z->t += 10; cpu_wr(z, HL(z), 0x68);
    z->t += 10; return;
x22E3:
    z->t += 4; cpu_xor(z, z->a);
    z->t += 13; cpu_wr(z, 0x9018, z->a);
    z->t += 13; cpu_wr(z, 0x9AA5, z->a);
    z->t += 13; cpu_wr(z, 0x9AA6, z->a);
    z->t += 13; z->a = cpu_rd(z, 0x920D);
    z->t += 4; cpu_and(z, z->a);
    z->t += 13; z->a = cpu_rd(z, 0x9829);
    if (!z->zf) { z->t += 12; goto x2305; } z->t += 7;
    z->t += 7; cpu_add(z, 0x0D);
    z->t += 4; z->e = z->a;
    z->t += 7; z->d = 0x91;
    z->t += 4; cpu_xor(z, z->a);
    z->t += 13; cpu_wr(z, 0x982B, z->a);
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 13; cpu_wr(z, 0x9828, z->a);
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 10; return;
x2305:
    z->t += 7; cpu_add(z, 0x08);
    z->t += 4; z->l = z->a;
    z->t += 7; z->h = 0x91;
    z->t += 10; SET_DE(z, 0x046B);
    z->t += 7; cpu_wr(z, HL(z), z->e);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 7; cpu_wr(z, HL(z), z->d);
    z->t += 4; cpu_xor(z, z->a);
    z->t += 13; cpu_wr(z, 0x99BA, z->a);
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 13; cpu_wr(z, 0x9011, z->a);
    z->t += 13; cpu_wr(z, 0x928E, z->a);
    z->t += 10; return;
x231C:
    z->t += 7; z->a = 0x40;
    z->t += 13; cpu_wr(z, 0x928C, z->a);
    z->t += 7; z->a = 0x40;
    z->t += 13; cpu_wr(z, 0x928B, z->a);
    z->t += 10; return;
x2327:
    z->t += 7; z->a = 0x03;
    z->t += 13; cpu_wr(z, 0x982A, z->a);
    z->t += 10; cpu_wr(z, HL(z), 0x80);
    z->t += 4; cpu_xor(z, z->a);
    z->t += 13; cpu_wr(z, 0x928D, z->a);
    z->t += 13; cpu_wr(z, 0x99BA, z->a);
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 13; cpu_wr(z, 0x928C, z->a);
    z->t += 13; cpu_wr(z, 0x9014, z->a);
    z->t += 10; return;
x233D:
    z->t += 13; z->a = cpu_rd(z, 0x928B);
    z->t += 7; cpu_cp(z, 0x40);
    if (!z->zf) { z->t += 11; return; } z->t += 5;
    z->t += 13; z->a = cpu_rd(z, 0x9215);
    z->t += 4; z->c = z->a;
    z->t += 13; z->a = cpu_rd(z, 0x9362);
    z->t += 8; cpu_bit(z, 0, z->c);
    if (z->zf) { z->t += 12; goto x2352; } z->t += 7;
    z->t += 7; cpu_add(z, 0x0E);
    z->t += 8; cpu_neg(z);
x2352:
    z->t += 4; z->b = z->a;
    z->t += 13; z->a = cpu_rd(z, 0x928A);
    z->t += 4; cpu_sub(z, z->b);
    z->t += 7; cpu_add(z, 0x1B);
    z->t += 7; cpu_cp(z, 0x36);
    if (!z->cf) { z->t += 11; return; } z->t += 5;
    z->t += 13; z->a = cpu_rd(z, 0x9201);
    z->t += 4; z->a = cpu_dec(z, z->a);
    if (z->zf) { z->t += 12; goto x236D; } z->t += 7;
    z->t += 13; z->a = cpu_rd(z, 0x9014);
    z->t += 4; z->c = z->a;
    z->t += 13; z->a = cpu_rd(z, 0x9213);
    z->t += 7; cpu_xor(z, 0x01);
    z->t += 4; cpu_and(z, z->c);
    if (z->zf) { z->t += 11; return; } z->t += 5;
x236D:
    z->t += 4; cpu_xor(z, z->a);
    z->t += 13; cpu_wr(z, 0x9014, z->a);
    z->t += 13; cpu_wr(z, 0x9AA5, z->a);
    z->t += 13; cpu_wr(z, 0x9025, z->a);
    z->t += 13; cpu_wr(z, 0x9213, z->a);
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 13; cpu_wr(z, 0x901C, z->a);
    z->t += 13; cpu_wr(z, 0x9AA6, z->a);
    z->t += 13; cpu_wr(z, 0x99BA, z->a);
    z->t += 7; z->a = 0x0A;
    z->t += 13; cpu_wr(z, 0x982A, z->a);
    z->t += 10; return;
}

static void t_23DD(Cpu *z)
{
    z->t += 13; z->a = cpu_rd(z, 0x92A0);
    z->t += 8; cpu_bit(z, 0, z->a);
    z->t += 10; if (z->zf) { goto x2596; }
    z->t += 7; cpu_and(z, 0x02);
    z->t += 4; z->e = z->a;
    z->t += 13; z->a = cpu_rd(z, 0x92A6);
    z->t += 8; z->ix = (uint16_t)((z->ix & 0xFF00) | (uint8_t)(z->a));
    z->t += 7; z->b = 0x20;
x23EF:
    z->t += 7; z->d = 0x88;
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 8; z->a = cpu_sla(z, z->a);
    if (z->cf) { z->t += 12; goto x2416; } z->t += 7;
    z->t += 10; SET_HL(z, 0x23FF);
    cpu_rst10(z);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; z->h = cpu_rd(z, HL(z));
    z->t += 4; z->l = z->a;
    z->t += 4; switch (HL(z)) { case 0x2416: goto x2416; case 0x2422: goto x2422; case 0x243C: goto x243C; case 0x245F: goto x245F; case 0x2488: goto x2488; case 0x24B2: goto x24B2; case 0x2535: goto x2535; case 0x254D: goto x254D; case 0x2590: goto x2590; default: game_unported(z->g, HL(z)); return; }
x2413:
    z->t += 4; z->e = cpu_dec(z, z->e);
x2414:
    z->t += 8; z->ix = (uint16_t)((z->ix & 0xFF00) | (uint8_t)(cpu_inc(z, (uint8_t)z->ix)));
x2416:
    z->t += 7; z->a = 0x04;
    z->t += 4; cpu_add(z, z->e);
    z->t += 4; z->e = z->a;
    if (--z->b) { z->t += 13; goto x23EF; } z->t += 8;
    z->t += 8; z->a = (uint8_t)z->ix;
    z->t += 13; cpu_wr(z, 0x92A6, z->a);
    z->t += 10; return;
x2422:
    z->t += 4; z->l = z->e;
    z->t += 7; z->h = 0x01;
    z->t += 7; z->c = cpu_rd(z, HL(z));
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 7; z->l = cpu_rd(z, HL(z));
    z->t += 7; z->h = 0x99;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; cpu_ex_af(z);
    z->t += 4; z->l = z->c;
    z->t += 7; z->c = cpu_rd(z, HL(z));
    z->t += 4; z->e = cpu_inc(z, z->e);
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 7; cpu_add(z, 0x11);
    z->t += 4; z->l = z->a;
    z->t += 7; z->h = 0x91;
    z->t += 4; cpu_ex_af(z);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 7; cpu_wr(z, HL(z), z->c);
    z->t += 10; goto x2413;
x243C:
    z->t += 7; z->h = 0x8B;
    z->t += 4; z->l = z->e;
    z->t += 4; z->e = cpu_inc(z, z->e);
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 4; z->a = cpu_dec(z, z->a);
    if (z->zf) { z->t += 12; goto x2451; } z->t += 7;
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 4; z->e = cpu_dec(z, z->e);
    z->t += 7; cpu_and(z, 0x03);
    if (!z->zf) { z->t += 12; goto x2416; } z->t += 7;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_add(z, 0x04);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 10; goto x2416;
x2451:
    z->t += 7; z->h = 0x93;
    z->t += 4; cpu_xor(z, z->a);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 7; z->h = 0x9B;
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 4; z->e = cpu_dec(z, z->e);
    z->t += 7; z->a = 0x80;
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 10; goto x2416;
x245F:
    z->t += 7; z->h = 0x9B;
    z->t += 4; z->l = z->e;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_and(z, 0x01);
    z->t += 7; z->h = 0x8B;
    if (!z->zf) { z->t += 12; goto x2473; } z->t += 7;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_and(z, 0x07);
    z->t += 7; cpu_cp(z, 0x06);
    if (z->zf) { z->t += 12; goto x2483; } z->t += 7;
    z->t += 11; cpu_wr(z, HL(z), cpu_inc(z, cpu_rd(z, HL(z))));
    z->t += 12; goto x249B;
x2473:
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_and(z, 0x07);
    if (!z->zf) { z->t += 12; goto x2480; } z->t += 7;
    z->t += 7; z->h = 0x9B;
    z->t += 15; cpu_wr(z, HL(z), (uint8_t)(cpu_rd(z, HL(z)) & 0xFE));
    z->t += 7; z->h = 0x8B;
    z->t += 12; goto x249B;
x2480:
    z->t += 11; cpu_wr(z, HL(z), cpu_dec(z, cpu_rd(z, HL(z))));
    z->t += 12; goto x249B;
x2483:
    z->t += 7; z->a = 0x01;
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 12; goto x249B;
x2488:
    z->t += 7; z->h = 0x8B;
    z->t += 4; z->l = z->e;
    z->t += 13; z->a = cpu_rd(z, 0x92A2);
    z->t += 15; cpu_wr(z, HL(z), cpu_rrc(z, cpu_rd(z, HL(z))));
    z->t += 4; cpu_rrca(z);
    z->t += 4; cpu_rrca(z);
    z->t += 15; cpu_wr(z, HL(z), cpu_rl(z, cpu_rd(z, HL(z))));
    z->t += 13; z->a = cpu_rd(z, 0x920B);
    z->t += 4; cpu_and(z, z->a);
    z->t += 10; if (z->zf) { goto x2414; }
x249B:
    z->t += 7; z->h = 0x01;
    z->t += 7; z->c = cpu_rd(z, HL(z));
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 7; z->l = cpu_rd(z, HL(z));
    z->t += 7; z->h = 0x98;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; z->d = 0x93;
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 4; z->e = cpu_inc(z, z->e);
    z->t += 4; z->l = z->c;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 7; z->d = 0x9B;
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 10; goto x2413;
x24B2:
    z->t += 4; z->l = z->e;
    z->t += 4; z->e = cpu_inc(z, z->e);
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 7; cpu_cp(z, 0x45);
    if (z->zf) { z->t += 12; goto x24E6; } z->t += 7;
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 4; z->e = cpu_dec(z, z->e);
    z->t += 7; cpu_cp(z, 0x45);
    if (!z->zf) { z->t += 12; goto x24C2; } z->t += 7;
    z->t += 7; cpu_add(z, 0x03);
x24C2:
    z->t += 7; cpu_cp(z, 0x44);
    if (!z->zf) { z->t += 12; goto x24E0; } z->t += 7;
    z->t += 7; z->h = 0x93;
    z->t += 4; cpu_ex_af(z);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_sub(z, 0x08);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_sub(z, 0x08);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    if (!z->cf) { z->t += 12; goto x24DA; } z->t += 7;
    z->t += 7; z->h = 0x9B;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_xor(z, 0x01);
    z->t += 7; cpu_wr(z, HL(z), z->a);
x24DA:
    z->t += 4; z->l = cpu_dec(z, z->l);
    z->t += 7; z->h = 0x9B;
    z->t += 10; cpu_wr(z, HL(z), 0x0C);
    z->t += 4; cpu_ex_af(z);
x24E0:
    z->t += 7; z->h = 0x8B;
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 10; goto x2416;
x24E6:
    z->t += 4; z->e = cpu_dec(z, z->e);
    z->t += 7; z->h = 0x92;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_cp(z, 0x01);
    if (!z->zf) { z->t += 12; goto x24FD; } z->t += 7;
    z->t += 7; z->h = 0x93;
    z->t += 10; cpu_wr(z, HL(z), 0x00);
    z->t += 7; z->h = 0x9B;
    z->t += 10; cpu_wr(z, HL(z), 0x00);
    z->t += 7; z->h = 0x88;
    z->t += 10; cpu_wr(z, HL(z), 0x80);
    z->t += 10; goto x2416;
x24FD:
    z->t += 7; z->h = 0x8B;
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 7; cpu_cp(z, 0x37);
    if (z->cf) { z->t += 12; goto x250E; } z->t += 7;
    z->t += 7; z->c = 0x0D;
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 7; cpu_cp(z, 0x3A);
    if (z->cf) { z->t += 12; goto x250C; } z->t += 7;
    z->t += 4; z->c = cpu_inc(z, z->c);
x250C:
    z->t += 7; cpu_wr(z, HL(z), z->c);
    z->t += 4; z->l = cpu_dec(z, z->l);
x250E:
    z->t += 7; z->h = 0x93;
    z->t += 7; z->c = 0x08;
    z->t += 7; cpu_cp(z, 0x3B);
    if (!z->cf) { z->t += 12; goto x251C; } z->t += 7;
    z->t += 7; z->c = 0x00;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_add(z, 0x08);
    z->t += 7; cpu_wr(z, HL(z), z->a);
x251C:
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_add(z, 0x08);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 7; z->h = 0x9B;
    if (!z->cf) { z->t += 12; goto x2529; } z->t += 7;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_xor(z, 0x01);
    z->t += 7; cpu_wr(z, HL(z), z->a);
x2529:
    z->t += 4; z->l = cpu_dec(z, z->l);
    z->t += 7; cpu_wr(z, HL(z), z->c);
    z->t += 7; z->h = 0x88;
    z->t += 10; cpu_wr(z, HL(z), 0x05);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 10; cpu_wr(z, HL(z), 0x13);
    z->t += 10; goto x2416;
x2535:
    z->t += 4; z->l = z->e;
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 4; z->h = z->d;
    z->t += 11; cpu_wr(z, HL(z), cpu_dec(z, cpu_rd(z, HL(z))));
    z->t += 10; if (!z->zf) { goto x2416; }
    z->t += 4; z->l = cpu_dec(z, z->l);
    z->t += 10; cpu_wr(z, HL(z), 0x80);
    z->t += 7; z->h = 0x93;
    z->t += 10; cpu_wr(z, HL(z), 0x00);
    z->t += 7; z->h = 0x9B;
    z->t += 10; cpu_wr(z, HL(z), 0x00);
    z->t += 7; z->a = 0x80;
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 10; goto x2416;
x254D:
    z->t += 7; z->h = 0x93;
    z->t += 4; z->l = z->e;
    z->t += 8; z->l = (uint8_t)(z->l | 0x80);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_cp(z, 0xF4);
    if (!z->cf) { z->t += 12; goto x2571; } z->t += 7;
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 7; z->c = cpu_rd(z, HL(z));
    z->t += 7; z->h = 0x9B;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; z->l = cpu_dec(z, z->l);
    z->t += 4; cpu_rrca(z);
    z->t += 4; z->a = z->c;
    z->t += 4; cpu_rra(z);
    z->t += 7; cpu_cp(z, 0x0B);
    if (z->cf) { z->t += 12; goto x2571; } z->t += 7;
    z->t += 7; cpu_cp(z, 0xA5);
    if (!z->cf) { z->t += 12; goto x2571; } z->t += 7;
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 7; cpu_cp(z, 0x06);
    z->t += 10; if (!z->zf) { goto x2414; }
    z->t += 10; goto x2416;
x2571:
    z->t += 8; z->l = (uint8_t)(z->l & 0x7F);
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 7; cpu_cp(z, 0x03);
    if (z->zf) { z->t += 12; goto x2582; } z->t += 7;
x2578:
    z->t += 7; z->a = 0x80;
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 7; z->h = 0x93;
    z->t += 10; cpu_wr(z, HL(z), 0x00);
    z->t += 10; goto x2416;
x2582:
    z->t += 4; z->e = cpu_inc(z, z->e);
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 4; z->e = cpu_dec(z, z->e);
    z->t += 7; cpu_add(z, 0x13);
    z->t += 4; z->l = z->a;
    z->t += 7; z->h = 0x91;
    z->t += 10; cpu_wr(z, HL(z), 0x00);
    z->t += 4; z->l = z->e;
    z->t += 10; goto x2578;
x2590:
    z->t += 7; z->a = 0x03;
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 10; goto x2414;
x2596:
    z->t += 8; cpu_bit(z, 1, z->a);
    if (z->zf) { z->t += 11; return; } z->t += 5;
    z->t += 10; SET_HL(z, 0x92A6);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 10; cpu_wr(z, HL(z), 0x00);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 10; return;
}

static void t_2916(Cpu *z)
{
    z->t += 16; SET_HL(z, cpu_rd16(z, 0x9822));
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_cp(z, 0x7F);
    z->t += 10; if (z->zf) { goto x2A29; }
    z->t += 7; cpu_cp(z, 0x7E);
    if (!z->zf) { z->t += 12; goto x2953; } z->t += 7;
    z->t += 13; z->a = cpu_rd(z, 0x9842);
    z->t += 4; cpu_and(z, z->a);
    if (z->zf) { z->t += 11; return; } z->t += 5;
    z->t += 13; z->a = cpu_rd(z, 0x9287);
    z->t += 4; cpu_and(z, z->a);
    if (!z->zf) { z->t += 12; goto x294D; } z->t += 7;
    z->t += 13; z->a = cpu_rd(z, 0x9825);
    z->t += 4; z->b = z->a;
    z->t += 4; cpu_and(z, z->a);
    if (!z->zf) { z->t += 12; goto x2944; } z->t += 7;
    z->t += 13; z->a = cpu_rd(z, 0x92AC);
    z->t += 7; cpu_cp(z, 0x01);
    if (!z->zf) { z->t += 12; goto x2942; } z->t += 7;
    z->t += 7; z->a = 0x08;
    z->t += 13; cpu_wr(z, 0x92A8, z->a);
    z->t += 10; return;
x2942:
    z->t += 4; cpu_and(z, z->a);
    if (!z->zf) { z->t += 11; return; } z->t += 5;
x2944:
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 16; game_wr16(z->g, 0x9822, HL(z));
    z->t += 10; SET_HL(z, 0x9826);
    z->t += 11; cpu_wr(z, HL(z), cpu_inc(z, cpu_rd(z, HL(z))));
    z->t += 10; return;
x294D:
    z->t += 7; z->a = 0x02;
    z->t += 13; cpu_wr(z, 0x92AC, z->a);
    z->t += 10; return;
x2953:
    z->t += 4; z->c = z->a;
    z->t += 8; cpu_bit(z, 7, z->a);
    if (!z->zf) { z->t += 12; goto x295E; } z->t += 7;
    z->t += 13; z->a = cpu_rd(z, 0x92A0);
    z->t += 7; cpu_and(z, 0x07);
    if (!z->zf) { z->t += 11; return; } z->t += 5;
x295E:
    z->t += 8; z->c = cpu_sla(z, z->c);
    z->t += 7; z->b = 0x0C;
    z->t += 10; SET_DE(z, 0x0014);
    z->t += 14; z->ix = (uint16_t)(0x9100);
x2969:
    z->t += 20; cpu_bit(z, 0, cpu_ix(z, 19));
    if (z->zf) { z->t += 12; goto x2974; } z->t += 7;
    z->t += 15; z->ix = (uint16_t)(cpu_add16(z, z->ix, DE(z)));
    if (--z->b) { z->t += 13; goto x2969; } z->t += 8;
    z->t += 10; return;
x2974:
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; z->b = z->a;
    z->t += 7; cpu_and(z, 0x78);
    z->t += 7; cpu_cp(z, 0x78);
    z->t += 4; z->a = z->b;
    if (!z->zf) { z->t += 12; goto x2980; } z->t += 7;
    z->t += 8; z->a = (uint8_t)(z->a & 0xBF);
x2980:
    z->t += 19; cpu_set_ix(z, 16, z->a);
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 16; game_wr16(z->g, 0x9822, HL(z));
    z->t += 7; z->h = 0x88;
    z->t += 4; z->l = z->a;
    z->t += 10; cpu_wr(z, HL(z), 0x07);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 8; z->e = (uint8_t)z->ix;
    z->t += 7; cpu_wr(z, HL(z), z->e);
    z->t += 7; z->h = 0x93;
    z->t += 7; cpu_and(z, 0x38);
    z->t += 7; cpu_cp(z, 0x38);
    if (z->zf) { z->t += 12; goto x29B3; } z->t += 7;
    z->t += 4; z->l = cpu_dec(z, z->l);
    z->t += 7; z->h = 0x8B;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; z->d = z->a;
    z->t += 7; cpu_and(z, 0x78);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 4; z->a = z->d;
    z->t += 7; cpu_and(z, 0x07);
    z->t += 8; cpu_bit(z, 7, z->d);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 7; z->a = 0x00;
    if (z->zf) { z->t += 12; goto x29AE; } z->t += 7;
    z->t += 13; z->a = cpu_rd(z, 0x92E3);
x29AE:
    z->t += 19; cpu_set_ix(z, 15, z->a);
    z->t += 12; goto x29D1;
x29B3:
    z->t += 10; SET_DE(z, 0x0210);
    z->t += 8; cpu_bit(z, 6, z->b);
    if (!z->zf) { z->t += 12; goto x29C7; } z->t += 7;
    z->t += 10; SET_DE(z, 0x0318);
    z->t += 13; z->a = cpu_rd(z, 0x9826);
    z->t += 7; cpu_cp(z, 0x02);
    if (!z->zf) { z->t += 12; goto x29C7; } z->t += 7;
    z->t += 10; SET_DE(z, 0x0008);
x29C7:
    z->t += 7; z->h = 0x8B;
    z->t += 7; cpu_wr(z, HL(z), z->d);
    z->t += 4; z->l = cpu_dec(z, z->l);
    z->t += 7; cpu_wr(z, HL(z), z->e);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 19; cpu_set_ix(z, 15, 0x00);
x29D1:
    z->t += 4; z->d = z->c;
    z->t += 8; z->c = (uint8_t)(z->c & 0x7F);
    z->t += 7; z->b = 0x08;
    z->t += 8; cpu_bit(z, 1, z->c);
    if (z->zf) { z->t += 12; goto x29DC; } z->t += 7;
    z->t += 7; z->b = 0x44;
x29DC:
    z->t += 19; cpu_set_ix(z, 14, z->b);
    z->t += 7; z->b = 0x00;
    z->t += 10; SET_HL(z, 0x2A3C);
    z->t += 11; SET_HL(z, cpu_add16(z, HL(z), BC(z)));
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 19; cpu_set_ix(z, 8, z->a);
    z->t += 4; cpu_xor(z, z->a);
    z->t += 18; cpu_rld(z);
    z->t += 4; z->b = z->a;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_and(z, 0x1F);
    z->t += 19; cpu_set_ix(z, 9, z->a);
    z->t += 4; z->a = z->b;
    z->t += 7; cpu_and(z, 0x0E);
    z->t += 4; z->b = z->a;
    z->t += 4; cpu_rlca(z);
    z->t += 4; cpu_add(z, z->b);
    z->t += 10; SET_HL(z, 0x2A6C);
    cpu_rst10(z);
    z->t += 8; cpu_bit(z, 7, z->d);
    if (z->zf) { z->t += 12; goto x2A05; } z->t += 7;
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 6; SET_HL(z, HL(z) + 1);
x2A05:
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 19; cpu_set_ix(z, 1, z->a);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 19; cpu_set_ix(z, 3, z->a);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 19; cpu_set_ix(z, 5, z->a);
    z->t += 4; cpu_xor(z, z->a);
    z->t += 19; cpu_set_ix(z, 0, z->a);
    z->t += 19; cpu_set_ix(z, 2, z->a);
    z->t += 19; cpu_set_ix(z, 4, z->a);
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 19; cpu_set_ix(z, 13, z->a);
    z->t += 4; cpu_or(z, z->d);
    z->t += 7; cpu_and(z, 0x81);
    z->t += 19; cpu_set_ix(z, 19, z->a);
    z->t += 10; return;
x2A29:
    z->t += 13; z->a = cpu_rd(z, 0x9287);
    z->t += 4; cpu_and(z, z->a);
    if (!z->zf) { z->t += 11; return; } z->t += 5;
    z->t += 13; cpu_wr(z, 0x9008, z->a);
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 13; cpu_wr(z, 0x9004, z->a);
    z->t += 13; cpu_wr(z, 0x9010, z->a);
    z->t += 13; cpu_wr(z, 0x9824, z->a);
    z->t += 10; return;
}

static void t_2A90(Cpu *z)
{
    z->t += 13; z->a = cpu_rd(z, 0x92A0);
    z->t += 4; z->a = cpu_dec(z, z->a);
    z->t += 7; cpu_and(z, 0x03);
    if (!z->zf) { z->t += 11; return; } z->t += 5;
    z->t += 13; z->a = cpu_rd(z, 0x92A7);
    z->t += 4; z->b = z->a;
    z->t += 13; z->a = cpu_rd(z, 0x9008);
    z->t += 4; cpu_or(z, z->b);
    if (z->zf) { z->t += 12; goto x2AE9; } z->t += 7;
    z->t += 13; z->a = cpu_rd(z, 0x920F);
    z->t += 4; cpu_and(z, z->a);
    z->t += 7; z->c = 0x01;
    if (z->zf) { z->t += 12; goto x2AAB; } z->t += 7;
    z->t += 4; z->c = cpu_dec(z, z->c);
    z->t += 4; z->c = cpu_dec(z, z->c);
x2AAB:
    z->t += 7; z->l = 0x00;
    z->t += 7; z->b = 0x0A;
x2AAF:
    z->t += 7; z->h = 0x99;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; cpu_add(z, z->c);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 7; z->h = 0x98;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; cpu_add(z, z->c);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 4; z->l = cpu_inc(z, z->l);
    if (--z->b) { z->t += 13; goto x2AAF; } z->t += 8;
    z->t += 13; z->a = cpu_rd(z, 0x9824);
    z->t += 4; cpu_and(z, z->a);
    z->t += 13; z->a = cpu_rd(z, 0x9900);
    if (z->zf) { z->t += 12; goto x2AC9; } z->t += 7;
    z->t += 4; cpu_and(z, z->a);
    if (z->zf) { z->t += 12; goto x2ADA; } z->t += 7;
x2AC9:
    z->t += 7; cpu_cp(z, 0x20);
    if (!z->zf) { z->t += 12; goto x2AD3; } z->t += 7;
    z->t += 7; z->a = 0x01;
    z->t += 13; cpu_wr(z, 0x920F, z->a);
    z->t += 10; return;
x2AD3:
    z->t += 7; cpu_sub(z, 0xE0);
    if (!z->zf) { z->t += 11; return; } z->t += 5;
    z->t += 13; cpu_wr(z, 0x920F, z->a);
    z->t += 10; return;
x2ADA:
    z->t += 4; cpu_xor(z, z->a);
    z->t += 13; cpu_wr(z, 0x920F, z->a);
    z->t += 13; cpu_wr(z, 0x900A, z->a);
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 13; cpu_wr(z, 0x9AA0, z->a);
    z->t += 13; cpu_wr(z, 0x9009, z->a);
    z->t += 10; return;
x2AE9:
    z->t += 13; cpu_wr(z, 0x900A, z->a);
    z->t += 10; return;
}

/* $003B: JP (HL), into a task */
static void task(Cpu *z)
{
    z->t += 4;
    switch (HL(z)) {
    case 0x0827: t_0827(z); return;
    case 0x0828: t_0828(z); return;
    case 0x0857: t_0857(z); return;
    case 0x0935: t_0935(z); return;
    case 0x0977: t_0977(z); return;
    case 0x1700: t_1700(z); return;
    case 0x17B2: t_17B2(z); return;
    case 0x19B2: t_19B2(z); return;
    case 0x1A86: t_1A86(z); return;
    case 0x1B6B: t_1B6B(z); return;
    case 0x1D38: t_1D38(z); return;
    case 0x1D7C: t_1D7C(z); return;
    case 0x1DB9: t_1DB9(z); return;
    case 0x1DD8: t_1DD8(z); return;
    case 0x1DEC: t_1DEC(z); return;
    case 0x1EAA: t_1EAA(z); return;
    case 0x1F0E: t_1F0E(z); return;
    case 0x1F8F: t_1F8F(z); return;
    case 0x2000: t_2000(z); return;
    case 0x20F2: t_20F2(z); return;
    case 0x21CB: t_21CB(z); return;
    case 0x2222: t_2222(z); return;
    case 0x23DD: t_23DD(z); return;
    case 0x2916: t_2916(z); return;
    case 0x2A90: t_2A90(z); return;
    default: game_unported(z->g, HL(z)); return;
    }
}

void task_run(Cpu *z, uint16_t addr)
{
    SET_HL(z, addr);
    task(z);
}
