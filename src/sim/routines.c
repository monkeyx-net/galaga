#include "sim/routines.h"

void m_0018(Cpu *z)
{
x0018:
    cpu_wr(z, HL(z), z->a);
    SET_HL(z, HL(z) + 1);
    if (--z->b) goto x0018;
    return;
}

void m_0020(Cpu *z)
{
    z->a = z->e;
    cpu_sub(z, 0x20);
    z->e = z->a;
    if (!z->cf) return;
    z->d = cpu_dec(z, z->d);
    return;
}

void m_0028(Cpu *z)
{
    SET_HL(z, 0x9100);
    z->b = 0xF0;
    cpu_xor(z, z->a);
    m_0018(z);
    return;
}

void m_0030(Cpu *z)
{
    cpu_scf(z);
    cpu_ex_af(z);
    goto x13B5;
x13B5:
    cpu_push(z, DE(z));
    cpu_ex_de_hl(z);
    z->a = z->c;
    SET_HL(z, 0x13EF);
    cpu_rst08(z);
    z->a = cpu_rd(z, HL(z));
    SET_HL(z, HL(z) + 1);
    z->h = cpu_rd(z, HL(z));
    z->l = z->a;
    cpu_ex_af(z);
    if (!z->cf) goto x13C9;
    SET_HL(z, HL(z) - 1);
    SET_HL(z, HL(z) - 1);
    z->e = cpu_rd(z, HL(z));
    SET_HL(z, HL(z) + 1);
    z->d = cpu_rd(z, HL(z));
    SET_HL(z, HL(z) + 1);
x13C9:
    z->c = cpu_rd(z, HL(z));
    SET_HL(z, HL(z) + 1);
    cpu_ex_de_hl(z);
x13CC:
    z->a = cpu_rd(z, DE(z));
    cpu_cp(z, 0x2F);
    if (z->zf) goto x13EF;
    cpu_sub(z, 0x30);
    if (!z->cf) goto x13D9;
    z->a = 0x24;
    goto x13DF;
x13D9:
    cpu_cp(z, 0x11);
    if (z->cf) goto x13DF;
    cpu_sub(z, 0x07);
x13DF:
    cpu_wr(z, HL(z), z->a);
    z->h = (uint8_t)(z->h | 0x04);
    cpu_wr(z, HL(z), z->c);
    z->h = (uint8_t)(z->h & 0xFB);
    SET_DE(z, DE(z) + 1);
    z->a = z->l;
    cpu_sub(z, 0x20);
    z->l = z->a;
    if (!z->cf) goto x13CC;
    z->h = cpu_dec(z, z->h);
    goto x13CC;
x13EF:
    SET_DE(z, cpu_pop(z));
    return;
}

void m_003C(Cpu *z)
{
    SET_HL(z, 0x9300);
    z->b = 0x80;
    cpu_xor(z, z->a);
    m_0018(z);
    SET_HL(z, 0x9B00);
    z->b = 0x80;
    m_0018(z);
    SET_HL(z, 0x8800);
    z->a = 0x80;
    z->b = 0x80;
    m_0018(z);
    return;
}

void m_0160(Cpu *z)
{
    SET_HL(z, 0x8040);
    SET_DE(z, 0x8041);
    SET_BC(z, 0x037F);
    cpu_wr(z, HL(z), 0x24);
    cpu_ldir(z);
    SET_HL(z, 0x8440);
    SET_DE(z, 0x8441);
    SET_BC(z, 0x037F);
    cpu_wr(z, HL(z), 0x00);
    cpu_ldir(z);
    z->a = 0x04;
    z->b = 0x20;
    m_0018(z);
    z->a = 0x4E;
    z->b = 0x20;
    m_0018(z);
    return;
}

void m_01C5(Cpu *z)
{
    goto x01C5;
x0185:
    SET_HL(z, 0x9821);
    cpu_wr(z, HL(z), cpu_inc(z, cpu_rd(z, HL(z))));
    z->a = cpu_rd(z, HL(z));
    z->a = cpu_inc(z, z->a);
    cpu_and(z, 0x03);
    cpu_wr(z, 0x9825, z->a);
    if (z->zf) goto x01A2;
    z->c = 0x06;
    m_0030(z);
    cpu_ex_de_hl(z);
    z->a = cpu_rd(z, 0x9821);
    z->l = z->a;
    z->h = 0x00;
    m_0A53(z);
    cpu_xor(z, z->a);
    goto x01AC;
x01A2:
    z->c = 0x07;
    m_0030(z);
    z->a = 0x01;
    cpu_wr(z, 0x9AAD, z->a);
    z->a = 0x08;
x01AC:
    cpu_wr(z, 0x92A8, z->a);
    z->a = 0x03;
    cpu_wr(z, 0x92AE, z->a);
    cpu_wr(z, 0x920B, z->a);
    z->a = cpu_rd(z, 0x9825);
    cpu_and(z, z->a);
    cpu_ex_af(z);
    m_117F(z);
    if (game_wait_for(z->g, 0x92AE, 0)) return; /* the IRQ's timer */
    z->a = cpu_rd(z, 0x92AE);
    cpu_and(z, z->a);
x01C5:
    z->a = 0x78;
    cpu_wr(z, 0x92AE, z->a);
    m_2896(z);
    m_25A2(z);
    z->a = 0x02;
    cpu_wr(z, 0x92AC, z->a);
    cpu_xor(z, z->a);
    m_12D5(z);
    cpu_xor(z, z->a);
    z->b = 0x30;
    SET_HL(z, 0x9200);
x01DF:
    cpu_wr(z, HL(z), z->a);
    z->l = cpu_inc(z, z->l);
    z->l = cpu_inc(z, z->l);
    if (--z->b) goto x01DF;
    cpu_wr(z, 0x9009, z->a);
    cpu_wr(z, 0x9010, z->a);
    cpu_wr(z, 0x9004, z->a);
    cpu_wr(z, 0x9288, z->a);
    cpu_wr(z, 0x982C, z->a);
    cpu_wr(z, 0x9841, z->a);
    cpu_wr(z, 0x9842, z->a);
    cpu_wr(z, 0x9826, z->a);
    cpu_wr(z, 0x99B0, z->a);
    cpu_wr(z, 0x9824, z->a);
    z->a = cpu_inc(z, z->a);
    cpu_wr(z, 0x982D, z->a);
    cpu_wr(z, 0x986D, z->a);
    cpu_wr(z, 0x9828, z->a);
    cpu_wr(z, 0x900B, z->a);
    cpu_wr(z, 0x9008, z->a);
    cpu_wr(z, 0x900A, z->a);
    m_2C00(z);
    SET_HL(z, 0x9830);
    SET_DE(z, 0x01B5);
    z->b = 0x04;
x0220:
    cpu_wr(z, HL(z), z->d);
    z->l = cpu_inc(z, z->l);
    cpu_wr(z, HL(z), z->e);
    z->l = cpu_inc(z, z->l);
    if (--z->b) goto x0220;
    z->a = cpu_rd(z, 0x6805);
    cpu_bit(z, 1, z->a);
    if (!z->zf) return;
    z->c = 0x0B;
    SET_HL(z, 0x83B0);
    m_13B3(z);
    goto x0185;
}

void m_0A53(Cpu *z)
{
    z->b = 0x01;
x0A55:
    z->h = cpu_dec(z, z->h);
    z->h = cpu_inc(z, z->h);
    if (!z->zf) goto x0A5E;
    z->a = z->l;
    cpu_cp(z, 0x0A);
    if (z->cf) goto x0A68;
x0A5E:
    z->a = 0x0A;
    m_1061(z);
    cpu_push(z, AF(z));
    z->b = cpu_inc(z, z->b);
    goto x0A55;
x0A67:
    cpu_set_af(z, cpu_pop(z));
x0A68:
    m_0A6E(z);
    if (--z->b) goto x0A67;
    return;
}

void m_0A6E(Cpu *z)
{
    goto x0A6E;
x0020:
    z->a = z->e;
    cpu_sub(z, 0x20);
    z->e = z->a;
    if (!z->cf) return;
    z->d = cpu_dec(z, z->d);
    return;
x0A6E:
    cpu_wr(z, DE(z), z->a);
    goto x0020;
}

void m_1000(Cpu *z)
{
    cpu_push(z, HL(z));
    z->a = game_r(z->g); /* LD A,R */
    z->h = z->a;
    z->a = cpu_rd(z, 0x92A0);
    cpu_add(z, z->h);
    z->l = z->a;
    z->h = 0x01;
    z->a = cpu_rd(z, HL(z));
    z->h = z->a;
    z->a = game_r(z->g); /* LD A,R */
    cpu_add(z, z->h);
    SET_HL(z, cpu_pop(z));
    return;
}

void m_104E(Cpu *z)
{
    cpu_push(z, DE(z));
    cpu_ex_de_hl(z);
    SET_HL(z, 0x0000);
x1053:
    z->a = cpu_srl(z, z->a);
    if (!z->cf) goto x1058;
    SET_HL(z, cpu_add16(z, HL(z), DE(z)));
x1058:
    z->e = cpu_sla(z, z->e);
    z->d = cpu_rl(z, z->d);
    cpu_and(z, z->a);
    if (!z->zf) goto x1053;
    SET_DE(z, cpu_pop(z));
    return;
}

void m_1061(Cpu *z)
{
    cpu_push(z, BC(z));
    z->c = z->a;
    cpu_xor(z, z->a);
    z->b = 0x11;
x1066:
    cpu_adc(z, z->a);
    if (z->cf) goto x1074;
    cpu_cp(z, z->c);
    if (z->cf) goto x106D;
    cpu_sub(z, z->c);
x106D:
    cpu_ccf(z);
x106E:
    SET_HL(z, cpu_adc16(z, HL(z), HL(z)));
    if (--z->b) goto x1066;
    SET_BC(z, cpu_pop(z));
    return;
x1074:
    cpu_sub(z, z->c);
    cpu_scf(z);
    goto x106E;
}

void m_1079(Cpu *z)
{
    z->a = z->l;
    cpu_and(z, 0x80);
    z->a = cpu_inc(z, z->a);
    cpu_ex_af(z);
    z->l = (uint8_t)(z->l & 0x7F);
    goto x108A;
x108A:
    cpu_push(z, DE(z));
    SET_DE(z, 0x0014);
    z->b = 0x0C;
    z->ix = (uint16_t)(0x9100);
x1094:
    cpu_bit(z, 0, cpu_ix(z, 19));
    if (z->zf) goto x10A0;
    z->ix = (uint16_t)(cpu_add16(z, z->ix, DE(z)));
    if (--z->b) goto x1094;
    SET_DE(z, cpu_pop(z));
    return;
x10A0:
    SET_DE(z, cpu_pop(z));
    cpu_set_ix(z, 8, z->e);
    cpu_set_ix(z, 9, z->d);
    cpu_set_ix(z, 13, 0x01);
    cpu_set_ix(z, 4, 0x00);
    cpu_set_ix(z, 5, 0x01);
    z->c = z->l;
    cpu_set_ix(z, 16, z->c);
    cpu_ex_af(z);
    z->d = z->a;
    cpu_wr(z, HL(z), 0x09);
    z->a = (uint8_t)z->ix;
    z->l = cpu_inc(z, z->l);
    cpu_wr(z, HL(z), z->a);
    z->a = cpu_rd(z, 0x9215);
    z->e = z->a;
    z->l = z->c;
    z->h = 0x93;
    z->c = cpu_rd(z, HL(z));
    z->l = cpu_inc(z, z->l);
    z->b = cpu_rd(z, HL(z));
    z->h = 0x9B;
    z->a = cpu_rd(z, HL(z));
    cpu_rrca(z);
    z->b = cpu_rr(z, z->b);
    cpu_bit(z, 0, z->e);
    if (!z->zf) goto x10DC;
    cpu_ex_af(z);
    z->a = z->b;
    cpu_add(z, 0x50);
    cpu_neg(z);
    z->b = z->a;
    cpu_ex_af(z);
    cpu_ccf(z);
x10DC:
    cpu_set_ix(z, 1, z->b);
    cpu_rra(z);
    cpu_and(z, 0x80);
    cpu_set_ix(z, 0, z->a);
    z->a = z->c;
    cpu_bit(z, 0, z->e);
    if (z->zf) goto x10ED;
    cpu_add(z, 0x0D);
    cpu_cpl(z);
x10ED:
    z->a = cpu_srl(z, z->a);
    cpu_set_ix(z, 3, z->a);
    cpu_rra(z);
    cpu_and(z, 0x80);
    cpu_set_ix(z, 2, z->a);
    cpu_set_ix(z, 19, z->d);
    cpu_set_ix(z, 14, 0x1E);
    z->a = cpu_rd(z, 0x920B);
    cpu_and(z, z->a);
    if (z->zf) goto x1108;
    z->a = cpu_rd(z, 0x92C8);
x1108:
    cpu_set_ix(z, 15, z->a);
    return;
}

void m_1083(Cpu *z)
{
    z->a = z->l;
    cpu_rrca(z);
    cpu_rrca(z);
    cpu_and(z, 0x80);
    z->a = cpu_inc(z, z->a);
    cpu_ex_af(z);
    cpu_push(z, DE(z));
    SET_DE(z, 0x0014);
    z->b = 0x0C;
    z->ix = (uint16_t)(0x9100);
x1094:
    cpu_bit(z, 0, cpu_ix(z, 19));
    if (z->zf) goto x10A0;
    z->ix = (uint16_t)(cpu_add16(z, z->ix, DE(z)));
    if (--z->b) goto x1094;
    SET_DE(z, cpu_pop(z));
    return;
x10A0:
    SET_DE(z, cpu_pop(z));
    cpu_set_ix(z, 8, z->e);
    cpu_set_ix(z, 9, z->d);
    cpu_set_ix(z, 13, 0x01);
    cpu_set_ix(z, 4, 0x00);
    cpu_set_ix(z, 5, 0x01);
    z->c = z->l;
    cpu_set_ix(z, 16, z->c);
    cpu_ex_af(z);
    z->d = z->a;
    cpu_wr(z, HL(z), 0x09);
    z->a = (uint8_t)z->ix;
    z->l = cpu_inc(z, z->l);
    cpu_wr(z, HL(z), z->a);
    z->a = cpu_rd(z, 0x9215);
    z->e = z->a;
    z->l = z->c;
    z->h = 0x93;
    z->c = cpu_rd(z, HL(z));
    z->l = cpu_inc(z, z->l);
    z->b = cpu_rd(z, HL(z));
    z->h = 0x9B;
    z->a = cpu_rd(z, HL(z));
    cpu_rrca(z);
    z->b = cpu_rr(z, z->b);
    cpu_bit(z, 0, z->e);
    if (!z->zf) goto x10DC;
    cpu_ex_af(z);
    z->a = z->b;
    cpu_add(z, 0x50);
    cpu_neg(z);
    z->b = z->a;
    cpu_ex_af(z);
    cpu_ccf(z);
x10DC:
    cpu_set_ix(z, 1, z->b);
    cpu_rra(z);
    cpu_and(z, 0x80);
    cpu_set_ix(z, 0, z->a);
    z->a = z->c;
    cpu_bit(z, 0, z->e);
    if (z->zf) goto x10ED;
    cpu_add(z, 0x0D);
    cpu_cpl(z);
x10ED:
    z->a = cpu_srl(z, z->a);
    cpu_set_ix(z, 3, z->a);
    cpu_rra(z);
    cpu_and(z, 0x80);
    cpu_set_ix(z, 2, z->a);
    cpu_set_ix(z, 19, z->d);
    cpu_set_ix(z, 14, 0x1E);
    z->a = cpu_rd(z, 0x920B);
    cpu_and(z, z->a);
    if (z->zf) goto x1108;
    z->a = cpu_rd(z, 0x92C8);
x1108:
    cpu_set_ix(z, 15, z->a);
    return;
}

void m_117F(Cpu *z)
{
    SET_HL(z, 0x8002);
    z->b = 0x12;
x1184:
    z->a = cpu_rd(z, HL(z));
    cpu_cp(z, 0x4A);
    if (!z->cf) goto x118B;
    cpu_wr(z, HL(z), 0x24);
x118B:
    z->l = cpu_inc(z, z->l);
    if (--z->b) goto x1184;
    z->l = 0x22;
    z->b = 0x12;
x1192:
    z->a = cpu_rd(z, HL(z));
    cpu_cp(z, 0x4A);
    if (!z->cf) goto x1199;
    cpu_wr(z, HL(z), 0x24);
x1199:
    z->l = cpu_inc(z, z->l);
    if (--z->b) goto x1192;
    z->a = cpu_rd(z, 0x9821);
    z->b = 0x00;
    SET_HL(z, 0x8001);
x11A4:
    cpu_cp(z, 0x32);
    if (z->cf) goto x11AF;
    cpu_sub(z, 0x32);
    z->b = cpu_inc(z, z->b);
    z->l = cpu_inc(z, z->l);
    z->l = cpu_inc(z, z->l);
    goto x11A4;
x11AF:
    cpu_ex_de_hl(z);
    z->l = z->a;
    z->h = 0x00;
    z->a = 0x0A;
    m_1061(z);
    z->h = z->a;
    cpu_push(z, HL(z));
    cpu_ex_de_hl(z);
    cpu_cp(z, 0x05);
    if (z->cf) goto x11C1;
    cpu_sub(z, 0x04);
x11C1:
    z->c = z->a;
    z->a = z->e;
    cpu_bit(z, 0, z->a);
    if (z->zf) goto x11C9;
    z->a = 0x02;
x11C9:
    cpu_add(z, z->c);
    cpu_rst10(z);
    z->b = cpu_inc(z, z->b);
x11CC:
    if (--z->b) goto x11EE;
    SET_BC(z, cpu_pop(z));
    z->a = z->c;
    m_11F5(z);
    z->a = z->b;
    cpu_cp(z, 0x05);
    if (z->cf) goto x11E0;
    z->d = 0x38;
    m_1213(z);
    z->a = z->b;
    cpu_sub(z, 0x05);
x11E0:
    z->b = z->a;
    z->b = cpu_inc(z, z->b);
x11E2:
    if (--z->b) goto x11E7;
    goto x137E;
x11E7:
    z->d = 0x36;
    m_1213(z);
    goto x11E2;
x11EE:
    z->a = 0x04;
    m_11FB(z);
    goto x11CC;
x137E:
    z->a = cpu_rd(z, 0x9820);
    cpu_cpl(z);
    cpu_add(z, 0x09);
    z->e = z->a;
    z->d = 0x49;
    SET_HL(z, 0x801D);
    m_1398(z);
    z->l = cpu_dec(z, z->l);
    m_1398(z);
    z->l = (uint8_t)(z->l | 0x20);
    z->l = cpu_inc(z, z->l);
    m_1398(z);
    z->l = cpu_dec(z, z->l);
    cpu_push(z, HL(z));
    z->d = cpu_inc(z, z->d);
    z->c = z->d;
    z->b = 0x08;
x139D:
    z->a = z->b;
    cpu_cp(z, z->e);
    if (!z->zf) goto x13A3;
    z->c = 0x24;
x13A3:
    z->a = cpu_rd(z, HL(z));
    cpu_cp(z, 0x36);
    if (z->cf) goto x13AC;
    cpu_cp(z, 0x4A);
    if (z->cf) goto x13AD;
x13AC:
    cpu_wr(z, HL(z), z->c);
x13AD:
    z->l = cpu_dec(z, z->l);
    z->l = cpu_dec(z, z->l);
    if (--z->b) goto x139D;
    SET_HL(z, cpu_pop(z));
    return;
}

void m_11F5(Cpu *z)
{
    cpu_and(z, z->a);
    if (z->zf) return;
    cpu_cp(z, 0x04);
    if (z->zf) goto x1202;
    cpu_rlca(z);
    cpu_rlca(z);
    cpu_add(z, 0x36);
    z->d = z->a;
    goto x120C;
x1202:
    z->d = 0x42;
    m_1213(z);
    m_1228(z);
    z->d = 0x3A;
x120C:
    m_1213(z);
    m_1228(z);
    return;
}

void m_11FB(Cpu *z)
{
    cpu_rlca(z);
    cpu_rlca(z);
    cpu_add(z, 0x36);
    z->d = z->a;
    goto x120C;
x120C:
    m_1213(z);
    m_1228(z);
    return;
}

void m_1213(Cpu *z)
{
    cpu_ex_af(z);
    if (z->cf) goto x1227;
    cpu_ex_af(z);
    z->a = cpu_rd(z, 0x92A0);
    cpu_add(z, 0x08);
    z->e = z->a;
    if (game_wait_for(z->g, 0x92A0, z->e)) return; /* the frame count */
    z->a = cpu_rd(z, 0x92A0);
    cpu_sub(z, z->e);
    cpu_ex_af(z);
    cpu_wr(z, 0x9AB5, z->a);
x1227:
    cpu_ex_af(z);
    cpu_wr(z, HL(z), z->d);
    z->d = cpu_inc(z, z->d);
    z->l = (uint8_t)(z->l | 0x20);
    cpu_wr(z, HL(z), z->d);
    z->d = cpu_inc(z, z->d);
    z->h = (uint8_t)(z->h | 0x04);
    z->a = z->d;
    cpu_and(z, 0x0C);
    cpu_cp(z, 0x08);
    z->a = 0x01;
    if (z->zf) goto x123A;
    z->a = cpu_inc(z, z->a);
x123A:
    cpu_wr(z, HL(z), z->a);
    z->l = (uint8_t)(z->l & 0xDF);
    cpu_wr(z, HL(z), z->a);
    z->h = (uint8_t)(z->h & 0xFB);
    z->l = cpu_dec(z, z->l);
    return;
}

void m_1228(Cpu *z)
{
    cpu_wr(z, HL(z), z->d);
    z->d = cpu_inc(z, z->d);
    z->l = (uint8_t)(z->l | 0x20);
    cpu_wr(z, HL(z), z->d);
    z->d = cpu_inc(z, z->d);
    z->h = (uint8_t)(z->h | 0x04);
    z->a = z->d;
    cpu_and(z, 0x0C);
    cpu_cp(z, 0x08);
    z->a = 0x01;
    if (z->zf) goto x123A;
    z->a = cpu_inc(z, z->a);
x123A:
    cpu_wr(z, HL(z), z->a);
    z->l = (uint8_t)(z->l & 0xDF);
    cpu_wr(z, HL(z), z->a);
    z->h = (uint8_t)(z->h & 0xFB);
    z->l = cpu_dec(z, z->l);
    return;
}

void m_1242(Cpu *z)
{
    SET_HL(z, 0x125B);
    SET_DE(z, 0x9000);
    SET_BC(z, 0x0020);
    cpu_push(z, BC(z));
    cpu_push(z, HL(z));
    cpu_ldir(z);
    SET_HL(z, cpu_pop(z));
    SET_BC(z, cpu_pop(z));
    SET_DE(z, 0x98E0);
    cpu_ldir(z);
    cpu_xor(z, z->a);
    cpu_wr(z, 0x9000, z->a);
    return;
}

void m_127B(Cpu *z)
{
    SET_HL(z, 0x8B64);
    SET_DE(z, 0x0930);
    z->c = 0x00;
    z->b = 0x0A;
x1285:
    cpu_wr(z, HL(z), z->e);
    z->h = 0x93;
    cpu_wr(z, HL(z), 0x00);
    z->h = 0x9B;
    cpu_wr(z, HL(z), z->c);
    z->h = 0x8B;
    z->l = cpu_inc(z, z->l);
    cpu_wr(z, HL(z), z->d);
    z->l = cpu_inc(z, z->l);
    z->a = z->b;
    cpu_cp(z, 0x09);
    if (!z->zf) goto x129B;
    z->c = 0x01;
    z->d = 0x0B;
x129B:
    if (--z->b) goto x1285;
    return;
}

void m_129E(Cpu *z)
{
    z->h = 0x8B;
    SET_DE(z, game_rd16(z->g, 0x9280));
    z->a = cpu_rd(z, DE(z));
    z->l = z->a;
    SET_DE(z, DE(z) + 1);
    z->a = cpu_rd(z, DE(z));
    z->c = z->a;
    cpu_and(z, 0x78);
    cpu_add(z, 0x06);
    cpu_wr(z, HL(z), z->a);
    z->l = cpu_inc(z, z->l);
    z->a = z->c;
    cpu_and(z, 0x07);
    cpu_bit(z, 7, z->c);
    if (z->zf) goto x12B8;
    cpu_or(z, 0x08);
x12B8:
    cpu_wr(z, HL(z), z->a);
    SET_DE(z, DE(z) + 1);
    z->l = cpu_dec(z, z->l);
    z->h = 0x88;
    cpu_wr(z, HL(z), 0x01);
    z->h = 0x93;
    z->a = cpu_rd(z, DE(z));
    cpu_wr(z, HL(z), z->a);
    SET_DE(z, DE(z) + 1);
    z->l = cpu_inc(z, z->l);
    z->a = cpu_rd(z, DE(z));
    z->a = cpu_sla(z, z->a);
    cpu_wr(z, HL(z), z->a);
    z->a = 0x00;
    cpu_rla(z);
    z->h = 0x9B;
    cpu_wr(z, HL(z), z->a);
    SET_DE(z, DE(z) + 1);
    game_wr16(z->g, 0x9280, DE(z));
    return;
}

void m_12D5(Cpu *z)
{
    z->ix = (uint16_t)((z->ix & 0xFF00) | (uint8_t)(z->a));
    z->a = cpu_rd(z, 0x9215);
    z->c = z->a;
    SET_HL(z, 0x9900);
    SET_DE(z, 0x1321);
    z->b = 0x10;
x12E3:
    cpu_wr(z, HL(z), 0x00);
    z->l = cpu_inc(z, z->l);
    z->a = cpu_rd(z, DE(z));
    SET_DE(z, DE(z) + 1);
    cpu_wr(z, HL(z), z->a);
    z->l = cpu_inc(z, z->l);
    if (--z->b) goto x12E3;
    SET_HL(z, 0x9800);
    SET_DE(z, 0x1321);
    z->b = 0x0A;
x12F4:
    z->a = cpu_rd(z, DE(z));
    SET_DE(z, DE(z) + 1);
    cpu_bit(z, 0, z->c);
    if (z->zf) goto x12FD;
    cpu_add(z, 0x0D);
    cpu_cpl(z);
x12FD:
    cpu_wr(z, HL(z), z->a);
    z->l = cpu_inc(z, z->l);
    z->l = cpu_inc(z, z->l);
    if (--z->b) goto x12F4;
    z->b = 0x06;
x1304:
    z->a = cpu_rd(z, DE(z));
    cpu_add(z, (uint8_t)z->ix);
    SET_DE(z, DE(z) + 1);
    cpu_bit(z, 0, z->c);
    if (!z->zf) goto x130F;
    cpu_add(z, 0x4F);
    cpu_cpl(z);
x130F:
    z->a = cpu_sla(z, z->a);
    cpu_wr(z, HL(z), z->a);
    z->l = cpu_inc(z, z->l);
    z->a = 0x00;
    cpu_rla(z);
    cpu_wr(z, HL(z), z->a);
    z->l = cpu_inc(z, z->l);
    if (--z->b) goto x1304;
    z->a = cpu_rd(z, 0x9215);
    cpu_wr(z, 0x920F, z->a);
    return;
}

void m_134C(Cpu *z)
{
    if (game_wait_for(z->g, 0x9287, 0)) return; /* sub's count of the enemies in flight */
    z->a = cpu_rd(z, 0x9287);
    cpu_and(z, z->a);
    m_137E(z);
    SET_HL(z, 0x0906);
    game_wr16(z->g, 0x8B62, HL(z));
    SET_HL(z, 0x9362);
    z->a = cpu_rd(z, 0x9215);
    cpu_and(z, 0x01);
    z->a = 0x29;
    z->c = 0x01;
    if (z->zf) goto x136C;
    cpu_add(z, 0x0E);
    z->c = cpu_dec(z, z->c);
x136C:
    cpu_wr(z, HL(z), 0x7A);
    z->l = cpu_inc(z, z->l);
    cpu_wr(z, HL(z), z->a);
    z->h = 0x9B;
    cpu_wr(z, HL(z), z->c);
    z->l = cpu_dec(z, z->l);
    cpu_xor(z, z->a);
    cpu_wr(z, HL(z), z->a);
    cpu_wr(z, 0x9213, z->a);
    z->a = cpu_inc(z, z->a);
    cpu_wr(z, 0x99B9, z->a);
    return;
}

void m_137E(Cpu *z)
{
    z->a = cpu_rd(z, 0x9820);
    cpu_cpl(z);
    cpu_add(z, 0x09);
    z->e = z->a;
    z->d = 0x49;
    SET_HL(z, 0x801D);
    m_1398(z);
    z->l = cpu_dec(z, z->l);
    m_1398(z);
    z->l = (uint8_t)(z->l | 0x20);
    z->l = cpu_inc(z, z->l);
    m_1398(z);
    z->l = cpu_dec(z, z->l);
    cpu_push(z, HL(z));
    z->d = cpu_inc(z, z->d);
    z->c = z->d;
    z->b = 0x08;
x139D:
    z->a = z->b;
    cpu_cp(z, z->e);
    if (!z->zf) goto x13A3;
    z->c = 0x24;
x13A3:
    z->a = cpu_rd(z, HL(z));
    cpu_cp(z, 0x36);
    if (z->cf) goto x13AC;
    cpu_cp(z, 0x4A);
    if (z->cf) goto x13AD;
x13AC:
    cpu_wr(z, HL(z), z->c);
x13AD:
    z->l = cpu_dec(z, z->l);
    z->l = cpu_dec(z, z->l);
    if (--z->b) goto x139D;
    SET_HL(z, cpu_pop(z));
    return;
}

void m_1398(Cpu *z)
{
    cpu_push(z, HL(z));
    z->d = cpu_inc(z, z->d);
    z->c = z->d;
    z->b = 0x08;
x139D:
    z->a = z->b;
    cpu_cp(z, z->e);
    if (!z->zf) goto x13A3;
    z->c = 0x24;
x13A3:
    z->a = cpu_rd(z, HL(z));
    cpu_cp(z, 0x36);
    if (z->cf) goto x13AC;
    cpu_cp(z, 0x4A);
    if (z->cf) goto x13AD;
x13AC:
    cpu_wr(z, HL(z), z->c);
x13AD:
    z->l = cpu_dec(z, z->l);
    z->l = cpu_dec(z, z->l);
    if (--z->b) goto x139D;
    SET_HL(z, cpu_pop(z));
    return;
}

void m_13B3(Cpu *z)
{
    cpu_and(z, z->a);
    cpu_ex_af(z);
    cpu_push(z, DE(z));
    cpu_ex_de_hl(z);
    z->a = z->c;
    SET_HL(z, 0x13EF);
    cpu_rst08(z);
    z->a = cpu_rd(z, HL(z));
    SET_HL(z, HL(z) + 1);
    z->h = cpu_rd(z, HL(z));
    z->l = z->a;
    cpu_ex_af(z);
    if (!z->cf) goto x13C9;
    SET_HL(z, HL(z) - 1);
    SET_HL(z, HL(z) - 1);
    z->e = cpu_rd(z, HL(z));
    SET_HL(z, HL(z) + 1);
    z->d = cpu_rd(z, HL(z));
    SET_HL(z, HL(z) + 1);
x13C9:
    z->c = cpu_rd(z, HL(z));
    SET_HL(z, HL(z) + 1);
    cpu_ex_de_hl(z);
x13CC:
    z->a = cpu_rd(z, DE(z));
    cpu_cp(z, 0x2F);
    if (z->zf) goto x13EF;
    cpu_sub(z, 0x30);
    if (!z->cf) goto x13D9;
    z->a = 0x24;
    goto x13DF;
x13D9:
    cpu_cp(z, 0x11);
    if (z->cf) goto x13DF;
    cpu_sub(z, 0x07);
x13DF:
    cpu_wr(z, HL(z), z->a);
    z->h = (uint8_t)(z->h | 0x04);
    cpu_wr(z, HL(z), z->c);
    z->h = (uint8_t)(z->h & 0xFB);
    SET_DE(z, DE(z) + 1);
    z->a = z->l;
    cpu_sub(z, 0x20);
    z->l = z->a;
    if (!z->cf) goto x13CC;
    z->h = cpu_dec(z, z->h);
    goto x13CC;
x13EF:
    SET_DE(z, cpu_pop(z));
    return;
}

int m_1C93(Cpu *z, uint16_t ret)
{
    z->a = z->b;
    cpu_bit(z, 1, z->a);
    if (z->zf) goto x1C9A;
    cpu_xor(z, 0x01);
x1C9A:
    cpu_and(z, 0x03);
    z->a = cpu_sla(z, z->a);
    cpu_add(z, 0x30);
    z->e = z->a;
    z->a = cpu_rd(z, DE(z));
    cpu_cp(z, 0x01);
    if (!z->zf) return 0;
    SET_HL(z, ret); /* POP HL: its return address, so its RET leaves the caller */
    z->iy = (uint16_t)(0x0411);
    z->a = cpu_rd(z, 0x920B);
    cpu_and(z, z->a);
    if (!z->zf) goto x1CB4;
    z->iy = (uint16_t)(0x00F1);
x1CB4:
    z->a = z->e;
    cpu_rrca(z);
    cpu_rrca(z);
    z->a = z->e;
    cpu_rla(z);
    cpu_rrca(z);
    cpu_wr(z, 0x92CA, z->a);
    cpu_ex_af(z);
    game_wr16(z->g, 0x92CB, z->iy);
    z->b = cpu_inc(z, z->b);
    z->a = z->e;
    cpu_and(z, 0x07);
    SET_HL(z, 0x9830);
    cpu_rst10(z);
    z->a = (uint8_t)z->ix;
    cpu_ex_de_hl(z);
    SET_HL(z, 0x1D03);
    cpu_rst08(z);
    z->a = cpu_rd(z, HL(z));
    cpu_wr(z, DE(z), z->a);
    SET_HL(z, HL(z) + 1);
    z->e = cpu_inc(z, z->e);
    z->a = cpu_rd(z, HL(z));
    cpu_wr(z, DE(z), z->a);
    z->a = (uint8_t)z->ix;
    cpu_cp(z, 0x02);
    if (z->zf) goto x1CE9;
    SET_DE(z, 0x92CD);
    z->a = cpu_dec(z, z->a);
    if (z->zf) goto x1CE6;
    m_1D09(z);
x1CE6:
    m_1D09(z);
x1CE9:
    z->a = cpu_rd(z, 0x92CA);
    cpu_and(z, 0x07);
    z->l = z->a;
    z->h = 0x88;
    z->a = cpu_rd(z, HL(z));
    z->a = cpu_dec(z, z->a);
    if (!z->zf) return 1;
    z->c = z->l;
    SET_HL(z, 0x92CA);
x1CF8:
    z->l = cpu_inc(z, z->l);
    z->l = cpu_inc(z, z->l);
    z->l = cpu_inc(z, z->l);
    z->a = cpu_rd(z, HL(z));
    z->a = cpu_inc(z, z->a);
    if (!z->zf) goto x1CF8;
    cpu_ex_af(z);
    z->a = z->c;
    goto x1D1C;
x1D1C:
    cpu_rla(z);
    cpu_rrca(z);
    cpu_wr(z, HL(z), z->a);
    cpu_ex_af(z);
    z->l = cpu_inc(z, z->l);
    z->a = (uint8_t)z->iy;
    cpu_wr(z, HL(z), z->a);
    z->l = cpu_inc(z, z->l);
    z->a = (uint8_t)(z->iy >> 8);
    cpu_wr(z, HL(z), z->a);
    z->l = cpu_inc(z, z->l);
    cpu_ex_de_hl(z);
    return 1;
}

void m_1D09(Cpu *z)
{
    z->c = cpu_rrc(z, z->c);
    if (z->cf) goto x1D13;
    z->b = cpu_dec(z, z->b);
    z->c = cpu_rrc(z, z->c);
    if (z->cf) goto x1D13;
    z->b = cpu_dec(z, z->b);
x1D13:
    z->a = z->b;
    z->b = cpu_dec(z, z->b);
    SET_HL(z, 0x1D32);
    cpu_rst10(z);
    cpu_ex_af(z);
    z->a = cpu_rd(z, HL(z));
    cpu_ex_de_hl(z);
    cpu_rla(z);
    cpu_rrca(z);
    cpu_wr(z, HL(z), z->a);
    cpu_ex_af(z);
    z->l = cpu_inc(z, z->l);
    z->a = (uint8_t)z->iy;
    cpu_wr(z, HL(z), z->a);
    z->l = cpu_inc(z, z->l);
    z->a = (uint8_t)(z->iy >> 8);
    cpu_wr(z, HL(z), z->a);
    z->l = cpu_inc(z, z->l);
    cpu_ex_de_hl(z);
    return;
}

void m_1E49(Cpu *z)
{
x1E49:
    cpu_wr(z, HL(z), cpu_rrc(z, cpu_rd(z, HL(z))));
    if (!z->cf) goto x1E62;
    z->a = cpu_rd(z, DE(z));
    cpu_add(z, z->b);
    cpu_wr(z, DE(z), z->a);
    z->d = 0x98;
    z->a = cpu_rd(z, DE(z));
    cpu_add(z, z->b);
    cpu_wr(z, DE(z), z->a);
    cpu_rra(z);
    cpu_xor(z, z->b);
    cpu_rlca(z);
    if (!z->cf) goto x1E60;
    z->e = cpu_inc(z, z->e);
    z->a = cpu_rd(z, DE(z));
    cpu_xor(z, 0x01);
    cpu_wr(z, DE(z), z->a);
    z->e = cpu_dec(z, z->e);
x1E60:
    z->d = 0x99;
x1E62:
    z->e = cpu_inc(z, z->e);
    z->e = cpu_inc(z, z->e);
    z->l = cpu_inc(z, z->l);
    z->ix = (uint16_t)((z->ix & 0xFF00) | (uint8_t)(cpu_dec(z, (uint8_t)z->ix)));
    if (!z->zf) goto x1E49;
    return;
}

void m_1F19(Cpu *z)
{
    SET_HL(z, 0x9364);
    SET_DE(z, 0x92A4);
    cpu_xor(z, z->a);
    cpu_cp(z, cpu_rd(z, HL(z)));
    if (z->zf) goto x1F28;
    z->l = 0x66;
    z->e = cpu_inc(z, z->e);
    cpu_cp(z, cpu_rd(z, HL(z)));
    if (!z->zf) return;
x1F28:
    cpu_push(z, DE(z));
    cpu_ex_de_hl(z);
    SET_HL(z, 0x9B63);
    z->d = z->h;
    z->e = cpu_inc(z, z->e);
    cpu_bit(z, 2, cpu_rd(z, HL(z)));
    if (z->zf) goto x1F35;
    SET_DE(z, cpu_pop(z));
    return;
x1F35:
    cpu_ldd(z);
    z->h = 0x93;
    z->d = z->h;
    cpu_ldi(z);
    cpu_ldd(z);
    z->h = 0x9B;
    z->d = z->h;
    z->b = cpu_rd(z, HL(z));
    cpu_ex_de_hl(z);
    z->a = cpu_rd(z, 0x9827);
    cpu_and(z, 0x01);
    cpu_rlca(z);
    cpu_rlca(z);
    cpu_rlca(z);
    cpu_or(z, z->b);
    cpu_wr(z, HL(z), z->a);
    z->d = 0x8B;
    z->a = cpu_rd(z, DE(z));
    z->h = z->d;
    cpu_and(z, 0x07);
    z->c = 0x30;
    cpu_cp(z, 0x05);
    if (!z->cf) goto x1F60;
    z->c = cpu_inc(z, z->c);
    cpu_cp(z, 0x02);
    if (!z->cf) goto x1F60;
    z->c = cpu_inc(z, z->c);
    z->c = cpu_inc(z, z->c);
x1F60:
    cpu_wr(z, HL(z), z->c);
    cpu_cp(z, 0x04);
    if (z->cf) goto x1F68;
    cpu_cpl(z);
    cpu_add(z, 0x47);
x1F68:
    z->a = cpu_sla(z, z->a);
    z->c = z->a;
    z->a = z->b;
    cpu_rrca(z);
    cpu_rrca(z);
    cpu_rrca(z);
    cpu_and(z, 0x60);
    z->b = z->a;
    z->a = cpu_rd(z, 0x9215);
    cpu_and(z, z->a);
    z->a = z->b;
    if (!z->zf) goto x1F7B;
    cpu_xor(z, 0x60);
x1F7B:
    cpu_or(z, z->c);
    SET_DE(z, cpu_pop(z));
    cpu_wr(z, DE(z), z->a);
    z->h = 0x88;
    cpu_wr(z, HL(z), 0x06);
    z->a = 0x01;
    cpu_wr(z, 0x9AAF, z->a);
    SET_HL(z, game_rd16(z->g, 0x9846));
    SET_HL(z, HL(z) + 1);
    game_wr16(z->g, 0x9846, HL(z));
    return;
}

void m_1F9C(Cpu *z)
{
    cpu_and(z, 0x0A);
    cpu_cp(z, 0x0A);
    if (z->zf) goto x1FD9;
    SET_HL(z, 0x9215);
    cpu_bit(z, 0, cpu_rd(z, HL(z)));
    if (z->zf) goto x1FAB;
    cpu_xor(z, 0x0A);
x1FAB:
    SET_HL(z, 0x92A3);
    z->b = z->a;
    z->c = 0x01;
    z->a = cpu_rd(z, HL(z));
    cpu_xor(z, 0x01);
    cpu_wr(z, HL(z), z->a);
    if (!z->zf) goto x1FB8;
    z->c = cpu_inc(z, z->c);
x1FB8:
    SET_HL(z, 0x9362);
    z->a = cpu_rd(z, HL(z));
    cpu_and(z, z->a);
    if (z->zf) return;
    cpu_bit(z, 1, z->b);
    if (!z->zf) goto x1FD1;
    z->a = cpu_rd(z, HL(z));
    cpu_cp(z, 0xD1);
    if (z->cf) goto x1FCA;
    cpu_bit(z, 0, z->e);
    if (!z->zf) return;
x1FCA:
    cpu_cp(z, 0xE1);
    if (!z->cf) return;
    cpu_add(z, z->c);
    cpu_wr(z, HL(z), z->a);
    goto x1FDE;
x1FD1:
    z->a = cpu_rd(z, HL(z));
    cpu_cp(z, 0x12);
    if (z->cf) return;
    cpu_sub(z, z->c);
    cpu_wr(z, HL(z), z->a);
    goto x1FDE;
x1FD9:
    cpu_xor(z, z->a);
    cpu_wr(z, 0x92A3, z->a);
    return;
x1FDE:
    cpu_bit(z, 0, z->e);
    if (z->zf) return;
    cpu_add(z, 0x0F);
    cpu_wr(z, 0x9360, z->a);
    return;
}

void m_2188(Cpu *z)
{
    z->a = cpu_rd(z, HL(z));
    z->c = z->a;
    z->a = cpu_srl(z, z->a);
    cpu_xor(z, z->c);
    z->c = z->a;
    z->h = 0x8B;
    z->b = 0x00;
    z->a = cpu_rd(z, HL(z));
    cpu_and(z, 0x07);
    cpu_cp(z, 0x06);
    if (!z->zf) goto x21A7;
    z->c = cpu_dec(z, z->c);
    z->c = cpu_inc(z, z->c);
    if (!z->zf) goto x21A7;
    cpu_ex_af(z);
    z->a = cpu_rd(z, 0x928D);
    cpu_and(z, z->a);
    if (!z->zf) goto x21A6;
    z->b = cpu_inc(z, z->b);
    return;
x21A6:
    cpu_ex_af(z);
x21A7:
    cpu_bit(z, 0, z->c);
    if (!z->zf) goto x21B2;
    cpu_cp(z, 0x06);
    if (z->zf) goto x21B8;
    cpu_wr(z, HL(z), cpu_inc(z, cpu_rd(z, HL(z))));
    goto x21C0;
x21B2:
    cpu_and(z, z->a);
    if (z->zf) goto x21B8;
    cpu_wr(z, HL(z), cpu_dec(z, cpu_rd(z, HL(z))));
    goto x21C0;
x21B8:
    z->c = cpu_dec(z, z->c);
    if (!z->sf) goto x21A7;
    z->c = 0x03;
    goto x21A7;
x21C0:
    z->a = z->c;
    cpu_bit(z, 1, z->a);
    if (z->zf) goto x21C7;
    cpu_xor(z, 0x01);
x21C7:
    z->h = 0x9B;
    cpu_wr(z, HL(z), z->a);
    return;
}

void m_238A(Cpu *z)
{
    z->c = z->a;
    z->a = cpu_rd(z, 0x928A);
    cpu_neg(z);
    cpu_add(z, 0x10);
    z->d = 0x20;
    cpu_rlca(z);
    z->d = cpu_rl(z, z->d);
    cpu_rlca(z);
    z->d = cpu_rl(z, z->d);
    cpu_and(z, 0xE0);
    cpu_add(z, 0x14);
    cpu_add(z, z->c);
    z->e = z->a;
    return;
}

void m_23E0(Cpu *z)
{
    cpu_bit(z, 0, z->a);
    if (z->zf) goto x2596;
    cpu_and(z, 0x02);
    z->e = z->a;
    z->a = cpu_rd(z, 0x92A6);
    z->ix = (uint16_t)((z->ix & 0xFF00) | (uint8_t)(z->a));
    z->b = 0x20;
x23EF:
    z->d = 0x88;
    z->a = cpu_rd(z, DE(z));
    z->a = cpu_sla(z, z->a);
    if (z->cf) goto x2416;
    SET_HL(z, 0x23FF);
    cpu_rst10(z);
    z->a = cpu_rd(z, HL(z));
    SET_HL(z, HL(z) + 1);
    z->h = cpu_rd(z, HL(z));
    z->l = z->a;
    switch (HL(z)) { case 0x2416: goto x2416; case 0x2422: goto x2422; case 0x243C: goto x243C; case 0x245F: goto x245F; case 0x2488: goto x2488; case 0x24B2: goto x24B2; case 0x2535: goto x2535; case 0x254D: goto x254D; case 0x2590: goto x2590; default: game_unported(z->g, HL(z)); return; }
x2413:
    z->e = cpu_dec(z, z->e);
x2414:
    z->ix = (uint16_t)((z->ix & 0xFF00) | (uint8_t)(cpu_inc(z, (uint8_t)z->ix)));
x2416:
    z->a = 0x04;
    cpu_add(z, z->e);
    z->e = z->a;
    if (--z->b) goto x23EF;
    z->a = (uint8_t)z->ix;
    cpu_wr(z, 0x92A6, z->a);
    return;
x2422:
    z->l = z->e;
    z->h = 0x01;
    z->c = cpu_rd(z, HL(z));
    z->l = cpu_inc(z, z->l);
    z->l = cpu_rd(z, HL(z));
    z->h = 0x99;
    z->a = cpu_rd(z, HL(z));
    cpu_ex_af(z);
    z->l = z->c;
    z->c = cpu_rd(z, HL(z));
    z->e = cpu_inc(z, z->e);
    z->a = cpu_rd(z, DE(z));
    cpu_add(z, 0x11);
    z->l = z->a;
    z->h = 0x91;
    cpu_ex_af(z);
    cpu_wr(z, HL(z), z->a);
    z->l = cpu_inc(z, z->l);
    cpu_wr(z, HL(z), z->c);
    goto x2413;
x243C:
    z->h = 0x8B;
    z->l = z->e;
    z->e = cpu_inc(z, z->e);
    z->a = cpu_rd(z, DE(z));
    z->a = cpu_dec(z, z->a);
    if (z->zf) goto x2451;
    cpu_wr(z, DE(z), z->a);
    z->e = cpu_dec(z, z->e);
    cpu_and(z, 0x03);
    if (!z->zf) goto x2416;
    z->a = cpu_rd(z, HL(z));
    cpu_add(z, 0x04);
    cpu_wr(z, HL(z), z->a);
    goto x2416;
x2451:
    z->h = 0x93;
    cpu_xor(z, z->a);
    cpu_wr(z, HL(z), z->a);
    z->h = 0x9B;
    cpu_wr(z, HL(z), z->a);
    z->e = cpu_dec(z, z->e);
    z->a = 0x80;
    cpu_wr(z, DE(z), z->a);
    goto x2416;
x245F:
    z->h = 0x9B;
    z->l = z->e;
    z->a = cpu_rd(z, HL(z));
    cpu_and(z, 0x01);
    z->h = 0x8B;
    if (!z->zf) goto x2473;
    z->a = cpu_rd(z, HL(z));
    cpu_and(z, 0x07);
    cpu_cp(z, 0x06);
    if (z->zf) goto x2483;
    cpu_wr(z, HL(z), cpu_inc(z, cpu_rd(z, HL(z))));
    goto x249B;
x2473:
    z->a = cpu_rd(z, HL(z));
    cpu_and(z, 0x07);
    if (!z->zf) goto x2480;
    z->h = 0x9B;
    cpu_wr(z, HL(z), (uint8_t)(cpu_rd(z, HL(z)) & 0xFE));
    z->h = 0x8B;
    goto x249B;
x2480:
    cpu_wr(z, HL(z), cpu_dec(z, cpu_rd(z, HL(z))));
    goto x249B;
x2483:
    z->a = 0x01;
    cpu_wr(z, DE(z), z->a);
    goto x249B;
x2488:
    z->h = 0x8B;
    z->l = z->e;
    z->a = cpu_rd(z, 0x92A2);
    cpu_wr(z, HL(z), cpu_rrc(z, cpu_rd(z, HL(z))));
    cpu_rrca(z);
    cpu_rrca(z);
    cpu_wr(z, HL(z), cpu_rl(z, cpu_rd(z, HL(z))));
    z->a = cpu_rd(z, 0x920B);
    cpu_and(z, z->a);
    if (z->zf) goto x2414;
x249B:
    z->h = 0x01;
    z->c = cpu_rd(z, HL(z));
    z->l = cpu_inc(z, z->l);
    z->l = cpu_rd(z, HL(z));
    z->h = 0x98;
    z->a = cpu_rd(z, HL(z));
    z->d = 0x93;
    cpu_wr(z, DE(z), z->a);
    z->e = cpu_inc(z, z->e);
    z->l = z->c;
    z->a = cpu_rd(z, HL(z));
    cpu_wr(z, DE(z), z->a);
    z->d = 0x9B;
    z->l = cpu_inc(z, z->l);
    z->a = cpu_rd(z, HL(z));
    cpu_wr(z, DE(z), z->a);
    goto x2413;
x24B2:
    z->l = z->e;
    z->e = cpu_inc(z, z->e);
    z->a = cpu_rd(z, DE(z));
    cpu_cp(z, 0x45);
    if (z->zf) goto x24E6;
    z->a = cpu_inc(z, z->a);
    cpu_wr(z, DE(z), z->a);
    z->e = cpu_dec(z, z->e);
    cpu_cp(z, 0x45);
    if (!z->zf) goto x24C2;
    cpu_add(z, 0x03);
x24C2:
    cpu_cp(z, 0x44);
    if (!z->zf) goto x24E0;
    z->h = 0x93;
    cpu_ex_af(z);
    z->a = cpu_rd(z, HL(z));
    cpu_sub(z, 0x08);
    cpu_wr(z, HL(z), z->a);
    z->l = cpu_inc(z, z->l);
    z->a = cpu_rd(z, HL(z));
    cpu_sub(z, 0x08);
    cpu_wr(z, HL(z), z->a);
    if (!z->cf) goto x24DA;
    z->h = 0x9B;
    z->a = cpu_rd(z, HL(z));
    cpu_xor(z, 0x01);
    cpu_wr(z, HL(z), z->a);
x24DA:
    z->l = cpu_dec(z, z->l);
    z->h = 0x9B;
    cpu_wr(z, HL(z), 0x0C);
    cpu_ex_af(z);
x24E0:
    z->h = 0x8B;
    cpu_wr(z, HL(z), z->a);
    goto x2416;
x24E6:
    z->e = cpu_dec(z, z->e);
    z->h = 0x92;
    z->a = cpu_rd(z, HL(z));
    cpu_cp(z, 0x01);
    if (!z->zf) goto x24FD;
    z->h = 0x93;
    cpu_wr(z, HL(z), 0x00);
    z->h = 0x9B;
    cpu_wr(z, HL(z), 0x00);
    z->h = 0x88;
    cpu_wr(z, HL(z), 0x80);
    goto x2416;
x24FD:
    z->h = 0x8B;
    cpu_wr(z, HL(z), z->a);
    cpu_cp(z, 0x37);
    if (z->cf) goto x250E;
    z->c = 0x0D;
    z->l = cpu_inc(z, z->l);
    cpu_cp(z, 0x3A);
    if (z->cf) goto x250C;
    z->c = cpu_inc(z, z->c);
x250C:
    cpu_wr(z, HL(z), z->c);
    z->l = cpu_dec(z, z->l);
x250E:
    z->h = 0x93;
    z->c = 0x08;
    cpu_cp(z, 0x3B);
    if (!z->cf) goto x251C;
    z->c = 0x00;
    z->a = cpu_rd(z, HL(z));
    cpu_add(z, 0x08);
    cpu_wr(z, HL(z), z->a);
x251C:
    z->l = cpu_inc(z, z->l);
    z->a = cpu_rd(z, HL(z));
    cpu_add(z, 0x08);
    cpu_wr(z, HL(z), z->a);
    z->h = 0x9B;
    if (!z->cf) goto x2529;
    z->a = cpu_rd(z, HL(z));
    cpu_xor(z, 0x01);
    cpu_wr(z, HL(z), z->a);
x2529:
    z->l = cpu_dec(z, z->l);
    cpu_wr(z, HL(z), z->c);
    z->h = 0x88;
    cpu_wr(z, HL(z), 0x05);
    z->l = cpu_inc(z, z->l);
    cpu_wr(z, HL(z), 0x13);
    goto x2416;
x2535:
    z->l = z->e;
    z->l = cpu_inc(z, z->l);
    z->h = z->d;
    cpu_wr(z, HL(z), cpu_dec(z, cpu_rd(z, HL(z))));
    if (!z->zf) goto x2416;
    z->l = cpu_dec(z, z->l);
    cpu_wr(z, HL(z), 0x80);
    z->h = 0x93;
    cpu_wr(z, HL(z), 0x00);
    z->h = 0x9B;
    cpu_wr(z, HL(z), 0x00);
    z->a = 0x80;
    cpu_wr(z, DE(z), z->a);
    goto x2416;
x254D:
    z->h = 0x93;
    z->l = z->e;
    z->l = (uint8_t)(z->l | 0x80);
    z->a = cpu_rd(z, HL(z));
    cpu_cp(z, 0xF4);
    if (!z->cf) goto x2571;
    z->l = cpu_inc(z, z->l);
    z->c = cpu_rd(z, HL(z));
    z->h = 0x9B;
    z->a = cpu_rd(z, HL(z));
    z->l = cpu_dec(z, z->l);
    cpu_rrca(z);
    z->a = z->c;
    cpu_rra(z);
    cpu_cp(z, 0x0B);
    if (z->cf) goto x2571;
    cpu_cp(z, 0xA5);
    if (!z->cf) goto x2571;
    z->a = cpu_rd(z, DE(z));
    cpu_cp(z, 0x06);
    if (!z->zf) goto x2414;
    goto x2416;
x2571:
    z->l = (uint8_t)(z->l & 0x7F);
    z->a = cpu_rd(z, DE(z));
    cpu_cp(z, 0x03);
    if (z->zf) goto x2582;
x2578:
    z->a = 0x80;
    cpu_wr(z, DE(z), z->a);
    z->h = 0x93;
    cpu_wr(z, HL(z), 0x00);
    goto x2416;
x2582:
    z->e = cpu_inc(z, z->e);
    z->a = cpu_rd(z, DE(z));
    z->e = cpu_dec(z, z->e);
    cpu_add(z, 0x13);
    z->l = z->a;
    z->h = 0x91;
    cpu_wr(z, HL(z), 0x00);
    z->l = z->e;
    goto x2578;
x2590:
    z->a = 0x03;
    cpu_wr(z, DE(z), z->a);
    goto x2414;
x2596:
    cpu_bit(z, 1, z->a);
    if (z->zf) return;
    SET_HL(z, 0x92A6);
    z->a = cpu_rd(z, HL(z));
    cpu_wr(z, HL(z), 0x00);
    z->l = cpu_inc(z, z->l);
    cpu_wr(z, HL(z), z->a);
    return;
}

void m_25A2(Cpu *z)
{
    SET_HL(z, 0x286E);
    game_wr16(z->g, 0x92E0, HL(z));
    z->a = cpu_rd(z, 0x9821);
    z->c = z->a;
x25AC:
    cpu_cp(z, 0x17);
    if (z->cf) goto x25B4;
    cpu_sub(z, 0x04);
    goto x25AC;
x25B4:
    z->b = z->a;
    z->a = cpu_inc(z, z->a);
    cpu_and(z, 0x03);
    if (z->zf) goto x25D3;
    z->a = cpu_rd(z, 0x9984);
    z->l = 0x11;
    m_104E(z);
    z->a = z->l;
    SET_HL(z, 0x26A8);
    cpu_rst10(z);
    SET_DE(z, 0x26F4);
    z->a = z->b;
    z->b = cpu_srl(z, z->b);
    z->b = cpu_srl(z, z->b);
    cpu_sub(z, z->b);
    z->a = cpu_dec(z, z->a);
    goto x25E0;
x25D3:
    SET_HL(z, 0x26EC);
    z->a = z->c;
    z->a = cpu_srl(z, z->a);
    z->a = cpu_srl(z, z->a);
    cpu_and(z, 0x07);
    SET_DE(z, 0x27DE);
x25E0:
    cpu_rst10(z);
    z->a = cpu_rd(z, HL(z));
    cpu_ex_de_hl(z);
    cpu_rst10(z);
    z->a = cpu_rd(z, HL(z));
    SET_HL(z, HL(z) + 1);
    cpu_wr(z, 0x92E2, z->a);
    z->a = cpu_rd(z, HL(z));
    SET_HL(z, HL(z) + 1);
    cpu_wr(z, 0x92E3, z->a);
    SET_DE(z, 0x8920);
    z->a = 0x7E;
    cpu_wr(z, DE(z), z->a);
    z->e = cpu_inc(z, z->e);
x25F5:
    z->a = cpu_rd(z, HL(z));
    SET_HL(z, HL(z) + 1);
    cpu_cp(z, 0xFF);
    if (z->zf) goto x2681;
    z->c = z->a;
    cpu_push(z, DE(z));
    cpu_push(z, HL(z));
    SET_HL(z, 0x9100);
    z->a = 0xFF;
    z->b = 0x10;
    m_0018(z);
    z->a = z->c;
    cpu_and(z, 0x0F);
    if (z->zf) goto x2636;
    z->b = z->a;
    z->a = cpu_srl(z, z->a);
    cpu_add(z, 0x04);
    z->e = z->a;
x2612:
    m_1000(z);
    z->l = z->a;
    z->h = 0x00;
    z->a = z->e;
    m_1061(z);
    cpu_bit(z, 0, z->b);
    if (z->zf) goto x2622;
    z->a = (uint8_t)(z->a | 0x08);
x2622:
    z->h = 0x91;
    z->l = z->a;
    z->a = cpu_rd(z, HL(z));
    z->a = cpu_inc(z, z->a);
    if (!z->zf) goto x2612;
    z->a = z->b;
    cpu_rlca(z);
    z->c = cpu_rlc(z, z->c);
    if (!z->cf) goto x2631;
    cpu_or(z, 0x40);
x2631:
    cpu_or(z, 0x38);
    cpu_wr(z, HL(z), z->a);
    if (--z->b) goto x2612;
x2636:
    SET_HL(z, 0x9100);
    SET_DE(z, game_rd16(z->g, 0x92E0));
    z->b = 0x08;
x263F:
    z->a = cpu_rd(z, HL(z));
    cpu_cp(z, 0xFF);
    if (z->zf) goto x2647;
    SET_HL(z, HL(z) + 1);
    goto x263F;
x2647:
    z->a = cpu_rd(z, DE(z));
    cpu_wr(z, HL(z), z->a);
    SET_DE(z, DE(z) + 1);
    SET_HL(z, HL(z) + 1);
    z->a = z->b;
    cpu_cp(z, 0x05);
    if (!z->zf) goto x2652;
    z->l = 0x08;
x2652:
    if (--z->b) goto x263F;
    game_wr16(z->g, 0x92E0, DE(z));
    SET_HL(z, cpu_pop(z));
    SET_DE(z, cpu_pop(z));
    z->b = cpu_rd(z, HL(z));
    SET_HL(z, HL(z) + 1);
    z->c = cpu_rd(z, HL(z));
    SET_HL(z, HL(z) + 1);
    cpu_push(z, HL(z));
    SET_HL(z, 0x9100);
x2662:
    z->a = z->b;
    cpu_wr(z, DE(z), z->a);
    z->a = cpu_rd(z, HL(z));
    cpu_cp(z, 0xFF);
    if (z->zf) goto x2679;
    z->e = cpu_inc(z, z->e);
    cpu_wr(z, DE(z), z->a);
    z->e = cpu_inc(z, z->e);
    z->a = z->c;
    cpu_wr(z, DE(z), z->a);
    z->e = cpu_inc(z, z->e);
    z->l = (uint8_t)(z->l | 0x08);
    z->a = cpu_rd(z, HL(z));
    cpu_wr(z, DE(z), z->a);
    z->e = cpu_inc(z, z->e);
    z->l = (uint8_t)(z->l & 0xF7);
    SET_HL(z, HL(z) + 1);
    goto x2662;
x2679:
    z->a = 0x7E;
    cpu_wr(z, DE(z), z->a);
    z->e = cpu_inc(z, z->e);
    SET_HL(z, cpu_pop(z));
    goto x25F5;
x2681:
    z->e = cpu_dec(z, z->e);
    z->a = cpu_rd(z, 0x982B);
    z->b = z->a;
    z->a = cpu_rd(z, 0x9827);
    z->a = cpu_dec(z, z->a);
    cpu_and(z, z->b);
    if (z->zf) goto x26A4;
    z->a = cpu_rd(z, 0x9825);
    cpu_and(z, z->a);
    if (z->zf) goto x26A4;
    z->h = z->d;
    z->a = z->e;
    cpu_sub(z, 0x04);
    z->l = z->a;
    z->a = cpu_rd(z, HL(z));
    cpu_wr(z, DE(z), z->a);
    z->e = cpu_inc(z, z->e);
    z->a = 0x04;
    cpu_wr(z, DE(z), z->a);
    z->e = cpu_inc(z, z->e);
    z->a = 0x87;
    cpu_wr(z, 0x8B04, z->a);
x26A4:
    z->a = 0x7F;
    cpu_wr(z, DE(z), z->a);
    return;
}

void m_2896(Cpu *z)
{
    SET_HL(z, 0x8920);
    game_wr16(z->g, 0x9822, HL(z));
    z->iy = (uint16_t)(0x2908);
    z->a = cpu_rd(z, 0x9825);
    cpu_and(z, z->a);
    if (!z->zf) goto x28CD;
    z->a = cpu_rd(z, 0x9821);
    cpu_rrca(z);
    cpu_rrca(z);
    z->c = z->a;
    cpu_rrca(z);
    z->b = z->a;
    cpu_and(z, 0x1C);
    z->a = z->b;
    if (z->zf) goto x28B5;
    z->a = 0x03;
x28B5:
    cpu_and(z, 0x03);
    SET_HL(z, 0x2900);
    cpu_rst08(z);
    SET_DE(z, 0x9284);
    z->a = z->c;
    cpu_ldi(z);
    cpu_ldi(z);
    SET_HL(z, 0x290E);
    cpu_and(z, 0x07);
    cpu_rst10(z);
    z->d = cpu_rd(z, HL(z));
    z->e = z->d;
    goto x28D0;
x28CD:
    SET_DE(z, 0x3624);
x28D0:
    SET_HL(z, 0x8B08);
    z->ix = (uint16_t)((z->ix & 0xFF00) | (uint8_t)(0x01));
    z->b = 0x14;
    z->ix = (uint16_t)((z->ix & 0x00FF) | (uint8_t)(z->d) << 8);
    m_28E9(z);
    z->b = 0x08;
    z->ix = (uint16_t)((z->ix & 0x00FF) | (uint8_t)(0x10) << 8);
    m_28E9(z);
    z->b = 0x10;
    z->ix = (uint16_t)((z->ix & 0x00FF) | (uint8_t)(z->e) << 8);
x28E9:
    z->ix = (uint16_t)((z->ix & 0xFF00) | (uint8_t)(cpu_dec(z, (uint8_t)z->ix)));
    if (!z->zf) goto x28F5;
    z->c = cpu_iy(z, 0);
    z->iy = (uint16_t)(z->iy + 1);
    z->ix = (uint16_t)((z->ix & 0xFF00) | (uint8_t)(0x08));
x28F5:
    z->c = cpu_rlc(z, z->c);
    z->a = (uint8_t)(z->ix >> 8);
    cpu_rra(z);
    cpu_wr(z, HL(z), z->a);
    z->l = cpu_inc(z, z->l);
    z->l = cpu_inc(z, z->l);
    if (--z->b) goto x28E9;
    return;
}

void m_28E9(Cpu *z)
{
x28E9:
    z->ix = (uint16_t)((z->ix & 0xFF00) | (uint8_t)(cpu_dec(z, (uint8_t)z->ix)));
    if (!z->zf) goto x28F5;
    z->c = cpu_iy(z, 0);
    z->iy = (uint16_t)(z->iy + 1);
    z->ix = (uint16_t)((z->ix & 0xFF00) | (uint8_t)(0x08));
x28F5:
    z->c = cpu_rlc(z, z->c);
    z->a = (uint8_t)(z->ix >> 8);
    cpu_rra(z);
    cpu_wr(z, HL(z), z->a);
    z->l = cpu_inc(z, z->l);
    z->l = cpu_inc(z, z->l);
    if (--z->b) goto x28E9;
    return;
}

void m_2C00(Cpu *z)
{
    z->a = cpu_rd(z, 0x9821);
x2C03:
    cpu_cp(z, 0x1B);
    if (z->cf) goto x2C0B;
    cpu_sub(z, 0x04);
    goto x2C03;
x2C0B:
    z->a = cpu_dec(z, z->a);
    z->l = z->a;
    cpu_rlca(z);
    cpu_rlca(z);
    cpu_add(z, z->l);
    z->e = z->a;
    z->a = cpu_rd(z, 0x9984);
    SET_HL(z, 0x2C65);
    cpu_rst08(z);
    z->a = cpu_rd(z, HL(z));
    SET_HL(z, HL(z) + 1);
    z->h = cpu_rd(z, HL(z));
    z->l = z->a;
    z->a = z->e;
    cpu_rst10(z);
    SET_DE(z, 0x99C0);
    z->b = 0x05;
x2C23:
    z->a = cpu_rd(z, HL(z));
    z->c = z->a;
    cpu_rlca(z);
    cpu_rlca(z);
    cpu_rlca(z);
    cpu_rlca(z);
    cpu_and(z, 0x0F);
    cpu_wr(z, DE(z), z->a);
    z->e = cpu_inc(z, z->e);
    z->a = z->c;
    cpu_and(z, 0x0F);
    cpu_wr(z, DE(z), z->a);
    z->e = cpu_inc(z, z->e);
    SET_HL(z, HL(z) + 1);
    if (--z->b) goto x2C23;
    z->a = cpu_rd(z, 0x9821);
    cpu_cp(z, 0x03);
    if (!z->cf) goto x2C3F;
    cpu_xor(z, z->a);
    goto x2C46;
x2C3F:
    cpu_or(z, 0xFC);
    z->a = cpu_inc(z, z->a);
    if (z->zf) goto x2C46;
    z->a = 0x0A;
x2C46:
    cpu_wr(z, DE(z), z->a);
    SET_BC(z, 0x0216);
    game_wr16(z->g, 0x92C1, BC(z));
    game_wr16(z->g, 0x92C0, BC(z));
    z->a = cpu_rd(z, 0x9821);
    cpu_cp(z, 0x10);
    if (z->cf) goto x2C5B;
    z->a = 0x10;
x2C5B:
    cpu_rlca(z);
    cpu_rlca(z);
    cpu_and(z, 0x70);
    cpu_add(z, 0x40);
    cpu_wr(z, 0x99BB, z->a);
    return;
}

void m_3214(Cpu *z)
{
    SET_HL(z, 0x3345);
    m_3328(z);
    m_3328(z);
    SET_HL(z, 0x32B4);
    m_331B(z);
    z->b = 0x01;
    m_3231(z);
    m_3231(z);
    m_3231(z);
    m_3231(z);
    z->a = z->b;
    z->a = cpu_dec(z, z->a);
    cpu_add(z, z->a);
    cpu_add(z, z->a);
    cpu_add(z, z->a);
    SET_HL(z, 0x32C5);
    cpu_rst10(z);
    z->e = cpu_rd(z, HL(z));
    SET_HL(z, HL(z) + 1);
    z->d = cpu_rd(z, HL(z));
    SET_HL(z, HL(z) + 1);
    z->a = z->b;
    cpu_wr(z, DE(z), z->a);
    m_3273(z);
    m_3270(z);
    m_3270(z);
    m_3273(z);
    m_3273(z);
    z->a = cpu_rd(z, HL(z));
    SET_HL(z, HL(z) + 1);
    z->c = cpu_rd(z, HL(z));
    SET_HL(z, HL(z) + 1);
    cpu_push(z, HL(z));
    z->h = z->c;
    z->l = z->a;
    m_3275(z);
    z->a = z->e;
    cpu_sub(z, 0xC0);
    z->e = z->a;
    if (!z->cf) goto x3260;
    z->d = cpu_dec(z, z->d);
x3260:
    SET_HL(z, cpu_pop(z));
    z->a = cpu_rd(z, HL(z));
    SET_HL(z, HL(z) + 1);
    z->h = cpu_rd(z, HL(z));
    z->l = z->a;
    m_3270(z);
    m_3270(z);
    m_3270(z);
    z->b = cpu_inc(z, z->b);
    return;
}

void m_3231(Cpu *z)
{
    z->a = z->b;
    z->a = cpu_dec(z, z->a);
    cpu_add(z, z->a);
    cpu_add(z, z->a);
    cpu_add(z, z->a);
    SET_HL(z, 0x32C5);
    cpu_rst10(z);
    z->e = cpu_rd(z, HL(z));
    SET_HL(z, HL(z) + 1);
    z->d = cpu_rd(z, HL(z));
    SET_HL(z, HL(z) + 1);
    z->a = z->b;
    cpu_wr(z, DE(z), z->a);
    m_3273(z);
    m_3270(z);
    m_3270(z);
    m_3273(z);
    m_3273(z);
    z->a = cpu_rd(z, HL(z));
    SET_HL(z, HL(z) + 1);
    z->c = cpu_rd(z, HL(z));
    SET_HL(z, HL(z) + 1);
    cpu_push(z, HL(z));
    z->h = z->c;
    z->l = z->a;
    m_3275(z);
    z->a = z->e;
    cpu_sub(z, 0xC0);
    z->e = z->a;
    if (!z->cf) goto x3260;
    z->d = cpu_dec(z, z->d);
x3260:
    SET_HL(z, cpu_pop(z));
    z->a = cpu_rd(z, HL(z));
    SET_HL(z, HL(z) + 1);
    z->h = cpu_rd(z, HL(z));
    z->l = z->a;
    m_3270(z);
    m_3270(z);
    m_3270(z);
    z->b = cpu_inc(z, z->b);
    return;
}

void m_3270(Cpu *z)
{
    z->a = cpu_rd(z, HL(z));
    cpu_wr(z, DE(z), z->a);
    SET_HL(z, HL(z) + 1);
    m_0020(z);
    return;
}

void m_3273(Cpu *z)
{
    m_0020(z);
    return;
}

void m_3275(Cpu *z)
{
    z->c = 0x06;
x3277:
    z->a = cpu_rd(z, HL(z));
    cpu_wr(z, DE(z), z->a);
    SET_HL(z, HL(z) - 1);
    m_0020(z);
    z->c = cpu_dec(z, z->c);
    if (!z->zf) goto x3277;
    return;
}

void m_331B(Cpu *z)
{
    z->e = cpu_rd(z, HL(z));
    SET_HL(z, HL(z) + 1);
    z->d = cpu_rd(z, HL(z));
    SET_HL(z, HL(z) + 1);
    z->b = cpu_rd(z, HL(z));
    SET_HL(z, HL(z) + 1);
x3321:
    z->a = cpu_rd(z, HL(z));
    cpu_wr(z, DE(z), z->a);
    SET_HL(z, HL(z) + 1);
    m_0020(z);
    if (--z->b) goto x3321;
    return;
}

void m_3328(Cpu *z)
{
    z->e = cpu_rd(z, HL(z));
    SET_HL(z, HL(z) + 1);
    z->d = cpu_rd(z, HL(z));
    SET_HL(z, HL(z) + 1);
    z->b = cpu_rd(z, HL(z));
    SET_HL(z, HL(z) + 1);
    z->c = cpu_rd(z, HL(z));
    SET_HL(z, HL(z) + 1);
    cpu_ex_de_hl(z);
x3331:
    z->a = cpu_rd(z, DE(z));
    cpu_wr(z, HL(z), z->a);
    z->h = (uint8_t)(z->h | 0x04);
    cpu_wr(z, HL(z), z->c);
    z->h = (uint8_t)(z->h & 0xFB);
    SET_DE(z, DE(z) + 1);
    z->a = 0xE0;
    z->h = cpu_dec(z, z->h);
    cpu_add(z, z->l);
    if (!z->cf) goto x3340;
    z->h = cpu_inc(z, z->h);
x3340:
    z->l = z->a;
    if (--z->b) goto x3331;
    cpu_ex_de_hl(z);
    return;
}
