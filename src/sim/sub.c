#include "sim/sub.h"

#include "sim/cpu.h"

/* The sprites' registers ($xx00-$xx3F the bank sub writes, $xx80 the one shown):
   $8800+n the object's state, $8B00 its code and colour, $9300 its
   position, $9B00 its flags and position's ninth bits. A motion slot:
     +0,+1 X (8.8)    +2,+3 Y (8.8)    +4,+5 its heading (10 bits)
     +6,+7 where it is heading for      +8,+9 its script
     +10,+11 its speed on odd and even frames   +12 its turn a frame
     +13 frames left of the script's step       +14,+15 its bombs' timer and mask
     +16 its sprite      +17,+18 its offset from the formation
     +19 flags: bit 0 in use, 5 the target X only, 6 joins the formation, 7 mirrored */

#define M(hh, ll) cpu_rd(z, (uint16_t)((hh) << 8 | (ll)))
#define MW(hh, ll, v) cpu_wr(z, (uint16_t)((hh) << 8 | (ll)), (uint8_t)(v))
#define MHL cpu_rd(z, HL(z))
#define MHL_W(v) cpu_wr(z, HL(z), (uint8_t)(v))
#define IX(n) cpu_ix(z, n)
#define SET_IX(n, v) cpu_set_ix(z, n, (uint8_t)(v))
#define IXL ((uint8_t)z->ix)
#define IXH ((uint8_t)(z->ix >> 8))

static Cpu cpu(Game *g)
{
    Cpu z = { 0 };
    z.g = g;
    z.rom = GAME_SUB;
    return z;
}

/* ---- arithmetic ----------------------------------------------------------- */

/* $0E5F: the heading from (H, L) to (D, E): H its octant, L the angle in it */
static void heading(Cpu *z)
{
    uint8_t b = z->b, c = z->c, d = z->d, e = z->e;
    z->a = z->e;
    cpu_sub(z, z->l);
    z->b = 0;
    if (z->cf) {
        z->b |= 1;
        cpu_neg(z);
    }
    z->c = z->a;
    z->a = z->d;
    cpu_sub(z, z->h);
    if (z->cf) {
        z->d = z->a;
        z->a = (uint8_t)((z->b ^ 1) | 2);
        z->b = z->a;
        z->a = z->d;
        cpu_neg(z);
    }
    cpu_cp(z, z->c);
    uint8_t a = z->a, cy = z->cf;
    cpu_rla(z);
    cpu_xor(z, z->b);
    cpu_rra(z);
    z->cf = !z->cf;
    z->b = cpu_rl(z, z->b);
    z->a = a;
    if (cy) { /* the larger in A, the smaller in C */
        z->d = z->c;
        z->c = z->a;
        z->a = z->d;
    }
    z->h = z->c;
    z->l = 0;
    cpu_divide(z); /* $0EAE */
    if ((z->h ^ z->b) & 1) z->l = (uint8_t)~z->l;
    z->h = z->b;
    z->b = b, z->c = c, z->d = d, z->e = e;
}

/* ---- hits ----------------------------------------------------------------- */

/* $07C2: what the ship or a shot (its sprite E) hit, the sprite L. 1 when
   the scan ends ($0815: the target was a shot-down ship's capture beam) */
static int hit(Cpu *z, int zf2)
{
    z->d = 0x93;
    z->a = 0;
    cpu_wr(z, DE(z), 0);
    z->d = 0x9B;
    cpu_wr(z, DE(z), 0);
    z->l++;
    z->h = 0x8B;
    z->a = MHL;
    z->c = z->a;
    if (!z->a) { /* $08CA */
        z->a = 1;
        MHL_W(1);
        cpu_wr(z, 0x9AA4, 1);
        z->l--;
        return 0;
    }
    z->l--;
    if (z->a == 0x0B) {
        MW(0x93, z->l, 0);
        z->h = 0x88;
        MHL_W(0x80);
        return 1;
    }
    if (!zf2) goto x081E;
x07DB:
    z->h = 0x92;
    MHL_W(0x81);
x07DF:
    z->a = cpu_rd(z, 0x9828);
    cpu_sub(z, z->l);
    if (z->zf) {
        cpu_wr(z, 0x982B, 0);
        z->a = 1;
        cpu_wr(z, 0x9828, 1);
    }
    {
        uint8_t h = z->h, l = z->l;
        z->a = z->c == 7 ? 6 : (uint8_t)((z->c - 1) & 3);
        SET_HL(z, 0x9AA1);
        cpu_rst10(z);
        MHL_W(1);
        z->a = z->c;
        if (z->c == 7) cpu_wr(z, 0x982B, 0);
        SET_HL(z, 0x9290);
        cpu_rst10(z);
        MHL_W(MHL + 1);
        if (!zf2) MHL_W(MHL + 1);
        z->h = h, z->l = l;
    }
    return 0;
x081E:
    z->h = 0x88;
    {
        uint8_t l = z->l;
        z->l++;
        z->a = (uint8_t)(MHL + 0x13);
        z->l = z->a;
        z->h = 0x91;
        MHL_W(0);
        cpu_wr(z, 0x9288, cpu_rd(z, 0x9288) + 1);
        uint8_t left = (uint8_t)(cpu_rd(z, 0x92A8) - 1);
        cpu_wr(z, 0x92A8, left);
        z->h = 0x88, z->l = l;
        if (!left) {
            z->h = 0x92;
            MHL_W(cpu_rd(z, 0x9285));
            z->h = cpu_rd(z, 0x9284);
            cpu_wr(z, 0x929F, cpu_rd(z, 0x929F) + z->h);
            goto x07DF;
        }
    }
    if (z->c == 7) {
        z->d = 0xB8;
        goto x08B0;
    }
    if (cpu_rd(z, 0x982D) == z->l || (z->l & 0x38) == 0x38) {
        uint8_t n = (uint8_t)(cpu_rd(z, 0x99B0) - 1);
        cpu_wr(z, 0x99B0, n);
        if (n) goto x07DB;
        z->d = cpu_rd(z, 0x99B2);
        z->a = cpu_rd(z, 0x99B1);
        goto x08AA;
    }
    if (z->c != 1) goto x07DB;
    {
        uint8_t d = z->d, e = z->e;
        z->e = z->l & 7;
        z->d = 0x88;
        if (cpu_rd(z, DE(z)) == 9) { /* $0873 */
            uint8_t h = z->h, l = z->l;
            z->h = z->d, z->l = z->e;
            z->l++;
            z->e = (uint8_t)(MHL + 0x13);
            z->d = 0x91;
            cpu_wr(z, DE(z), 0);
            z->h = 0x8B;
            MHL_W(9);
            z->l--;
            cpu_wr(z, 0x9828, z->l);
            z->h = 0x88;
            MHL_W(0);
            cpu_wr(z, 0x928B, 0);
            cpu_wr(z, 0x901D, 1);
            cpu_wr(z, 0x928D, 1);
            cpu_wr(z, 0x9AB1, 1);
            z->h = h, z->l = l;
        }
        z->d = d, z->e = e;
    }
    cpu_wr(z, 0x92AD, 6);
    {
        uint8_t h = z->h, l = z->l;
        z->a = z->l & 7;
        SET_HL(z, 0x9830);
        cpu_rst10(z);
        z->a = MHL;
        z->l++;
        z->d = MHL;
        z->h = h, z->l = l;
    }
x08AA:
    cpu_wr(z, 0x929F, cpu_rd(z, 0x929F) + z->a);
x08B0:
    z->h = 0x92;
    MHL_W(z->d);
    goto x07DF;
}

/* $06B7 (the ship, at IXL, IXH) and $076A (a shot): B sprites from L
   against it; after a hit the scan goes on as a shot's */
static void scan(Cpu *z, int ship)
{
    int zf2 = 0; /* the zero flag set aside (EX AF,AF') for hit() */
    for (;; z->l += 2, z->b--) {
        if (ship) {
            uint8_t s = cpu_rd(z, (uint16_t)(0x9200 | z->l)) | M(0x88, z->l);
            z->h = 0x88;
            if (s & 0x80 || (MHL & 0xFE) == 4) goto next;
            z->h = 0x93;
            z->a = MHL;
            if (!z->a) goto next;
            cpu_sub(z, IXL);
            cpu_sub(z, 7);
            cpu_add(z, 0x0D);
            if (!z->cf) goto next;
            z->l++;
            z->a = MHL;
            z->h = 0x9B;
            z->c = MHL;
            z->l--;
            z->c = cpu_rrc(z, z->c);
            cpu_rra(z);
            cpu_sub(z, IXH);
            cpu_sub(z, 4);
            cpu_add(z, 7);
            if (!z->cf) goto next;
            z->a = 1;
            cpu_wr(z, 0x99BF, 1);
            zf2 = 0;
            ship = 0;
        } else {
            uint8_t s = cpu_rd(z, (uint16_t)(0x9200 | z->l)) | M(0x88, z->l);
            z->h = 0x88;
            if (s & 0x80) goto next;
            z->c = MHL;
            if ((z->c & 0xFE) == 4) goto next;
            z->l++;
            z->h = 0x9B;
            z->d = MHL;
            z->h = 0x93;
            z->a = MHL;
            z->d = cpu_rrc(z, z->d);
            cpu_rra(z);
            z->l--;
            cpu_sub(z, IXH);
            cpu_sub(z, 3);
            cpu_add(z, 6);
            if (!z->cf) goto next;
            zf2 = !((z->c - 1) & 0xFE);
            z->a = MHL;
            if (cpu_rd(z, 0x9827)) { /* the ship's double: a wider target */
                cpu_sub(z, IXL);
                cpu_sub(z, 0x14);
                cpu_add(z, 0x0B);
                if (!z->cf) {
                    cpu_add(z, 4);
                    if (z->cf) goto next;
                    cpu_add(z, 0x0B);
                    if (!z->cf) goto next;
                }
            } else {
                cpu_sub(z, IXL);
                cpu_sub(z, 6);
                cpu_add(z, 0x0B);
                if (!z->cf) goto next;
            }
            /* $07B9: hits counted */
            uint16_t n = (uint16_t)(game_rd16(z->g, 0x9844) + 1);
            game_wr16(z->g, 0x9844, n);
            z->h = (uint8_t)(n >> 8);
        }
        if (hit(z, zf2)) return;
    next:
        if (z->b == 1) return;
    }
}

/* ---- the shots ------------------------------------------------------------ */

/* $0704: the shot at HL ($9364 or $9366), its direction at DE */
static void shot(Cpu *z)
{
    if (!MHL) return;
    z->b = cpu_rd(z, DE(z));
    uint8_t a = 6, other = z->b & 7;
    if (z->b & 0x80) {
        a = other;
        other = 6;
    }
    z->a = a;
    if (z->b & 0x40) cpu_neg(z);
    cpu_add(z, MHL);
    MHL_W(z->a);
    if (z->a >= 0xF0) goto gone;
    z->ix = (uint16_t)((z->ix & 0xFF00) | z->a);
    z->l++;
    z->a = other;
    if (z->b & 0x20) cpu_neg(z);
    z->c = z->a;
    cpu_add(z, MHL);
    MHL_W(z->a);
    cpu_rra(z);
    cpu_xor(z, z->c);
    z->h = 0x9B;
    cpu_rlca(z);
    if (z->cf) MHL_W(MHL ^ 1);
    z->c = MHL;
    z->h = 0x93;
    z->a = MHL;
    z->c = cpu_rrc(z, z->c);
    cpu_rra(z);
    z->ix = (uint16_t)((z->ix & 0x00FF) | z->a << 8);
    if (z->a < 0x14 || z->a >= 0x9C) {
        z->l--;
        goto gone;
    }
    z->e = z->l;
    if (cpu_rd(z, 0x901D)) {
        SET_HL(z, 0x9308);
        z->b = 0x2C;
    } else {
        SET_HL(z, 0x9300);
        z->b = 0x30;
    }
    scan(z, 0);
    return;
gone:
    z->h = 0x93;
    MHL_W(0);
    z->h = 0x9B;
    MHL_W(0);
}

void sub_shots(Game *g)
{
    Cpu z0 = cpu(g), *z = &z0;
    SET_DE(z, 0x92A4);
    SET_HL(z, 0x9364);
    shot(z);
    SET_DE(z, 0x92A5);
    SET_HL(z, 0x9366);
    shot(z);
}

/* ---- the ship ------------------------------------------------------------- */

/* $0681: the enemies against the ship at HL; $99BF set when one hit it */
static void ship_scan(Cpu *z)
{
    cpu_wr(z, 0x99BF, 0);
    z->h = 0x88;
    z->a = MHL;
    z->h = 0x93;
    if (z->a == 8) return;
    z->a = MHL;
    z->ix = (uint16_t)((z->ix & 0xFF00) | z->a);
    z->l++;
    z->b = MHL;
    z->h = 0x9B;
    z->a = MHL;
    cpu_rrca(z);
    z->b = cpu_rr(z, z->b);
    z->ix = (uint16_t)((z->ix & 0x00FF) | z->b << 8);
    z->l--;
    z->e = z->l;
    if (cpu_rd(z, 0x9008)) {
        z->l = 0x38;
        z->b = 4;
    } else {
        z->l = 0;
        z->b = 0x30;
    }
    scan(z, 1);
    z->l = 0x68;
    z->b = 8;
    scan(z, 1);
}

/* $064F: the ship at L ($93xx, H $93) shot down: A its Y less 8 */
static void ship_explodes(Cpu *z)
{
    cpu_sub(z, 8);
    z->l &= 0x7F;
    MHL_W(z->a);
    z->l++;
    MHL_W(MHL - 8);
    z->h = 0x8B;
    MHL_W(0x0B);
    z->l--;
    MHL_W(0x20);
    z->h = 0x88;
    MHL_W(8);
    z->l++;
    MHL_W(0x0F);
    z->l--;
    z->h = 0x9B;
    MHL_W(0x0C);
    cpu_wr(z, 0x9827, 0);
    cpu_wr(z, 0x9AB9, cpu_rd(z, 0x9201) - 1);
    if (cpu_rd(z, 0x9217)) return;
    cpu_wr(z, 0x9213, 1);
}

/* $0649: the ship at E */
static void ship_lost(Cpu *z)
{
    uint8_t e = z->e;
    z->d = z->h, z->e = z->l;
    z->h = 0x93;
    z->l = (uint8_t)(e | 0x80);
    z->a = MHL;
    ship_explodes(z);
}

void sub_ship(Game *g)
{
    Cpu z0 = cpu(g), *z = &z0;
    z->a = cpu_rd(z, 0x9014);
    if (!z->a) return;
    cpu_wr(z, 0x9217, z->a);
    if (cpu_rd(z, 0x9827)) {
        SET_HL(z, 0x9360);
        if (MHL) {
            ship_scan(z);
            if (cpu_rd(z, 0x99BF)) {
                ship_lost(z);
                cpu_wr(z, 0x982B, 0);
            }
        }
    }
    SET_HL(z, 0x9362);
    if (!MHL) return;
    ship_scan(z);
    if (!cpu_rd(z, 0x99BF)) return;
    if (cpu_rd(z, 0x9827)) { /* the right ship of two: the left one takes its place */
        cpu_wr(z, 0x982B, 0);
        cpu_wr(z, 0x9362, cpu_rd(z, 0x9360));
        z->a = cpu_rd(z, 0x93E2);
        SET_HL(z, 0x93E0);
        ship_explodes(z);
        return;
    }
    cpu_wr(z, 0x9014, 0);
    cpu_wr(z, 0x9015, 0);
    cpu_wr(z, 0x9025, 0);
    cpu_wr(z, 0x99B9, 0);
    cpu_wr(z, 0x9217, 0);
    ship_lost(z);
}

/* ---- the motion ----------------------------------------------------------- */

static void ldir(Cpu *z, int n)
{
    for (; n; n--) {
        cpu_wr(z, DE(z), MHL);
        SET_HL(z, HL(z) + 1);
        SET_DE(z, DE(z) + 1);
    }
    SET_BC(z, 0);
}

void sub_motion(Game *g, uint8_t d, uint8_t e)
{
    Cpu z0 = cpu(g), *z = &z0;
    uint16_t ptr = 0;
    z->d = d;
    z->e = e;
    z->ix = 0x9100;
    cpu_wr(z, 0x9289, 12);
    z->a = cpu_rd(z, 0x9286);
    cpu_wr(z, 0x9286, 0);
    cpu_wr(z, 0x9287, z->a);
    SET_HL(z, 0x9287);
x08E4:
    if (!(IX(19) & 1)) goto x0DFF;
    SET_HL(z, 0x9286);
    MHL_W(MHL + 1);
    z->l = IX(16);
    z->h = 0x88;
    z->a = MHL;
    if (z->a != 3 && z->a != 9 && z->a != 7) goto x0E4D;
    SET_IX(13, cpu_dec(z, IX(13)));
    if (!z->zf) goto x0C05;
    z->l = IX(8);
    z->h = IX(9);
x090E:
    z->a = MHL;
    if (z->a < 0xEF) goto x0BDC;
    /* a command: $FF - its byte through $0920 */
    {
        int command = (uint8_t)~z->a;
        static const uint16_t handlers[17] = { 0x0E4D, 0x0B16, 0x0B46, 0x0B4E, 0x0AA0, 0x0BD1, 0x0B5F, 0x0B87, 0x0B98,
                                               0x0BA8, 0x0942, 0x0A53, 0x0A01, 0x097B, 0x0968, 0x0955, 0x094E };
        ptr = HL(z);
        z->a = (uint8_t)~z->a;
        SET_HL(z, 0x0920);
        cpu_rst08(z);
        z->a = (uint8_t)handlers[command];
        SET_HL(z, ptr);
        switch (command) {
        case 0: goto x0E4D;          /* $FF: the end */
        case 1: goto x0B16;          /* $FE: a step towards the ship */
        case 2: goto x0B46;          /* $FD: a jump */
        case 3: goto x0B4E;          /* $FC: head for an X */
        case 4: goto x0AA0;          /* $FB: home to the formation */
        case 5: goto x0BD1;          /* $FA */
        case 6: goto x0B5F;          /* $F9 */
        case 7: goto x0B87;          /* $F8 */
        case 8: goto x0B98;          /* $F7 */
        case 9: goto x0BA8;          /* $F6 */
        case 10: goto x0942;         /* $F5 */
        case 11: goto x0A53;         /* $F4 */
        case 12: goto x0A01;         /* $F3 */
        case 13: goto x097B;         /* $F2 */
        case 14: goto x0968;         /* $F1 */
        case 15: z->a = cpu_rd(z, 0x99C8); goto x0959; /* $F0 */
        default: z->a = cpu_rd(z, 0x99C9); goto x0959; /* $EF */
        }
    }

x0942:
    z->e = IX(16);
    z->d = 0x88;
    cpu_wr(z, DE(z), 3);
    SET_HL(z, HL(z) + 1);
    goto x090E;

x0959:
    if (z->a) {
        SET_HL(z, HL(z) + 1);
        z->a = MHL;
        SET_HL(z, HL(z) + 1);
        z->h = MHL;
        z->l = z->a;
        goto x0B8C;
    }
    SET_HL(z, HL(z) + 2);
    goto x0B8B;

x0968:
    z->e = IX(16);
    z->d = 0x01;
    z->a = cpu_rd(z, DE(z));
    z->e = z->a;
    z->d = 0x99;
    z->e++;
    z->a = cpu_rd(z, DE(z));
    cpu_add(z, 0x20);
    SET_IX(1, z->a);
    goto x0B8B;

x097B:
    ptr = HL(z);
    z->e = IX(16);
    SET_HL(z, 0x8838);
    for (z->b = 4;; z->l += 2) {
        z->a = MHL;
        cpu_rlca(z);
        if (z->cf) break;
        if (!--z->b) goto x09FA;
    }
    {
        uint8_t saved;
        z->h = 0x8B;
        z->d = z->h;
        MHL_W(cpu_rd(z, DE(z)));
        z->l++;
        z->e++;
        MHL_W(cpu_rd(z, DE(z)));
        z->l--;
        saved = z->l;
        SET_HL(z, 0x91EF);
        SET_DE(z, 0xFFEC);
        for (z->b = 12;; z->b--) {
            if (!(MHL & 1)) break;
            SET_HL(z, HL(z) + DE(z));
            if (z->b == 1) goto x09FA;
        }
        SET_HL(z, HL(z) + DE(z) + 1);
        z->a = IX(0);
        SET_DE(z, z->ix);
        { uint16_t t = HL(z); SET_HL(z, DE(z)); SET_DE(z, t); }
        z->iy = DE(z);
        ldir(z, 6);
        z->c = 6;
        SET_HL(z, HL(z) + BC(z));
        { uint16_t t = HL(z); SET_HL(z, DE(z)); SET_DE(z, t); }
        SET_HL(z, HL(z) + DE(z));
        { uint16_t t = HL(z); SET_HL(z, DE(z)); SET_DE(z, t); }
        ldir(z, 4); /* (to an address in ROM: lost) */
        cpu_wr(z, (uint16_t)(z->iy + 19), IX(19));
        SET_HL(z, ptr + 1);
        cpu_wr(z, (uint16_t)(z->iy + 8), MHL);
        SET_HL(z, HL(z) + 1);
        cpu_wr(z, (uint16_t)(z->iy + 9), MHL);
        cpu_wr(z, (uint16_t)(z->iy + 10), 1);
        cpu_wr(z, (uint16_t)(z->iy + 11), 2);
        cpu_wr(z, (uint16_t)(z->iy + 13), 1);
        z->a = saved;
        cpu_wr(z, (uint16_t)(z->iy + 16), z->a);
        z->e = z->a;
        z->d = 0x88;
        cpu_wr(z, DE(z), 9);
        z->e++;
        cpu_wr(z, DE(z), (uint8_t)z->iy);
        SET_HL(z, HL(z) + 1);
        goto x090E;
    }
x09FA:
    SET_HL(z, ptr + 3);
    goto x090E;

x0A01:
    ptr = HL(z);
    { uint16_t t = HL(z); SET_HL(z, DE(z)); SET_DE(z, t); }
    z->c = cpu_rd(z, 0x9215);
    z->a = cpu_rd(z, 0x9362);
    if (z->a < 0x1E) z->a = 0x1E;
    if (z->a >= 0xD1) z->a = 0xD1;
    if (z->c & 1) {
        cpu_add(z, 0x0E);
        cpu_neg(z);
    }
    z->a = cpu_srl(z, z->a);
    cpu_sub(z, IX(3));
    cpu_rra(z);
    if (IX(19) & 0x80) cpu_neg(z);
    cpu_add(z, 0x18);
    if (z->sf) z->a = 0;
    if (z->a >= 0x30) z->a = 0x2F;
    z->h = z->a;
    z->a = 6;
    cpu_divide(z); /* $0EAE */
    z->a = (uint8_t)(z->h + 1);
    { uint16_t t = HL(z); SET_HL(z, DE(z)); SET_DE(z, t); }
    cpu_rst10(z);
    SET_IX(13, MHL);
    SET_HL(z, ptr);
    z->a = 9;
    cpu_rst10(z);
    goto x0BFF;

x0A53:
    ptr = HL(z);
    z->c = cpu_rd(z, 0x9215);
    z->a = (uint8_t)(((cpu_rd(z, 0x9362) + 3) & 0xF8) + 1);
    if (z->a < 0x29) z->a = 0x29;
    if (z->a >= 0xCA) z->a = 0xC9;
    if (z->c & 1) {
        cpu_add(z, 0x0D);
        z->a = (uint8_t)~z->a;
    }
    cpu_wr(z, 0x928A, z->a);
    z->e = cpu_srl(z, z->a);
    z->d = 0x48;
    z->h = IX(1);
    z->l = IX(3);
    heading(z);
    z->h = cpu_srl(z, z->h);
    z->l = cpu_rr(z, z->l);
    SET_IX(4, z->l);
    SET_IX(5, z->h);
    cpu_wr(z, 0x928B, 0);
    cpu_wr(z, 0x9019, 1);
    cpu_wr(z, 0x9829, IXL);
    SET_HL(z, ptr + 1);
    goto x090E;

x0AA0:
    ptr = HL(z);
    {
        uint8_t pd, pe;
        z->l = IX(16);
        z->h = 0x88;
        MHL_W(9);
        z->h = 0x01;
        z->c = MHL;
        z->l++;
        z->l = MHL;
        z->h = 0x99;
        z->b = MHL;
        z->l++;
        z->e = MHL;
        z->l = z->c;
        z->c = MHL;
        z->l++;
        z->d = MHL;
        z->e = cpu_srl(z, z->e);
        pd = z->d, pe = z->e;
        SET_IX(17, z->b);
        SET_IX(18, z->c);
        if (cpu_rd(z, 0x9215)) {
            z->b = (uint8_t)-z->b;
            z->c = (uint8_t)-z->c;
        }
        SET_HL(z, IX(0) | IX(1) << 8);
        z->d = z->c;
        z->e = 0;
        z->d = cpu_sra(z, z->d);
        z->e = cpu_rr(z, z->e);
        SET_HL(z, cpu_add16(z, HL(z), DE(z)));
        SET_IX(0, z->l);
        SET_IX(1, z->h);
        z->e = z->h;
        SET_HL(z, IX(2) | IX(3) << 8);
        z->c = 0;
        z->b = cpu_sra(z, z->b);
        z->c = cpu_rr(z, z->c);
        SET_HL(z, cpu_sbc16(z, HL(z), BC(z)));
        SET_IX(2, z->l);
        SET_IX(3, z->h);
        z->l = z->h;
        z->h = z->e;
        z->c = z->d;
        z->d = pd, z->e = pe;
        heading(z);
        z->h = cpu_srl(z, z->h);
        z->l = cpu_rr(z, z->l);
        SET_IX(4, z->l);
        SET_IX(5, z->h);
        SET_IX(6, z->d);
        SET_IX(7, z->e);
        SET_IX(19, IX(19) | 0x40);
    }
    SET_HL(z, ptr + 1);
    goto x090E;

x0B16:
    ptr = HL(z);
    { uint16_t t = HL(z); SET_HL(z, DE(z)); SET_DE(z, t); }
    z->a = cpu_rd(z, 0x9215);
    cpu_rrca(z);
    z->b = IX(19);
    cpu_xor(z, z->b);
    cpu_rlca(z);
    z->a = cpu_rd(z, 0x93E2);
    if (!z->a) z->a = 0x80;
    if (!z->cf) {
        cpu_neg(z);
        cpu_add(z, 0xF2);
    }
    cpu_add(z, 0x0E);
    z->h = z->a;
    z->a = 0x1E;
    cpu_divide(z); /* $0EAE */
    z->a = z->h;
    { uint16_t t = HL(z); SET_HL(z, DE(z)); SET_DE(z, t); }
    cpu_rst10(z);
    SET_IX(13, MHL);
    SET_HL(z, ptr);
    z->a = 9;
    cpu_rst10(z);
    goto x0BFF;

x0B46:
    SET_HL(z, HL(z) + 1);
    z->e = MHL;
    SET_HL(z, HL(z) + 1);
    z->d = MHL;
    { uint16_t t = HL(z); SET_HL(z, DE(z)); SET_DE(z, t); }
    goto x090E;

x0B4E:
    SET_HL(z, HL(z) + 1);
    z->e = MHL;
    SET_HL(z, HL(z) + 1);
    SET_IX(6, z->e);
    SET_IX(7, 0);
    SET_IX(19, IX(19) | 0x20);
    goto x0BFF;

x0B5F:
    z->c = cpu_rd(z, 0x9215);
    z->e = (uint8_t)(IX(16) + 1);
    z->d = 0x01;
    z->e = cpu_rd(z, DE(z));
    z->d = 0x98;
    z->a = cpu_rd(z, DE(z));
    if (z->c & 1) {
        cpu_add(z, 0x0E);
        cpu_neg(z);
    }
    z->a = cpu_srl(z, z->a);
    SET_IX(3, z->a);
    z->a = cpu_rd(z, 0x92AA);
    if (z->a) cpu_wr(z, 0x9AB3, z->a);
    goto x0B8B;

x0B87:
    SET_IX(1, 0x9C);
x0B8B:
    SET_HL(z, HL(z) + 1);
x0B8C:
    SET_IX(8, z->l);
    SET_IX(9, z->h);
    SET_IX(13, IX(13) + 1);
    goto x0DFF;

x0B98:
    z->zf = (IX(16) & 0x38) == 0x38;
x0B9F:
    if (z->zf) goto x0B46;
    SET_HL(z, HL(z) + 3);
    goto x090E;

x0BA8:
    SET_HL(z, HL(z) + 1);
    z->a = MHL;
    if (IX(19) & 0x80) {
        cpu_add(z, 0x80);
        cpu_neg(z);
    }
    z->c = 0;
    z->a = cpu_sla(z, z->a);
    z->c = cpu_rl(z, z->c);
    z->a = cpu_sla(z, z->a);
    z->c = cpu_rl(z, z->c);
    SET_IX(4, z->a);
    SET_IX(5, z->c);
    SET_IX(14, 0x1E);
    SET_IX(15, cpu_rd(z, 0x92C8));
    goto x0B8B;

x0BD1:
    z->c = cpu_rd(z, 0x92AA);
    z->zf = !((uint8_t)(cpu_rd(z, 0x901D) - 1) & z->c);
    goto x0B9F;

x0BDC: /* a step: speeds, turn, frames */
    z->c = z->a;
    SET_IX(10, z->a & 0x0F);
    SET_IX(11, z->c >> 4);
    SET_HL(z, HL(z) + 1);
    z->a = MHL;
    SET_HL(z, HL(z) + 1);
    if (IX(19) & 0x80) cpu_neg(z);
    SET_IX(12, z->a);
    SET_IX(13, MHL);
    SET_HL(z, HL(z) + 1);
x0BFF:
    SET_IX(8, z->l);
    SET_IX(9, z->h);
x0C05:
    if (IX(19) & 0x40) { /* joining the formation: there? */
        z->a = IX(1);
        cpu_sub(z, IX(6));
        if (!z->zf) {
            if (z->sf) cpu_neg(z);
            z->a = cpu_dec(z, z->a);
            if (!z->zf) goto x0C2D;
        }
        z->a = IX(3);
        cpu_sub(z, IX(7));
        if (z->zf) goto x0E0C;
        if (z->sf) cpu_neg(z);
        z->a = cpu_dec(z, z->a);
        if (z->zf) goto x0E0C;
    }
x0C2D:
    if (IX(19) & 0x20) {
        z->a = (uint8_t)(IX(1) - IX(6));
        if (!z->a || z->a == 0xFF) {
            SET_IX(13, 1);
            SET_IX(19, IX(19) & ~0x20);
        }
    }
    /* the turn, and the sprite's frame from the heading */
    z->b = IX(12);
    z->a = IX(4);
    z->e = z->a;
    cpu_add(z, z->b);
    SET_IX(4, z->a);
    z->d = IX(5);
    z->l = z->b & 0x80 ? 0xFF : 1;
    cpu_rra(z);
    cpu_xor(z, z->b);
    {
        int minus = z->sf;
        z->a = z->d;
        if (minus) cpu_add(z, z->l);
    }
    SET_IX(5, z->a);
    z->a = z->e;
    z->c = z->d;
    if (z->c & 1) z->a = (uint8_t)~z->a;
    cpu_add(z, 0x15);
    if (z->cf) {
        z->b = 6;
    } else {
        z->a = cpu_srl(z, z->a);
        z->b = cpu_srl(z, z->a);
        cpu_add(z, z->b);
        z->b = (uint8_t)(z->a >> 5 & 7);
    }
    z->h = 0x8B;
    z->l = IX(16);
    MHL_W((MHL & 0xF8) | z->b);
    z->h = 0x9B;
    z->a = z->c;
    z->c = cpu_rrc(z, z->c);
    cpu_xor(z, z->c);
    z->a = cpu_inc(z, z->a);
    z->c = cpu_rrc(z, z->c);
    cpu_rla(z);
    MHL_W(z->a & 3);
    /* the move: the speed of this frame along the heading */
    z->a = cpu_rd(z, 0x92A0) & 1 ? IX(10) : IX(11);
    if (!z->a) goto x0D03;
    {
        uint8_t ph = z->h, pl = z->l, pd, pe;
        SET_HL(z, z->ix);
        z->b = z->a;
        z->a = z->d & 3;
        z->d = z->a;
        z->e = cpu_rlc(z, z->e);
        z->d = cpu_rl(z, z->d);
        pd = z->d, pe = z->e;
        cpu_xor(z, z->d);
        cpu_rrca(z);
        if (!z->cf) z->l += 2;
        z->d = cpu_inc(z, z->d);
        z->a = z->b;
        if (z->d & 4) cpu_neg(z);
        z->c = cpu_sra(z, z->a);
        if (z->cf) {
            z->a = MHL;
            cpu_add(z, 0x80);
            MHL_W(z->a);
        }
        z->l++;
        z->a = MHL;
        cpu_adc(z, z->c);
        MHL_W(z->a);
        z->l--;
        { uint16_t t = HL(z); SET_HL(z, DE(z)); SET_DE(z, t); }
        z->e ^= 2;
        z->h = pd, z->l = pe;
        z->l = cpu_srl(z, z->l);
        if (z->cf) z->l ^= 0x7F;
        z->a = z->b;
        z->b = z->h;
        z->h = 0;
        cpu_multiply(z); /* $0E9B */
        z->a = (uint8_t)((z->b ^ 2) - 1);
        if (z->a & 4) {
            z->b = z->h;
            z->c = z->l;
            SET_HL(z, (uint16_t)-(z->b << 8 | z->c));
        }
        { uint16_t t = HL(z); SET_HL(z, DE(z)); SET_DE(z, t); }
        z->a = z->e;
        cpu_add(z, MHL);
        MHL_W(z->a);
        z->l++;
        z->a = z->d;
        cpu_adc(z, MHL);
        MHL_W(z->a);
        z->h = ph, z->l = pl;
    }
x0D03: /* the sprite's position from the slot's */
    z->c = cpu_rd(z, 0x9215);
    z->h = 0x93;
    z->d = IX(3);
    z->cf = IX(2) > 0x7F;
    z->a = z->d;
    cpu_rla(z);
    if (z->c & 1) {
        cpu_add(z, 0x0D);
        z->a = (uint8_t)~z->a;
    }
    if (IX(19) & 0x40) cpu_add(z, IX(17));
    MHL_W(z->a);
    z->l++;
    z->b = IX(1);
    z->cf = IX(0) > 0x7F;
    z->e = cpu_rl(z, z->e);
    z->a = z->b;
    if (!(z->c & 1)) {
        cpu_add(z, 0x4F);
        z->a = (uint8_t)~z->a;
        z->e--;
    }
    z->e = cpu_rr(z, z->e);
    cpu_rla(z);
    z->e = cpu_rl(z, z->e);
    if (IX(19) & 0x40) {
        cpu_add(z, IX(18));
        z->d = z->a;
        cpu_rra(z);
        cpu_xor(z, IX(18));
        cpu_rlca(z);
        z->a = z->d;
        if (z->cf) z->e++;
    }
    MHL_W(z->a);
    z->h = 0x9B;
    {
        uint8_t v = cpu_rrc(z, MHL);
        MHL_W(v);
        z->e = cpu_rrc(z, z->e);
        MHL_W(cpu_rl(z, v));
    }
    /* bombs */
    SET_IX(14, cpu_dec(z, IX(14)));
    if (!z->zf) goto x0DFF;
    {
        uint8_t v = cpu_srl(z, IX(15));
        SET_IX(15, v);
    }
    if (!z->cf || IX(1) < 0x4C || !cpu_rd(z, 0x9015) || cpu_rd(z, 0x92AD)) goto x0DF9;
    { uint16_t t = HL(z); SET_HL(z, DE(z)); SET_DE(z, t); }
    SET_HL(z, 0x8868);
    for (z->b = 8;; z->l += 2) {
        if (MHL == 0x80) break;
        if (!--z->b) goto x0DF9;
    }
    {
        uint8_t ph, pl, cy;
        MHL_W(6);
        z->h = 0x9B;
        MHL_W(1);
        ph = z->h, pl = z->l;
        z->h = 0x93;
        z->d = z->h;
        z->e--;
        z->c = cpu_rd(z, DE(z));
        MHL_W(z->c);
        z->e++;
        z->l++;
        z->b = cpu_rd(z, DE(z));
        MHL_W(z->b);
        z->h = 0x9B;
        z->d = z->h;
        z->a = cpu_rd(z, DE(z));
        {
            uint8_t v = cpu_rrc(z, MHL);
            MHL_W(v);
            cpu_rrca(z);
            MHL_W(cpu_rl(z, v));
        }
        cpu_rlca(z);
        z->b = cpu_rr(z, z->b);
        z->a = cpu_rd(z, 0x9362);
        cpu_sub(z, z->c);
        cy = z->cf;
        if (z->cf) cpu_neg(z);
        z->h = z->a;
        z->a = cpu_rd(z, 0x9215) ? 0x1C : 0x95;
        cpu_sub(z, z->b);
        if (z->cf) cpu_neg(z);
        cpu_divide(z); /* $0EAE */
        {
            uint16_t q = HL(z), r = (uint16_t)(q >> 2);
            r = (uint16_t)((uint16_t)(r + q) >> 2);
            SET_HL(z, r);
        }
        z->a = z->h || z->l >= 0x60 ? 0x60 : z->l;
        z->b = z->a;
        z->cf = cy;
        z->b = cpu_rr(z, z->b);
        z->h = ph, z->l = pl;
        z->a = (uint8_t)((z->l + 8) & 0x0F);
        SET_HL(z, 0x92B0 + z->a);
        MHL_W(z->b);
        SET_HL(z, HL(z) + 1);
        MHL_W(0);
    }
x0DF9:
    SET_IX(14, cpu_rd(z, 0x92E2));
x0DFF:
    SET_HL(z, 0x9289);
    MHL_W(MHL - 1);
    if (!MHL) return;
    SET_DE(z, 0x0014);
    z->ix = (uint16_t)(z->ix + 0x14);
    goto x08E4;

x0E0C: /* in its place in the formation */
    SET_IX(19, IX(19) & ~1);
    SET_IX(0, 0);
    SET_IX(2, 0);
    z->h = 0x88;
    z->l = IX(16);
    MHL_W(2);
    z->h = 0x8B;
    z->l++;
    z->a = (uint8_t)((MHL + 1) & 7);
    z->l--;
    if (z->a >= 5) {
        z->a = cpu_rd(z, 0x982E);
        z->c = z->a;
        MHL_W((z->a & 0xF8) + 6);
        z->l++;
        MHL_W(z->c & 7);
        z->l--;
        cpu_wr(z, 0x982D, 1);
    }
    SET_IX(1, IX(6));
    SET_IX(3, IX(7));
    goto x0D03;

x0E4D: /* the slot freed */
    z->h = 0x88;
    z->l = IX(16);
    MHL_W(0x80);
    z->h = 0x93;
    MHL_W(0);
    SET_IX(19, 0);
    goto x0DFF;
}

/* ---- the IRQ -------------------------------------------------------------- */

void sub_sprites(Game *g)
{
    WR(0x92D7, 1);
    game_copy(g, 0x8B80, 0x8B00, 0x40);
    game_copy(g, 0x9380, 0x9300, 0x40);
    game_copy(g, 0x9B80, 0x9B00, 0x40);
    WR(0x92D7, 0);
    game_wait(g, 0x92D6, 1); /* $05E7: main's handshake */
}

void sub_debug(Game *g)
{
    if (RD(0x6802) & 2) return;
    /* $0ED0: reads $10DF, where nothing answers, and restarts the CPU (RST 0) */
    game_unported(g, 0x0ED0);
}

void sub_irq_start(Game *g)
{
    WR(0x6821, 0);
    if (!(RD(0x6804) & 2)) return; /* frozen (DSWA bit 4) */
    uint8_t n = (uint8_t)(RD(0x92A0) + 1), l = RD(0x92A1), h = RD(0x92A2);
    WR(0x92A0, n);
    if ((n & 31) == 1) {
        h++;
    } else if (!(n & 31)) {
        h = (uint8_t)((h | 1) + 1);
        l++;
    }
    WR(0x92A1, l);
    WR(0x92A2, h);
    WR(0x92AA, RD(0x9015) & (RD(0x92A7) < RD(0x99C7)));
}

void sub_irq_tasks(Game *g)
{
    if (RD(0x6804) & 2) {
        uint8_t c = 0;
        do {
            uint8_t b;
            while (!(b = RD(0x9000 | (uint8_t)(0x20 + c)))) c++;
            uint8_t at = (uint8_t)(0x3B + 2 * c);
            uint16_t task = (uint16_t)(game_data(g, GAME_SUB + at) | game_data(g, GAME_SUB + at + 1u) << 8);
            uint16_t de = (uint16_t)(at + 1);
            switch (task) {
            case 0x05BE: break;
            case 0x05BF: sub_sprites(g); break;
            case 0x08D3: sub_motion(g, (uint8_t)(de >> 8), (uint8_t)de); break;
            case 0x06F5: sub_shots(g); break;
            case 0x05EE: sub_ship(g); break;
            case 0x0ECA: sub_debug(g); break;
            default: game_unported(g, task); break;
            }
            c = (uint8_t)(c + b);
        } while (!(c & 0xF8));
    }
    WR(0x6821, 1);
}

void sub_irq(Game *g)
{
    sub_irq_start(g);
    sub_irq_tasks(g);
}
