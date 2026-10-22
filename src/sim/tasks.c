#include "sim/tasks.h"

#include "sim/routines.h"

static void t_1700(Cpu *z)
{
    SET_DE(z, game_rd16(z->g, 0x9282));
    z->a = cpu_rd(z, DE(z));
    cpu_rlca(z);
    cpu_rlca(z);
    cpu_rlca(z);
    cpu_and(z, 0x07);
    SET_HL(z, 0x1713);
    cpu_rst08(z);
    z->a = cpu_rd(z, HL(z));
    SET_HL(z, HL(z) + 1);
    z->h = cpu_rd(z, HL(z));
    z->l = z->a;
    switch (HL(z)) { case 0x171F: goto x171F; case 0x172D: goto x172D; case 0x1734: goto x1734; case 0x1766: goto x1766; default: game_unported(z->g, HL(z)); return; }
x171F:
    z->a = cpu_rd(z, 0x92A0);
    cpu_and(z, 0x0F);
    if (!z->zf) return;
    SET_HL(z, 0x9207);
    cpu_wr(z, HL(z), cpu_dec(z, cpu_rd(z, HL(z))));
    if (!z->zf) return;
    goto x1766;
x172D:
    m_1F19(z);
    SET_DE(z, game_rd16(z->g, 0x9282));
x1734:
    z->a = cpu_rd(z, DE(z));
    SET_HL(z, 0x9827);
    z->e = cpu_rd(z, HL(z));
    cpu_bit(z, 0, z->a);
    if (!z->zf) goto x1741;
    cpu_and(z, 0x0A);
    goto x1755;
x1741:
    z->a = cpu_rd(z, 0x9209);
    z->l = z->a;
    z->h = 0x93;
    z->a = cpu_rd(z, 0x9362);
    cpu_sub(z, cpu_rd(z, HL(z)));
    z->a = 0x0A;
    if (z->zf) goto x1755;
    z->a = 0x08;
    if (z->cf) goto x1755;
    z->a = 0x02;
x1755:
    m_1F9C(z);
    z->a = cpu_rd(z, 0x92A0);
    cpu_and(z, 0x03);
    if (!z->zf) return;
    SET_HL(z, 0x9207);
    cpu_wr(z, HL(z), cpu_dec(z, cpu_rd(z, HL(z))));
    if (!z->zf) return;
    m_1F19(z);
x1766:
    SET_DE(z, game_rd16(z->g, 0x9282));
    z->a = cpu_rd(z, DE(z));
    cpu_and(z, 0xC0);
    cpu_cp(z, 0x80);
    if (!z->zf) goto x1772;
    SET_DE(z, DE(z) + 1);
x1772:
    SET_DE(z, DE(z) + 1);
    z->a = cpu_rd(z, DE(z));
    game_wr16(z->g, 0x9282, DE(z));
    cpu_rlca(z);
    cpu_rlca(z);
    cpu_rlca(z);
    cpu_and(z, 0x07);
    SET_HL(z, 0x1786);
    cpu_rst08(z);
    z->a = cpu_rd(z, HL(z));
    SET_HL(z, HL(z) + 1);
    z->h = cpu_rd(z, HL(z));
    z->l = z->a;
    switch (HL(z)) { case 0x1794: goto x1794; case 0x179C: goto x179C; case 0x17A1: goto x17A1; case 0x17A8: goto x17A8; case 0x17AE: goto x17AE; default: game_unported(z->g, HL(z)); return; }
x1794:
    z->a = cpu_rd(z, DE(z));
    cpu_rlca(z);
    cpu_and(z, 0x7E);
    cpu_wr(z, 0x9209, z->a);
    return;
x179C:
    cpu_xor(z, z->a);
    cpu_wr(z, 0x9003, z->a);
    return;
x17A1:
    z->a = cpu_rd(z, DE(z));
    cpu_and(z, 0x1F);
x17A4:
    cpu_wr(z, 0x9207, z->a);
    return;
x17A8:
    z->a = cpu_rd(z, DE(z));
    cpu_and(z, 0x1F);
    z->c = z->a;
    m_0030(z);
    return;
x17AE:
    SET_DE(z, DE(z) + 1);
    z->a = cpu_rd(z, DE(z));
    goto x17A4;
}

void task_1700(Game *g)
{
    Cpu z = { 0 };
    z.g = g;
    t_1700(&z);
}

static void t_17B2(Cpu *z)
{
    z->a = cpu_rd(z, 0x9201);
    z->a = cpu_dec(z, z->a);
    if (!z->zf) return;
    z->a = cpu_rd(z, 0x9203);
    SET_HL(z, 0x17C3);
    cpu_rst08(z);
    z->e = cpu_rd(z, HL(z));
    SET_HL(z, HL(z) + 1);
    z->d = cpu_rd(z, HL(z));
    cpu_ex_de_hl(z);
    switch (HL(z)) { case 0x17E1: goto x17E1; case 0x17F5: goto x17F5; case 0x1808: goto x1808; case 0x1840: goto x1840; case 0x1852: goto x1852; case 0x18AC: goto x18AC; case 0x18D1: goto x18D1; case 0x18D9: goto x18D9; case 0x1940: goto x1940; case 0x1948: goto x1948; case 0x1984: goto x1984; default: game_unported(z->g, HL(z)); return; }
x17E1:
    z->a = cpu_rd(z, 0x92AF);
    cpu_and(z, z->a);
    if (z->zf) goto x17EC;
    z->a = cpu_dec(z, z->a);
    if (z->zf) goto x19A7;
    return;
x17EC:
    m_3214(z);
    z->a = 0x0A;
    cpu_wr(z, 0x92AF, z->a);
    return;
x17F5:
    z->a = cpu_rd(z, 0x92A0);
    cpu_and(z, 0x1F);
    cpu_cp(z, 0x1F);
    if (!z->zf) return;
    z->a = 0x01;
    cpu_wr(z, 0x9005, z->a);
    z->c = 0x02;
    m_0030(z);
    goto x19A7;
x1808:
    m_134C(z);
    SET_HL(z, 0x181F);
    game_wr16(z->g, 0x9282, HL(z));
    z->a = 0x01;
    cpu_wr(z, 0x9003, z->a);
    cpu_wr(z, 0x9015, z->a);
    cpu_wr(z, 0x9025, z->a);
    goto x19A7;
x1840:
    m_0028(z);
    m_1242(z);
    cpu_xor(z, z->a);
    cpu_wr(z, 0x9010, z->a);
    cpu_wr(z, 0x920B, z->a);
    z->a = cpu_inc(z, z->a);
    cpu_wr(z, 0x9002, z->a);
    goto x19A7;
x1852:
    cpu_xor(z, z->a);
    cpu_wr(z, 0x982B, z->a);
    z->a = cpu_inc(z, z->a);
    cpu_wr(z, 0x9AB7, z->a);
    cpu_wr(z, 0x9821, z->a);
    cpu_wr(z, 0x9003, z->a);
    cpu_wr(z, 0x9015, z->a);
    cpu_wr(z, 0x9825, z->a);
    SET_HL(z, 0x1887);
    game_wr16(z->g, 0x9282, HL(z));
    m_01C5(z);
    m_134C(z);
    z->a = 0x01;
    cpu_wr(z, 0x920B, z->a);
    cpu_wr(z, 0x9842, z->a);
    cpu_wr(z, 0x982C, z->a);
    z->a = cpu_inc(z, z->a);
    cpu_wr(z, 0x99C4, z->a);
    cpu_wr(z, 0x99C5, z->a);
    goto x19A7;
x18AC:
    z->a = cpu_rd(z, 0x92AE);
    cpu_and(z, z->a);
    if (z->zf) goto x18BB;
    z->a = cpu_dec(z, z->a);
    if (z->zf) goto x19A7;
    cpu_cp(z, 0x05);
    if (z->zf) goto x18C6;
    return;
x18BB:
    z->a = 0x34;
    cpu_wr(z, 0x9234, z->a);
    z->a = 0x09;
    cpu_wr(z, 0x92AE, z->a);
    return;
x18C6:
    cpu_xor(z, z->a);
    cpu_wr(z, 0x9362, z->a);
    z->c = 0x13;
    m_0030(z);
    z->c = 0x14;
    m_0030(z);
    return;
x18D1:
    z->a = cpu_rd(z, 0x9003);
    cpu_and(z, z->a);
    if (z->zf) goto x19A7;
    return;
x18D9:
    z->b = 0x07;
x18DB:
    m_129E(z);
    if (--z->b) goto x18DB;
    cpu_xor(z, z->a);
    cpu_wr(z, 0x9820, z->a);
    cpu_wr(z, 0x9005, z->a);
    m_134C(z);
    SET_HL(z, 0xFF0D);
    game_wr16(z->g, 0x92C5, HL(z));
    game_wr16(z->g, 0x92C4, HL(z));
    game_wr16(z->g, 0x92C1, HL(z));
    game_wr16(z->g, 0x92C0, HL(z));
    SET_HL(z, 0x1928);
    game_wr16(z->g, 0x9282, HL(z));
    cpu_xor(z, z->a);
    z->b = 0x10;
    SET_HL(z, 0x92CA);
    m_0018(z);
    cpu_wr(z, 0x9827, z->a);
    cpu_wr(z, 0x920B, z->a);
    z->a = cpu_inc(z, z->a);
    cpu_wr(z, 0x982B, z->a);
    cpu_wr(z, 0x9010, z->a);
    cpu_wr(z, 0x900B, z->a);
    cpu_wr(z, 0x9003, z->a);
    z->a = cpu_rd(z, 0x6803);
    cpu_rrca(z);
    cpu_and(z, 0x01);
    cpu_wr(z, 0x9AB7, z->a);
    m_127B(z);
    goto x19A7;
x1940:
    m_0160(z);
    m_003C(z);
    goto x19A7;
x1948:
    SET_HL(z, 0x195C);
    game_wr16(z->g, 0x9280, HL(z));
    cpu_xor(z, z->a);
    cpu_wr(z, 0x9205, z->a);
    cpu_wr(z, 0x92A8, z->a);
    z->a = 0x02;
    cpu_wr(z, 0x92AE, z->a);
    goto x19A7;
x1984:
    z->a = cpu_rd(z, 0x92AE);
    cpu_and(z, z->a);
    if (!z->zf) return;
    z->a = 0x02;
    cpu_wr(z, 0x92AE, z->a);
    z->a = cpu_rd(z, 0x9205);
    cpu_cp(z, 0x05);
    if (z->zf) goto x19A7;
    z->a = cpu_inc(z, z->a);
    cpu_wr(z, 0x9205, z->a);
    cpu_add(z, 0x0D);
    z->c = z->a;
    m_0030(z);
    z->a = cpu_rd(z, 0x9205);
    cpu_cp(z, 0x03);
    if (z->cf) return;
    m_129E(z);
    return;
x19A7:
    SET_HL(z, 0x9203);
    cpu_wr(z, HL(z), cpu_inc(z, cpu_rd(z, HL(z))));
    z->a = cpu_rd(z, HL(z));
    cpu_cp(z, 0x0F);
    if (!z->zf) return;
    cpu_wr(z, HL(z), 0x00);
    return;
}

void task_17B2(Game *g)
{
    Cpu z = { 0 };
    z.g = g;
    t_17B2(&z);
}

static void t_19B2(Cpu *z)
{
    z->a = cpu_rd(z, 0x928E);
    cpu_and(z, z->a);
    if (!z->zf) goto x19D2;
    SET_HL(z, 0x92AD);
    cpu_or(z, cpu_rd(z, HL(z)));
    if (z->zf) goto x19E6;
    cpu_cp(z, 0x04);
    if (!z->zf) goto x19C7;
    z->a = cpu_dec(z, z->a);
    cpu_wr(z, HL(z), z->a);
    cpu_wr(z, 0x9AA9, z->a);
x19C7:
    z->a = cpu_rd(z, 0x9829);
    cpu_add(z, 0x0D);
    z->l = z->a;
    z->h = 0x91;
    cpu_wr(z, HL(z), 0x04);
    return;
x19D2:
    z->c = 0x0A;
    m_0030(z);
    z->a = 0x06;
    cpu_wr(z, 0x92AD, z->a);
    z->a = cpu_inc(z, z->a);
    cpu_wr(z, 0x8B63, z->a);
    cpu_xor(z, z->a);
    cpu_wr(z, 0x928B, z->a);
    cpu_wr(z, 0x928E, z->a);
    return;
x19E6:
    z->a = cpu_rd(z, 0x82D1);
    cpu_cp(z, 0x24);
    if (z->zf) goto x1A16;
    SET_HL(z, 0x9362);
    z->a = cpu_rd(z, 0x9828);
    cpu_and(z, 0x07);
    z->e = z->a;
    z->d = z->h;
    z->a = cpu_rd(z, HL(z));
    cpu_wr(z, DE(z), z->a);
    cpu_wr(z, HL(z), 0x00);
    z->l = cpu_inc(z, z->l);
    z->e = cpu_inc(z, z->e);
    z->a = cpu_rd(z, HL(z));
    cpu_wr(z, DE(z), z->a);
    z->h = 0x9B;
    z->d = z->h;
    cpu_ldd(z);
    cpu_ldi(z);
    z->h = 0x8B;
    z->l = z->e;
    cpu_wr(z, HL(z), 0x07);
    z->l = cpu_dec(z, z->l);
    cpu_wr(z, HL(z), 0x07);
    z->c = 0x0B;
    SET_HL(z, 0x83B1);
    m_13B3(z);
x1A16:
    z->a = cpu_rd(z, 0x9828);
    z->l = z->a;
    cpu_and(z, 0x07);
    z->e = z->a;
    z->h = 0x88;
    z->a = cpu_rd(z, 0x9215);
    z->c = z->a;
    z->a = cpu_rd(z, HL(z));
    cpu_cp(z, 0x09);
    if (!z->zf) goto x1A45;
    z->h = 0x93;
    z->d = z->h;
    z->a = cpu_rd(z, HL(z));
    cpu_wr(z, DE(z), z->a);
    z->l = cpu_inc(z, z->l);
    z->e = cpu_inc(z, z->e);
    z->a = 0x10;
    cpu_bit(z, 0, z->c);
    if (z->zf) goto x1A37;
    cpu_neg(z);
x1A37:
    z->b = z->a;
    cpu_add(z, cpu_rd(z, HL(z)));
    cpu_wr(z, DE(z), z->a);
    cpu_rra(z);
    cpu_xor(z, z->b);
    cpu_rlca(z);
    cpu_and(z, 0x01);
    z->h = 0x9B;
    z->d = z->h;
    cpu_xor(z, cpu_rd(z, HL(z)));
    cpu_wr(z, DE(z), z->a);
    return;
x1A45:
    SET_HL(z, 0x928B);
    z->a = cpu_rd(z, HL(z));
    cpu_and(z, z->a);
    if (!z->zf) goto x1A51;
    z->d = 0x8B;
    z->a = 0x06;
    cpu_wr(z, DE(z), z->a);
x1A51:
    cpu_wr(z, HL(z), cpu_inc(z, cpu_rd(z, HL(z))));
    cpu_cp(z, 0x24);
    if (z->zf) goto x1A70;
    z->b = 0x01;
    cpu_bit(z, 0, z->c);
    if (!z->zf) goto x1A5E;
    z->b = cpu_dec(z, z->b);
    z->b = cpu_dec(z, z->b);
x1A5E:
    z->l = z->e;
    z->l = cpu_inc(z, z->l);
    z->h = 0x93;
    z->a = z->b;
    cpu_add(z, cpu_rd(z, HL(z)));
    cpu_wr(z, HL(z), z->a);
    cpu_rra(z);
    cpu_xor(z, z->b);
    cpu_rlca(z);
    if (!z->cf) return;
    z->h = 0x9B;
    z->a = cpu_rd(z, HL(z));
    cpu_xor(z, 0x01);
    cpu_wr(z, HL(z), z->a);
    return;
x1A70:
    cpu_xor(z, z->a);
    cpu_wr(z, 0x9011, z->a);
    cpu_wr(z, 0x9AA9, z->a);
    z->d = 0x88;
    z->a = cpu_inc(z, z->a);
    cpu_wr(z, DE(z), z->a);
    cpu_wr(z, 0x9828, z->a);
    cpu_wr(z, 0x99B9, z->a);
    z->a = cpu_inc(z, z->a);
    cpu_wr(z, 0x9213, z->a);
    return;
}

void task_19B2(Game *g)
{
    Cpu z = { 0 };
    z.g = g;
    t_19B2(&z);
}

static void t_1A86(Cpu *z)
{
    z->a = cpu_rd(z, 0x99CA);
    z->c = z->a;
    z->a = cpu_rd(z, 0x92A7);
    cpu_cp(z, z->c);
    if (!z->cf) return;
    z->a = cpu_rd(z, 0x9841);
    cpu_and(z, z->a);
    if (!z->zf) goto x1ADB;
    SET_HL(z, 0x8807);
    SET_BC(z, 0x14FF);
    z->a = 0x01;
x1A9D:
    z->l = cpu_inc(z, z->l);
    cpu_cpi(z);
    if (z->zf) goto x1AB1;
    if (--z->b) goto x1A9D;
    SET_HL(z, 0x883F);
    z->b = 0x10;
x1AA9:
    z->l = cpu_inc(z, z->l);
    cpu_cpi(z);
    if (z->zf) goto x1AB1;
    if (--z->b) goto x1AA9;
    return;
x1AB1:
    z->a = 0xC0;
    cpu_wr(z, 0x9841, z->a);
    z->l = cpu_dec(z, z->l);
    z->e = z->l;
    z->d = 0x8B;
    z->e = cpu_inc(z, z->e);
    z->a = cpu_rd(z, DE(z));
    z->e = cpu_dec(z, z->e);
    z->c = z->a;
    z->a = cpu_rd(z, 0x9821);
    z->a = cpu_srl(z, z->a);
    z->a = cpu_srl(z, z->a);
    z->l = z->a;
    z->h = 0x00;
    z->a = 0x03;
    m_1061(z);
    cpu_add(z, 0x04);
    SET_HL(z, 0x982D);
    cpu_wr(z, HL(z), z->e);
    z->l = cpu_inc(z, z->l);
    cpu_wr(z, HL(z), z->c);
    z->l = cpu_inc(z, z->l);
    cpu_wr(z, HL(z), z->a);
    cpu_wr(z, 0x9AB2, z->a);
    return;
x1ADB:
    z->a = cpu_inc(z, z->a);
    if (z->zf) goto x1AFA;
    cpu_wr(z, 0x9841, z->a);
    cpu_ex_af(z);
    SET_HL(z, 0x982D);
    z->e = cpu_rd(z, HL(z));
    z->d = 0x88;
    z->a = cpu_rd(z, DE(z));
    z->a = cpu_dec(z, z->a);
    if (!z->zf) goto x1B5A;
    z->d = 0x8B;
    z->l = cpu_inc(z, z->l);
    cpu_ex_af(z);
    cpu_bit(z, 4, z->a);
    if (z->zf) goto x1AF6;
    z->l = cpu_inc(z, z->l);
x1AF6:
    z->a = cpu_rd(z, HL(z));
    z->e = cpu_inc(z, z->e);
    cpu_wr(z, DE(z), z->a);
    return;
x1AFA:
    z->a = cpu_rd(z, 0x9015);
    cpu_and(z, z->a);
    if (!z->zf) goto x1B06;
    z->a = 0xE0;
    cpu_wr(z, 0x9841, z->a);
    return;
x1B06:
    z->a = cpu_rd(z, 0x982D);
    z->l = z->a;
    z->h = 0x88;
    z->a = cpu_rd(z, HL(z));
    z->a = cpu_dec(z, z->a);
    if (!z->zf) goto x1B5A;
    z->h = 0x92;
    z->a = cpu_rd(z, HL(z));
    cpu_bit(z, 7, z->a);
    if (!z->zf) goto x1B5A;
    z->a = cpu_rd(z, 0x982F);
    cpu_sub(z, 0x04);
    SET_HL(z, 0x1B5F);
    cpu_rst08(z);
    SET_DE(z, 0x99B0);
    z->a = 0x03;
    cpu_wr(z, DE(z), z->a);
    z->e = cpu_inc(z, z->e);
    cpu_ldi(z);
    cpu_ldi(z);
    z->a = cpu_rd(z, 0x982F);
    cpu_sub(z, 0x04);
    cpu_and(z, 0x0F);
    z->c = z->a;
    SET_HL(z, 0x1B65);
    cpu_rst08(z);
    z->e = cpu_rd(z, HL(z));
    SET_HL(z, HL(z) + 1);
    z->d = cpu_rd(z, HL(z));
    z->h = 0x8B;
    z->a = cpu_rd(z, 0x982D);
    z->l = z->a;
    z->a = z->c;
    cpu_rlca(z);
    cpu_rlca(z);
    cpu_rlca(z);
    cpu_add(z, 0x56);
    z->c = cpu_rd(z, HL(z));
    cpu_wr(z, HL(z), z->a);
    z->a = z->c;
    cpu_and(z, 0xF8);
    z->c = z->a;
    z->a = cpu_rd(z, 0x982E);
    cpu_and(z, 0x07);
    cpu_or(z, z->c);
    cpu_wr(z, 0x982E, z->a);
    z->h = 0x88;
    m_1083(z);
x1B5A:
    cpu_xor(z, z->a);
    cpu_wr(z, 0x9004, z->a);
    return;
}

void task_1A86(Game *g)
{
    Cpu z = { 0 };
    z.g = g;
    t_1A86(&z);
}

static void t_1B6B(Cpu *z)
{
    z->a = cpu_rd(z, 0x920B);
    cpu_and(z, z->a);
    if (z->zf) goto x1B7B;
    z->a = cpu_rd(z, 0x9015);
    z->c = z->a;
    z->a = cpu_rd(z, 0x901D);
    cpu_cpl(z);
    cpu_and(z, z->c);
    if (z->zf) return;
x1B7B:
    z->b = 0x04;
    SET_HL(z, 0x92CA);
x1B80:
    z->a = cpu_rd(z, HL(z));
    z->a = cpu_inc(z, z->a);
    if (!z->zf) goto x1B91;
    z->l = cpu_inc(z, z->l);
    z->l = cpu_inc(z, z->l);
    z->l = cpu_inc(z, z->l);
    if (--z->b) goto x1B80;
    z->a = cpu_rd(z, 0x92A0);
    cpu_and(z, 0x0F);
    if (z->zf) goto x1BAE;
    return;
x1B91:
    cpu_wr(z, HL(z), 0xFF);
    z->a = cpu_dec(z, z->a);
    z->d = 0x88;
    z->e = z->a;
    z->e = (uint8_t)(z->e & 0x7F);
    cpu_ex_af(z);
    z->a = cpu_rd(z, DE(z));
    z->a = cpu_dec(z, z->a);
    if (!z->zf) return;
    z->l = cpu_inc(z, z->l);
    z->e = cpu_rd(z, HL(z));
    z->l = cpu_inc(z, z->l);
    z->d = cpu_rd(z, HL(z));
    cpu_ex_af(z);
    z->l = z->a;
    z->h = 0x88;
    m_1079(z);
    z->a = 0x01;
    cpu_wr(z, 0x9AB3, z->a);
    return;
x1BAE:
    SET_HL(z, 0x92C0);
    z->b = 0x03;
x1BB3:
    cpu_wr(z, HL(z), cpu_dec(z, cpu_rd(z, HL(z))));
    if (z->zf) goto x1BBA;
    z->l = cpu_inc(z, z->l);
    if (--z->b) goto x1BB3;
    return;
x1BBA:
    z->a = cpu_rd(z, 0x99C4);
    z->c = z->a;
    z->a = cpu_rd(z, 0x9287);
    cpu_cp(z, z->c);
    if (z->cf) goto x1BC6;
    cpu_wr(z, HL(z), cpu_inc(z, cpu_rd(z, HL(z))));
    return;
x1BC6:
    z->l = (uint8_t)(z->l | 0x04);
    z->a = cpu_rd(z, HL(z));
    z->l = (uint8_t)(z->l & 0xFB);
    cpu_wr(z, HL(z), z->a);
    z->a = z->b;
    z->a = cpu_dec(z, z->a);
    SET_HL(z, 0x1BD7);
    cpu_rst08(z);
    z->a = cpu_rd(z, HL(z));
    SET_HL(z, HL(z) + 1);
    z->h = cpu_rd(z, HL(z));
    z->l = z->a;
    switch (HL(z)) { case 0x1BDD: goto x1BDD; case 0x1BFD: goto x1BFD; case 0x1C07: goto x1C07; default: game_unported(z->g, HL(z)); return; }
x1BDD:
    z->b = 0x14;
    SET_HL(z, 0x8808);
    SET_DE(z, 0x034F);
x1BE5:
    z->a = cpu_rd(z, 0x982D);
    z->c = z->a;
x1BE9:
    z->a = cpu_rd(z, HL(z));
    z->a = cpu_dec(z, z->a);
    if (!z->zf) goto x1BF1;
    z->a = z->c;
    cpu_cp(z, z->l);
    if (!z->zf) goto x1BF6;
x1BF1:
    z->l = cpu_inc(z, z->l);
    z->l = cpu_inc(z, z->l);
    if (--z->b) goto x1BE9;
    return;
x1BF6:
    cpu_wr(z, 0x9AB3, z->a);
    m_1083(z);
    return;
x1BFD:
    z->b = 0x10;
    SET_HL(z, 0x8840);
    SET_DE(z, 0x03A9);
    goto x1BE5;
x1C07:
    z->a = cpu_rd(z, 0x982B);
    cpu_and(z, z->a);
    if (!z->zf) goto x1C36;
    SET_HL(z, 0x982C);
    cpu_wr(z, HL(z), cpu_inc(z, cpu_rd(z, HL(z))));
    cpu_bit(z, 0, cpu_rd(z, HL(z)));
    if (!z->zf) goto x1C36;
    z->ix = (uint16_t)((z->ix & 0xFF00) | (uint8_t)(0x02));
    z->iy = (uint16_t)(0x0454);
    SET_DE(z, 0x8830);
    z->b = 0x04;
x1C21:
    z->a = cpu_rd(z, DE(z));
    z->a = cpu_dec(z, z->a);
    if (z->zf) goto x1C2A;
    z->e = cpu_inc(z, z->e);
    z->e = cpu_inc(z, z->e);
    if (--z->b) goto x1C21;
    return;
x1C2A:
    z->a = 0x01;
    cpu_wr(z, 0x982B, z->a);
    z->a = z->e;
    cpu_wr(z, 0x9828, z->a);
    goto x1CB4;
x1C36:
    SET_HL(z, 0x1D32);
    z->d = 0x88;
    SET_BC(z, 0x0600);
x1C3E:
    z->e = cpu_rd(z, HL(z));
    SET_HL(z, HL(z) + 1);
    z->a = cpu_rd(z, 0x982D);
    cpu_cp(z, z->e);
    if (z->zf) goto x1C4A;
    z->a = cpu_rd(z, DE(z));
    z->a = cpu_dec(z, z->a);
    cpu_sub(z, 0x01);
x1C4A:
    z->c = cpu_rl(z, z->c);
    if (--z->b) goto x1C3E;
    z->ix = (uint16_t)((z->ix & 0xFF00) | (uint8_t)(0x00));
    z->b = 0x04;
    z->ix = (uint16_t)((z->ix & 0x00FF) | (uint8_t)(z->c) << 8);
x1C55:
    z->a = z->c;
    cpu_and(z, 0x07);
    cpu_cp(z, 0x04);
    if (z->zf) goto x1C61;
    cpu_cp(z, 0x03);
    if (!z->cf && m_1C93(z, 0x1C61)) return;
x1C61:
    z->c = cpu_rr(z, z->c);
    if (--z->b) goto x1C55;
    z->ix = (uint16_t)((z->ix & 0xFF00) | (uint8_t)(cpu_inc(z, (uint8_t)z->ix)));
    z->c = (uint8_t)(z->ix >> 8);
    z->b = 0x04;
x1C6B:
    z->a = z->c;
    cpu_and(z, 0x07);
    if (!z->zf && m_1C93(z, 0x1C71)) return;
    z->c = cpu_rr(z, z->c);
    if (--z->b) goto x1C6B;
    z->ix = (uint16_t)((z->ix & 0xFF00) | (uint8_t)(cpu_inc(z, (uint8_t)z->ix)));
    SET_DE(z, 0x8830);
    z->b = 0x04;
x1C7C:
    z->a = cpu_rd(z, DE(z));
    z->a = cpu_dec(z, z->a);
    if (z->zf) goto x1CA6;
    z->e = cpu_inc(z, z->e);
    z->e = cpu_inc(z, z->e);
    if (--z->b) goto x1C7C;
    SET_HL(z, 0x8800);
    z->b = 0x04;
x1C89:
    z->a = cpu_rd(z, HL(z));
    z->a = cpu_dec(z, z->a);
    if (z->zf) goto x1D2B;
    z->l = cpu_inc(z, z->l);
    z->l = cpu_inc(z, z->l);
    if (--z->b) goto x1C89;
    return;
x1CA6:
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
    if (!z->zf) return;
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
    return;
x1D2B:
    SET_DE(z, 0x0444);
    m_1083(z);
    return;
}

void task_1B6B(Game *g)
{
    Cpu z = { 0 };
    z.g = g;
    t_1B6B(&z);
}

static void t_1D38(Cpu *z)
{
    SET_HL(z, 0x99B4);
    z->a = cpu_rd(z, HL(z));
    cpu_and(z, 0x7F);
    cpu_sub(z, 0x7E);
    if (z->zf) goto x1D78;
    z->c = cpu_rd(z, HL(z));
    cpu_wr(z, HL(z), cpu_inc(z, cpu_rd(z, HL(z))));
    z->a = cpu_rd(z, 0x9215);
    z->c = cpu_rlc(z, z->c);
    cpu_xor(z, z->c);
    cpu_rrca(z);
    z->a = 0x01;
    if (z->cf) goto x1D51;
    cpu_neg(z);
x1D51:
    z->c = z->a;
    SET_HL(z, 0x9814);
    z->b = 0x06;
x1D57:
    z->a = cpu_rd(z, HL(z));
    cpu_add(z, z->c);
    cpu_wr(z, HL(z), z->a);
    cpu_rra(z);
    cpu_xor(z, z->c);
    z->l = cpu_inc(z, z->l);
    cpu_rlca(z);
    if (!z->cf) goto x1D64;
    z->a = cpu_rd(z, HL(z));
    cpu_xor(z, 0x01);
    cpu_wr(z, HL(z), z->a);
x1D64:
    z->l = cpu_inc(z, z->l);
    if (--z->b) goto x1D57;
    z->a = cpu_rd(z, 0x92A0);
    cpu_and(z, 0xFC);
    z->a = cpu_inc(z, z->a);
    cpu_push(z, AF(z));
    m_23E0(z);
    cpu_set_af(z, cpu_pop(z));
    cpu_add(z, 0x02);
    m_23E0(z);
    return;
x1D78:
    cpu_wr(z, 0x900E, z->a);
    return;
}

void task_1D38(Game *g)
{
    Cpu z = { 0 };
    z.g = g;
    t_1D38(&z);
}

static void t_1D7C(Cpu *z)
{
    z->a = cpu_rd(z, 0x9215);
    z->b = z->a;
    SET_HL(z, 0x99B9);
    z->a = cpu_rd(z, HL(z));
    z->l = cpu_inc(z, z->l);
    cpu_and(z, z->a);
    if (z->zf) goto x1DAE;
    z->a = cpu_rd(z, HL(z));
    cpu_and(z, z->a);
    z->a = 0xFD;
    if (!z->zf) goto x1DA1;
    z->l = cpu_inc(z, z->l);
    z->a = cpu_rd(z, HL(z));
    z->l = cpu_inc(z, z->l);
    cpu_cp(z, cpu_rd(z, HL(z)));
    if (z->zf) goto x1D95;
    cpu_wr(z, HL(z), cpu_inc(z, cpu_rd(z, HL(z))));
x1D95:
    z->a = cpu_rd(z, HL(z));
    z->l = cpu_inc(z, z->l);
    cpu_add(z, cpu_rd(z, HL(z)));
    z->c = z->a;
    cpu_and(z, 0x3F);
    cpu_wr(z, HL(z), z->a);
    z->a = z->c;
    cpu_rlca(z);
    cpu_rlca(z);
    cpu_and(z, 0x03);
x1DA1:
    cpu_bit(z, 0, z->b);
    if (!z->zf) goto x1DA7;
    cpu_neg(z);
x1DA7:
    z->a = cpu_dec(z, z->a);
    cpu_and(z, 0x07);
x1DAA:
    cpu_wr(z, 0x99BE, z->a);
    return;
x1DAE:
    cpu_xor(z, z->a);
    cpu_wr(z, HL(z), z->a);
    z->l = cpu_inc(z, z->l);
    z->l = cpu_inc(z, z->l);
    cpu_wr(z, HL(z), z->a);
    z->l = cpu_inc(z, z->l);
    cpu_wr(z, HL(z), z->a);
    z->a = 0x07;
    goto x1DAA;
}

void task_1D7C(Game *g)
{
    Cpu z = { 0 };
    z.g = g;
    t_1D7C(&z);
}

static void t_1DB9(Cpu *z)
{
    SET_HL(z, 0x9200);
    z->b = 0x30;
x1DBE:
    cpu_bit(z, 7, cpu_rd(z, HL(z)));
    if (!z->zf) goto x1DC7;
    z->l = cpu_inc(z, z->l);
x1DC3:
    z->l = cpu_inc(z, z->l);
    if (--z->b) goto x1DBE;
    return;
x1DC7:
    cpu_wr(z, HL(z), (uint8_t)(cpu_rd(z, HL(z)) & 0x7F));
    z->h = 0x88;
    cpu_wr(z, HL(z), 0x04);
    z->l = cpu_inc(z, z->l);
    cpu_wr(z, HL(z), 0x40);
    z->h = 0x8B;
    cpu_wr(z, HL(z), 0x0A);
    z->h = 0x92;
    goto x1DC3;
}

void task_1DB9(Game *g)
{
    Cpu z = { 0 };
    z.g = g;
    t_1DB9(&z);
}

static void t_1DD8(Cpu *z)
{
    z->a = cpu_rd(z, 0x92A2);
    cpu_and(z, 0x01);
    if (!z->zf) return;
    SET_HL(z, 0x92AC);
    z->b = 0x04;
x1DE3:
    z->a = cpu_rd(z, HL(z));
    cpu_and(z, z->a);
    if (z->zf) goto x1DE8;
    cpu_wr(z, HL(z), cpu_dec(z, cpu_rd(z, HL(z))));
x1DE8:
    z->l = cpu_inc(z, z->l);
    if (--z->b) goto x1DE3;
    return;
}

void task_1DD8(Game *g)
{
    Cpu z = { 0 };
    z.g = g;
    t_1DD8(&z);
}

static void t_1DEC(Cpu *z)
{
    z->a = cpu_rd(z, 0x92A0);
    cpu_and(z, 0x03);
    if (!z->zf) return;
    SET_HL(z, 0x920F);
    z->a = cpu_rd(z, HL(z));
    z->e = z->a;
    z->d = 0xFF;
    cpu_bit(z, 7, z->a);
    if (!z->zf) goto x1E02;
    z->d = cpu_inc(z, z->d);
    z->d = cpu_inc(z, z->d);
    cpu_wr(z, HL(z), cpu_inc(z, cpu_rd(z, HL(z))));
    goto x1E03;
x1E02:
    cpu_wr(z, HL(z), cpu_dec(z, cpu_rd(z, HL(z))));
x1E03:
    cpu_cp(z, 0x1F);
    if (!z->zf) goto x1E09;
    cpu_wr(z, HL(z), (uint8_t)(cpu_rd(z, HL(z)) | 0x80));
x1E09:
    cpu_cp(z, 0x81);
    if (!z->zf) goto x1E0F;
    cpu_wr(z, HL(z), (uint8_t)(cpu_rd(z, HL(z)) & 0x7F));
x1E0F:
    z->c = cpu_rd(z, HL(z));
    cpu_and(z, 0x07);
    z->a = z->d;
    cpu_wr(z, 0x9211, z->a);
    z->a = z->e;
    if (!z->zf) goto x1E29;
    SET_HL(z, 0x1E6A);
    z->a = z->c;
    cpu_and(z, 0x18);
    cpu_rst08(z);
    z->a = z->e;
    SET_DE(z, 0x9920);
    SET_BC(z, 0x0010);
    cpu_ldir(z);
x1E29:
    SET_HL(z, 0x9215);
    cpu_rlca(z);
    cpu_xor(z, cpu_rd(z, HL(z)));
    cpu_rrca(z);
    SET_HL(z, 0x9920);
    SET_DE(z, 0x9900);
    if (!z->cf) goto x1E3C;
    SET_BC(z, 0x01FF);
    goto x1E3F;
x1E3C:
    SET_BC(z, 0xFF01);
x1E3F:
    z->ix = (uint16_t)((z->ix & 0xFF00) | (uint8_t)(0x05));
    m_1E49(z);
    z->b = z->c;
    z->ix = (uint16_t)((z->ix & 0xFF00) | (uint8_t)(0x0B));
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

void task_1DEC(Game *g)
{
    Cpu z = { 0 };
    z.g = g;
    t_1DEC(&z);
}

static void t_1EAA(Cpu *z)
{
    z->a = cpu_rd(z, 0x92A0);
    cpu_and(z, 0x01);
    cpu_add(z, 0x02);
    z->b = z->a;
    z->a = cpu_rd(z, 0x9215);
    cpu_and(z, z->a);
    z->a = z->b;
    if (z->zf) goto x1EBB;
    cpu_neg(z);
x1EBB:
    z->ix = (uint16_t)((z->ix & 0x00FF) | (uint8_t)(z->a) << 8);
    z->l = 0x68;
    SET_DE(z, 0x92B0);
    z->ix = (uint16_t)((z->ix & 0xFF00) | (uint8_t)(0x08));
x1EC5:
    z->h = 0x8B;
    z->a = cpu_rd(z, HL(z));
    cpu_cp(z, 0x30);
    if (!z->zf) goto x1F05;
    z->h = 0x93;
    z->a = cpu_rd(z, HL(z));
    cpu_and(z, z->a);
    if (z->zf) goto x1F05;
    cpu_ex_de_hl(z);
    z->b = cpu_rd(z, HL(z));
    z->a = z->b;
    cpu_and(z, 0x7E);
    z->l = cpu_inc(z, z->l);
    cpu_add(z, cpu_rd(z, HL(z)));
    z->c = z->a;
    cpu_and(z, 0x1F);
    cpu_wr(z, HL(z), z->a);
    z->l = cpu_inc(z, z->l);
    z->a = z->c;
    cpu_rlca(z);
    cpu_rlca(z);
    cpu_rlca(z);
    cpu_and(z, 0x07);
    cpu_bit(z, 7, z->b);
    if (z->zf) goto x1EEA;
    cpu_neg(z);
x1EEA:
    cpu_ex_de_hl(z);
    cpu_add(z, cpu_rd(z, HL(z)));
    cpu_wr(z, HL(z), z->a);
    z->l = cpu_inc(z, z->l);
    z->a = cpu_rd(z, HL(z));
    cpu_add(z, (uint8_t)(z->ix >> 8));
    cpu_wr(z, HL(z), z->a);
    cpu_rra(z);
    cpu_xor(z, (uint8_t)(z->ix >> 8));
    cpu_rlca(z);
    if (!z->cf) goto x1EFF;
    z->h = 0x9B;
    cpu_wr(z, HL(z), cpu_rrc(z, cpu_rd(z, HL(z))));
    cpu_ccf(z);
    cpu_wr(z, HL(z), cpu_rl(z, cpu_rd(z, HL(z))));
x1EFF:
    z->l = cpu_inc(z, z->l);
    z->ix = (uint16_t)((z->ix & 0xFF00) | (uint8_t)(cpu_dec(z, (uint8_t)z->ix)));
    if (!z->zf) goto x1EC5;
    return;
x1F05:
    z->h = 0x88;
    cpu_wr(z, HL(z), 0x80);
    z->l = cpu_inc(z, z->l);
    z->e = cpu_inc(z, z->e);
    z->e = cpu_inc(z, z->e);
    goto x1EFF;
}

void task_1EAA(Game *g)
{
    Cpu z = { 0 };
    z.g = g;
    t_1EAA(&z);
}

static void t_1F0E(Cpu *z)
{
    z->a = cpu_rd(z, 0x9215);
    cpu_add(z, 0xB6);
    z->l = z->a;
    z->h = 0x99;
    cpu_bit(z, 4, cpu_rd(z, HL(z)));
    if (!z->zf) return;
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

void task_1F0E(Game *g)
{
    Cpu z = { 0 };
    z.g = g;
    t_1F0E(&z);
}

static void t_1F8F(Cpu *z)
{
    z->a = cpu_rd(z, 0x9827);
    z->e = z->a;
    z->a = cpu_rd(z, 0x9215);
    cpu_add(z, 0xB6);
    z->l = z->a;
    z->h = 0x99;
    z->a = cpu_rd(z, HL(z));
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

void task_1F8F(Game *g)
{
    Cpu z = { 0 };
    z.g = g;
    t_1F8F(&z);
}

static void t_2000(Cpu *z)
{
    z->a = cpu_rd(z, 0x9828);
    z->l = z->a;
    z->h = 0x88;
    z->a = cpu_rd(z, HL(z));
    cpu_and(z, z->a);
    if (!z->zf) goto x20BF;
    z->a = cpu_rd(z, 0x928B);
    cpu_and(z, z->a);
    if (z->zf) goto x20C7;
    z->a = cpu_dec(z, z->a);
    if (z->zf) goto x20D1;
    z->h = 0x93;
    z->a = cpu_rd(z, HL(z));
    cpu_cp(z, 0x80);
    if (z->zf) goto x2026;
    if (!z->sf) goto x2023;
    cpu_wr(z, HL(z), cpu_inc(z, cpu_rd(z, HL(z))));
    goto x205E;
x2023:
    cpu_wr(z, HL(z), cpu_dec(z, cpu_rd(z, HL(z))));
    goto x205E;
x2026:
    z->l = cpu_inc(z, z->l);
    z->a = cpu_rd(z, 0x9215);
    cpu_and(z, z->a);
    if (!z->zf) goto x204C;
    z->a = cpu_rd(z, HL(z));
    cpu_cp(z, 0x29);
    if (!z->zf) goto x2041;
    z->h = 0x9B;
    z->a = cpu_rd(z, HL(z));
    z->h = 0x93;
    z->a = cpu_dec(z, z->a);
    if (!z->zf) goto x2041;
x203A:
    z->a = 0x03;
    cpu_wr(z, 0x928B, z->a);
    goto x205E;
x2041:
    cpu_wr(z, HL(z), cpu_inc(z, cpu_rd(z, HL(z))));
    if (!z->zf) goto x205E;
x2044:
    z->h = 0x9B;
    z->a = cpu_rd(z, HL(z));
    cpu_xor(z, 0x01);
    cpu_wr(z, HL(z), z->a);
    goto x205E;
x204C:
    z->a = cpu_rd(z, HL(z));
    cpu_cp(z, 0x37);
    if (!z->zf) goto x2059;
    z->h = 0x9B;
    z->a = cpu_rd(z, HL(z));
    z->h = 0x93;
    cpu_and(z, z->a);
    if (z->zf) goto x203A;
x2059:
    cpu_wr(z, HL(z), cpu_dec(z, cpu_rd(z, HL(z))));
    z->a = cpu_rd(z, HL(z));
    z->a = cpu_inc(z, z->a);
    if (z->zf) goto x2044;
x205E:
    SET_HL(z, 0x8B62);
    z->a = cpu_rd(z, HL(z));
    cpu_sub(z, 0x06);
    z->c = z->a;
    z->h = 0x93;
    if (!z->zf) goto x2075;
    z->a = cpu_rd(z, HL(z));
    cpu_cp(z, 0x71);
    if (z->zf) goto x2075;
    if (!z->sf) goto x2073;
    cpu_wr(z, HL(z), cpu_inc(z, cpu_rd(z, HL(z))));
    return;
x2073:
    cpu_wr(z, HL(z), cpu_dec(z, cpu_rd(z, HL(z))));
    return;
x2075:
    z->a = cpu_rd(z, 0x928B);
    cpu_cp(z, 0x03);
    if (!z->zf) return;
    z->a = cpu_rd(z, 0x9828);
    z->l = z->a;
    cpu_wr(z, HL(z), 0x00);
    z->l = cpu_inc(z, z->l);
    z->c = cpu_dec(z, z->c);
    z->c = cpu_inc(z, z->c);
    if (z->zf) goto x208F;
    SET_DE(z, 0x9363);
    cpu_xor(z, z->a);
    cpu_wr(z, 0x982B, z->a);
    goto x2097;
x208F:
    z->a = 0x01;
    cpu_wr(z, 0x9827, z->a);
    SET_DE(z, 0x9361);
x2097:
    z->a = cpu_rd(z, HL(z));
    cpu_wr(z, DE(z), z->a);
    z->h = 0x9B;
    z->d = z->h;
    z->a = cpu_rd(z, HL(z));
    cpu_wr(z, DE(z), z->a);
    z->l = cpu_dec(z, z->l);
    z->h = 0x88;
    cpu_wr(z, HL(z), 0x80);
    z->h = 0x8B;
    z->l = z->e;
    z->l = cpu_dec(z, z->l);
    cpu_wr(z, HL(z), 0x06);
    z->l = cpu_inc(z, z->l);
    cpu_wr(z, HL(z), 0x09);
    z->l = cpu_dec(z, z->l);
    z->h = 0x93;
    cpu_wr(z, HL(z), 0x80);
    z->a = 0x01;
    cpu_wr(z, 0x9014, z->a);
    cpu_wr(z, 0x9015, z->a);
    cpu_wr(z, 0x9025, z->a);
    cpu_wr(z, 0x99B9, z->a);
x20BF:
    cpu_xor(z, z->a);
    cpu_wr(z, 0x901D, z->a);
    cpu_wr(z, 0x9AB1, z->a);
    return;
x20C7:
    z->a = cpu_inc(z, z->a);
    cpu_wr(z, 0x928B, z->a);
    z->a = 0x02;
    cpu_wr(z, 0x92AD, z->a);
    return;
x20D1:
    z->h = 0x9B;
    z->a = cpu_rd(z, 0x92AD);
    z->e = z->a;
    z->a = cpu_rd(z, 0x9287);
    cpu_or(z, z->e);
    cpu_wr(z, 0x928D, z->a);
    m_2188(z);
    z->b = cpu_dec(z, z->b);
    if (!z->zf) return;
    cpu_wr(z, 0x9014, z->a);
    cpu_wr(z, 0x9015, z->a);
    cpu_wr(z, 0x9025, z->a);
    z->a = 0x02;
    cpu_wr(z, 0x928B, z->a);
    return;
}

void task_2000(Game *g)
{
    Cpu z = { 0 };
    z.g = g;
    t_2000(&z);
}

static void t_20F2(Cpu *z)
{
    SET_HL(z, 0x9B62);
    m_2188(z);
    cpu_bit(z, 0, z->b);
    if (!z->zf) goto x2151;
    z->a = cpu_rd(z, 0x928B);
    cpu_bit(z, 7, z->a);
    if (!z->zf) goto x215D;
    z->a = cpu_rd(z, 0x928D);
    cpu_and(z, z->a);
    if (z->zf) return;
    z->h = 0x93;
    z->a = cpu_rd(z, 0x9828);
    z->e = z->a;
    z->d = z->h;
    z->a = cpu_rd(z, DE(z));
    cpu_cp(z, cpu_rd(z, HL(z)));
    if (z->zf) goto x211A;
    if (!z->sf) goto x2119;
    cpu_wr(z, HL(z), cpu_dec(z, cpu_rd(z, HL(z))));
    goto x211A;
x2119:
    cpu_wr(z, HL(z), cpu_inc(z, cpu_rd(z, HL(z))));
x211A:
    z->l = cpu_inc(z, z->l);
    z->a = cpu_rd(z, 0x9215);
    cpu_and(z, z->a);
    if (z->zf) goto x212C;
    cpu_wr(z, HL(z), cpu_inc(z, cpu_rd(z, HL(z))));
    z->a = cpu_rd(z, HL(z));
    cpu_cp(z, 0x7A);
    if (z->zf) goto x214C;
    cpu_cp(z, 0x80);
    if (z->zf) goto x2141;
    return;
x212C:
    cpu_wr(z, HL(z), cpu_dec(z, cpu_rd(z, HL(z))));
    z->a = cpu_rd(z, HL(z));
    z->a = cpu_inc(z, z->a);
    if (!z->zf) goto x2139;
    z->h = 0x9B;
    z->a = cpu_rd(z, HL(z));
    cpu_xor(z, 0x01);
    cpu_wr(z, HL(z), z->a);
    z->h = 0x93;
x2139:
    z->a = cpu_rd(z, HL(z));
    cpu_cp(z, 0xE6);
    if (z->zf) goto x214C;
    cpu_cp(z, 0xE0);
    if (!z->zf) return;
x2141:
    cpu_xor(z, z->a);
    cpu_wr(z, 0x928D, z->a);
    z->a = cpu_inc(z, z->a);
    z->a = 0x07;
    cpu_wr(z, 0x8B63, z->a);
    return;
x214C:
    cpu_xor(z, z->a);
    cpu_wr(z, 0x9015, z->a);
    return;
x2151:
    z->a = cpu_rd(z, 0x9015);
    cpu_and(z, z->a);
    if (!z->zf) goto x215D;
    z->a = cpu_inc(z, z->a);
    cpu_wr(z, 0x920D, z->a);
    goto x217F;
x215D:
    z->h = 0x93;
    z->l = cpu_inc(z, z->l);
    z->a = cpu_rd(z, 0x9215);
    cpu_and(z, z->a);
    if (z->zf) goto x216D;
    z->a = cpu_rd(z, HL(z));
    cpu_cp(z, 0x37);
    if (z->zf) goto x217D;
    cpu_wr(z, HL(z), cpu_dec(z, cpu_rd(z, HL(z))));
    return;
x216D:
    z->a = cpu_rd(z, HL(z));
    cpu_cp(z, 0x29);
    if (z->zf) goto x217D;
    cpu_wr(z, HL(z), cpu_inc(z, cpu_rd(z, HL(z))));
    if (!z->zf) return;
    z->h = 0x9B;
    z->a = cpu_rd(z, HL(z));
    cpu_xor(z, 0x01);
    cpu_wr(z, HL(z), z->a);
    z->h = 0x93;
    return;
x217D:
    z->b = cpu_dec(z, z->b);
    if (!z->zf) return;
x217F:
    cpu_xor(z, z->a);
    cpu_wr(z, 0x901C, z->a);
    z->a = cpu_inc(z, z->a);
    cpu_wr(z, 0x9025, z->a);
    return;
}

void task_20F2(Game *g)
{
    Cpu z = { 0 };
    z.g = g;
    t_20F2(&z);
}

static void t_21CB(Cpu *z)
{
    SET_HL(z, 0x9828);
    z->e = cpu_rd(z, HL(z));
    z->d = 0x88;
    z->a = cpu_rd(z, DE(z));
    cpu_cp(z, 0x09);
    if (!z->zf) goto x221A;
    z->l = cpu_inc(z, z->l);
    z->a = cpu_rd(z, HL(z));
    z->ix = (uint16_t)((z->ix & 0xFF00) | (uint8_t)(z->a));
    z->ix = (uint16_t)((z->ix & 0x00FF) | (uint8_t)(0x91) << 8);
    z->a = cpu_ix(z, 10);
    cpu_and(z, z->a);
    if (!z->zf) return;
    z->a = 0x0C;
    cpu_bit(z, 0, cpu_ix(z, 5));
    if (z->zf) goto x21EC;
    cpu_neg(z);
x21EC:
    cpu_set_ix(z, 12, z->a);
    z->a = cpu_ix(z, 5);
    cpu_rrca(z);
    z->a = cpu_ix(z, 4);
    cpu_rra(z);
    cpu_sub(z, 0x78);
    cpu_cp(z, 0x10);
    if (!z->cf) return;
    z->a = cpu_rd(z, 0x99C6);
    cpu_wr(z, 0x982A, z->a);
    cpu_xor(z, z->a);
    cpu_set_ix(z, 12, z->a);
    cpu_wr(z, 0x9019, z->a);
    cpu_wr(z, 0x928B, z->a);
    cpu_wr(z, 0x920D, z->a);
    z->a = cpu_inc(z, z->a);
    cpu_wr(z, 0x9018, z->a);
    cpu_wr(z, 0x928C, z->a);
    cpu_wr(z, 0x928D, z->a);
    return;
x221A:
    cpu_xor(z, z->a);
    cpu_wr(z, 0x9019, z->a);
    cpu_wr(z, 0x982B, z->a);
    return;
}

void task_21CB(Game *g)
{
    Cpu z = { 0 };
    z.g = g;
    t_21CB(&z);
}

static void t_2222(Cpu *z)
{
    z->a = cpu_rd(z, 0x92A0);
    z->c = z->a;
    cpu_and(z, 0x03);
    if (!z->zf) goto x2257;
    z->a = cpu_rd(z, 0x928A);
    cpu_neg(z);
    cpu_sub(z, 0x18);
    z->h = 0x21;
    cpu_rlca(z);
    z->h = cpu_rl(z, z->h);
    cpu_rlca(z);
    z->h = cpu_rl(z, z->h);
    cpu_and(z, 0xE0);
    cpu_add(z, 0x15);
    z->l = z->a;
    z->a = z->c;
    cpu_rrca(z);
    cpu_rrca(z);
    cpu_and(z, 0x03);
    if (!z->zf) goto x2246;
    z->a = cpu_inc(z, z->a);
x2246:
    cpu_add(z, 0x17);
    SET_DE(z, 0x0016);
    z->c = 0x06;
x224D:
    z->b = 0x0A;
x224F:
    cpu_wr(z, HL(z), z->a);
    z->l = cpu_inc(z, z->l);
    if (--z->b) goto x224F;
    SET_HL(z, cpu_add16(z, HL(z), DE(z)));
    z->c = cpu_dec(z, z->c);
    if (!z->zf) goto x224D;
x2257:
    SET_HL(z, 0x928B);
    cpu_bit(z, 7, cpu_rd(z, HL(z)));
    if (!z->zf) goto x226A;
    z->a = cpu_rd(z, 0x9828);
    z->e = z->a;
    z->d = 0x88;
    z->a = cpu_rd(z, DE(z));
    cpu_cp(z, 0x09);
    if (!z->zf) goto x2327;
x226A:
    SET_HL(z, 0x928C);
    cpu_wr(z, HL(z), cpu_dec(z, cpu_rd(z, HL(z))));
    if (!z->zf) goto x233D;
    z->a = cpu_rd(z, 0x982A);
    cpu_wr(z, HL(z), z->a);
    SET_HL(z, 0x928B);
    cpu_bit(z, 7, cpu_rd(z, HL(z)));
    if (!z->zf) goto x22AB;
    cpu_wr(z, 0x9AA5, z->a);
    z->a = cpu_rd(z, 0x9829);
    cpu_add(z, 0x0D);
    z->e = z->a;
    z->d = 0x91;
    z->a = 0xFF;
    cpu_wr(z, DE(z), z->a);
    cpu_wr(z, HL(z), cpu_inc(z, cpu_rd(z, HL(z))));
    z->a = cpu_rd(z, HL(z));
    cpu_and(z, 0x0F);
    cpu_cp(z, 0x0B);
    if (z->zf) goto x22D2;
    cpu_bit(z, 6, cpu_rd(z, HL(z)));
    if (!z->zf) goto x22C1;
    cpu_push(z, AF(z));
    z->c = z->a;
    cpu_rlca(z);
    cpu_add(z, z->c);
    SET_HL(z, 0x239B);
    cpu_rst08(z);
    cpu_set_af(z, cpu_pop(z));
    m_238A(z);
    z->b = 0x06;
x22A4:
    z->a = cpu_rd(z, HL(z));
    cpu_wr(z, DE(z), z->a);
    SET_HL(z, HL(z) + 1);
    m_0020(z);
    if (--z->b) goto x22A4;
    return;
x22AB:
    cpu_wr(z, HL(z), cpu_inc(z, cpu_rd(z, HL(z))));
    z->a = cpu_rd(z, HL(z));
    cpu_and(z, 0x0F);
    cpu_cp(z, 0x0B);
    if (!z->zf) goto x22C5;
    cpu_xor(z, z->a);
    cpu_wr(z, 0x9018, z->a);
    cpu_wr(z, 0x9AA5, z->a);
    cpu_wr(z, 0x9AA6, z->a);
    cpu_wr(z, 0x982B, z->a);
    return;
x22C1:
    cpu_neg(z);
    cpu_add(z, 0x0B);
x22C5:
    m_238A(z);
    z->b = 0x06;
    z->c = 0x24;
x22CC:
    z->a = z->c;
    cpu_wr(z, DE(z), z->a);
    m_0020(z);
    if (--z->b) goto x22CC;
    return;
x22D2:
    cpu_bit(z, 6, cpu_rd(z, HL(z)));
    if (z->zf) goto x231C;
    z->a = cpu_rd(z, 0x920D);
    cpu_and(z, z->a);
    if (z->zf) goto x22E3;
    cpu_bit(z, 5, cpu_rd(z, HL(z)));
    if (!z->zf) goto x22E3;
    cpu_wr(z, HL(z), 0x68);
    return;
x22E3:
    cpu_xor(z, z->a);
    cpu_wr(z, 0x9018, z->a);
    cpu_wr(z, 0x9AA5, z->a);
    cpu_wr(z, 0x9AA6, z->a);
    z->a = cpu_rd(z, 0x920D);
    cpu_and(z, z->a);
    z->a = cpu_rd(z, 0x9829);
    if (!z->zf) goto x2305;
    cpu_add(z, 0x0D);
    z->e = z->a;
    z->d = 0x91;
    cpu_xor(z, z->a);
    cpu_wr(z, 0x982B, z->a);
    z->a = cpu_inc(z, z->a);
    cpu_wr(z, 0x9828, z->a);
    cpu_wr(z, DE(z), z->a);
    return;
x2305:
    cpu_add(z, 0x08);
    z->l = z->a;
    z->h = 0x91;
    SET_DE(z, 0x046B);
    cpu_wr(z, HL(z), z->e);
    z->l = cpu_inc(z, z->l);
    cpu_wr(z, HL(z), z->d);
    cpu_xor(z, z->a);
    cpu_wr(z, 0x99BA, z->a);
    z->a = cpu_inc(z, z->a);
    cpu_wr(z, 0x9011, z->a);
    cpu_wr(z, 0x928E, z->a);
    return;
x231C:
    z->a = 0x40;
    cpu_wr(z, 0x928C, z->a);
    z->a = 0x40;
    cpu_wr(z, 0x928B, z->a);
    return;
x2327:
    z->a = 0x03;
    cpu_wr(z, 0x982A, z->a);
    cpu_wr(z, HL(z), 0x80);
    cpu_xor(z, z->a);
    cpu_wr(z, 0x928D, z->a);
    cpu_wr(z, 0x99BA, z->a);
    z->a = cpu_inc(z, z->a);
    cpu_wr(z, 0x928C, z->a);
    cpu_wr(z, 0x9014, z->a);
    return;
x233D:
    z->a = cpu_rd(z, 0x928B);
    cpu_cp(z, 0x40);
    if (!z->zf) return;
    z->a = cpu_rd(z, 0x9215);
    z->c = z->a;
    z->a = cpu_rd(z, 0x9362);
    cpu_bit(z, 0, z->c);
    if (z->zf) goto x2352;
    cpu_add(z, 0x0E);
    cpu_neg(z);
x2352:
    z->b = z->a;
    z->a = cpu_rd(z, 0x928A);
    cpu_sub(z, z->b);
    cpu_add(z, 0x1B);
    cpu_cp(z, 0x36);
    if (!z->cf) return;
    z->a = cpu_rd(z, 0x9201);
    z->a = cpu_dec(z, z->a);
    if (z->zf) goto x236D;
    z->a = cpu_rd(z, 0x9014);
    z->c = z->a;
    z->a = cpu_rd(z, 0x9213);
    cpu_xor(z, 0x01);
    cpu_and(z, z->c);
    if (z->zf) return;
x236D:
    cpu_xor(z, z->a);
    cpu_wr(z, 0x9014, z->a);
    cpu_wr(z, 0x9AA5, z->a);
    cpu_wr(z, 0x9025, z->a);
    cpu_wr(z, 0x9213, z->a);
    z->a = cpu_inc(z, z->a);
    cpu_wr(z, 0x901C, z->a);
    cpu_wr(z, 0x9AA6, z->a);
    cpu_wr(z, 0x99BA, z->a);
    z->a = 0x0A;
    cpu_wr(z, 0x982A, z->a);
    return;
}

void task_2222(Game *g)
{
    Cpu z = { 0 };
    z.g = g;
    t_2222(&z);
}

static void t_23DD(Cpu *z)
{
    z->a = cpu_rd(z, 0x92A0);
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

void task_23DD(Game *g)
{
    Cpu z = { 0 };
    z.g = g;
    t_23DD(&z);
}

static void t_2916(Cpu *z)
{
    SET_HL(z, game_rd16(z->g, 0x9822));
    z->a = cpu_rd(z, HL(z));
    cpu_cp(z, 0x7F);
    if (z->zf) goto x2A29;
    cpu_cp(z, 0x7E);
    if (!z->zf) goto x2953;
    z->a = cpu_rd(z, 0x9842);
    cpu_and(z, z->a);
    if (z->zf) return;
    z->a = cpu_rd(z, 0x9287);
    cpu_and(z, z->a);
    if (!z->zf) goto x294D;
    z->a = cpu_rd(z, 0x9825);
    z->b = z->a;
    cpu_and(z, z->a);
    if (!z->zf) goto x2944;
    z->a = cpu_rd(z, 0x92AC);
    cpu_cp(z, 0x01);
    if (!z->zf) goto x2942;
    z->a = 0x08;
    cpu_wr(z, 0x92A8, z->a);
    return;
x2942:
    cpu_and(z, z->a);
    if (!z->zf) return;
x2944:
    SET_HL(z, HL(z) + 1);
    game_wr16(z->g, 0x9822, HL(z));
    SET_HL(z, 0x9826);
    cpu_wr(z, HL(z), cpu_inc(z, cpu_rd(z, HL(z))));
    return;
x294D:
    z->a = 0x02;
    cpu_wr(z, 0x92AC, z->a);
    return;
x2953:
    z->c = z->a;
    cpu_bit(z, 7, z->a);
    if (!z->zf) goto x295E;
    z->a = cpu_rd(z, 0x92A0);
    cpu_and(z, 0x07);
    if (!z->zf) return;
x295E:
    z->c = cpu_sla(z, z->c);
    z->b = 0x0C;
    SET_DE(z, 0x0014);
    z->ix = (uint16_t)(0x9100);
x2969:
    cpu_bit(z, 0, cpu_ix(z, 19));
    if (z->zf) goto x2974;
    z->ix = (uint16_t)(cpu_add16(z, z->ix, DE(z)));
    if (--z->b) goto x2969;
    return;
x2974:
    SET_HL(z, HL(z) + 1);
    z->a = cpu_rd(z, HL(z));
    z->b = z->a;
    cpu_and(z, 0x78);
    cpu_cp(z, 0x78);
    z->a = z->b;
    if (!z->zf) goto x2980;
    z->a = (uint8_t)(z->a & 0xBF);
x2980:
    cpu_set_ix(z, 16, z->a);
    SET_HL(z, HL(z) + 1);
    game_wr16(z->g, 0x9822, HL(z));
    z->h = 0x88;
    z->l = z->a;
    cpu_wr(z, HL(z), 0x07);
    z->l = cpu_inc(z, z->l);
    z->e = (uint8_t)z->ix;
    cpu_wr(z, HL(z), z->e);
    z->h = 0x93;
    cpu_and(z, 0x38);
    cpu_cp(z, 0x38);
    if (z->zf) goto x29B3;
    z->l = cpu_dec(z, z->l);
    z->h = 0x8B;
    z->a = cpu_rd(z, HL(z));
    z->d = z->a;
    cpu_and(z, 0x78);
    cpu_wr(z, HL(z), z->a);
    z->l = cpu_inc(z, z->l);
    z->a = z->d;
    cpu_and(z, 0x07);
    cpu_bit(z, 7, z->d);
    cpu_wr(z, HL(z), z->a);
    z->a = 0x00;
    if (z->zf) goto x29AE;
    z->a = cpu_rd(z, 0x92E3);
x29AE:
    cpu_set_ix(z, 15, z->a);
    goto x29D1;
x29B3:
    SET_DE(z, 0x0210);
    cpu_bit(z, 6, z->b);
    if (!z->zf) goto x29C7;
    SET_DE(z, 0x0318);
    z->a = cpu_rd(z, 0x9826);
    cpu_cp(z, 0x02);
    if (!z->zf) goto x29C7;
    SET_DE(z, 0x0008);
x29C7:
    z->h = 0x8B;
    cpu_wr(z, HL(z), z->d);
    z->l = cpu_dec(z, z->l);
    cpu_wr(z, HL(z), z->e);
    z->l = cpu_inc(z, z->l);
    cpu_set_ix(z, 15, 0x00);
x29D1:
    z->d = z->c;
    z->c = (uint8_t)(z->c & 0x7F);
    z->b = 0x08;
    cpu_bit(z, 1, z->c);
    if (z->zf) goto x29DC;
    z->b = 0x44;
x29DC:
    cpu_set_ix(z, 14, z->b);
    z->b = 0x00;
    SET_HL(z, 0x2A3C);
    SET_HL(z, cpu_add16(z, HL(z), BC(z)));
    z->a = cpu_rd(z, HL(z));
    SET_HL(z, HL(z) + 1);
    cpu_set_ix(z, 8, z->a);
    cpu_xor(z, z->a);
    cpu_rld(z);
    z->b = z->a;
    z->a = cpu_rd(z, HL(z));
    cpu_and(z, 0x1F);
    cpu_set_ix(z, 9, z->a);
    z->a = z->b;
    cpu_and(z, 0x0E);
    z->b = z->a;
    cpu_rlca(z);
    cpu_add(z, z->b);
    SET_HL(z, 0x2A6C);
    cpu_rst10(z);
    cpu_bit(z, 7, z->d);
    if (z->zf) goto x2A05;
    SET_HL(z, HL(z) + 1);
    SET_HL(z, HL(z) + 1);
    SET_HL(z, HL(z) + 1);
x2A05:
    z->a = cpu_rd(z, HL(z));
    SET_HL(z, HL(z) + 1);
    cpu_set_ix(z, 1, z->a);
    z->a = cpu_rd(z, HL(z));
    SET_HL(z, HL(z) + 1);
    cpu_set_ix(z, 3, z->a);
    z->a = cpu_rd(z, HL(z));
    SET_HL(z, HL(z) + 1);
    cpu_set_ix(z, 5, z->a);
    cpu_xor(z, z->a);
    cpu_set_ix(z, 0, z->a);
    cpu_set_ix(z, 2, z->a);
    cpu_set_ix(z, 4, z->a);
    z->a = cpu_inc(z, z->a);
    cpu_set_ix(z, 13, z->a);
    cpu_or(z, z->d);
    cpu_and(z, 0x81);
    cpu_set_ix(z, 19, z->a);
    return;
x2A29:
    z->a = cpu_rd(z, 0x9287);
    cpu_and(z, z->a);
    if (!z->zf) return;
    cpu_wr(z, 0x9008, z->a);
    z->a = cpu_inc(z, z->a);
    cpu_wr(z, 0x9004, z->a);
    cpu_wr(z, 0x9010, z->a);
    cpu_wr(z, 0x9824, z->a);
    return;
}

void task_2916(Game *g)
{
    Cpu z = { 0 };
    z.g = g;
    t_2916(&z);
}

static void t_2A90(Cpu *z)
{
    z->a = cpu_rd(z, 0x92A0);
    z->a = cpu_dec(z, z->a);
    cpu_and(z, 0x03);
    if (!z->zf) return;
    z->a = cpu_rd(z, 0x92A7);
    z->b = z->a;
    z->a = cpu_rd(z, 0x9008);
    cpu_or(z, z->b);
    if (z->zf) goto x2AE9;
    z->a = cpu_rd(z, 0x920F);
    cpu_and(z, z->a);
    z->c = 0x01;
    if (z->zf) goto x2AAB;
    z->c = cpu_dec(z, z->c);
    z->c = cpu_dec(z, z->c);
x2AAB:
    z->l = 0x00;
    z->b = 0x0A;
x2AAF:
    z->h = 0x99;
    z->a = cpu_rd(z, HL(z));
    cpu_add(z, z->c);
    cpu_wr(z, HL(z), z->a);
    z->h = 0x98;
    z->a = cpu_rd(z, HL(z));
    cpu_add(z, z->c);
    cpu_wr(z, HL(z), z->a);
    z->l = cpu_inc(z, z->l);
    z->l = cpu_inc(z, z->l);
    if (--z->b) goto x2AAF;
    z->a = cpu_rd(z, 0x9824);
    cpu_and(z, z->a);
    z->a = cpu_rd(z, 0x9900);
    if (z->zf) goto x2AC9;
    cpu_and(z, z->a);
    if (z->zf) goto x2ADA;
x2AC9:
    cpu_cp(z, 0x20);
    if (!z->zf) goto x2AD3;
    z->a = 0x01;
    cpu_wr(z, 0x920F, z->a);
    return;
x2AD3:
    cpu_sub(z, 0xE0);
    if (!z->zf) return;
    cpu_wr(z, 0x920F, z->a);
    return;
x2ADA:
    cpu_xor(z, z->a);
    cpu_wr(z, 0x920F, z->a);
    cpu_wr(z, 0x900A, z->a);
    z->a = cpu_inc(z, z->a);
    cpu_wr(z, 0x9AA0, z->a);
    cpu_wr(z, 0x9009, z->a);
    return;
x2AE9:
    cpu_wr(z, 0x900A, z->a);
    return;
}

void task_2A90(Game *g)
{
    Cpu z = { 0 };
    z.g = g;
    t_2A90(&z);
}
