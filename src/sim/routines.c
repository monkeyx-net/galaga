#include "sim/routines.h"

void m_0018(Cpu *z)
{
x0018:
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 6; SET_HL(z, HL(z) + 1);
    if (--z->b) { z->t += 13; goto x0018; } z->t += 8;
    z->t += 10; return;
}

void m_0020(Cpu *z)
{
    z->t += 4; z->a = z->e;
    z->t += 7; cpu_sub(z, 0x20);
    z->t += 4; z->e = z->a;
    if (!z->cf) { z->t += 11; return; } z->t += 5;
    z->t += 4; z->d = cpu_dec(z, z->d);
    z->t += 10; return;
}

void m_0028(Cpu *z)
{
    z->t += 10; SET_HL(z, 0x9100);
    z->t += 7; z->b = 0xF0;
    z->t += 4; cpu_xor(z, z->a);
    z->t += 11; m_0018(z);
    z->t += 10; return;
}

void m_0030(Cpu *z)
{
    z->t += 4; cpu_scf(z);
    z->t += 4; cpu_ex_af(z);
    z->t += 10; goto x13B5;
x13B5:
    z->t += 11; cpu_push(z, DE(z));
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 4; z->a = z->c;
    z->t += 10; SET_HL(z, 0x13EF);
    cpu_rst08(z);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; z->h = cpu_rd(z, HL(z));
    z->t += 4; z->l = z->a;
    z->t += 4; cpu_ex_af(z);
    if (!z->cf) { z->t += 12; goto x13C9; } z->t += 7;
    z->t += 6; SET_HL(z, HL(z) - 1);
    z->t += 6; SET_HL(z, HL(z) - 1);
    z->t += 7; z->e = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; z->d = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
x13C9:
    z->t += 7; z->c = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 4; cpu_ex_de_hl(z);
x13CC:
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 7; cpu_cp(z, 0x2F);
    if (z->zf) { z->t += 12; goto x13EF; } z->t += 7;
    z->t += 7; cpu_sub(z, 0x30);
    if (!z->cf) { z->t += 12; goto x13D9; } z->t += 7;
    z->t += 7; z->a = 0x24;
    z->t += 12; goto x13DF;
x13D9:
    z->t += 7; cpu_cp(z, 0x11);
    if (z->cf) { z->t += 12; goto x13DF; } z->t += 7;
    z->t += 7; cpu_sub(z, 0x07);
x13DF:
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 8; z->h = (uint8_t)(z->h | 0x04);
    z->t += 7; cpu_wr(z, HL(z), z->c);
    z->t += 8; z->h = (uint8_t)(z->h & 0xFB);
    z->t += 6; SET_DE(z, DE(z) + 1);
    z->t += 4; z->a = z->l;
    z->t += 7; cpu_sub(z, 0x20);
    z->t += 4; z->l = z->a;
    if (!z->cf) { z->t += 12; goto x13CC; } z->t += 7;
    z->t += 4; z->h = cpu_dec(z, z->h);
    z->t += 12; goto x13CC;
x13EF:
    z->t += 10; SET_DE(z, cpu_pop(z));
    z->t += 10; return;
}

void m_003C(Cpu *z)
{
    z->t += 10; SET_HL(z, 0x9300);
    z->t += 7; z->b = 0x80;
    z->t += 4; cpu_xor(z, z->a);
    z->t += 11; m_0018(z);
    z->t += 10; SET_HL(z, 0x9B00);
    z->t += 7; z->b = 0x80;
    z->t += 11; m_0018(z);
    z->t += 10; SET_HL(z, 0x8800);
    z->t += 7; z->a = 0x80;
    z->t += 7; z->b = 0x80;
    z->t += 11; m_0018(z);
    z->t += 10; return;
}

void m_00D6(Cpu *z)
{
    z->t += 10; SET_DE(z, 0x83ED);
    z->t += 10; SET_HL(z, 0x02B9);
    z->t += 10; SET_BC(z, 0x0005);
    cpu_ldir(z);
    z->t += 7; z->e = 0xCB;
    z->t += 10; SET_HL(z, 0x00EB);
    z->t += 7; z->c = 0x11;
    cpu_ldir(z);
    z->t += 10; return;
}

void m_0160(Cpu *z)
{
    z->t += 10; SET_HL(z, 0x8040);
    z->t += 10; SET_DE(z, 0x8041);
    z->t += 10; SET_BC(z, 0x037F);
    z->t += 10; cpu_wr(z, HL(z), 0x24);
    cpu_ldir(z);
    z->t += 10; SET_HL(z, 0x8440);
    z->t += 10; SET_DE(z, 0x8441);
    z->t += 10; SET_BC(z, 0x037F);
    z->t += 10; cpu_wr(z, HL(z), 0x00);
    cpu_ldir(z);
    z->t += 7; z->a = 0x04;
    z->t += 7; z->b = 0x20;
    z->t += 11; m_0018(z);
    z->t += 7; z->a = 0x4E;
    z->t += 7; z->b = 0x20;
    z->t += 11; m_0018(z);
    z->t += 10; return;
}

void m_0185(Cpu *z)
{
x0185:
    z->t += 10; SET_HL(z, 0x9821);
    z->t += 11; cpu_wr(z, HL(z), cpu_inc(z, cpu_rd(z, HL(z))));
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 7; cpu_and(z, 0x03);
    z->t += 13; cpu_wr(z, 0x9825, z->a);
    if (z->zf) { z->t += 12; goto x01A2; } z->t += 7;
    z->t += 7; z->c = 0x06;
    z->t += 11; m_0030(z);
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 13; z->a = cpu_rd(z, 0x9821);
    z->t += 4; z->l = z->a;
    z->t += 7; z->h = 0x00;
    z->t += 17; m_0A53(z);
    z->t += 4; cpu_xor(z, z->a);
    z->t += 12; goto x01AC;
x01A2:
    z->t += 7; z->c = 0x07;
    z->t += 11; m_0030(z);
    z->t += 7; z->a = 0x01;
    z->t += 13; cpu_wr(z, 0x9AAD, z->a);
    z->t += 7; z->a = 0x08;
x01AC:
    z->t += 13; cpu_wr(z, 0x92A8, z->a);
    z->t += 7; z->a = 0x03;
    z->t += 13; cpu_wr(z, 0x92AE, z->a);
    z->t += 13; cpu_wr(z, 0x920B, z->a);
    z->t += 13; z->a = cpu_rd(z, 0x9825);
    z->t += 4; cpu_and(z, z->a);
    z->t += 4; cpu_ex_af(z);
    z->t += 17; m_117F(z);
x01BF:
    z->t += 13; z->a = cpu_rd(z, 0x92AE);
    z->t += 4; cpu_and(z, z->a);
    if (!z->zf) { z->t += 12; goto x01BF; } z->t += 7;
    z->t += 7; z->a = 0x78;
    z->t += 13; cpu_wr(z, 0x92AE, z->a);
    z->t += 17; m_2896(z);
    z->t += 17; m_25A2(z);
    z->t += 7; z->a = 0x02;
    z->t += 13; cpu_wr(z, 0x92AC, z->a);
    z->t += 4; cpu_xor(z, z->a);
    z->t += 17; m_12D5(z);
    z->t += 4; cpu_xor(z, z->a);
    z->t += 7; z->b = 0x30;
    z->t += 10; SET_HL(z, 0x9200);
x01DF:
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 4; z->l = cpu_inc(z, z->l);
    if (--z->b) { z->t += 13; goto x01DF; } z->t += 8;
    z->t += 13; cpu_wr(z, 0x9009, z->a);
    z->t += 13; cpu_wr(z, 0x9010, z->a);
    z->t += 13; cpu_wr(z, 0x9004, z->a);
    z->t += 13; cpu_wr(z, 0x9288, z->a);
    z->t += 13; cpu_wr(z, 0x982C, z->a);
    z->t += 13; cpu_wr(z, 0x9841, z->a);
    z->t += 13; cpu_wr(z, 0x9842, z->a);
    z->t += 13; cpu_wr(z, 0x9826, z->a);
    z->t += 13; cpu_wr(z, 0x99B0, z->a);
    z->t += 13; cpu_wr(z, 0x9824, z->a);
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 13; cpu_wr(z, 0x982D, z->a);
    z->t += 13; cpu_wr(z, 0x986D, z->a);
    z->t += 13; cpu_wr(z, 0x9828, z->a);
    z->t += 13; cpu_wr(z, 0x900B, z->a);
    z->t += 13; cpu_wr(z, 0x9008, z->a);
    z->t += 13; cpu_wr(z, 0x900A, z->a);
    z->t += 17; m_2C00(z);
    z->t += 10; SET_HL(z, 0x9830);
    z->t += 10; SET_DE(z, 0x01B5);
    z->t += 7; z->b = 0x04;
x0220:
    z->t += 7; cpu_wr(z, HL(z), z->d);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 7; cpu_wr(z, HL(z), z->e);
    z->t += 4; z->l = cpu_inc(z, z->l);
    if (--z->b) { z->t += 13; goto x0220; } z->t += 8;
    z->t += 13; z->a = cpu_rd(z, 0x6805);
    z->t += 8; cpu_bit(z, 1, z->a);
    if (!z->zf) { z->t += 11; return; } z->t += 5;
    z->t += 7; z->c = 0x0B;
    z->t += 10; SET_HL(z, 0x83B0);
    z->t += 17; m_13B3(z);
    z->t += 10; goto x0185;
}

void m_01C5(Cpu *z)
{
    goto x01C5;
x0185:
    z->t += 10; SET_HL(z, 0x9821);
    z->t += 11; cpu_wr(z, HL(z), cpu_inc(z, cpu_rd(z, HL(z))));
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 7; cpu_and(z, 0x03);
    z->t += 13; cpu_wr(z, 0x9825, z->a);
    if (z->zf) { z->t += 12; goto x01A2; } z->t += 7;
    z->t += 7; z->c = 0x06;
    z->t += 11; m_0030(z);
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 13; z->a = cpu_rd(z, 0x9821);
    z->t += 4; z->l = z->a;
    z->t += 7; z->h = 0x00;
    z->t += 17; m_0A53(z);
    z->t += 4; cpu_xor(z, z->a);
    z->t += 12; goto x01AC;
x01A2:
    z->t += 7; z->c = 0x07;
    z->t += 11; m_0030(z);
    z->t += 7; z->a = 0x01;
    z->t += 13; cpu_wr(z, 0x9AAD, z->a);
    z->t += 7; z->a = 0x08;
x01AC:
    z->t += 13; cpu_wr(z, 0x92A8, z->a);
    z->t += 7; z->a = 0x03;
    z->t += 13; cpu_wr(z, 0x92AE, z->a);
    z->t += 13; cpu_wr(z, 0x920B, z->a);
    z->t += 13; z->a = cpu_rd(z, 0x9825);
    z->t += 4; cpu_and(z, z->a);
    z->t += 4; cpu_ex_af(z);
    z->t += 17; m_117F(z);
x01BF:
    if (game_wait_for(z->g, 0x92AE, 0)) return; /* the IRQ's timer */
    z->t += 13; z->a = cpu_rd(z, 0x92AE);
    z->t += 4; cpu_and(z, z->a);
    if (!z->zf) { z->t += 12; goto x01BF; } z->t += 7;
x01C5:
    z->t += 7; z->a = 0x78;
    z->t += 13; cpu_wr(z, 0x92AE, z->a);
    z->t += 17; m_2896(z);
    z->t += 17; m_25A2(z);
    z->t += 7; z->a = 0x02;
    z->t += 13; cpu_wr(z, 0x92AC, z->a);
    z->t += 4; cpu_xor(z, z->a);
    z->t += 17; m_12D5(z);
    z->t += 4; cpu_xor(z, z->a);
    z->t += 7; z->b = 0x30;
    z->t += 10; SET_HL(z, 0x9200);
x01DF:
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 4; z->l = cpu_inc(z, z->l);
    if (--z->b) { z->t += 13; goto x01DF; } z->t += 8;
    z->t += 13; cpu_wr(z, 0x9009, z->a);
    z->t += 13; cpu_wr(z, 0x9010, z->a);
    z->t += 13; cpu_wr(z, 0x9004, z->a);
    z->t += 13; cpu_wr(z, 0x9288, z->a);
    z->t += 13; cpu_wr(z, 0x982C, z->a);
    z->t += 13; cpu_wr(z, 0x9841, z->a);
    z->t += 13; cpu_wr(z, 0x9842, z->a);
    z->t += 13; cpu_wr(z, 0x9826, z->a);
    z->t += 13; cpu_wr(z, 0x99B0, z->a);
    z->t += 13; cpu_wr(z, 0x9824, z->a);
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 13; cpu_wr(z, 0x982D, z->a);
    z->t += 13; cpu_wr(z, 0x986D, z->a);
    z->t += 13; cpu_wr(z, 0x9828, z->a);
    z->t += 13; cpu_wr(z, 0x900B, z->a);
    z->t += 13; cpu_wr(z, 0x9008, z->a);
    z->t += 13; cpu_wr(z, 0x900A, z->a);
    z->t += 17; m_2C00(z);
    z->t += 10; SET_HL(z, 0x9830);
    z->t += 10; SET_DE(z, 0x01B5);
    z->t += 7; z->b = 0x04;
x0220:
    z->t += 7; cpu_wr(z, HL(z), z->d);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 7; cpu_wr(z, HL(z), z->e);
    z->t += 4; z->l = cpu_inc(z, z->l);
    if (--z->b) { z->t += 13; goto x0220; } z->t += 8;
    z->t += 13; z->a = cpu_rd(z, 0x6805);
    z->t += 8; cpu_bit(z, 1, z->a);
    if (!z->zf) { z->t += 11; return; } z->t += 5;
    z->t += 7; z->c = 0x0B;
    z->t += 10; SET_HL(z, 0x83B0);
    z->t += 17; m_13B3(z);
    z->t += 10; goto x0185;
}

void m_043D(Cpu *z)
{
    z->t += 11; m_0030(z);
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 4; z->a = z->e;
    z->t += 7; cpu_add(z, 0x40);
    z->t += 4; z->e = z->a;
    z->t += 7; z->h = 0x00;
    z->t += 17; m_0A53(z);
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 7; z->c = 0x1E;
    z->t += 17; m_13B3(z);
    z->t += 17; m_129E(z);
    z->t += 10; return;
}

void m_0466(Cpu *z)
{
    z->t += 13; z->a = cpu_rd(z, 0x9982);
    z->t += 13; cpu_wr(z, 0x9820, z->a);
    z->t += 13; cpu_wr(z, 0x9860, z->a);
    z->t += 10; SET_DE(z, 0x83F8);
    z->t += 10; SET_HL(z, 0x0495);
    z->t += 17; m_0486(z);
    z->t += 10; SET_DE(z, 0x83E3);
    z->t += 10; SET_HL(z, 0x0495);
    z->t += 13; z->a = cpu_rd(z, 0x99B3);
    z->t += 4; cpu_and(z, z->a);
    if (!z->zf) { z->t += 12; goto x0486; } z->t += 7;
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 6; SET_HL(z, HL(z) + 1);
x0486:
    z->t += 7; z->c = 0x07;
    cpu_ldir(z);
    z->t += 10; SET_HL(z, 0x0497);
    z->t += 10; SET_DE(z, 0x83C3);
    z->t += 7; z->c = 0x04;
    cpu_ldir(z);
    z->t += 10; return;
}

void m_0486(Cpu *z)
{
    z->t += 7; z->c = 0x07;
    cpu_ldir(z);
    z->t += 10; SET_HL(z, 0x0497);
    z->t += 10; SET_DE(z, 0x83C3);
    z->t += 7; z->c = 0x04;
    cpu_ldir(z);
    z->t += 10; return;
}

void m_0728(Cpu *z)
{
    z->t += 13; z->a = cpu_rd(z, 0x9840);
    z->t += 4; cpu_and(z, z->a);
    z->t += 7; z->a = 0xF9;
    if (z->zf) { z->t += 12; goto x0732; } z->t += 7;
    z->t += 7; z->a = 0xE4;
x0732:
    z->t += 8; z->ix = (uint16_t)((z->ix & 0xFF00) | (uint8_t)(z->a));
    z->t += 7; z->b = 0x10;
    z->t += 10; SET_HL(z, 0x9290);
x0739:
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 10; SET_HL(z, 0x07FA);
    z->t += 4; z->a = z->b;
    cpu_rst10(z);
    z->t += 7; z->c = cpu_rd(z, HL(z));
x0740:
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; cpu_and(z, z->a);
    if (z->zf) { z->t += 12; goto x0762; } z->t += 7;
    z->t += 11; cpu_wr(z, HL(z), cpu_dec(z, cpu_rd(z, HL(z))));
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 7; z->h = 0x83;
    z->t += 8; z->a = (uint8_t)z->ix;
    z->t += 4; z->l = z->a;
    z->t += 4; z->a = z->c;
    z->t += 7; cpu_and(z, 0x0F);
    z->t += 17; m_07D8(z);
    z->t += 8; z->a = (uint8_t)z->ix;
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 4; z->l = z->a;
    z->t += 4; z->a = z->c;
    z->t += 4; cpu_rlca(z);
    z->t += 4; cpu_rlca(z);
    z->t += 4; cpu_rlca(z);
    z->t += 4; cpu_rlca(z);
    z->t += 7; cpu_and(z, 0x0F);
    z->t += 17; m_07D8(z);
    z->t += 12; goto x0740;
x0762:
    z->t += 4; z->l = cpu_inc(z, z->l);
    if (--z->b) { z->t += 13; goto x0739; } z->t += 8;
    z->t += 8; z->a = (uint8_t)z->ix;
    z->t += 7; cpu_add(z, 0x04);
    z->t += 4; z->e = z->a;
    z->t += 10; SET_HL(z, 0x83F2);
    z->t += 7; z->d = 0x83;
    z->t += 7; z->b = 0x06;
x0771:
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 7; cpu_sub(z, cpu_rd(z, HL(z)));
    z->t += 7; cpu_add(z, 0x09);
    z->t += 7; cpu_cp(z, 0xE5);
    if (!z->cf) { z->t += 12; goto x0788; } z->t += 7;
    z->t += 7; cpu_sub(z, 0x0A);
    z->t += 7; cpu_cp(z, 0x09);
    if (z->cf) { z->t += 12; goto x0788; } z->t += 7;
    z->t += 4; z->a = cpu_inc(z, z->a);
    if (!z->zf) { z->t += 12; goto x078E; } z->t += 7;
    z->t += 4; z->l = cpu_dec(z, z->l);
    z->t += 4; z->e = cpu_dec(z, z->e);
    if (--z->b) { z->t += 13; goto x0771; } z->t += 8;
    z->t += 12; goto x078E;
x0788:
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 4; z->l = cpu_dec(z, z->l);
    z->t += 4; z->e = cpu_dec(z, z->e);
    if (--z->b) { z->t += 13; goto x0788; } z->t += 8;
x078E:
    z->t += 8; z->a = (uint8_t)z->ix;
    z->t += 7; cpu_add(z, 0x04);
    z->t += 4; z->l = z->a;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_cp(z, 0x24);
    if (!z->zf) { z->t += 12; goto x0799; } z->t += 7;
    z->t += 4; cpu_xor(z, z->a);
x0799:
    z->t += 7; cpu_and(z, 0x3F);
    z->t += 4; cpu_rlca(z);
    z->t += 4; z->c = z->a;
    z->t += 4; cpu_rlca(z);
    z->t += 4; cpu_rlca(z);
    z->t += 4; cpu_add(z, z->c);
    z->t += 4; z->c = z->a;
    z->t += 4; z->l = cpu_dec(z, z->l);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_cp(z, 0x24);
    if (!z->zf) { z->t += 12; goto x07A8; } z->t += 7;
    z->t += 4; cpu_xor(z, z->a);
x07A8:
    z->t += 4; cpu_add(z, z->c);
    z->t += 10; SET_HL(z, 0x983E);
    z->t += 7; cpu_cp(z, cpu_rd(z, HL(z)));
    if (!z->zf) { z->t += 11; return; } z->t += 5;
    z->t += 13; z->a = cpu_rd(z, 0x9981);
    z->t += 4; z->b = z->a;
    z->t += 7; cpu_and(z, 0x7F);
    z->t += 4; z->c = z->a;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; cpu_cp(z, z->c);
    if (!z->cf) { z->t += 12; goto x07BC; } z->t += 7;
    z->t += 4; z->a = z->c;
    z->t += 12; goto x07BD;
x07BC:
    z->t += 4; cpu_add(z, z->b);
x07BD:
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 13; cpu_wr(z, 0x9AAA, z->a);
    z->t += 10; SET_HL(z, 0x9820);
    z->t += 11; cpu_wr(z, HL(z), cpu_inc(z, cpu_rd(z, HL(z))));
    z->t += 17; m_137E(z);
    z->t += 10; SET_HL(z, 0x99EB);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_add(z, 0x01);
    z->t += 4; cpu_daa(z);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    if (!z->cf) { z->t += 11; return; } z->t += 5;
    z->t += 4; z->l = cpu_dec(z, z->l);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_add(z, 0x01);
    z->t += 4; cpu_daa(z);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 10; return;
}

void m_07D8(Cpu *z)
{
    z->t += 4; cpu_and(z, z->a);
    if (z->zf) { z->t += 11; return; } z->t += 5;
    z->t += 7; cpu_add(z, cpu_rd(z, HL(z)));
    z->t += 7; cpu_cp(z, 0x24);
    if (z->cf) { z->t += 12; goto x07E1; } z->t += 7;
    z->t += 7; cpu_sub(z, 0x24);
x07E1:
    z->t += 7; cpu_cp(z, 0x0A);
    if (!z->cf) { z->t += 12; goto x07E7; } z->t += 7;
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 10; return;
x07E7:
    z->t += 7; cpu_sub(z, 0x0A);
x07E9:
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_cp(z, 0x24);
    if (!z->zf) { z->t += 12; goto x07F1; } z->t += 7;
    z->t += 4; cpu_xor(z, z->a);
x07F1:
    z->t += 7; cpu_cp(z, 0x09);
    if (z->zf) { z->t += 12; goto x07F8; } z->t += 7;
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 10; return;
x07F8:
    z->t += 4; cpu_xor(z, z->a);
    z->t += 12; goto x07E9;
}

void m_080B(Cpu *z)
{
    goto x080B;
x045E:
    z->t += 17; m_0728(z);
    z->t += 17; m_080B(z);
    z->t += 12; goto x045E;
x049E:
    z->t += 10; SET_HL(z, cpu_pop(z));
    z->t += 10; SET_HL(z, 0x92AF);
    z->t += 10; cpu_wr(z, HL(z), 0x04);
x04A4:
    z->t += 13; z->a = cpu_rd(z, 0x901D);
    z->t += 4; cpu_and(z, z->a);
    if (z->zf) { z->t += 12; goto x04C1; } z->t += 7;
    z->t += 4; cpu_xor(z, z->a);
    z->t += 13; cpu_wr(z, 0x9213, z->a);
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 13; cpu_wr(z, 0x9025, z->a);
    z->t += 13; z->a = cpu_rd(z, 0x92A7);
    z->t += 4; cpu_and(z, z->a);
    z->t += 10; if (!z->zf) { goto x045E; }
x04B9:
    z->t += 13; z->a = cpu_rd(z, 0x901D);
    z->t += 4; cpu_and(z, z->a);
    if (!z->zf) { z->t += 12; goto x04B9; } z->t += 7;
    z->t += 12; goto x04DC;
x04C1:
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; cpu_and(z, z->a);
    if (!z->zf) { z->t += 12; goto x04A4; } z->t += 7;
    z->t += 17; m_0728(z);
    z->t += 13; z->a = cpu_rd(z, 0x92A7);
    z->t += 13; cpu_wr(z, 0x9843, z->a);
    z->t += 4; z->c = z->a;
    z->t += 13; z->a = cpu_rd(z, 0x9213);
    z->t += 4; cpu_or(z, z->c);
    if (!z->zf) { z->t += 12; goto x04E2; } z->t += 7;
    z->t += 13; z->a = cpu_rd(z, 0x9825);
    z->t += 4; cpu_and(z, z->a);
    z->t += 10; if (z->zf) { goto x0650; }
x04DC:
    z->t += 17; m_0185(z);
    z->t += 10; goto x0632;
x04E2:
    z->t += 10; SET_HL(z, 0x9820);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 11; cpu_wr(z, HL(z), cpu_dec(z, cpu_rd(z, HL(z))));
    z->t += 4; cpu_and(z, z->a);
    z->t += 10; if (!z->zf) { goto x0579; }
    z->t += 13; z->a = cpu_rd(z, 0x99B3);
    z->t += 4; cpu_and(z, z->a);
    if (z->zf) { z->t += 12; goto x04FD; } z->t += 7;
    z->t += 10; SET_HL(z, 0x824E);
    z->t += 13; z->a = cpu_rd(z, 0x9840);
    z->t += 7; cpu_add(z, 0x04);
    z->t += 4; z->c = z->a;
    z->t += 17; m_13B3(z);
x04FD:
    z->t += 7; z->c = 0x02;
    z->t += 11; m_0030(z);
    z->t += 17; m_1331(z);
    z->t += 17; m_1331(z);
    z->t += 10; SET_HL(z, 0x9018);
x0509:
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; cpu_and(z, z->a);
    if (!z->zf) { z->t += 12; goto x0509; } z->t += 7;
    z->t += 11; m_0028(z);
    z->t += 17; m_003C(z);
    z->t += 17; m_0160(z);
    z->t += 7; z->c = 0x15;
    z->t += 11; m_0030(z);
    z->t += 7; z->c = 0x16;
    z->t += 11; m_0030(z);
    z->t += 10; SET_DE(z, 0x8132);
    z->t += 16; SET_HL(z, cpu_rd16(z, 0x9846));
    z->t += 17; m_0A53(z);
    z->t += 7; z->c = 0x18;
    z->t += 11; m_0030(z);
    z->t += 10; SET_DE(z, 0x8135);
    z->t += 16; SET_HL(z, cpu_rd16(z, 0x9844));
    z->t += 17; m_0A53(z);
    z->t += 7; z->c = 0x19;
    z->t += 11; m_0030(z);
    z->t += 17; m_0A72(z);
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 7; z->c = 0x1A;
    z->t += 17; m_13B3(z);
    z->t += 10; SET_HL(z, 0x92AE);
    z->t += 10; cpu_wr(z, HL(z), 0x0E);
x0540:
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; cpu_and(z, z->a);
    if (!z->zf) { z->t += 12; goto x0540; } z->t += 7;
    z->t += 17; m_0160(z);
    z->t += 17; m_3000(z);
    z->t += 4; cpu_xor(z, z->a);
    z->t += 13; cpu_wr(z, 0x9AB0, z->a);
    z->t += 10; SET_HL(z, 0x9AAC);
    z->t += 10; SET_DE(z, 0x9AB6);
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 7; z->b = cpu_rd(z, HL(z));
    z->t += 4; cpu_or(z, z->b);
    if (z->zf) { z->t += 12; goto x0562; } z->t += 7;
    z->t += 4; z->b = cpu_inc(z, z->b);
    z->t += 4; z->b = cpu_dec(z, z->b);
    if (z->zf) { z->t += 12; goto x055F; } z->t += 7;
    z->t += 10; cpu_wr(z, HL(z), 0x01);
x055F:
    z->t += 4; z->halted = 1; return; /* HALT */
x0562:
    z->t += 17; m_0160(z);
    z->t += 13; z->a = cpu_rd(z, 0x99B3);
    z->t += 4; cpu_and(z, z->a);
    z->t += 10; if (z->zf) { goto x06DE; }
    z->t += 13; z->a = cpu_rd(z, 0x9860);
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 10; if (z->zf) { goto x06DE; }
    z->t += 13; z->a = cpu_rd(z, 0x9213);
    z->t += 4; z->a = cpu_dec(z, z->a);
    if (!z->zf) { z->t += 12; goto x058E; } z->t += 7;
x0579:
    z->t += 13; z->a = cpu_rd(z, 0x99B3);
    z->t += 4; cpu_and(z, z->a);
    z->t += 10; if (z->zf) { goto x0604; }
    z->t += 13; z->a = cpu_rd(z, 0x9860);
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 10; if (z->zf) { goto x0612; }
    z->t += 13; z->a = cpu_rd(z, 0x9213);
    z->t += 4; z->a = cpu_dec(z, z->a);
    z->t += 10; if (!z->zf) { goto x0612; }
x058E:
    z->t += 13; z->a = cpu_rd(z, 0x92A7);
    z->t += 4; cpu_and(z, z->a);
    if (z->zf) { z->t += 12; goto x059A; } z->t += 7;
x0594:
    z->t += 13; z->a = cpu_rd(z, 0x9287);
    z->t += 4; cpu_and(z, z->a);
    if (!z->zf) { z->t += 12; goto x0594; } z->t += 7;
x059A:
    z->t += 4; cpu_xor(z, z->a);
    z->t += 13; cpu_wr(z, 0x99B4, z->a);
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 10; SET_HL(z, 0x900E);
    z->t += 7; cpu_wr(z, HL(z), z->a);
x05A3:
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; cpu_and(z, z->a);
    if (!z->zf) { z->t += 12; goto x05A3; } z->t += 7;
    z->t += 13; z->a = cpu_rd(z, 0x9AA0);
    z->t += 13; cpu_wr(z, 0x9848, z->a);
    z->t += 13; z->a = cpu_rd(z, 0x92AE);
    z->t += 13; cpu_wr(z, 0x983F, z->a);
    z->t += 17; m_110C(z);
    z->t += 17; m_2C00(z);
    z->t += 13; z->a = cpu_rd(z, 0x983F);
    z->t += 13; cpu_wr(z, 0x92AE, z->a);
    z->t += 13; z->a = cpu_rd(z, 0x9848);
    z->t += 13; cpu_wr(z, 0x9AA0, z->a);
    z->t += 17; m_137E(z);
    z->t += 13; z->a = cpu_rd(z, 0x9843);
    z->t += 4; cpu_and(z, z->a);
    if (z->zf) { z->t += 12; goto x05D1; } z->t += 7;
    z->t += 17; m_25A2(z);
x05D1:
    z->t += 13; z->a = cpu_rd(z, 0x9840);
    z->t += 4; z->c = z->a;
    z->t += 13; z->a = cpu_rd(z, 0x9983);
    z->t += 4; cpu_and(z, z->c);
    z->t += 13; cpu_wr(z, 0xA007, z->a);
    z->t += 13; cpu_wr(z, 0x9215, z->a);
    z->t += 7; z->a = 0x3F;
    z->t += 17; m_12D5(z);
    z->t += 4; cpu_scf(z);
    z->t += 4; cpu_ex_af(z);
    z->t += 17; m_117F(z);
    z->t += 13; z->a = cpu_rd(z, 0x9843);
    z->t += 4; cpu_and(z, z->a);
    if (z->zf) { z->t += 12; goto x060F; } z->t += 7;
    z->t += 7; z->c = 0x03;
    z->t += 11; m_0030(z);
    z->t += 7; z->a = 0x80;
    z->t += 13; cpu_wr(z, 0x99B4, z->a);
    z->t += 10; SET_HL(z, 0x900E);
    z->t += 7; z->a = 0x01;
    z->t += 7; cpu_wr(z, HL(z), z->a);
x05FD:
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; cpu_and(z, z->a);
    if (!z->zf) { z->t += 12; goto x05FD; } z->t += 7;
    z->t += 10; goto x0612;
x0604:
    z->t += 13; z->a = cpu_rd(z, 0x9843);
    z->t += 4; cpu_and(z, z->a);
    if (!z->zf) { z->t += 12; goto x061E; } z->t += 7;
    z->t += 17; m_0185(z);
    z->t += 12; goto x061E;
x060F:
    z->t += 17; m_0185(z);
x0612:
    z->t += 13; z->a = cpu_rd(z, 0x9840);
    z->t += 7; cpu_add(z, 0x04);
    z->t += 4; z->c = z->a;
    z->t += 10; SET_HL(z, 0x826E);
    z->t += 17; m_13B3(z);
x061E:
    z->t += 17; m_133D(z);
    z->t += 13; z->a = cpu_rd(z, 0x92AE);
    z->t += 7; cpu_add(z, 0x1E);
    z->t += 7; cpu_cp(z, 0x78);
    if (z->cf) { z->t += 12; goto x062C; } z->t += 7;
    z->t += 7; z->a = 0x78;
x062C:
    z->t += 13; cpu_wr(z, 0x92AE, z->a);
    z->t += 17; m_1331(z);
x0632:
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
x0650:
    z->t += 13; z->a = cpu_rd(z, 0x9288);
    z->t += 4; z->e = z->a;
    z->t += 10; SET_HL(z, 0x9AAE);
    z->t += 7; cpu_cp(z, 0x28);
    if (!z->zf) { z->t += 12; goto x065E; } z->t += 7;
    z->t += 10; SET_HL(z, 0x9AB4);
x065E:
    z->t += 10; cpu_wr(z, HL(z), 0x01);
    z->t += 17; m_1331(z);
    z->t += 7; z->c = 0x08;
    z->t += 11; m_0030(z);
    z->t += 17; m_1331(z);
    z->t += 4; z->l = z->e;
    z->t += 7; z->h = 0x00;
    z->t += 10; SET_DE(z, 0x8110);
    z->t += 17; m_0A53(z);
    z->t += 17; m_1331(z);
    z->t += 13; z->a = cpu_rd(z, 0x9288);
    z->t += 7; cpu_cp(z, 0x28);
    if (z->zf) { z->t += 12; goto x0699; } z->t += 7;
    z->t += 7; z->c = 0x09;
    z->t += 11; m_0030(z);
    z->t += 17; m_1331(z);
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 13; z->a = cpu_rd(z, 0x9288);
    z->t += 4; cpu_and(z, z->a);
    if (z->zf) { z->t += 12; goto x0693; } z->t += 7;
    z->t += 4; z->l = z->a;
    z->t += 7; z->h = 0x00;
    z->t += 17; m_0A53(z);
    z->t += 4; cpu_xor(z, z->a);
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 11; m_0020(z);
    z->t += 4; cpu_xor(z, z->a);
x0693:
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 13; z->a = cpu_rd(z, 0x9288);
    z->t += 12; goto x06BA;
x0699:
    z->t += 7; z->b = 0x07;
x069B:
    z->t += 13; z->a = cpu_rd(z, 0x92A0);
    z->t += 7; cpu_and(z, 0x0F);
    if (!z->zf) { z->t += 12; goto x069B; } z->t += 7;
    z->t += 7; z->c = 0x0B;
    z->t += 8; cpu_bit(z, 0, z->b);
    if (z->zf) { z->t += 12; goto x06A9; } z->t += 7;
    z->t += 4; z->c = cpu_inc(z, z->c);
x06A9:
    z->t += 11; cpu_push(z, BC(z));
    z->t += 11; m_0030(z);
    z->t += 10; SET_BC(z, cpu_pop(z));
x06AC:
    z->t += 13; z->a = cpu_rd(z, 0x92A0);
    z->t += 7; cpu_and(z, 0x0F);
    if (z->zf) { z->t += 12; goto x06AC; } z->t += 7;
    if (--z->b) { z->t += 13; goto x069B; } z->t += 8;
    z->t += 7; z->c = 0x0D;
    z->t += 11; m_0030(z);
    z->t += 7; z->a = 0x64;
x06BA:
    z->t += 10; SET_HL(z, 0x929F);
    z->t += 7; cpu_add(z, cpu_rd(z, HL(z)));
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 17; m_0728(z);
    z->t += 17; m_1331(z);
    z->t += 17; m_1331(z);
    z->t += 10; SET_HL(z, 0x83B0);
    z->t += 7; z->c = 0x0B;
    z->t += 17; m_13B3(z);
    z->t += 10; SET_HL(z, 0x83B3);
    z->t += 7; z->c = 0x0B;
    z->t += 17; m_13B3(z);
    z->t += 7; z->c = 0x0B;
    z->t += 11; m_0030(z);
    z->t += 10; goto x04DC;
x06DE:
    z->t += 4; z->halted = 1; return; /* HALT */
x080B:
    z->t += 13; z->a = cpu_rd(z, 0x9008);
    z->t += 4; z->b = z->a;
    z->t += 13; z->a = cpu_rd(z, 0x92A7);
    z->t += 4; cpu_or(z, z->b);
    if (!z->zf) { z->t += 12; goto x081B; } z->t += 7;
    z->t += 13; cpu_wr(z, 0x9AA0, z->a);
    z->t += 10; goto x049E;
x081B:
    z->t += 13; z->a = cpu_rd(z, 0x9213);
    z->t += 4; cpu_and(z, z->a);
    if (z->zf) { z->t += 11; return; } z->t += 5;
    z->t += 4; cpu_xor(z, z->a);
    z->t += 13; cpu_wr(z, 0x9842, z->a);
    z->t += 10; goto x049E;
}

void m_08AD(Cpu *z)
{
    z->t += 4; z->e = z->a;
    z->t += 8; z->a = cpu_sla(z, z->a);
    z->t += 4; cpu_add(z, z->e);
    cpu_rst10(z);
    z->t += 4; z->a = z->b;
    z->t += 7; cpu_cp(z, 0x28);
    if (!z->cf) { z->t += 12; goto x08B8; } z->t += 7;
    z->t += 6; SET_HL(z, HL(z) + 1);
x08B8:
    z->t += 4; cpu_and(z, z->a);
    if (!z->zf) { z->t += 12; goto x08BC; } z->t += 7;
    z->t += 6; SET_HL(z, HL(z) + 1);
x08BC:
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 10; return;
}

void m_08BE(Cpu *z)
{
    z->t += 8; z->a = cpu_sla(z, z->a);
    cpu_rst08(z);
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 4; z->h = z->c;
    z->t += 7; z->a = 0x0A;
    z->t += 17; m_1061(z);
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 4; z->a = z->d;
    cpu_rst10(z);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 10; return;
}

void m_095F(Cpu *z)
{
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

void m_0A53(Cpu *z)
{
    z->t += 7; z->b = 0x01;
x0A55:
    z->t += 4; z->h = cpu_dec(z, z->h);
    z->t += 4; z->h = cpu_inc(z, z->h);
    if (!z->zf) { z->t += 12; goto x0A5E; } z->t += 7;
    z->t += 4; z->a = z->l;
    z->t += 7; cpu_cp(z, 0x0A);
    if (z->cf) { z->t += 12; goto x0A68; } z->t += 7;
x0A5E:
    z->t += 7; z->a = 0x0A;
    z->t += 17; m_1061(z);
    z->t += 11; cpu_push(z, AF(z));
    z->t += 4; z->b = cpu_inc(z, z->b);
    z->t += 12; goto x0A55;
x0A67:
    z->t += 10; cpu_set_af(z, cpu_pop(z));
x0A68:
    z->t += 17; m_0A6E(z);
    if (--z->b) { z->t += 13; goto x0A67; } z->t += 8;
    z->t += 10; return;
}

void m_0A6E(Cpu *z)
{
    goto x0A6E;
x0020:
    z->t += 4; z->a = z->e;
    z->t += 7; cpu_sub(z, 0x20);
    z->t += 4; z->e = z->a;
    if (!z->cf) { z->t += 11; return; } z->t += 5;
    z->t += 4; z->d = cpu_dec(z, z->d);
    z->t += 10; return;
x0A6E:
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 10; goto x0020;
}

void m_0A72(Cpu *z)
{
    z->t += 16; SET_HL(z, cpu_rd16(z, 0x9844));
    z->t += 20; SET_DE(z, cpu_rd16(z, 0x9846));
    z->t += 4; z->a = z->d;
    z->t += 4; cpu_or(z, z->e);
    if (!z->zf) { z->t += 12; goto x0A82; } z->t += 7;
    z->t += 10; SET_DE(z, 0x0000);
    z->t += 12; goto x0AD3;
x0A82:
    z->t += 8; cpu_bit(z, 7, z->d);
    if (!z->zf) { z->t += 12; goto x0A90; } z->t += 7;
    z->t += 8; cpu_bit(z, 7, z->h);
    if (!z->zf) { z->t += 12; goto x0A90; } z->t += 7;
    z->t += 11; SET_HL(z, cpu_add16(z, HL(z), HL(z)));
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 11; SET_HL(z, cpu_add16(z, HL(z), HL(z)));
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 12; goto x0A82;
x0A90:
    z->t += 4; z->a = z->d;
    z->t += 17; m_1061(z);
    z->t += 11; cpu_push(z, HL(z));
    z->t += 4; z->h = z->a;
    z->t += 7; z->l = 0x00;
    z->t += 4; z->a = z->d;
    z->t += 17; m_1061(z);
    z->t += 19; { uint16_t t = cpu_pop(z); cpu_push(z, HL(z)); SET_HL(z, t); }
    z->t += 10; SET_DE(z, 0x99B0);
    z->t += 7; z->b = 0x04;
    z->t += 4; z->a = z->h;
    z->t += 7; z->h = 0x00;
x0AA5:
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 18; cpu_rld(z);
    z->t += 8; cpu_bit(z, 0, z->b);
    if (z->zf) { z->t += 12; goto x0AAD; } z->t += 7;
    z->t += 4; z->l = cpu_inc(z, z->l);
x0AAD:
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 17; m_0B06(z);
    z->t += 4; cpu_ex_af(z);
    z->t += 19; { uint16_t t = cpu_pop(z); cpu_push(z, HL(z)); SET_HL(z, t); }
    z->t += 17; m_0B06(z);
    z->t += 19; { uint16_t t = cpu_pop(z); cpu_push(z, HL(z)); SET_HL(z, t); }
    cpu_rst10(z);
    z->t += 4; cpu_ex_af(z);
    z->t += 4; cpu_add(z, z->h);
    z->t += 7; z->h = 0x00;
    if (--z->b) { z->t += 13; goto x0AA5; } z->t += 8;
    z->t += 10; SET_DE(z, cpu_pop(z));
    z->t += 7; cpu_cp(z, 0x05);
    if (z->cf) { z->t += 12; goto x0AD7; } z->t += 7;
    z->t += 20; SET_DE(z, cpu_rd16(z, 0x99B0));
    z->t += 4; z->a = z->d;
    z->t += 7; cpu_add(z, 0x01);
    z->t += 4; cpu_daa(z);
    z->t += 4; z->d = z->a;
    if (!z->cf) { z->t += 12; goto x0AD3; } z->t += 7;
    z->t += 4; z->a = z->e;
    z->t += 7; cpu_add(z, 0x01);
    z->t += 4; cpu_daa(z);
    z->t += 4; z->e = z->a;
x0AD3:
    z->t += 20; game_wr16(z->g, 0x99B0, DE(z));
x0AD7:
    z->t += 7; z->b = 0x04;
    z->t += 7; z->c = 0x00;
    z->t += 10; SET_HL(z, 0x99B0);
    z->t += 10; SET_DE(z, 0x8138);
x0AE1:
    z->t += 4; z->b = cpu_dec(z, z->b);
    if (!z->zf) { z->t += 12; goto x0AE8; } z->t += 7;
    z->t += 7; z->a = 0x2A;
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 11; m_0020(z);
x0AE8:
    z->t += 4; z->b = cpu_inc(z, z->b);
    z->t += 4; cpu_xor(z, z->a);
    z->t += 18; cpu_rld(z);
    z->t += 8; cpu_bit(z, 0, z->b);
    if (z->zf) { z->t += 12; goto x0AF1; } z->t += 7;
    z->t += 4; z->l = cpu_inc(z, z->l);
x0AF1:
    z->t += 4; cpu_and(z, z->a);
    if (!z->zf) { z->t += 12; goto x0AF8; } z->t += 7;
    z->t += 8; cpu_bit(z, 0, z->c);
    if (z->zf) { z->t += 12; goto x0AFC; } z->t += 7;
x0AF8:
    z->t += 8; z->c = (uint8_t)(z->c | 0x01);
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 11; m_0020(z);
x0AFC:
    z->t += 4; z->a = z->b;
    z->t += 7; cpu_cp(z, 0x03);
    if (!z->zf) { z->t += 12; goto x0B03; } z->t += 7;
    z->t += 8; z->c = (uint8_t)(z->c | 0x01);
x0B03:
    if (--z->b) { z->t += 13; goto x0AE1; } z->t += 8;
    z->t += 10; return;
}

void m_0B06(Cpu *z)
{
    z->t += 7; z->a = 0x0A;
    z->t += 17; m_104E(z);
    z->t += 4; z->a = z->h;
    z->t += 7; z->h = 0x00;
    z->t += 10; return;
}

void m_1000(Cpu *z)
{
    z->t += 11; cpu_push(z, HL(z));
    z->t += 9; z->a = game_r(z->g); /* LD A,R */
    z->t += 4; z->h = z->a;
    z->t += 13; z->a = cpu_rd(z, 0x92A0);
    z->t += 4; cpu_add(z, z->h);
    z->t += 4; z->l = z->a;
    z->t += 7; z->h = 0x01;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; z->h = z->a;
    z->t += 9; z->a = game_r(z->g); /* LD A,R */
    z->t += 4; cpu_add(z, z->h);
    z->t += 10; SET_HL(z, cpu_pop(z));
    z->t += 10; return;
}

void m_104E(Cpu *z)
{
    z->t += 11; cpu_push(z, DE(z));
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 10; SET_HL(z, 0x0000);
x1053:
    z->t += 8; z->a = cpu_srl(z, z->a);
    if (!z->cf) { z->t += 12; goto x1058; } z->t += 7;
    z->t += 11; SET_HL(z, cpu_add16(z, HL(z), DE(z)));
x1058:
    z->t += 8; z->e = cpu_sla(z, z->e);
    z->t += 8; z->d = cpu_rl(z, z->d);
    z->t += 4; cpu_and(z, z->a);
    if (!z->zf) { z->t += 12; goto x1053; } z->t += 7;
    z->t += 10; SET_DE(z, cpu_pop(z));
    z->t += 10; return;
}

void m_1061(Cpu *z)
{
    z->t += 11; cpu_push(z, BC(z));
    z->t += 4; z->c = z->a;
    z->t += 4; cpu_xor(z, z->a);
    z->t += 7; z->b = 0x11;
x1066:
    z->t += 4; cpu_adc(z, z->a);
    if (z->cf) { z->t += 12; goto x1074; } z->t += 7;
    z->t += 4; cpu_cp(z, z->c);
    if (z->cf) { z->t += 12; goto x106D; } z->t += 7;
    z->t += 4; cpu_sub(z, z->c);
x106D:
    z->t += 4; cpu_ccf(z);
x106E:
    z->t += 15; SET_HL(z, cpu_adc16(z, HL(z), HL(z)));
    if (--z->b) { z->t += 13; goto x1066; } z->t += 8;
    z->t += 10; SET_BC(z, cpu_pop(z));
    z->t += 10; return;
x1074:
    z->t += 4; cpu_sub(z, z->c);
    z->t += 4; cpu_scf(z);
    z->t += 10; goto x106E;
}

void m_1079(Cpu *z)
{
    z->t += 4; z->a = z->l;
    z->t += 7; cpu_and(z, 0x80);
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 4; cpu_ex_af(z);
    z->t += 8; z->l = (uint8_t)(z->l & 0x7F);
    z->t += 10; goto x108A;
x108A:
    z->t += 11; cpu_push(z, DE(z));
    z->t += 10; SET_DE(z, 0x0014);
    z->t += 7; z->b = 0x0C;
    z->t += 14; z->ix = (uint16_t)(0x9100);
x1094:
    z->t += 20; cpu_bit(z, 0, cpu_ix(z, 19));
    if (z->zf) { z->t += 12; goto x10A0; } z->t += 7;
    z->t += 15; z->ix = (uint16_t)(cpu_add16(z, z->ix, DE(z)));
    if (--z->b) { z->t += 13; goto x1094; } z->t += 8;
    z->t += 10; SET_DE(z, cpu_pop(z));
    z->t += 10; return;
x10A0:
    z->t += 10; SET_DE(z, cpu_pop(z));
    z->t += 19; cpu_set_ix(z, 8, z->e);
    z->t += 19; cpu_set_ix(z, 9, z->d);
    z->t += 19; cpu_set_ix(z, 13, 0x01);
    z->t += 19; cpu_set_ix(z, 4, 0x00);
    z->t += 19; cpu_set_ix(z, 5, 0x01);
    z->t += 4; z->c = z->l;
    z->t += 19; cpu_set_ix(z, 16, z->c);
    z->t += 4; cpu_ex_af(z);
    z->t += 4; z->d = z->a;
    z->t += 10; cpu_wr(z, HL(z), 0x09);
    z->t += 8; z->a = (uint8_t)z->ix;
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 13; z->a = cpu_rd(z, 0x9215);
    z->t += 4; z->e = z->a;
    z->t += 4; z->l = z->c;
    z->t += 7; z->h = 0x93;
    z->t += 7; z->c = cpu_rd(z, HL(z));
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 7; z->b = cpu_rd(z, HL(z));
    z->t += 7; z->h = 0x9B;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; cpu_rrca(z);
    z->t += 8; z->b = cpu_rr(z, z->b);
    z->t += 8; cpu_bit(z, 0, z->e);
    if (!z->zf) { z->t += 12; goto x10DC; } z->t += 7;
    z->t += 4; cpu_ex_af(z);
    z->t += 4; z->a = z->b;
    z->t += 7; cpu_add(z, 0x50);
    z->t += 8; cpu_neg(z);
    z->t += 4; z->b = z->a;
    z->t += 4; cpu_ex_af(z);
    z->t += 4; cpu_ccf(z);
x10DC:
    z->t += 19; cpu_set_ix(z, 1, z->b);
    z->t += 4; cpu_rra(z);
    z->t += 7; cpu_and(z, 0x80);
    z->t += 19; cpu_set_ix(z, 0, z->a);
    z->t += 4; z->a = z->c;
    z->t += 8; cpu_bit(z, 0, z->e);
    if (z->zf) { z->t += 12; goto x10ED; } z->t += 7;
    z->t += 7; cpu_add(z, 0x0D);
    z->t += 4; cpu_cpl(z);
x10ED:
    z->t += 8; z->a = cpu_srl(z, z->a);
    z->t += 19; cpu_set_ix(z, 3, z->a);
    z->t += 4; cpu_rra(z);
    z->t += 7; cpu_and(z, 0x80);
    z->t += 19; cpu_set_ix(z, 2, z->a);
    z->t += 19; cpu_set_ix(z, 19, z->d);
    z->t += 19; cpu_set_ix(z, 14, 0x1E);
    z->t += 13; z->a = cpu_rd(z, 0x920B);
    z->t += 4; cpu_and(z, z->a);
    if (z->zf) { z->t += 12; goto x1108; } z->t += 7;
    z->t += 13; z->a = cpu_rd(z, 0x92C8);
x1108:
    z->t += 19; cpu_set_ix(z, 15, z->a);
    z->t += 10; return;
}

void m_1083(Cpu *z)
{
    z->t += 4; z->a = z->l;
    z->t += 4; cpu_rrca(z);
    z->t += 4; cpu_rrca(z);
    z->t += 7; cpu_and(z, 0x80);
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 4; cpu_ex_af(z);
    z->t += 11; cpu_push(z, DE(z));
    z->t += 10; SET_DE(z, 0x0014);
    z->t += 7; z->b = 0x0C;
    z->t += 14; z->ix = (uint16_t)(0x9100);
x1094:
    z->t += 20; cpu_bit(z, 0, cpu_ix(z, 19));
    if (z->zf) { z->t += 12; goto x10A0; } z->t += 7;
    z->t += 15; z->ix = (uint16_t)(cpu_add16(z, z->ix, DE(z)));
    if (--z->b) { z->t += 13; goto x1094; } z->t += 8;
    z->t += 10; SET_DE(z, cpu_pop(z));
    z->t += 10; return;
x10A0:
    z->t += 10; SET_DE(z, cpu_pop(z));
    z->t += 19; cpu_set_ix(z, 8, z->e);
    z->t += 19; cpu_set_ix(z, 9, z->d);
    z->t += 19; cpu_set_ix(z, 13, 0x01);
    z->t += 19; cpu_set_ix(z, 4, 0x00);
    z->t += 19; cpu_set_ix(z, 5, 0x01);
    z->t += 4; z->c = z->l;
    z->t += 19; cpu_set_ix(z, 16, z->c);
    z->t += 4; cpu_ex_af(z);
    z->t += 4; z->d = z->a;
    z->t += 10; cpu_wr(z, HL(z), 0x09);
    z->t += 8; z->a = (uint8_t)z->ix;
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 13; z->a = cpu_rd(z, 0x9215);
    z->t += 4; z->e = z->a;
    z->t += 4; z->l = z->c;
    z->t += 7; z->h = 0x93;
    z->t += 7; z->c = cpu_rd(z, HL(z));
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 7; z->b = cpu_rd(z, HL(z));
    z->t += 7; z->h = 0x9B;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; cpu_rrca(z);
    z->t += 8; z->b = cpu_rr(z, z->b);
    z->t += 8; cpu_bit(z, 0, z->e);
    if (!z->zf) { z->t += 12; goto x10DC; } z->t += 7;
    z->t += 4; cpu_ex_af(z);
    z->t += 4; z->a = z->b;
    z->t += 7; cpu_add(z, 0x50);
    z->t += 8; cpu_neg(z);
    z->t += 4; z->b = z->a;
    z->t += 4; cpu_ex_af(z);
    z->t += 4; cpu_ccf(z);
x10DC:
    z->t += 19; cpu_set_ix(z, 1, z->b);
    z->t += 4; cpu_rra(z);
    z->t += 7; cpu_and(z, 0x80);
    z->t += 19; cpu_set_ix(z, 0, z->a);
    z->t += 4; z->a = z->c;
    z->t += 8; cpu_bit(z, 0, z->e);
    if (z->zf) { z->t += 12; goto x10ED; } z->t += 7;
    z->t += 7; cpu_add(z, 0x0D);
    z->t += 4; cpu_cpl(z);
x10ED:
    z->t += 8; z->a = cpu_srl(z, z->a);
    z->t += 19; cpu_set_ix(z, 3, z->a);
    z->t += 4; cpu_rra(z);
    z->t += 7; cpu_and(z, 0x80);
    z->t += 19; cpu_set_ix(z, 2, z->a);
    z->t += 19; cpu_set_ix(z, 19, z->d);
    z->t += 19; cpu_set_ix(z, 14, 0x1E);
    z->t += 13; z->a = cpu_rd(z, 0x920B);
    z->t += 4; cpu_and(z, z->a);
    if (z->zf) { z->t += 12; goto x1108; } z->t += 7;
    z->t += 13; z->a = cpu_rd(z, 0x92C8);
x1108:
    z->t += 19; cpu_set_ix(z, 15, z->a);
    z->t += 10; return;
}

void m_110C(Cpu *z)
{
    z->t += 7; z->a = 0x1F;
    z->t += 13; cpu_wr(z, 0x9000, z->a);
    z->t += 13; cpu_wr(z, 0x98E0, z->a);
    z->t += 10; SET_HL(z, 0x9820);
    z->t += 10; SET_DE(z, 0x9860);
    z->t += 7; z->b = 0x40;
x111C:
    z->t += 7; z->c = cpu_rd(z, HL(z));
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 4; z->a = z->c;
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 4; z->e = cpu_inc(z, z->e);
    if (--z->b) { z->t += 13; goto x111C; } z->t += 8;
    z->t += 10; SET_HL(z, 0x8800);
    z->t += 10; SET_DE(z, 0x98B0);
    z->t += 7; z->b = 0x30;
x112D:
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; z->c = z->a;
    z->t += 7; z->h = 0x8B;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_and(z, 0x7F);
    z->t += 4; z->c = cpu_dec(z, z->c);
    if (!z->zf) { z->t += 12; goto x1142; } z->t += 7;
    z->t += 7; cpu_and(z, 0x78);
    z->t += 4; z->c = z->a;
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; z->l = cpu_dec(z, z->l);
    z->t += 7; cpu_and(z, 0x07);
    z->t += 4; cpu_or(z, z->c);
    z->t += 7; cpu_or(z, 0x80);
x1142:
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 7; z->c = cpu_rd(z, HL(z));
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 8; cpu_bit(z, 7, z->c);
    if (z->zf) { z->t += 12; goto x115A; } z->t += 7;
    z->t += 4; z->a = z->c;
    z->t += 7; cpu_and(z, 0x78);
    z->t += 7; cpu_add(z, 0x06);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 4; z->a = z->c;
    z->t += 7; cpu_and(z, 0x07);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 4; z->l = cpu_dec(z, z->l);
    z->t += 7; z->a = 0x01;
    z->t += 12; goto x1161;
x115A:
    z->t += 7; cpu_wr(z, HL(z), z->c);
    z->t += 7; z->h = 0x93;
    z->t += 10; cpu_wr(z, HL(z), 0x00);
    z->t += 7; z->a = 0x80;
x1161:
    z->t += 7; z->h = 0x88;
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 6; SET_DE(z, DE(z) + 1);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 4; z->l = cpu_inc(z, z->l);
    if (--z->b) { z->t += 13; goto x112D; } z->t += 8;
    z->t += 10; SET_HL(z, 0x9000);
    z->t += 10; SET_DE(z, 0x98E0);
    z->t += 7; z->b = 0x20;
x1171:
    z->t += 7; z->c = cpu_rd(z, HL(z));
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 4; z->a = z->c;
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 4; z->e = cpu_inc(z, z->e);
    if (--z->b) { z->t += 13; goto x1171; } z->t += 8;
    z->t += 4; cpu_xor(z, z->a);
    z->t += 13; cpu_wr(z, 0x9000, z->a);
    z->t += 10; return;
}

void m_117F(Cpu *z)
{
    z->t += 10; SET_HL(z, 0x8002);
    z->t += 7; z->b = 0x12;
x1184:
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_cp(z, 0x4A);
    if (!z->cf) { z->t += 12; goto x118B; } z->t += 7;
    z->t += 10; cpu_wr(z, HL(z), 0x24);
x118B:
    z->t += 4; z->l = cpu_inc(z, z->l);
    if (--z->b) { z->t += 13; goto x1184; } z->t += 8;
    z->t += 7; z->l = 0x22;
    z->t += 7; z->b = 0x12;
x1192:
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_cp(z, 0x4A);
    if (!z->cf) { z->t += 12; goto x1199; } z->t += 7;
    z->t += 10; cpu_wr(z, HL(z), 0x24);
x1199:
    z->t += 4; z->l = cpu_inc(z, z->l);
    if (--z->b) { z->t += 13; goto x1192; } z->t += 8;
    z->t += 13; z->a = cpu_rd(z, 0x9821);
    z->t += 7; z->b = 0x00;
    z->t += 10; SET_HL(z, 0x8001);
x11A4:
    z->t += 7; cpu_cp(z, 0x32);
    if (z->cf) { z->t += 12; goto x11AF; } z->t += 7;
    z->t += 7; cpu_sub(z, 0x32);
    z->t += 4; z->b = cpu_inc(z, z->b);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 12; goto x11A4;
x11AF:
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 4; z->l = z->a;
    z->t += 7; z->h = 0x00;
    z->t += 7; z->a = 0x0A;
    z->t += 17; m_1061(z);
    z->t += 4; z->h = z->a;
    z->t += 11; cpu_push(z, HL(z));
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 7; cpu_cp(z, 0x05);
    if (z->cf) { z->t += 12; goto x11C1; } z->t += 7;
    z->t += 7; cpu_sub(z, 0x04);
x11C1:
    z->t += 4; z->c = z->a;
    z->t += 4; z->a = z->e;
    z->t += 8; cpu_bit(z, 0, z->a);
    if (z->zf) { z->t += 12; goto x11C9; } z->t += 7;
    z->t += 7; z->a = 0x02;
x11C9:
    z->t += 4; cpu_add(z, z->c);
    cpu_rst10(z);
    z->t += 4; z->b = cpu_inc(z, z->b);
x11CC:
    if (--z->b) { z->t += 13; goto x11EE; } z->t += 8;
    z->t += 10; SET_BC(z, cpu_pop(z));
    z->t += 4; z->a = z->c;
    z->t += 17; m_11F5(z);
    z->t += 4; z->a = z->b;
    z->t += 7; cpu_cp(z, 0x05);
    if (z->cf) { z->t += 12; goto x11E0; } z->t += 7;
    z->t += 7; z->d = 0x38;
    z->t += 17; m_1213(z);
    z->t += 4; z->a = z->b;
    z->t += 7; cpu_sub(z, 0x05);
x11E0:
    z->t += 4; z->b = z->a;
    z->t += 4; z->b = cpu_inc(z, z->b);
x11E2:
    if (--z->b) { z->t += 13; goto x11E7; } z->t += 8;
    z->t += 10; goto x137E;
x11E7:
    z->t += 7; z->d = 0x36;
    z->t += 17; m_1213(z);
    z->t += 12; goto x11E2;
x11EE:
    z->t += 7; z->a = 0x04;
    z->t += 17; m_11FB(z);
    z->t += 12; goto x11CC;
x137E:
    z->t += 13; z->a = cpu_rd(z, 0x9820);
    z->t += 4; cpu_cpl(z);
    z->t += 7; cpu_add(z, 0x09);
    z->t += 4; z->e = z->a;
    z->t += 7; z->d = 0x49;
    z->t += 10; SET_HL(z, 0x801D);
    z->t += 17; m_1398(z);
    z->t += 4; z->l = cpu_dec(z, z->l);
    z->t += 17; m_1398(z);
    z->t += 8; z->l = (uint8_t)(z->l | 0x20);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 17; m_1398(z);
    z->t += 4; z->l = cpu_dec(z, z->l);
    z->t += 11; cpu_push(z, HL(z));
    z->t += 4; z->d = cpu_inc(z, z->d);
    z->t += 4; z->c = z->d;
    z->t += 7; z->b = 0x08;
x139D:
    z->t += 4; z->a = z->b;
    z->t += 4; cpu_cp(z, z->e);
    if (!z->zf) { z->t += 12; goto x13A3; } z->t += 7;
    z->t += 7; z->c = 0x24;
x13A3:
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_cp(z, 0x36);
    if (z->cf) { z->t += 12; goto x13AC; } z->t += 7;
    z->t += 7; cpu_cp(z, 0x4A);
    if (z->cf) { z->t += 12; goto x13AD; } z->t += 7;
x13AC:
    z->t += 7; cpu_wr(z, HL(z), z->c);
x13AD:
    z->t += 4; z->l = cpu_dec(z, z->l);
    z->t += 4; z->l = cpu_dec(z, z->l);
    if (--z->b) { z->t += 13; goto x139D; } z->t += 8;
    z->t += 10; SET_HL(z, cpu_pop(z));
    z->t += 10; return;
}

void m_11F5(Cpu *z)
{
    z->t += 4; cpu_and(z, z->a);
    if (z->zf) { z->t += 11; return; } z->t += 5;
    z->t += 7; cpu_cp(z, 0x04);
    if (z->zf) { z->t += 12; goto x1202; } z->t += 7;
    z->t += 4; cpu_rlca(z);
    z->t += 4; cpu_rlca(z);
    z->t += 7; cpu_add(z, 0x36);
    z->t += 4; z->d = z->a;
    z->t += 12; goto x120C;
x1202:
    z->t += 7; z->d = 0x42;
    z->t += 17; m_1213(z);
    z->t += 17; m_1228(z);
    z->t += 7; z->d = 0x3A;
x120C:
    z->t += 17; m_1213(z);
    z->t += 17; m_1228(z);
    z->t += 10; return;
}

void m_11FB(Cpu *z)
{
    z->t += 4; cpu_rlca(z);
    z->t += 4; cpu_rlca(z);
    z->t += 7; cpu_add(z, 0x36);
    z->t += 4; z->d = z->a;
    z->t += 12; goto x120C;
x120C:
    z->t += 17; m_1213(z);
    z->t += 17; m_1228(z);
    z->t += 10; return;
}

void m_1213(Cpu *z)
{
    z->t += 4; cpu_ex_af(z);
    if (z->cf) { z->t += 12; goto x1227; } z->t += 7;
    z->t += 4; cpu_ex_af(z);
    z->t += 13; z->a = cpu_rd(z, 0x92A0);
    z->t += 7; cpu_add(z, 0x08);
    z->t += 4; z->e = z->a;
x121D:
    if (game_wait_for(z->g, 0x92A0, z->e)) return; /* the frame count */
    z->t += 13; z->a = cpu_rd(z, 0x92A0);
    z->t += 4; cpu_sub(z, z->e);
    if (!z->zf) { z->t += 12; goto x121D; } z->t += 7;
    z->t += 4; cpu_ex_af(z);
    z->t += 13; cpu_wr(z, 0x9AB5, z->a);
x1227:
    z->t += 4; cpu_ex_af(z);
    z->t += 7; cpu_wr(z, HL(z), z->d);
    z->t += 4; z->d = cpu_inc(z, z->d);
    z->t += 8; z->l = (uint8_t)(z->l | 0x20);
    z->t += 7; cpu_wr(z, HL(z), z->d);
    z->t += 4; z->d = cpu_inc(z, z->d);
    z->t += 8; z->h = (uint8_t)(z->h | 0x04);
    z->t += 4; z->a = z->d;
    z->t += 7; cpu_and(z, 0x0C);
    z->t += 7; cpu_cp(z, 0x08);
    z->t += 7; z->a = 0x01;
    if (z->zf) { z->t += 12; goto x123A; } z->t += 7;
    z->t += 4; z->a = cpu_inc(z, z->a);
x123A:
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 8; z->l = (uint8_t)(z->l & 0xDF);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 8; z->h = (uint8_t)(z->h & 0xFB);
    z->t += 4; z->l = cpu_dec(z, z->l);
    z->t += 10; return;
}

void m_1228(Cpu *z)
{
    z->t += 7; cpu_wr(z, HL(z), z->d);
    z->t += 4; z->d = cpu_inc(z, z->d);
    z->t += 8; z->l = (uint8_t)(z->l | 0x20);
    z->t += 7; cpu_wr(z, HL(z), z->d);
    z->t += 4; z->d = cpu_inc(z, z->d);
    z->t += 8; z->h = (uint8_t)(z->h | 0x04);
    z->t += 4; z->a = z->d;
    z->t += 7; cpu_and(z, 0x0C);
    z->t += 7; cpu_cp(z, 0x08);
    z->t += 7; z->a = 0x01;
    if (z->zf) { z->t += 12; goto x123A; } z->t += 7;
    z->t += 4; z->a = cpu_inc(z, z->a);
x123A:
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 8; z->l = (uint8_t)(z->l & 0xDF);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 8; z->h = (uint8_t)(z->h & 0xFB);
    z->t += 4; z->l = cpu_dec(z, z->l);
    z->t += 10; return;
}

void m_1242(Cpu *z)
{
    z->t += 10; SET_HL(z, 0x125B);
    z->t += 10; SET_DE(z, 0x9000);
    z->t += 10; SET_BC(z, 0x0020);
    z->t += 11; cpu_push(z, BC(z));
    z->t += 11; cpu_push(z, HL(z));
    cpu_ldir(z);
    z->t += 10; SET_HL(z, cpu_pop(z));
    z->t += 10; SET_BC(z, cpu_pop(z));
    z->t += 10; SET_DE(z, 0x98E0);
    cpu_ldir(z);
    z->t += 4; cpu_xor(z, z->a);
    z->t += 13; cpu_wr(z, 0x9000, z->a);
    z->t += 10; return;
}

void m_127B(Cpu *z)
{
    z->t += 10; SET_HL(z, 0x8B64);
    z->t += 10; SET_DE(z, 0x0930);
    z->t += 7; z->c = 0x00;
    z->t += 7; z->b = 0x0A;
x1285:
    z->t += 7; cpu_wr(z, HL(z), z->e);
    z->t += 7; z->h = 0x93;
    z->t += 10; cpu_wr(z, HL(z), 0x00);
    z->t += 7; z->h = 0x9B;
    z->t += 7; cpu_wr(z, HL(z), z->c);
    z->t += 7; z->h = 0x8B;
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 7; cpu_wr(z, HL(z), z->d);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 4; z->a = z->b;
    z->t += 7; cpu_cp(z, 0x09);
    if (!z->zf) { z->t += 12; goto x129B; } z->t += 7;
    z->t += 7; z->c = 0x01;
    z->t += 7; z->d = 0x0B;
x129B:
    if (--z->b) { z->t += 13; goto x1285; } z->t += 8;
    z->t += 10; return;
}

void m_129E(Cpu *z)
{
    z->t += 7; z->h = 0x8B;
    z->t += 20; SET_DE(z, cpu_rd16(z, 0x9280));
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 4; z->l = z->a;
    z->t += 6; SET_DE(z, DE(z) + 1);
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 4; z->c = z->a;
    z->t += 7; cpu_and(z, 0x78);
    z->t += 7; cpu_add(z, 0x06);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 4; z->a = z->c;
    z->t += 7; cpu_and(z, 0x07);
    z->t += 8; cpu_bit(z, 7, z->c);
    if (z->zf) { z->t += 12; goto x12B8; } z->t += 7;
    z->t += 7; cpu_or(z, 0x08);
x12B8:
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 6; SET_DE(z, DE(z) + 1);
    z->t += 4; z->l = cpu_dec(z, z->l);
    z->t += 7; z->h = 0x88;
    z->t += 10; cpu_wr(z, HL(z), 0x01);
    z->t += 7; z->h = 0x93;
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 6; SET_DE(z, DE(z) + 1);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 8; z->a = cpu_sla(z, z->a);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 7; z->a = 0x00;
    z->t += 4; cpu_rla(z);
    z->t += 7; z->h = 0x9B;
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 6; SET_DE(z, DE(z) + 1);
    z->t += 20; game_wr16(z->g, 0x9280, DE(z));
    z->t += 10; return;
}

void m_12D5(Cpu *z)
{
    z->t += 8; z->ix = (uint16_t)((z->ix & 0xFF00) | (uint8_t)(z->a));
    z->t += 13; z->a = cpu_rd(z, 0x9215);
    z->t += 4; z->c = z->a;
    z->t += 10; SET_HL(z, 0x9900);
    z->t += 10; SET_DE(z, 0x1321);
    z->t += 7; z->b = 0x10;
x12E3:
    z->t += 10; cpu_wr(z, HL(z), 0x00);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 6; SET_DE(z, DE(z) + 1);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 4; z->l = cpu_inc(z, z->l);
    if (--z->b) { z->t += 13; goto x12E3; } z->t += 8;
    z->t += 10; SET_HL(z, 0x9800);
    z->t += 10; SET_DE(z, 0x1321);
    z->t += 7; z->b = 0x0A;
x12F4:
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 6; SET_DE(z, DE(z) + 1);
    z->t += 8; cpu_bit(z, 0, z->c);
    if (z->zf) { z->t += 12; goto x12FD; } z->t += 7;
    z->t += 7; cpu_add(z, 0x0D);
    z->t += 4; cpu_cpl(z);
x12FD:
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 4; z->l = cpu_inc(z, z->l);
    if (--z->b) { z->t += 13; goto x12F4; } z->t += 8;
    z->t += 7; z->b = 0x06;
x1304:
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 8; cpu_add(z, (uint8_t)z->ix);
    z->t += 6; SET_DE(z, DE(z) + 1);
    z->t += 8; cpu_bit(z, 0, z->c);
    if (!z->zf) { z->t += 12; goto x130F; } z->t += 7;
    z->t += 7; cpu_add(z, 0x4F);
    z->t += 4; cpu_cpl(z);
x130F:
    z->t += 8; z->a = cpu_sla(z, z->a);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 7; z->a = 0x00;
    z->t += 4; cpu_rla(z);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 4; z->l = cpu_inc(z, z->l);
    if (--z->b) { z->t += 13; goto x1304; } z->t += 8;
    z->t += 13; z->a = cpu_rd(z, 0x9215);
    z->t += 13; cpu_wr(z, 0x920F, z->a);
    z->t += 10; return;
}

void m_1331(Cpu *z)
{
    z->t += 11; cpu_push(z, HL(z));
    z->t += 10; SET_HL(z, 0x92AF);
    z->t += 10; cpu_wr(z, HL(z), 0x03);
x1337:
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; cpu_and(z, z->a);
    if (!z->zf) { z->t += 12; goto x1337; } z->t += 7;
    z->t += 10; SET_HL(z, cpu_pop(z));
    z->t += 10; return;
}

void m_133D(Cpu *z)
{
    z->t += 7; z->a = 0x01;
    z->t += 13; cpu_wr(z, 0x9014, z->a);
    z->t += 13; z->a = cpu_rd(z, 0x8270);
    z->t += 7; cpu_cp(z, 0x24);
    if (!z->zf) { z->t += 12; goto x134C; } z->t += 7;
    z->t += 7; z->c = 0x03;
    z->t += 11; m_0030(z);
x134C:
    z->t += 13; z->a = cpu_rd(z, 0x9287);
    z->t += 4; cpu_and(z, z->a);
    if (!z->zf) { z->t += 12; goto x134C; } z->t += 7;
    z->t += 17; m_137E(z);
    z->t += 10; SET_HL(z, 0x0906);
    z->t += 16; game_wr16(z->g, 0x8B62, HL(z));
    z->t += 10; SET_HL(z, 0x9362);
    z->t += 13; z->a = cpu_rd(z, 0x9215);
    z->t += 7; cpu_and(z, 0x01);
    z->t += 7; z->a = 0x29;
    z->t += 7; z->c = 0x01;
    if (z->zf) { z->t += 12; goto x136C; } z->t += 7;
    z->t += 7; cpu_add(z, 0x0E);
    z->t += 4; z->c = cpu_dec(z, z->c);
x136C:
    z->t += 10; cpu_wr(z, HL(z), 0x7A);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 7; z->h = 0x9B;
    z->t += 7; cpu_wr(z, HL(z), z->c);
    z->t += 4; z->l = cpu_dec(z, z->l);
    z->t += 4; cpu_xor(z, z->a);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 13; cpu_wr(z, 0x9213, z->a);
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 13; cpu_wr(z, 0x99B9, z->a);
    z->t += 10; return;
}

void m_134C(Cpu *z)
{
x134C:
    if (game_wait_for(z->g, 0x9287, 0)) return; /* sub's count of the enemies in flight */
    z->t += 13; z->a = cpu_rd(z, 0x9287);
    z->t += 4; cpu_and(z, z->a);
    if (!z->zf) { z->t += 12; goto x134C; } z->t += 7;
    z->t += 17; m_137E(z);
    z->t += 10; SET_HL(z, 0x0906);
    z->t += 16; game_wr16(z->g, 0x8B62, HL(z));
    z->t += 10; SET_HL(z, 0x9362);
    z->t += 13; z->a = cpu_rd(z, 0x9215);
    z->t += 7; cpu_and(z, 0x01);
    z->t += 7; z->a = 0x29;
    z->t += 7; z->c = 0x01;
    if (z->zf) { z->t += 12; goto x136C; } z->t += 7;
    z->t += 7; cpu_add(z, 0x0E);
    z->t += 4; z->c = cpu_dec(z, z->c);
x136C:
    z->t += 10; cpu_wr(z, HL(z), 0x7A);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 7; z->h = 0x9B;
    z->t += 7; cpu_wr(z, HL(z), z->c);
    z->t += 4; z->l = cpu_dec(z, z->l);
    z->t += 4; cpu_xor(z, z->a);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 13; cpu_wr(z, 0x9213, z->a);
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 13; cpu_wr(z, 0x99B9, z->a);
    z->t += 10; return;
}

void m_137E(Cpu *z)
{
    z->t += 13; z->a = cpu_rd(z, 0x9820);
    z->t += 4; cpu_cpl(z);
    z->t += 7; cpu_add(z, 0x09);
    z->t += 4; z->e = z->a;
    z->t += 7; z->d = 0x49;
    z->t += 10; SET_HL(z, 0x801D);
    z->t += 17; m_1398(z);
    z->t += 4; z->l = cpu_dec(z, z->l);
    z->t += 17; m_1398(z);
    z->t += 8; z->l = (uint8_t)(z->l | 0x20);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 17; m_1398(z);
    z->t += 4; z->l = cpu_dec(z, z->l);
    z->t += 11; cpu_push(z, HL(z));
    z->t += 4; z->d = cpu_inc(z, z->d);
    z->t += 4; z->c = z->d;
    z->t += 7; z->b = 0x08;
x139D:
    z->t += 4; z->a = z->b;
    z->t += 4; cpu_cp(z, z->e);
    if (!z->zf) { z->t += 12; goto x13A3; } z->t += 7;
    z->t += 7; z->c = 0x24;
x13A3:
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_cp(z, 0x36);
    if (z->cf) { z->t += 12; goto x13AC; } z->t += 7;
    z->t += 7; cpu_cp(z, 0x4A);
    if (z->cf) { z->t += 12; goto x13AD; } z->t += 7;
x13AC:
    z->t += 7; cpu_wr(z, HL(z), z->c);
x13AD:
    z->t += 4; z->l = cpu_dec(z, z->l);
    z->t += 4; z->l = cpu_dec(z, z->l);
    if (--z->b) { z->t += 13; goto x139D; } z->t += 8;
    z->t += 10; SET_HL(z, cpu_pop(z));
    z->t += 10; return;
}

void m_1398(Cpu *z)
{
    z->t += 11; cpu_push(z, HL(z));
    z->t += 4; z->d = cpu_inc(z, z->d);
    z->t += 4; z->c = z->d;
    z->t += 7; z->b = 0x08;
x139D:
    z->t += 4; z->a = z->b;
    z->t += 4; cpu_cp(z, z->e);
    if (!z->zf) { z->t += 12; goto x13A3; } z->t += 7;
    z->t += 7; z->c = 0x24;
x13A3:
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_cp(z, 0x36);
    if (z->cf) { z->t += 12; goto x13AC; } z->t += 7;
    z->t += 7; cpu_cp(z, 0x4A);
    if (z->cf) { z->t += 12; goto x13AD; } z->t += 7;
x13AC:
    z->t += 7; cpu_wr(z, HL(z), z->c);
x13AD:
    z->t += 4; z->l = cpu_dec(z, z->l);
    z->t += 4; z->l = cpu_dec(z, z->l);
    if (--z->b) { z->t += 13; goto x139D; } z->t += 8;
    z->t += 10; SET_HL(z, cpu_pop(z));
    z->t += 10; return;
}

void m_13B3(Cpu *z)
{
    z->t += 4; cpu_and(z, z->a);
    z->t += 4; cpu_ex_af(z);
    z->t += 11; cpu_push(z, DE(z));
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 4; z->a = z->c;
    z->t += 10; SET_HL(z, 0x13EF);
    cpu_rst08(z);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; z->h = cpu_rd(z, HL(z));
    z->t += 4; z->l = z->a;
    z->t += 4; cpu_ex_af(z);
    if (!z->cf) { z->t += 12; goto x13C9; } z->t += 7;
    z->t += 6; SET_HL(z, HL(z) - 1);
    z->t += 6; SET_HL(z, HL(z) - 1);
    z->t += 7; z->e = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; z->d = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
x13C9:
    z->t += 7; z->c = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 4; cpu_ex_de_hl(z);
x13CC:
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 7; cpu_cp(z, 0x2F);
    if (z->zf) { z->t += 12; goto x13EF; } z->t += 7;
    z->t += 7; cpu_sub(z, 0x30);
    if (!z->cf) { z->t += 12; goto x13D9; } z->t += 7;
    z->t += 7; z->a = 0x24;
    z->t += 12; goto x13DF;
x13D9:
    z->t += 7; cpu_cp(z, 0x11);
    if (z->cf) { z->t += 12; goto x13DF; } z->t += 7;
    z->t += 7; cpu_sub(z, 0x07);
x13DF:
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 8; z->h = (uint8_t)(z->h | 0x04);
    z->t += 7; cpu_wr(z, HL(z), z->c);
    z->t += 8; z->h = (uint8_t)(z->h & 0xFB);
    z->t += 6; SET_DE(z, DE(z) + 1);
    z->t += 4; z->a = z->l;
    z->t += 7; cpu_sub(z, 0x20);
    z->t += 4; z->l = z->a;
    if (!z->cf) { z->t += 12; goto x13CC; } z->t += 7;
    z->t += 4; z->h = cpu_dec(z, z->h);
    z->t += 12; goto x13CC;
x13EF:
    z->t += 10; SET_DE(z, cpu_pop(z));
    z->t += 10; return;
}

int m_1C93(Cpu *z, uint16_t ret)
{
    z->t += 4; z->a = z->b;
    z->t += 8; cpu_bit(z, 1, z->a);
    if (z->zf) { z->t += 12; goto x1C9A; } z->t += 7;
    z->t += 7; cpu_xor(z, 0x01);
x1C9A:
    z->t += 7; cpu_and(z, 0x03);
    z->t += 8; z->a = cpu_sla(z, z->a);
    z->t += 7; cpu_add(z, 0x30);
    z->t += 4; z->e = z->a;
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 7; cpu_cp(z, 0x01);
    if (!z->zf) { z->t += 11; return 0; } z->t += 5;
    z->t += 10; SET_HL(z, ret); /* POP HL: its return address, so its RET leaves the caller */
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
    if (!z->zf) { z->t += 11; return 1; } z->t += 5;
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
    z->t += 10; return 1;
}

void m_1D09(Cpu *z)
{
    z->t += 8; z->c = cpu_rrc(z, z->c);
    if (z->cf) { z->t += 12; goto x1D13; } z->t += 7;
    z->t += 4; z->b = cpu_dec(z, z->b);
    z->t += 8; z->c = cpu_rrc(z, z->c);
    if (z->cf) { z->t += 12; goto x1D13; } z->t += 7;
    z->t += 4; z->b = cpu_dec(z, z->b);
x1D13:
    z->t += 4; z->a = z->b;
    z->t += 4; z->b = cpu_dec(z, z->b);
    z->t += 10; SET_HL(z, 0x1D32);
    cpu_rst10(z);
    z->t += 4; cpu_ex_af(z);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; cpu_ex_de_hl(z);
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
}

void m_1E49(Cpu *z)
{
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

void m_1F19(Cpu *z)
{
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

void m_1F9C(Cpu *z)
{
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

void m_2188(Cpu *z)
{
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; z->c = z->a;
    z->t += 8; z->a = cpu_srl(z, z->a);
    z->t += 4; cpu_xor(z, z->c);
    z->t += 4; z->c = z->a;
    z->t += 7; z->h = 0x8B;
    z->t += 7; z->b = 0x00;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_and(z, 0x07);
    z->t += 7; cpu_cp(z, 0x06);
    if (!z->zf) { z->t += 12; goto x21A7; } z->t += 7;
    z->t += 4; z->c = cpu_dec(z, z->c);
    z->t += 4; z->c = cpu_inc(z, z->c);
    if (!z->zf) { z->t += 12; goto x21A7; } z->t += 7;
    z->t += 4; cpu_ex_af(z);
    z->t += 13; z->a = cpu_rd(z, 0x928D);
    z->t += 4; cpu_and(z, z->a);
    if (!z->zf) { z->t += 12; goto x21A6; } z->t += 7;
    z->t += 4; z->b = cpu_inc(z, z->b);
    z->t += 10; return;
x21A6:
    z->t += 4; cpu_ex_af(z);
x21A7:
    z->t += 8; cpu_bit(z, 0, z->c);
    if (!z->zf) { z->t += 12; goto x21B2; } z->t += 7;
    z->t += 7; cpu_cp(z, 0x06);
    if (z->zf) { z->t += 12; goto x21B8; } z->t += 7;
    z->t += 11; cpu_wr(z, HL(z), cpu_inc(z, cpu_rd(z, HL(z))));
    z->t += 12; goto x21C0;
x21B2:
    z->t += 4; cpu_and(z, z->a);
    if (z->zf) { z->t += 12; goto x21B8; } z->t += 7;
    z->t += 11; cpu_wr(z, HL(z), cpu_dec(z, cpu_rd(z, HL(z))));
    z->t += 12; goto x21C0;
x21B8:
    z->t += 4; z->c = cpu_dec(z, z->c);
    z->t += 10; if (!z->sf) { goto x21A7; }
    z->t += 7; z->c = 0x03;
    z->t += 12; goto x21A7;
x21C0:
    z->t += 4; z->a = z->c;
    z->t += 8; cpu_bit(z, 1, z->a);
    if (z->zf) { z->t += 12; goto x21C7; } z->t += 7;
    z->t += 7; cpu_xor(z, 0x01);
x21C7:
    z->t += 7; z->h = 0x9B;
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 10; return;
}

void m_238A(Cpu *z)
{
    z->t += 4; z->c = z->a;
    z->t += 13; z->a = cpu_rd(z, 0x928A);
    z->t += 8; cpu_neg(z);
    z->t += 7; cpu_add(z, 0x10);
    z->t += 7; z->d = 0x20;
    z->t += 4; cpu_rlca(z);
    z->t += 8; z->d = cpu_rl(z, z->d);
    z->t += 4; cpu_rlca(z);
    z->t += 8; z->d = cpu_rl(z, z->d);
    z->t += 7; cpu_and(z, 0xE0);
    z->t += 7; cpu_add(z, 0x14);
    z->t += 4; cpu_add(z, z->c);
    z->t += 4; z->e = z->a;
    z->t += 10; return;
}

void m_23E0(Cpu *z)
{
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

void m_25A2(Cpu *z)
{
    z->t += 10; SET_HL(z, 0x286E);
    z->t += 16; game_wr16(z->g, 0x92E0, HL(z));
    z->t += 13; z->a = cpu_rd(z, 0x9821);
    z->t += 4; z->c = z->a;
x25AC:
    z->t += 7; cpu_cp(z, 0x17);
    if (z->cf) { z->t += 12; goto x25B4; } z->t += 7;
    z->t += 7; cpu_sub(z, 0x04);
    z->t += 12; goto x25AC;
x25B4:
    z->t += 4; z->b = z->a;
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 7; cpu_and(z, 0x03);
    if (z->zf) { z->t += 12; goto x25D3; } z->t += 7;
    z->t += 13; z->a = cpu_rd(z, 0x9984);
    z->t += 7; z->l = 0x11;
    z->t += 17; m_104E(z);
    z->t += 4; z->a = z->l;
    z->t += 10; SET_HL(z, 0x26A8);
    cpu_rst10(z);
    z->t += 10; SET_DE(z, 0x26F4);
    z->t += 4; z->a = z->b;
    z->t += 8; z->b = cpu_srl(z, z->b);
    z->t += 8; z->b = cpu_srl(z, z->b);
    z->t += 4; cpu_sub(z, z->b);
    z->t += 4; z->a = cpu_dec(z, z->a);
    z->t += 12; goto x25E0;
x25D3:
    z->t += 10; SET_HL(z, 0x26EC);
    z->t += 4; z->a = z->c;
    z->t += 8; z->a = cpu_srl(z, z->a);
    z->t += 8; z->a = cpu_srl(z, z->a);
    z->t += 7; cpu_and(z, 0x07);
    z->t += 10; SET_DE(z, 0x27DE);
x25E0:
    cpu_rst10(z);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; cpu_ex_de_hl(z);
    cpu_rst10(z);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 13; cpu_wr(z, 0x92E2, z->a);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 13; cpu_wr(z, 0x92E3, z->a);
    z->t += 10; SET_DE(z, 0x8920);
    z->t += 7; z->a = 0x7E;
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 4; z->e = cpu_inc(z, z->e);
x25F5:
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; cpu_cp(z, 0xFF);
    z->t += 10; if (z->zf) { goto x2681; }
    z->t += 4; z->c = z->a;
    z->t += 11; cpu_push(z, DE(z));
    z->t += 11; cpu_push(z, HL(z));
    z->t += 10; SET_HL(z, 0x9100);
    z->t += 7; z->a = 0xFF;
    z->t += 7; z->b = 0x10;
    z->t += 11; m_0018(z);
    z->t += 4; z->a = z->c;
    z->t += 7; cpu_and(z, 0x0F);
    if (z->zf) { z->t += 12; goto x2636; } z->t += 7;
    z->t += 4; z->b = z->a;
    z->t += 8; z->a = cpu_srl(z, z->a);
    z->t += 7; cpu_add(z, 0x04);
    z->t += 4; z->e = z->a;
x2612:
    z->t += 17; m_1000(z);
    z->t += 4; z->l = z->a;
    z->t += 7; z->h = 0x00;
    z->t += 4; z->a = z->e;
    z->t += 17; m_1061(z);
    z->t += 8; cpu_bit(z, 0, z->b);
    if (z->zf) { z->t += 12; goto x2622; } z->t += 7;
    z->t += 8; z->a = (uint8_t)(z->a | 0x08);
x2622:
    z->t += 7; z->h = 0x91;
    z->t += 4; z->l = z->a;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; z->a = cpu_inc(z, z->a);
    if (!z->zf) { z->t += 12; goto x2612; } z->t += 7;
    z->t += 4; z->a = z->b;
    z->t += 4; cpu_rlca(z);
    z->t += 8; z->c = cpu_rlc(z, z->c);
    if (!z->cf) { z->t += 12; goto x2631; } z->t += 7;
    z->t += 7; cpu_or(z, 0x40);
x2631:
    z->t += 7; cpu_or(z, 0x38);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    if (--z->b) { z->t += 13; goto x2612; } z->t += 8;
x2636:
    z->t += 10; SET_HL(z, 0x9100);
    z->t += 20; SET_DE(z, cpu_rd16(z, 0x92E0));
    z->t += 7; z->b = 0x08;
x263F:
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_cp(z, 0xFF);
    if (z->zf) { z->t += 12; goto x2647; } z->t += 7;
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 12; goto x263F;
x2647:
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 6; SET_DE(z, DE(z) + 1);
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 4; z->a = z->b;
    z->t += 7; cpu_cp(z, 0x05);
    if (!z->zf) { z->t += 12; goto x2652; } z->t += 7;
    z->t += 7; z->l = 0x08;
x2652:
    if (--z->b) { z->t += 13; goto x263F; } z->t += 8;
    z->t += 20; game_wr16(z->g, 0x92E0, DE(z));
    z->t += 10; SET_HL(z, cpu_pop(z));
    z->t += 10; SET_DE(z, cpu_pop(z));
    z->t += 7; z->b = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; z->c = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 11; cpu_push(z, HL(z));
    z->t += 10; SET_HL(z, 0x9100);
x2662:
    z->t += 4; z->a = z->b;
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_cp(z, 0xFF);
    if (z->zf) { z->t += 12; goto x2679; } z->t += 7;
    z->t += 4; z->e = cpu_inc(z, z->e);
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 4; z->e = cpu_inc(z, z->e);
    z->t += 4; z->a = z->c;
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 4; z->e = cpu_inc(z, z->e);
    z->t += 8; z->l = (uint8_t)(z->l | 0x08);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 4; z->e = cpu_inc(z, z->e);
    z->t += 8; z->l = (uint8_t)(z->l & 0xF7);
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 12; goto x2662;
x2679:
    z->t += 7; z->a = 0x7E;
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 4; z->e = cpu_inc(z, z->e);
    z->t += 10; SET_HL(z, cpu_pop(z));
    z->t += 10; goto x25F5;
x2681:
    z->t += 4; z->e = cpu_dec(z, z->e);
    z->t += 13; z->a = cpu_rd(z, 0x982B);
    z->t += 4; z->b = z->a;
    z->t += 13; z->a = cpu_rd(z, 0x9827);
    z->t += 4; z->a = cpu_dec(z, z->a);
    z->t += 4; cpu_and(z, z->b);
    if (z->zf) { z->t += 12; goto x26A4; } z->t += 7;
    z->t += 13; z->a = cpu_rd(z, 0x9825);
    z->t += 4; cpu_and(z, z->a);
    if (z->zf) { z->t += 12; goto x26A4; } z->t += 7;
    z->t += 4; z->h = z->d;
    z->t += 4; z->a = z->e;
    z->t += 7; cpu_sub(z, 0x04);
    z->t += 4; z->l = z->a;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 4; z->e = cpu_inc(z, z->e);
    z->t += 7; z->a = 0x04;
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 4; z->e = cpu_inc(z, z->e);
    z->t += 7; z->a = 0x87;
    z->t += 13; cpu_wr(z, 0x8B04, z->a);
x26A4:
    z->t += 7; z->a = 0x7F;
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 10; return;
}

void m_2896(Cpu *z)
{
    z->t += 10; SET_HL(z, 0x8920);
    z->t += 16; game_wr16(z->g, 0x9822, HL(z));
    z->t += 14; z->iy = (uint16_t)(0x2908);
    z->t += 13; z->a = cpu_rd(z, 0x9825);
    z->t += 4; cpu_and(z, z->a);
    if (!z->zf) { z->t += 12; goto x28CD; } z->t += 7;
    z->t += 13; z->a = cpu_rd(z, 0x9821);
    z->t += 4; cpu_rrca(z);
    z->t += 4; cpu_rrca(z);
    z->t += 4; z->c = z->a;
    z->t += 4; cpu_rrca(z);
    z->t += 4; z->b = z->a;
    z->t += 7; cpu_and(z, 0x1C);
    z->t += 4; z->a = z->b;
    if (z->zf) { z->t += 12; goto x28B5; } z->t += 7;
    z->t += 7; z->a = 0x03;
x28B5:
    z->t += 7; cpu_and(z, 0x03);
    z->t += 10; SET_HL(z, 0x2900);
    cpu_rst08(z);
    z->t += 10; SET_DE(z, 0x9284);
    z->t += 4; z->a = z->c;
    z->t += 16; cpu_ldi(z);
    z->t += 16; cpu_ldi(z);
    z->t += 10; SET_HL(z, 0x290E);
    z->t += 7; cpu_and(z, 0x07);
    cpu_rst10(z);
    z->t += 7; z->d = cpu_rd(z, HL(z));
    z->t += 4; z->e = z->d;
    z->t += 12; goto x28D0;
x28CD:
    z->t += 10; SET_DE(z, 0x3624);
x28D0:
    z->t += 10; SET_HL(z, 0x8B08);
    z->t += 11; z->ix = (uint16_t)((z->ix & 0xFF00) | (uint8_t)(0x01));
    z->t += 7; z->b = 0x14;
    z->t += 8; z->ix = (uint16_t)((z->ix & 0x00FF) | (uint8_t)(z->d) << 8);
    z->t += 17; m_28E9(z);
    z->t += 7; z->b = 0x08;
    z->t += 11; z->ix = (uint16_t)((z->ix & 0x00FF) | (uint8_t)(0x10) << 8);
    z->t += 17; m_28E9(z);
    z->t += 7; z->b = 0x10;
    z->t += 8; z->ix = (uint16_t)((z->ix & 0x00FF) | (uint8_t)(z->e) << 8);
x28E9:
    z->t += 8; z->ix = (uint16_t)((z->ix & 0xFF00) | (uint8_t)(cpu_dec(z, (uint8_t)z->ix)));
    if (!z->zf) { z->t += 12; goto x28F5; } z->t += 7;
    z->t += 19; z->c = cpu_iy(z, 0);
    z->t += 10; z->iy = (uint16_t)(z->iy + 1);
    z->t += 11; z->ix = (uint16_t)((z->ix & 0xFF00) | (uint8_t)(0x08));
x28F5:
    z->t += 8; z->c = cpu_rlc(z, z->c);
    z->t += 8; z->a = (uint8_t)(z->ix >> 8);
    z->t += 4; cpu_rra(z);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 4; z->l = cpu_inc(z, z->l);
    if (--z->b) { z->t += 13; goto x28E9; } z->t += 8;
    z->t += 10; return;
}

void m_28E9(Cpu *z)
{
x28E9:
    z->t += 8; z->ix = (uint16_t)((z->ix & 0xFF00) | (uint8_t)(cpu_dec(z, (uint8_t)z->ix)));
    if (!z->zf) { z->t += 12; goto x28F5; } z->t += 7;
    z->t += 19; z->c = cpu_iy(z, 0);
    z->t += 10; z->iy = (uint16_t)(z->iy + 1);
    z->t += 11; z->ix = (uint16_t)((z->ix & 0xFF00) | (uint8_t)(0x08));
x28F5:
    z->t += 8; z->c = cpu_rlc(z, z->c);
    z->t += 8; z->a = (uint8_t)(z->ix >> 8);
    z->t += 4; cpu_rra(z);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 4; z->l = cpu_inc(z, z->l);
    if (--z->b) { z->t += 13; goto x28E9; } z->t += 8;
    z->t += 10; return;
}

void m_2C00(Cpu *z)
{
    z->t += 13; z->a = cpu_rd(z, 0x9821);
x2C03:
    z->t += 7; cpu_cp(z, 0x1B);
    if (z->cf) { z->t += 12; goto x2C0B; } z->t += 7;
    z->t += 7; cpu_sub(z, 0x04);
    z->t += 12; goto x2C03;
x2C0B:
    z->t += 4; z->a = cpu_dec(z, z->a);
    z->t += 4; z->l = z->a;
    z->t += 4; cpu_rlca(z);
    z->t += 4; cpu_rlca(z);
    z->t += 4; cpu_add(z, z->l);
    z->t += 4; z->e = z->a;
    z->t += 13; z->a = cpu_rd(z, 0x9984);
    z->t += 10; SET_HL(z, 0x2C65);
    cpu_rst08(z);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; z->h = cpu_rd(z, HL(z));
    z->t += 4; z->l = z->a;
    z->t += 4; z->a = z->e;
    cpu_rst10(z);
    z->t += 10; SET_DE(z, 0x99C0);
    z->t += 7; z->b = 0x05;
x2C23:
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; z->c = z->a;
    z->t += 4; cpu_rlca(z);
    z->t += 4; cpu_rlca(z);
    z->t += 4; cpu_rlca(z);
    z->t += 4; cpu_rlca(z);
    z->t += 7; cpu_and(z, 0x0F);
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 4; z->e = cpu_inc(z, z->e);
    z->t += 4; z->a = z->c;
    z->t += 7; cpu_and(z, 0x0F);
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 4; z->e = cpu_inc(z, z->e);
    z->t += 6; SET_HL(z, HL(z) + 1);
    if (--z->b) { z->t += 13; goto x2C23; } z->t += 8;
    z->t += 13; z->a = cpu_rd(z, 0x9821);
    z->t += 7; cpu_cp(z, 0x03);
    if (!z->cf) { z->t += 12; goto x2C3F; } z->t += 7;
    z->t += 4; cpu_xor(z, z->a);
    z->t += 12; goto x2C46;
x2C3F:
    z->t += 7; cpu_or(z, 0xFC);
    z->t += 4; z->a = cpu_inc(z, z->a);
    if (z->zf) { z->t += 12; goto x2C46; } z->t += 7;
    z->t += 7; z->a = 0x0A;
x2C46:
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 10; SET_BC(z, 0x0216);
    z->t += 20; game_wr16(z->g, 0x92C1, BC(z));
    z->t += 20; game_wr16(z->g, 0x92C0, BC(z));
    z->t += 13; z->a = cpu_rd(z, 0x9821);
    z->t += 7; cpu_cp(z, 0x10);
    if (z->cf) { z->t += 12; goto x2C5B; } z->t += 7;
    z->t += 7; z->a = 0x10;
x2C5B:
    z->t += 4; cpu_rlca(z);
    z->t += 4; cpu_rlca(z);
    z->t += 7; cpu_and(z, 0x70);
    z->t += 7; cpu_add(z, 0x40);
    z->t += 13; cpu_wr(z, 0x99BB, z->a);
    z->t += 10; return;
}

void m_3000(Cpu *z)
{
    z->t += 10; SET_HL(z, 0x83FD);
    z->t += 13; z->a = cpu_rd(z, 0x9840);
    z->t += 4; cpu_and(z, z->a);
    if (z->zf) { z->t += 12; goto x300C; } z->t += 7;
    z->t += 10; SET_HL(z, 0x83E8);
x300C:
    z->t += 16; game_wr16(z->g, 0x8A00, HL(z));
    z->t += 10; SET_DE(z, 0x8A3D);
    z->t += 17; m_31F7(z);
    if (!z->cf) { z->t += 11; return; } z->t += 5;
    z->t += 10; SET_DE(z, 0x8A37);
    z->t += 17; m_31F7(z);
    z->t += 7; z->a = 0x05;
    if (!z->cf) { z->t += 12; goto x3047; } z->t += 7;
    z->t += 10; SET_DE(z, 0x8A31);
    z->t += 17; m_31F7(z);
    z->t += 7; z->a = 0x04;
    if (!z->cf) { z->t += 12; goto x3047; } z->t += 7;
    z->t += 10; SET_DE(z, 0x8A2B);
    z->t += 17; m_31F7(z);
    z->t += 7; z->a = 0x03;
    if (!z->cf) { z->t += 12; goto x3047; } z->t += 7;
    z->t += 10; SET_DE(z, 0x8A25);
    z->t += 17; m_31F7(z);
    z->t += 7; z->a = 0x02;
    if (!z->cf) { z->t += 12; goto x3047; } z->t += 7;
    z->t += 7; z->a = 0xFF;
    z->t += 13; cpu_wr(z, 0x9AAC, z->a);
    z->t += 7; z->a = 0x01;
    z->t += 12; goto x304A;
x3047:
    z->t += 13; cpu_wr(z, 0x9AB0, z->a);
x304A:
    z->t += 13; cpu_wr(z, 0x8A11, z->a);
    z->t += 10; SET_HL(z, 0x31A6);
    z->t += 4; z->a = cpu_dec(z, z->a);
    cpu_rst08(z);
    z->t += 17; m_3118(z);
    z->t += 13; z->a = cpu_rd(z, 0x8A11);
    z->t += 10; SET_HL(z, 0x31A1);
    z->t += 4; z->a = cpu_dec(z, z->a);
    cpu_rst10(z);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 10; SET_HL(z, 0x8A49);
    z->t += 10; SET_DE(z, 0x8A4C);
    z->t += 4; cpu_and(z, z->a);
    if (z->zf) { z->t += 12; goto x306C; } z->t += 7;
    z->t += 4; z->c = z->a;
    z->t += 7; z->b = 0x00;
    cpu_lddr(z);
x306C:
    z->t += 7; z->b = 0x03;
    z->t += 7; z->a = 0x24;
    z->t += 16; game_wr16(z->g, 0x8A04, HL(z));
x3073:
    z->t += 4; z->l = cpu_inc(z, z->l);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    if (--z->b) { z->t += 13; goto x3073; } z->t += 8;
    z->t += 7; z->a = 0x49;
    z->t += 13; cpu_wr(z, 0x8A10, z->a);
    z->t += 10; SET_HL(z, 0x327F);
    z->t += 17; m_3328(z);
    z->t += 17; m_331B(z);
    z->t += 17; m_3328(z);
    z->t += 10; SET_DE(z, 0x8309);
    z->t += 16; SET_HL(z, cpu_rd16(z, 0x8A00));
    z->t += 17; m_3275(z);
    z->t += 10; SET_HL(z, 0x8149);
    z->t += 10; SET_DE(z, 0xFFE0);
    z->t += 10; cpu_wr(z, HL(z), 0x0A);
    z->t += 11; SET_HL(z, cpu_add16(z, HL(z), DE(z)));
    z->t += 10; cpu_wr(z, HL(z), 0x0A);
    z->t += 11; SET_HL(z, cpu_add16(z, HL(z), DE(z)));
    z->t += 10; cpu_wr(z, HL(z), 0x0A);
    z->t += 17; m_321D(z);
    z->t += 17; m_3180(z);
    z->t += 7; z->a = 0x04;
    z->t += 13; cpu_wr(z, 0x92AE, z->a);
x30AA:
    z->t += 13; z->a = cpu_rd(z, 0x92AE);
    z->t += 4; cpu_and(z, z->a);
    if (!z->zf) { z->t += 12; goto x30AA; } z->t += 7;
    z->t += 7; z->a = 0x28;
    z->t += 13; cpu_wr(z, 0x92AE, z->a);
x30B5:
    z->t += 17; m_321D(z);
    z->t += 17; m_3180(z);
    z->t += 13; z->a = cpu_rd(z, 0x92A0);
    z->t += 4; z->c = z->a;
x30BF:
    z->t += 17; m_32ED(z);
    z->t += 13; z->a = cpu_rd(z, 0x92A0);
    z->t += 4; cpu_cp(z, z->c);
    if (z->zf) { z->t += 12; goto x30BF; } z->t += 7;
    z->t += 4; z->c = z->a;
    z->t += 7; cpu_and(z, 0x0F);
    if (z->zf) { z->t += 17; m_3141(z); } else z->t += 10;
    z->t += 10; SET_HL(z, 0x99B6);
    z->t += 13; z->a = cpu_rd(z, 0x9215);
    z->t += 4; cpu_and(z, z->a);
    if (z->zf) { z->t += 12; goto x30D8; } z->t += 7;
    z->t += 6; SET_HL(z, HL(z) + 1);
x30D8:
    z->t += 12; cpu_bit(z, 4, cpu_rd(z, HL(z)));
    z->t += 10; if (z->zf) { goto x314C; }
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_and(z, 0x0A);
    z->t += 10; SET_HL(z, 0x8A02);
    z->t += 10; SET_DE(z, 0x8A03);
    z->t += 7; cpu_cp(z, cpu_rd(z, HL(z)));
    if (z->zf) { z->t += 12; goto x30ED; } z->t += 7;
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 7; z->a = 0xFD;
    z->t += 7; cpu_wr(z, DE(z), z->a);
x30ED:
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 7; cpu_and(z, 0x0F);
    if (!z->zf) { z->t += 12; goto x30BF; } z->t += 7;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_cp(z, 0x08);
    if (z->zf) { z->t += 12; goto x311D; } z->t += 7;
    z->t += 7; cpu_cp(z, 0x02);
    if (!z->zf) { z->t += 12; goto x30BF; } z->t += 7;
    z->t += 7; z->a = 0x28;
    z->t += 13; cpu_wr(z, 0x92AE, z->a);
    z->t += 13; z->a = cpu_rd(z, 0x8A10);
    z->t += 4; z->l = z->a;
    z->t += 7; z->h = 0x81;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; z->a = cpu_dec(z, z->a);
    z->t += 7; cpu_cp(z, 0x09);
    if (z->zf) { z->t += 17; m_3138(z); } else z->t += 10;
    z->t += 7; cpu_cp(z, 0x29);
    if (z->zf) { z->t += 17; m_313B(z); } else z->t += 10;
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 10; goto x30BF;
x311D:
    z->t += 13; z->a = cpu_rd(z, 0x8A10);
    z->t += 4; z->l = z->a;
    z->t += 7; z->h = 0x81;
    z->t += 7; z->a = 0x28;
    z->t += 13; cpu_wr(z, 0x92AE, z->a);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 7; cpu_cp(z, 0x2B);
    if (z->zf) { z->t += 17; m_313E(z); } else z->t += 10;
    z->t += 7; cpu_cp(z, 0x25);
    if (z->zf) { z->t += 17; m_3138(z); } else z->t += 10;
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 10; goto x30BF;
x314C:
    z->t += 13; z->a = cpu_rd(z, 0x8A10);
    z->t += 4; z->l = z->a;
    z->t += 7; z->h = 0x85;
    z->t += 10; cpu_wr(z, HL(z), 0x00);
    z->t += 7; z->h = 0x81;
    z->t += 7; z->c = cpu_rd(z, HL(z));
    z->t += 7; z->a = 0x28;
    z->t += 13; cpu_wr(z, 0x92AE, z->a);
    z->t += 16; SET_HL(z, cpu_rd16(z, 0x8A04));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; cpu_wr(z, HL(z), z->c);
    z->t += 16; game_wr16(z->g, 0x8A04, HL(z));
    z->t += 10; SET_HL(z, 0x8A10);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_sub(z, 0x20);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 10; if (!z->cf) { goto x30B5; }
    z->t += 17; m_321D(z);
    z->t += 17; m_3180(z);
    z->t += 7; z->a = 0x4C;
    z->t += 13; cpu_wr(z, 0x92A0, z->a);
x3179:
    z->t += 13; z->a = cpu_rd(z, 0x92A0);
    z->t += 4; cpu_and(z, z->a);
    if (!z->zf) { z->t += 12; goto x3179; } z->t += 7;
    z->t += 10; return;
}

void m_3118(Cpu *z)
{
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; z->h = cpu_rd(z, HL(z));
    z->t += 4; z->l = z->a;
    z->t += 4; switch (HL(z)) { case 0x31B0: goto x31B0; case 0x31B4: goto x31B4; case 0x31B8: goto x31B8; case 0x31CE: goto x31CE; case 0x31D9: goto x31D9; default: game_unported(z->g, HL(z)); return; }
x31B0:
    z->t += 7; z->a = 0x12;
    z->t += 12; goto x31BA;
x31B4:
    z->t += 7; z->a = 0x0C;
    z->t += 12; goto x31BA;
x31B8:
    z->t += 7; z->a = 0x06;
x31BA:
    z->t += 10; SET_HL(z, 0x8A37);
    z->t += 10; SET_DE(z, 0x8A3D);
    z->t += 10; SET_BC(z, 0x0006);
    cpu_lddr(z);
    z->t += 10; SET_DE(z, 0x8A37);
    z->t += 4; z->c = z->a;
    cpu_lddr(z);
    z->t += 10; goto x31D9;
x31CE:
    z->t += 10; SET_DE(z, 0x8A3D);
    z->t += 10; SET_HL(z, 0x8A37);
    z->t += 10; SET_BC(z, 0x0006);
    cpu_lddr(z);
x31D9:
    z->t += 13; z->a = cpu_rd(z, 0x8A11);
    z->t += 4; z->a = cpu_dec(z, z->a);
    z->t += 10; SET_HL(z, 0x31ED);
    cpu_rst08(z);
    z->t += 7; z->e = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; z->d = cpu_rd(z, HL(z));
    z->t += 16; SET_HL(z, cpu_rd16(z, 0x8A00));
    z->t += 10; SET_BC(z, 0x0006);
    cpu_lddr(z);
    z->t += 10; return;
}

void m_3138(Cpu *z)
{
    z->t += 7; z->a = 0x2A;
    z->t += 10; return;
}

void m_313B(Cpu *z)
{
    z->t += 7; z->a = 0x24;
    z->t += 10; return;
}

void m_313E(Cpu *z)
{
    z->t += 7; z->a = 0x0A;
    z->t += 10; return;
}

void m_3141(Cpu *z)
{
    z->t += 13; z->a = cpu_rd(z, 0x8A10);
    z->t += 4; z->l = z->a;
    z->t += 7; z->h = 0x85;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_xor(z, 0x05);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 10; return;
}

void m_3180(Cpu *z)
{
    z->t += 13; z->a = cpu_rd(z, 0x8A11);
    z->t += 10; SET_HL(z, 0x3197);
    z->t += 4; z->a = cpu_dec(z, z->a);
    cpu_rst08(z);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; z->h = cpu_rd(z, HL(z));
    z->t += 4; z->l = z->a;
    z->t += 7; z->b = 0x16;
    z->t += 10; SET_DE(z, 0xFFE0);
x3191:
    z->t += 10; cpu_wr(z, HL(z), 0x05);
    z->t += 11; SET_HL(z, cpu_add16(z, HL(z), DE(z)));
    if (--z->b) { z->t += 13; goto x3191; } z->t += 8;
    z->t += 10; return;
}

void m_31F7(Cpu *z)
{
    z->t += 16; SET_HL(z, cpu_rd16(z, 0x8A00));
    z->t += 7; z->b = 0x06;
x31FC:
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 7; cpu_cp(z, 0x24);
    if (z->zf) { z->t += 12; goto x320E; } z->t += 7;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_cp(z, 0x24);
    if (z->zf) { z->t += 11; return; } z->t += 5;
    z->t += 7; z->a = cpu_rd(z, DE(z));
x3206:
    z->t += 7; cpu_cp(z, cpu_rd(z, HL(z)));
    if (!z->zf) { z->t += 11; return; } z->t += 5;
x3208:
    z->t += 4; z->l = cpu_dec(z, z->l);
    z->t += 4; z->e = cpu_dec(z, z->e);
    if (--z->b) { z->t += 13; goto x31FC; } z->t += 8;
    z->t += 4; cpu_xor(z, z->a);
    z->t += 10; return;
x320E:
    z->t += 7; cpu_cp(z, cpu_rd(z, HL(z)));
    if (z->zf) { z->t += 12; goto x3208; } z->t += 7;
    z->t += 4; cpu_xor(z, z->a);
    z->t += 12; goto x3206;
}

void m_3214(Cpu *z)
{
    z->t += 10; SET_HL(z, 0x3345);
    z->t += 17; m_3328(z);
    z->t += 17; m_3328(z);
    z->t += 10; SET_HL(z, 0x32B4);
    z->t += 17; m_331B(z);
    z->t += 7; z->b = 0x01;
    z->t += 17; m_3231(z);
    z->t += 17; m_3231(z);
    z->t += 17; m_3231(z);
    z->t += 17; m_3231(z);
    z->t += 4; z->a = z->b;
    z->t += 4; z->a = cpu_dec(z, z->a);
    z->t += 4; cpu_add(z, z->a);
    z->t += 4; cpu_add(z, z->a);
    z->t += 4; cpu_add(z, z->a);
    z->t += 10; SET_HL(z, 0x32C5);
    cpu_rst10(z);
    z->t += 7; z->e = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; z->d = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 4; z->a = z->b;
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 17; m_3273(z);
    z->t += 17; m_3270(z);
    z->t += 17; m_3270(z);
    z->t += 17; m_3273(z);
    z->t += 17; m_3273(z);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; z->c = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 11; cpu_push(z, HL(z));
    z->t += 4; z->h = z->c;
    z->t += 4; z->l = z->a;
    z->t += 17; m_3275(z);
    z->t += 4; z->a = z->e;
    z->t += 7; cpu_sub(z, 0xC0);
    z->t += 4; z->e = z->a;
    if (!z->cf) { z->t += 12; goto x3260; } z->t += 7;
    z->t += 4; z->d = cpu_dec(z, z->d);
x3260:
    z->t += 10; SET_HL(z, cpu_pop(z));
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; z->h = cpu_rd(z, HL(z));
    z->t += 4; z->l = z->a;
    z->t += 17; m_3270(z);
    z->t += 17; m_3270(z);
    z->t += 17; m_3270(z);
    z->t += 4; z->b = cpu_inc(z, z->b);
    z->t += 10; return;
}

void m_321D(Cpu *z)
{
    z->t += 10; SET_HL(z, 0x32B4);
    z->t += 17; m_331B(z);
    z->t += 7; z->b = 0x01;
    z->t += 17; m_3231(z);
    z->t += 17; m_3231(z);
    z->t += 17; m_3231(z);
    z->t += 17; m_3231(z);
    z->t += 4; z->a = z->b;
    z->t += 4; z->a = cpu_dec(z, z->a);
    z->t += 4; cpu_add(z, z->a);
    z->t += 4; cpu_add(z, z->a);
    z->t += 4; cpu_add(z, z->a);
    z->t += 10; SET_HL(z, 0x32C5);
    cpu_rst10(z);
    z->t += 7; z->e = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; z->d = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 4; z->a = z->b;
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 17; m_3273(z);
    z->t += 17; m_3270(z);
    z->t += 17; m_3270(z);
    z->t += 17; m_3273(z);
    z->t += 17; m_3273(z);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; z->c = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 11; cpu_push(z, HL(z));
    z->t += 4; z->h = z->c;
    z->t += 4; z->l = z->a;
    z->t += 17; m_3275(z);
    z->t += 4; z->a = z->e;
    z->t += 7; cpu_sub(z, 0xC0);
    z->t += 4; z->e = z->a;
    if (!z->cf) { z->t += 12; goto x3260; } z->t += 7;
    z->t += 4; z->d = cpu_dec(z, z->d);
x3260:
    z->t += 10; SET_HL(z, cpu_pop(z));
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; z->h = cpu_rd(z, HL(z));
    z->t += 4; z->l = z->a;
    z->t += 17; m_3270(z);
    z->t += 17; m_3270(z);
    z->t += 17; m_3270(z);
    z->t += 4; z->b = cpu_inc(z, z->b);
    z->t += 10; return;
}

void m_3231(Cpu *z)
{
    z->t += 4; z->a = z->b;
    z->t += 4; z->a = cpu_dec(z, z->a);
    z->t += 4; cpu_add(z, z->a);
    z->t += 4; cpu_add(z, z->a);
    z->t += 4; cpu_add(z, z->a);
    z->t += 10; SET_HL(z, 0x32C5);
    cpu_rst10(z);
    z->t += 7; z->e = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; z->d = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 4; z->a = z->b;
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 17; m_3273(z);
    z->t += 17; m_3270(z);
    z->t += 17; m_3270(z);
    z->t += 17; m_3273(z);
    z->t += 17; m_3273(z);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; z->c = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 11; cpu_push(z, HL(z));
    z->t += 4; z->h = z->c;
    z->t += 4; z->l = z->a;
    z->t += 17; m_3275(z);
    z->t += 4; z->a = z->e;
    z->t += 7; cpu_sub(z, 0xC0);
    z->t += 4; z->e = z->a;
    if (!z->cf) { z->t += 12; goto x3260; } z->t += 7;
    z->t += 4; z->d = cpu_dec(z, z->d);
x3260:
    z->t += 10; SET_HL(z, cpu_pop(z));
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; z->h = cpu_rd(z, HL(z));
    z->t += 4; z->l = z->a;
    z->t += 17; m_3270(z);
    z->t += 17; m_3270(z);
    z->t += 17; m_3270(z);
    z->t += 4; z->b = cpu_inc(z, z->b);
    z->t += 10; return;
}

void m_3270(Cpu *z)
{
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 11; m_0020(z);
    z->t += 10; return;
}

void m_3273(Cpu *z)
{
    z->t += 11; m_0020(z);
    z->t += 10; return;
}

void m_3275(Cpu *z)
{
    z->t += 7; z->c = 0x06;
x3277:
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 6; SET_HL(z, HL(z) - 1);
    z->t += 11; m_0020(z);
    z->t += 4; z->c = cpu_dec(z, z->c);
    if (!z->zf) { z->t += 12; goto x3277; } z->t += 7;
    z->t += 10; return;
}

void m_32ED(Cpu *z)
{
    z->t += 13; z->a = cpu_rd(z, 0x99B5);
    z->t += 7; cpu_cp(z, 0xA0);
    if (z->zf) { z->t += 12; goto x32FB; } z->t += 7;
    z->t += 4; z->b = z->a;
    z->t += 13; z->a = cpu_rd(z, 0x99B8);
    z->t += 4; cpu_cp(z, z->b);
    if (z->cf) { z->t += 12; goto x3300; } z->t += 7;
x32FB:
    z->t += 13; z->a = cpu_rd(z, 0x92AE);
    z->t += 4; cpu_and(z, z->a);
    if (!z->zf) { z->t += 11; return; } z->t += 5;
x3300:
    z->t += 10; SET_HL(z, cpu_pop(z));
    z->t += 7; z->h = 0x81;
    z->t += 13; z->a = cpu_rd(z, 0x8A10);
    z->t += 4; z->l = z->a;
    z->t += 20; SET_DE(z, cpu_rd16(z, 0x8A04));
    z->t += 6; SET_DE(z, DE(z) + 1);
x330C:
    z->t += 16; cpu_ldi(z);
    z->t += 7; z->a = 0xDF;
    z->t += 4; z->h = cpu_dec(z, z->h);
    z->t += 4; cpu_add(z, z->l);
    if (!z->cf) { z->t += 12; goto x3315; } z->t += 7;
    z->t += 4; z->h = cpu_inc(z, z->h);
x3315:
    z->t += 4; z->l = z->a;
    z->t += 8; cpu_bit(z, 0, z->h);
    if (!z->zf) { z->t += 12; goto x330C; } z->t += 7;
    z->t += 10; return;
}

void m_331B(Cpu *z)
{
    z->t += 7; z->e = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; z->d = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; z->b = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
x3321:
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 11; m_0020(z);
    if (--z->b) { z->t += 13; goto x3321; } z->t += 8;
    z->t += 10; return;
}

void m_3328(Cpu *z)
{
    z->t += 7; z->e = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; z->d = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; z->b = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; z->c = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 4; cpu_ex_de_hl(z);
x3331:
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 8; z->h = (uint8_t)(z->h | 0x04);
    z->t += 7; cpu_wr(z, HL(z), z->c);
    z->t += 8; z->h = (uint8_t)(z->h & 0xFB);
    z->t += 6; SET_DE(z, DE(z) + 1);
    z->t += 7; z->a = 0xE0;
    z->t += 4; z->h = cpu_dec(z, z->h);
    z->t += 4; cpu_add(z, z->l);
    if (!z->cf) { z->t += 12; goto x3340; } z->t += 7;
    z->t += 4; z->h = cpu_inc(z, z->h);
x3340:
    z->t += 4; z->l = z->a;
    if (--z->b) { z->t += 13; goto x3331; } z->t += 8;
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 10; return;
}

void m_3489(Cpu *z)
{
    z->t += 7; z->b = 0x1E;
    z->t += 10; SET_HL(z, 0x0000);
x348E:
    z->t += 11; cpu_push(z, BC(z));
    z->t += 17; m_3496(z);
    z->t += 10; SET_BC(z, cpu_pop(z));
    if (--z->b) { z->t += 13; goto x348E; } z->t += 8;
    z->t += 10; return;
}

void m_3496(Cpu *z)
{
    z->t += 11; cpu_push(z, DE(z));
    z->t += 11; cpu_push(z, HL(z));
    z->t += 10; SET_BC(z, 0x0400);
x349B:
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
    if (!z->zf) { z->t += 12; goto x349B; } z->t += 7;
    z->t += 10; SET_HL(z, cpu_pop(z));
    z->t += 10; SET_DE(z, cpu_pop(z));
    z->t += 11; cpu_push(z, DE(z));
    z->t += 10; SET_BC(z, 0x0400);
x34B3:
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
    if (!z->zf) { z->t += 12; goto x34B3; } z->t += 7;
    z->t += 10; SET_DE(z, cpu_pop(z));
    z->t += 10; return;
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
}

void m_352B(Cpu *z)
{
    z->t += 11; cpu_push(z, HL(z));
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 7; z->d = 0x10;
    z->t += 4; cpu_xor(z, z->a);
    z->t += 4; z->b = z->a;
x3531:
    z->t += 7; cpu_add(z, cpu_rd(z, HL(z)));
    z->t += 13; cpu_wr(z, 0x6830, z->a);
    z->t += 6; SET_HL(z, HL(z) + 1);
    if (--z->b) { z->t += 13; goto x3531; } z->t += 8;
    z->t += 4; z->d = cpu_dec(z, z->d);
    if (!z->zf) { z->t += 12; goto x3531; } z->t += 7;
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 10; SET_HL(z, cpu_pop(z));
    z->t += 4; cpu_cp(z, z->c);
    if (z->zf) { z->t += 11; return; } z->t += 5;
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
}

void m_3770(Cpu *z)
{
    goto x3770;
x0010:
    z->t += 4; cpu_add(z, z->l);
    z->t += 4; z->l = z->a;
    if (!z->cf) { z->t += 11; return; } z->t += 5;
    z->t += 4; z->h = cpu_inc(z, z->h);
    z->t += 10; return;
x3770:
    z->t += 17; m_377E(z);
    z->t += 17; m_377E(z);
    z->t += 17; m_377E(z);
    z->t += 7; z->a = 0x05;
    z->t += 10; goto x0010;
}

void m_377E(Cpu *z)
{
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 7; z->c = 0x08;
x3781:
    z->t += 4; cpu_add(z, z->a);
    if (!z->cf) { z->t += 12; goto x3785; } z->t += 7;
    z->t += 11; cpu_wr(z, HL(z), cpu_inc(z, cpu_rd(z, HL(z))));
x3785:
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 4; z->c = cpu_dec(z, z->c);
    if (!z->zf) { z->t += 12; goto x3781; } z->t += 7;
    z->t += 6; SET_DE(z, DE(z) + 1);
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 10; return;
}

void m_37F6(Cpu *z)
{
x37F6:
    z->t += 13; z->a = cpu_rd(z, 0x7100);
    z->t += 7; cpu_cp(z, 0x10);
    if (z->zf) { z->t += 11; return; } z->t += 5;
    z->t += 12; goto x37F6;
}

void m_37FE(Cpu *z)
{
    z->t += 13; z->a = cpu_rd(z, 0x6807);
    z->t += 4; cpu_rra(z);
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 7; cpu_and(z, 0x01);
    z->t += 13; cpu_wr(z, 0x9983, z->a);
    z->t += 10; SET_HL(z, 0x3AD6);
    cpu_rst08(z);
    z->t += 17; m_3A6B(z);
    z->t += 13; z->a = cpu_rd(z, 0x99B5);
    z->t += 7; z->c = 0x00;
    z->t += 7; cpu_and(z, 0x0C);
    if (!z->zf) { z->t += 12; goto x3819; } z->t += 7;
    z->t += 4; z->c = cpu_inc(z, z->c);
x3819:
    z->t += 4; z->a = z->c;
    z->t += 13; cpu_wr(z, 0xA007, z->a);
    z->t += 10; SET_HL(z, 0x6800);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; cpu_rra(z);
    z->t += 7; cpu_and(z, 0x01);
    z->t += 4; z->c = z->a;
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_and(z, 0x02);
    z->t += 4; cpu_or(z, z->c);
    z->t += 13; cpu_wr(z, 0x9984, z->a);
    z->t += 10; SET_HL(z, 0x3A72);
    cpu_rst10(z);
    z->t += 10; SET_DE(z, 0x822C);
    z->t += 16; cpu_ldi(z);
    z->t += 10; SET_HL(z, 0x3AEE);
    z->t += 17; m_331B(z);
    z->t += 10; SET_HL(z, 0x6806);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; z->c = cpu_rd(z, HL(z));
    z->t += 8; z->c = cpu_rr(z, z->c);
    z->t += 4; cpu_adc(z, z->a);
    z->t += 7; cpu_and(z, 0x03);
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 13; cpu_wr(z, 0x9982, z->a);
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 13; cpu_wr(z, 0x82EA, z->a);
    z->t += 10; SET_HL(z, 0x3AF5);
    z->t += 17; m_331B(z);
    z->t += 10; SET_HL(z, 0x3ACE);
    z->t += 10; SET_DE(z, 0x9280);
    z->t += 10; SET_BC(z, 0x0008);
    cpu_ldir(z);
    z->t += 10; SET_HL(z, 0x6800);
    z->t += 7; z->b = 0x03;
    z->t += 4; cpu_xor(z, z->a);
x3866:
    z->t += 7; z->c = cpu_rd(z, HL(z));
    z->t += 8; z->c = cpu_rr(z, z->c);
    z->t += 4; cpu_adc(z, z->a);
    z->t += 6; SET_HL(z, HL(z) + 1);
    if (--z->b) { z->t += 13; goto x3866; } z->t += 8;
    z->t += 7; cpu_and(z, 0x07);
    if (z->zf) { z->t += 12; goto x38A5; } z->t += 7;
    z->t += 4; z->a = cpu_dec(z, z->a);
    z->t += 4; cpu_add(z, z->a);
    z->t += 4; cpu_add(z, z->a);
    z->t += 4; cpu_add(z, z->a);
    z->t += 10; SET_HL(z, 0x3A76);
    cpu_rst10(z);
    z->t += 10; SET_DE(z, 0x9281);
    z->t += 10; SET_BC(z, 0x0004);
    cpu_ldir(z);
    z->t += 10; SET_DE(z, 0x82E8);
    z->t += 16; cpu_ldi(z);
    z->t += 10; SET_DE(z, 0x8228);
    z->t += 16; cpu_ldi(z);
    z->t += 10; SET_DE(z, 0x81E8);
    z->t += 16; cpu_ldi(z);
    z->t += 10; SET_DE(z, 0x80E8);
    z->t += 16; cpu_ldi(z);
    z->t += 7; z->a = 0x24;
    z->t += 13; cpu_wr(z, 0x8208, z->a);
    z->t += 10; SET_HL(z, 0x3B00);
    z->t += 17; m_331B(z);
    z->t += 17; m_331B(z);
    z->t += 12; goto x38B5;
x38A5:
    z->t += 10; SET_HL(z, 0x9281);
    z->t += 7; z->b = 0x04;
x38AA:
    z->t += 10; cpu_wr(z, HL(z), 0x00);
    z->t += 6; SET_HL(z, HL(z) + 1);
    if (--z->b) { z->t += 13; goto x38AA; } z->t += 8;
    z->t += 10; SET_HL(z, 0x3B11);
    z->t += 17; m_331B(z);
x38B5:
    z->t += 10; SET_HL(z, 0x6803);
    z->t += 7; z->b = 0x03;
    z->t += 4; cpu_xor(z, z->a);
x38BB:
    z->t += 7; z->c = cpu_rd(z, HL(z));
    z->t += 8; z->c = cpu_rr(z, z->c);
    z->t += 4; cpu_adc(z, z->a);
    z->t += 6; SET_HL(z, HL(z) + 1);
    if (--z->b) { z->t += 13; goto x38BB; } z->t += 8;
    z->t += 7; cpu_and(z, 0x07);
    z->t += 10; if (z->zf) { goto x3937; }
    z->t += 4; z->c = z->a;
    z->t += 13; z->a = cpu_rd(z, 0x9982);
    z->t += 7; cpu_and(z, 0x04);
    z->t += 4; cpu_add(z, z->a);
    z->t += 4; cpu_add(z, z->c);
    z->t += 4; cpu_add(z, z->a);
    z->t += 10; SET_HL(z, 0x3AAE);
    cpu_rst10(z);
    z->t += 10; SET_DE(z, 0x9980);
    z->t += 16; cpu_ldi(z);
    z->t += 16; cpu_ldi(z);
    z->t += 6; SET_HL(z, HL(z) - 1);
    z->t += 7; z->c = 0x01;
    z->t += 17; m_38E4(z);
    z->t += 6; SET_HL(z, HL(z) - 1);
    z->t += 7; z->c = 0x00;
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 10; if (z->zf) { goto x3945; }
    z->t += 4; z->a = z->c;
    z->t += 4; cpu_add(z, z->a);
    z->t += 11; cpu_push(z, HL(z));
    z->t += 10; SET_HL(z, 0x3B27);
    cpu_rst10(z);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; z->h = cpu_rd(z, HL(z));
    z->t += 4; z->l = z->a;
    z->t += 11; cpu_push(z, BC(z));
    z->t += 17; m_331B(z);
    z->t += 17; m_331B(z);
    z->t += 10; SET_BC(z, cpu_pop(z));
    z->t += 10; SET_HL(z, cpu_pop(z));
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_and(z, 0x7F);
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 10; SET_HL(z, 0x81F0);
    z->t += 4; z->b = z->c;
    if (--z->b) { z->t += 13; goto x3909; } z->t += 8;
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 6; SET_HL(z, HL(z) + 1);
x3909:
    z->t += 17; m_3928(z);
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 4; z->c = cpu_dec(z, z->c);
    if (!z->zf) { z->t += 11; return; } z->t += 5;
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 8; cpu_bit(z, 7, z->a);
    z->t += 10; if (!z->zf) { goto x3953; }
    z->t += 10; SET_HL(z, 0x81F4);
    z->t += 17; m_3928(z);
    z->t += 11; cpu_push(z, DE(z));
    z->t += 10; SET_HL(z, 0x3B5A);
    z->t += 17; m_331B(z);
    z->t += 17; m_331B(z);
    z->t += 10; SET_HL(z, cpu_pop(z));
    z->t += 10; return;
x3937:
    z->t += 10; SET_HL(z, 0x3B71);
    z->t += 17; m_331B(z);
    z->t += 10; SET_HL(z, 0x9980);
    z->t += 10; cpu_wr(z, HL(z), 0xFF);
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 10; cpu_wr(z, HL(z), 0xFF);
x3945:
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 10; SET_HL(z, 0x8332);
    z->t += 7; z->b = 0x16;
x394B:
    z->t += 10; cpu_wr(z, HL(z), 0x24);
    z->t += 7; z->a = 0xE0;
    z->t += 4; z->h = cpu_dec(z, z->h);
    cpu_rst10(z);
    if (--z->b) { z->t += 13; goto x394B; } z->t += 8;
x3953:
    z->t += 10; SET_HL(z, 0x8334);
    z->t += 7; z->b = 0x16;
x3958:
    z->t += 10; cpu_wr(z, HL(z), 0x24);
    z->t += 7; z->a = 0xE0;
    z->t += 4; z->h = cpu_dec(z, z->h);
    cpu_rst10(z);
    if (--z->b) { z->t += 13; goto x3958; } z->t += 8;
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 10; return;
}

void m_38E4(Cpu *z)
{
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 10; if (z->zf) { goto x3945; }
    z->t += 4; z->a = z->c;
    z->t += 4; cpu_add(z, z->a);
    z->t += 11; cpu_push(z, HL(z));
    z->t += 10; SET_HL(z, 0x3B27);
    cpu_rst10(z);
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; z->h = cpu_rd(z, HL(z));
    z->t += 4; z->l = z->a;
    z->t += 11; cpu_push(z, BC(z));
    z->t += 17; m_331B(z);
    z->t += 17; m_331B(z);
    z->t += 10; SET_BC(z, cpu_pop(z));
    z->t += 10; SET_HL(z, cpu_pop(z));
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_and(z, 0x7F);
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 10; SET_HL(z, 0x81F0);
    z->t += 4; z->b = z->c;
    if (--z->b) { z->t += 13; goto x3909; } z->t += 8;
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 6; SET_HL(z, HL(z) + 1);
x3909:
    z->t += 17; m_3928(z);
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 4; z->c = cpu_dec(z, z->c);
    if (!z->zf) { z->t += 11; return; } z->t += 5;
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 7; z->a = cpu_rd(z, DE(z));
    z->t += 8; cpu_bit(z, 7, z->a);
    z->t += 10; if (!z->zf) { goto x3953; }
    z->t += 10; SET_HL(z, 0x81F4);
    z->t += 17; m_3928(z);
    z->t += 11; cpu_push(z, DE(z));
    z->t += 10; SET_HL(z, 0x3B5A);
    z->t += 17; m_331B(z);
    z->t += 17; m_331B(z);
    z->t += 10; SET_HL(z, cpu_pop(z));
    z->t += 10; return;
x3945:
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 10; SET_HL(z, 0x8332);
    z->t += 7; z->b = 0x16;
x394B:
    z->t += 10; cpu_wr(z, HL(z), 0x24);
    z->t += 7; z->a = 0xE0;
    z->t += 4; z->h = cpu_dec(z, z->h);
    cpu_rst10(z);
    if (--z->b) { z->t += 13; goto x394B; } z->t += 8;
x3953:
    z->t += 10; SET_HL(z, 0x8334);
    z->t += 7; z->b = 0x16;
x3958:
    z->t += 10; cpu_wr(z, HL(z), 0x24);
    z->t += 7; z->a = 0xE0;
    z->t += 4; z->h = cpu_dec(z, z->h);
    cpu_rst10(z);
    if (--z->b) { z->t += 13; goto x3958; } z->t += 8;
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 10; return;
}

void m_3928(Cpu *z)
{
    z->t += 7; cpu_cp(z, 0x0A);
    z->t += 7; z->b = 0x24;
    if (z->cf) { z->t += 12; goto x3932; } z->t += 7;
    z->t += 7; z->b = 0x01;
    z->t += 7; cpu_sub(z, 0x0A);
x3932:
    z->t += 7; cpu_wr(z, HL(z), z->b);
    z->t += 8; z->l = (uint8_t)(z->l & 0xDF);
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 10; return;
}

void m_3962(Cpu *z)
{
    z->t += 10; SET_HL(z, 0x8000);
    z->t += 10; SET_DE(z, 0x8001);
    z->t += 10; SET_BC(z, 0x0400);
    z->t += 10; cpu_wr(z, HL(z), 0x24);
    cpu_ldir(z);
    z->t += 10; cpu_wr(z, HL(z), 0x03);
    z->t += 10; SET_BC(z, 0x03FF);
    cpu_ldir(z);
    z->t += 7; z->a = 0x07;
    z->t += 13; cpu_wr(z, 0x99BE, z->a);
    z->t += 10; return;
}

void m_397C(Cpu *z)
{
    z->t += 10; SET_HL(z, 0x9380);
    z->t += 7; z->b = 0x80;
x3981:
    z->t += 10; cpu_wr(z, HL(z), 0xF1);
    z->t += 6; SET_HL(z, HL(z) + 1);
    if (--z->b) { z->t += 13; goto x3981; } z->t += 8;
    z->t += 10; return;
}

void m_3987(Cpu *z)
{
    z->t += 10; SET_HL(z, 0x99E0);
    z->t += 10; SET_DE(z, 0x835E);
    z->t += 7; z->c = 0x02;
    z->t += 7; z->b = 0x01;
    z->t += 17; m_39A1(z);
    z->t += 7; z->b = 0x03;
    z->t += 17; m_39A1(z);
    z->t += 7; z->b = 0x02;
    z->t += 17; m_39A1(z);
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; z->b = 0x01;
    z->t += 17; m_39B4(z);
x39A4:
    z->t += 17; m_39AA(z);
    if (--z->b) { z->t += 13; goto x39A4; } z->t += 8;
    z->t += 10; return;
}

void m_39A1(Cpu *z)
{
    z->t += 17; m_39B4(z);
x39A4:
    z->t += 17; m_39AA(z);
    if (--z->b) { z->t += 13; goto x39A4; } z->t += 8;
    z->t += 10; return;
}

void m_39AA(Cpu *z)
{
    z->t += 7; z->a = 0x99;
    z->t += 7; cpu_sub(z, cpu_rd(z, HL(z)));
    z->t += 4; cpu_rra(z);
    z->t += 4; cpu_rra(z);
    z->t += 4; cpu_rra(z);
    z->t += 4; cpu_rra(z);
    z->t += 17; m_39B8(z);
    z->t += 7; z->a = 0x99;
    z->t += 7; cpu_sub(z, cpu_rd(z, HL(z)));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; cpu_and(z, 0x0F);
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 11; m_0020(z);
    z->t += 4; z->c = cpu_dec(z, z->c);
    if (!z->zf) { z->t += 11; return; } z->t += 5;
    z->t += 7; z->a = 0x2A;
    z->t += 7; z->c = 0x04;
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 11; m_0020(z);
    z->t += 10; return;
}

void m_39B4(Cpu *z)
{
    z->t += 7; z->a = 0x99;
    z->t += 7; cpu_sub(z, cpu_rd(z, HL(z)));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; cpu_and(z, 0x0F);
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 11; m_0020(z);
    z->t += 4; z->c = cpu_dec(z, z->c);
    if (!z->zf) { z->t += 11; return; } z->t += 5;
    z->t += 7; z->a = 0x2A;
    z->t += 7; z->c = 0x04;
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 11; m_0020(z);
    z->t += 10; return;
}

void m_39B8(Cpu *z)
{
    z->t += 7; cpu_and(z, 0x0F);
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 11; m_0020(z);
    z->t += 4; z->c = cpu_dec(z, z->c);
    if (!z->zf) { z->t += 11; return; } z->t += 5;
    z->t += 7; z->a = 0x2A;
    z->t += 7; z->c = 0x04;
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 11; m_0020(z);
    z->t += 10; return;
}

void m_39C5(Cpu *z)
{
    z->t += 10; SET_HL(z, 0x835E);
    z->t += 7; z->b = 0x17;
    z->t += 10; SET_DE(z, 0xFFE0);
x39CD:
    z->t += 10; cpu_wr(z, HL(z), 0x24);
    z->t += 11; SET_HL(z, cpu_add16(z, HL(z), DE(z)));
    if (--z->b) { z->t += 13; goto x39CD; } z->t += 8;
    z->t += 10; return;
}

void m_39E0(Cpu *z)
{
    goto x39E0;
x39D3:
    z->t += 11; cpu_push(z, HL(z));
    z->t += 17; m_3987(z);
    z->t += 10; SET_HL(z, 0x0384);
    z->t += 16; game_wr16(z->g, 0x9272, HL(z));
    z->t += 10; SET_HL(z, cpu_pop(z));
    z->t += 10; SET_BC(z, cpu_pop(z));
    z->t += 10; return;
x39E0:
    z->t += 11; cpu_push(z, BC(z));
    z->t += 4; z->a = z->b;
    z->t += 7; cpu_cp(z, 0x0F);
    if (z->zf) { z->t += 12; goto x39D3; } z->t += 7;
    z->t += 7; cpu_cp(z, 0x02);
    if (z->zf) { z->t += 12; goto x39FF; } z->t += 7;
    z->t += 7; cpu_cp(z, 0x04);
    if (!z->zf) { z->t += 12; goto x3A2B; } z->t += 7;
    z->t += 13; z->a = cpu_rd(z, 0x9270);
    z->t += 7; cpu_sub(z, 0x01);
    if (!z->cf) { z->t += 12; goto x39F7; } z->t += 7;
    z->t += 7; z->a = 0x11;
x39F7:
    z->t += 13; cpu_wr(z, 0x9270, z->a);
    z->t += 12; goto x3A06;
x39FF:
    z->t += 13; z->a = cpu_rd(z, 0x9270);
    z->t += 4; z->a = cpu_inc(z, z->a);
    z->t += 13; cpu_wr(z, 0x9270, z->a);
x3A06:
    z->t += 13; z->a = cpu_rd(z, 0x9270);
    z->t += 7; cpu_cp(z, 0x12);
    if (z->cf) { z->t += 12; goto x3A0E; } z->t += 7;
    z->t += 4; cpu_xor(z, z->a);
x3A0E:
    z->t += 13; cpu_wr(z, 0x9270, z->a);
    z->t += 11; cpu_push(z, HL(z));
    z->t += 7; z->c = 0x00;
    z->t += 7; cpu_cp(z, 0x0A);
    if (z->cf) { z->t += 12; goto x3A1B; } z->t += 7;
    z->t += 4; z->c = cpu_inc(z, z->c);
    z->t += 7; cpu_sub(z, 0x0A);
x3A1B:
    z->t += 10; SET_HL(z, 0x822E);
    z->t += 7; cpu_wr(z, HL(z), z->c);
    z->t += 7; z->l = 0x0E;
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 10; SET_HL(z, 0x3A51);
    z->t += 17; m_331B(z);
    z->t += 10; SET_HL(z, cpu_pop(z));
    z->t += 10; SET_BC(z, cpu_pop(z));
    z->t += 10; return;
x3A2B:
    z->t += 13; z->a = cpu_rd(z, 0x9270);
    z->t += 7; cpu_cp(z, 0x12);
    if (z->cf) { z->t += 12; goto x3A33; } z->t += 7;
    z->t += 4; cpu_xor(z, z->a);
x3A33:
    z->t += 13; cpu_wr(z, 0x9270, z->a);
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 17; m_3A46(z);
    z->t += 10; SET_HL(z, 0x3A59);
    cpu_rst10(z);
    z->t += 7; z->l = cpu_rd(z, HL(z));
    z->t += 7; z->h = 0x9A;
    z->t += 10; cpu_wr(z, HL(z), 0x01);
    z->t += 4; cpu_ex_de_hl(z);
    z->t += 10; SET_BC(z, cpu_pop(z));
    z->t += 10; return;
}

void m_39FC(Cpu *z)
{
    z->t += 11; cpu_push(z, BC(z));
    z->t += 12; goto x3A06;
x3A06:
    z->t += 13; z->a = cpu_rd(z, 0x9270);
    z->t += 7; cpu_cp(z, 0x12);
    if (z->cf) { z->t += 12; goto x3A0E; } z->t += 7;
    z->t += 4; cpu_xor(z, z->a);
x3A0E:
    z->t += 13; cpu_wr(z, 0x9270, z->a);
    z->t += 11; cpu_push(z, HL(z));
    z->t += 7; z->c = 0x00;
    z->t += 7; cpu_cp(z, 0x0A);
    if (z->cf) { z->t += 12; goto x3A1B; } z->t += 7;
    z->t += 4; z->c = cpu_inc(z, z->c);
    z->t += 7; cpu_sub(z, 0x0A);
x3A1B:
    z->t += 10; SET_HL(z, 0x822E);
    z->t += 7; cpu_wr(z, HL(z), z->c);
    z->t += 7; z->l = 0x0E;
    z->t += 7; cpu_wr(z, HL(z), z->a);
    z->t += 10; SET_HL(z, 0x3A51);
    z->t += 17; m_331B(z);
    z->t += 10; SET_HL(z, cpu_pop(z));
    z->t += 10; SET_BC(z, cpu_pop(z));
    z->t += 10; return;
}

void m_3A46(Cpu *z)
{
    z->t += 10; SET_HL(z, 0x9AA0);
    z->t += 7; z->b = 0x40;
x3A4B:
    z->t += 10; cpu_wr(z, HL(z), 0x00);
    z->t += 6; SET_HL(z, HL(z) + 1);
    if (--z->b) { z->t += 13; goto x3A4B; } z->t += 8;
    z->t += 10; return;
}

void m_3A6B(Cpu *z)
{
    goto x3A6B;
x331B:
    z->t += 7; z->e = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; z->d = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; z->b = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
x3321:
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 7; cpu_wr(z, DE(z), z->a);
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 11; m_0020(z);
    if (--z->b) { z->t += 13; goto x3321; } z->t += 8;
    z->t += 10; return;
x3A6B:
    z->t += 7; z->a = cpu_rd(z, HL(z));
    z->t += 6; SET_HL(z, HL(z) + 1);
    z->t += 7; z->h = cpu_rd(z, HL(z));
    z->t += 4; z->l = z->a;
    z->t += 10; goto x331B;
}
