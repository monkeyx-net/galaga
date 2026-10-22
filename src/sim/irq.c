#include "sim/irq.h"

#include "sim/cpu.h"
#include "sim/tasks.h"

static Cpu cpu(Game *g)
{
    Cpu z = { 0 };
    z.g = g;
    return z;
}

#define D(a) game_data(g, (unsigned)(a))

/* ---- the tasks -------------------------------------------------------------- */

void task_sprites(Game *g)
{
    WR(0x92D6, 1);
    game_copy(g, 0x8BC0, 0x8B40, 0x40);
    game_copy(g, 0x93C0, 0x9340, 0x40);
    game_copy(g, 0x9BC0, 0x9B40, 0x40);
    WR(0x92D6, 0);
    game_wait(g, 0x92D7, 1); /* $0850: sub's handshake */
}

/* $08BE: the byte of a table of rows of four at (row A, column $92A7 / 10) */
static uint8_t by_rank(Cpu *z, uint16_t table, uint8_t row)
{
    SET_HL(z, table);
    z->a = cpu_sla(z, row);
    cpu_rst08(z);
    cpu_ex_de_hl(z);
    z->h = z->c;
    z->a = 0x0A;
    cpu_divide(z);
    cpu_ex_de_hl(z);
    z->a = z->d;
    cpu_rst10(z);
    return cpu_rd(z, HL(z));
}

/* $08AD: the byte of a table of rows of three at (row A, column by the
   frames left B: 0 under $28, 1 from there, 2 at none) */
static uint8_t by_time(Cpu *z, uint16_t table, uint8_t row)
{
    SET_HL(z, table);
    z->e = row;
    z->a = (uint8_t)(cpu_sla(z, row) + row);
    cpu_rst10(z);
    z->a = z->b;
    if (z->b < 0x28) SET_HL(z, HL(z) + 1);
    if (!z->b) SET_HL(z, HL(z) + 1);
    return cpu_rd(z, HL(z));
}

void task_difficulty(Game *g)
{
    Cpu z0 = cpu(g), *z = &z0;
    z->b = RD(0x92AE);
    if (z->b < 0x3C) WR(0x99C4, RD(0x99C5));
    z->c = RD(0x92A7);
    WR(0x92C8, by_rank(z, 0x0909, RD(0x99C0)));
    if (RD(0x92AA)) {
        game_fill(g, 0x92C4, 2, 3);
        WR(0x9AA0, 0);
        return;
    }
    WR(0x92C4, by_rank(z, 0x0929, RD(0x99C1)));
    WR(0x92C5, by_time(z, 0x08CD, RD(0x99C2)));
    WR(0x92C6, by_time(z, 0x08EB, RD(0x99C3)));
}

/* $095F: three tiles of text from HL (or the blanks at $0974 when A's bit 0) to DE */
static void three(Game *g, uint16_t text, uint16_t at, uint8_t a)
{
    if (a & 1) text = 0x0974;
    for (int i = 0; i < 3; i++) WR(at + i, D(text + i));
}

/* $093C: in a game, 1UP and (two players) 2UP shown or hidden by C's bit 0,
   the player up's ($9840) blinking */
static void blink(Game *g, uint8_t c)
{
    if (RD(0x9201) != 3) return;
    uint8_t b = RD(0x9840);
    three(g, 0x096E, 0x83D9, (uint8_t)(~b & c));
    if (!RD(0x99B3)) return;
    three(g, 0x0971, 0x83C4, (uint8_t)(b & c));
}

void task_blink(Game *g)
{
    uint8_t a = RD(0x92A0);
    blink(g, (uint8_t)(a << 4 | a >> 4));
}

void task_credits(Game *g)
{
    Cpu z0 = cpu(g), *z = &z0;
    if (RD(0x99B5) == 0xBB) { /* the service switch: the test mode */
        game_unported(g, 0x336C);
        return;
    }
    if (RD(0x9201) == 3) { /* in a game: the clock, BCD frames and seconds at $99E5-$99E9 */
        SET_HL(z, 0x99E9);
        z->a = cpu_rd(z, HL(z));
        cpu_add(z, 1);
        cpu_daa(z);
        cpu_cp(z, 0x60);
        if (z->zf) cpu_xor(z, z->a);
        cpu_ccf(z);
        for (z->b = 4; z->b; z->b--) {
            cpu_wr(z, HL(z), z->a);
            z->l--;
            z->a = cpu_rd(z, HL(z));
            cpu_adc(z, 0);
            cpu_daa(z);
        }
    } else if (RD(0x99B8) == 0xA0) { /* free play */
        for (int i = 0; i < 9; i++) WR(0x803C - i, D(0x09D8 - i));
    } else { /* CREDIT and the count */
        uint8_t n = RD(0x99B5);
        uint16_t de = 0x803C;
        for (int i = 0; i < 6; i++, de--) WR(de, D(0x09CF - i));
        de--;
        if (n >> 4) WR(de--, n >> 4);
        WR(de--, n & 15);
        WR(de, 0x24);
    }
    uint8_t state = RD(0x9201);
    if (!state) return;
    if (state == 1 && RD(0x99B5)) { /* credits in the attract mode: the start screen, sounds off */
        WR(0x9201, 2);
        game_fill(g, 0x9AA0, 0, 8);
        game_fill(g, 0x9AA9, 0, 15);
    }
    z->c = RD(0x99B5);
    z->b = RD(0x99B8);
    z->a = z->b;
    cpu_sub(z, z->c);
    if (z->zf) return;
    if (!z->cf) { /* fewer credits: a game started, with the players they paid for */
        cpu_daa(z);
        z->a--;
        WR(0x99B3, z->a);
        WR(0x99B8, z->c);
        WR(0x9201, 3);
        return;
    }
    WR(0x99B8, z->c);
    if (z->c == 0xA0) return;
    z->a = z->c;
    cpu_sub(z, z->b);
    cpu_daa(z);
    WR(0x9A79, z->a); /* coins in: their sound */
}

/* ---- the IRQ ------------------------------------------------------------------ */

static void run_task(Game *g, uint16_t task)
{
    switch (task) {
    case 0x0827: break;
    case 0x0828: task_sprites(g); break;
    case 0x0857: task_difficulty(g); break;
    case 0x0935: task_blink(g); break;
    case 0x0977: task_credits(g); break;
    case 0x17B2: task_17B2(g); break;
    case 0x1700: task_1700(g); break;
    case 0x1A86: task_1A86(g); break;
    case 0x2916: task_2916(g); break;
    case 0x1DEC: task_1DEC(g); break;
    case 0x2A90: task_2A90(g); break;
    case 0x1DB9: task_1DB9(g); break;
    case 0x23DD: task_23DD(g); break;
    case 0x1EAA: task_1EAA(g); break;
    case 0x1D38: task_1D38(g); break;
    case 0x1B6B: task_1B6B(g); break;
    case 0x19B2: task_19B2(g); break;
    case 0x1D7C: task_1D7C(g); break;
    case 0x1F8F: task_1F8F(g); break;
    case 0x1F0E: task_1F0E(g); break;
    case 0x1DD8: task_1DD8(g); break;
    case 0x2222: task_2222(g); break;
    case 0x21CB: task_21CB(g); break;
    case 0x20F2: task_20F2(g); break;
    case 0x2000: task_2000(g); break;
    default: game_unported(g, task); break;
    }
}

void irq_main(Game *g)
{
    uint8_t d = RD(0x6804);
    uint8_t c = RD(0x92A0) & 0x1C;
    c = (uint8_t)(((c >> 1 | c << 7) ^ c) & 0x18);
    uint8_t a = (uint8_t)(((d & 2) ? RD(0x99BE) : 7) & 7) | c;
    for (int i = 0; i < 5; i++, a = (uint8_t)(a >> 1 | a << 7)) WR(0xA000 + i, a);
    WR(0x6830, a); /* the watchdog */
    WR(0x6820, 0);
    if (d & 2) { /* not frozen (DSWA bit 4) */
        c = 0;
        do {
            uint8_t b;
            while (!(b = RD(0x9000 | (uint8_t)c))) c++;
            uint8_t at = (uint8_t)(0x96 + 2 * c);
            run_task(g, (uint16_t)(D(at) | D(at + 1u) << 8));
            c = (uint8_t)(c + b);
        } while (!(c & 0xE0));
        /* $0299: the 51xx's three bytes to $99B5 (06xx control $71) */
    }
    WR(0x6820, 1);
}
