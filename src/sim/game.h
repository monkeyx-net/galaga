/*
 * The C game's state: the board's memory as the ROMs lay it out (video RAM
 * $8000-$87FF, RAM $8800-$8BFF, $9000-$93FF, $9800-$9BFF), the latches
 * the programs write ($6820-$6827, $A000-$A007), the WSG's registers and
 * the switches. The three Z80 programs share it, as on the board.
 * Routines are written against the same addresses as the ROMs' (game_rd /
 * game_wr map them as the board does), so their arithmetic and pointers
 * are the ROMs' and the whole state compares byte for byte with the
 * board's.
 *
 * `data` is the read-only data the ported routines use, at its ROM
 * addresses: main's at $0000-$3FFF, sub's at $4000 + its address, sub2's
 * at $5000 + its (assets/extract.h); the game fills it from assets/, the
 * tests from the ROM set. Routines that take or return registers use Regs.
 */
#ifndef GALAGA_GAME_H
#define GALAGA_GAME_H

#include <stdint.h>

#define GAME_DATA_SIZE 0x6000
#define GAME_SUB 0x4000  /* sub's ROM in data */
#define GAME_SUB2 0x5000 /* sub2's */

typedef struct {
    uint8_t a, f, b, c, d, e, h, l;
    uint16_t ix, iy;
} Regs;

#define REGS_BC(r) ((uint16_t)((r)->b << 8 | (r)->c))
#define REGS_DE(r) ((uint16_t)((r)->d << 8 | (r)->e))
#define REGS_HL(r) ((uint16_t)((r)->h << 8 | (r)->l))
#define REGS_SET_BC(r, v) ((r)->b = (uint8_t)((v) >> 8), (r)->c = (uint8_t)(v))
#define REGS_SET_DE(r, v) ((r)->d = (uint8_t)((v) >> 8), (r)->e = (uint8_t)(v))
#define REGS_SET_HL(r, v) ((r)->h = (uint8_t)((v) >> 8), (r)->l = (uint8_t)(v))

typedef struct Game {
    uint8_t video[0x800];    /* $8000: tile codes, then colours */
    uint8_t ram[3][0x400];   /* $8800, $9000, $9800 */
    uint8_t misc[8];         /* $6820-$6827 (bit 0 each) */
    uint8_t vlatch[8];       /* $A000-$A007 (bit 0 each) */
    uint8_t wsg[0x20];       /* $6800-$681F (low nibbles) */
    uint8_t in0, in1, dswa, dswb; /* the switches (active low) */
    const uint8_t *data;     /* GAME_DATA_SIZE bytes: the ROMs' tables */
    uint16_t unported;       /* the ROM address the C game last reached unported, or 0 */
    uint16_t stalled;        /* the address of a handshake a routine found not yet given, or 0 */
    uint8_t *data_read;      /* optional, for tools: a flag per data byte the game reads */
    uint8_t r;               /* the C game's refresh register: its own count */
    /* optional, for tests: the board's refresh register at the LD A,R */
    uint8_t (*r_hook)(void *user);
    /* optional, for tests: called before each write */
    void (*on_write)(void *user, uint16_t addr, uint8_t value);
    void *user;
} Game;

/* A byte as the CPUs read it (the DIP switches at $6800-$6807; 0 where
   nothing answers). */
uint8_t game_rd(const Game *g, uint16_t addr);
/* A byte as the CPUs write it (the latches keep bit 0, the WSG a nibble). */
void game_wr(Game *g, uint16_t addr, uint8_t value);
/* Little-endian words, as the Z80 keeps them. */
uint16_t game_rd16(const Game *g, uint16_t addr);
void game_wr16(Game *g, uint16_t addr, uint16_t value);
/* `n` bytes of `value` from `addr` up (RST $18). */
void game_fill(Game *g, uint16_t addr, uint8_t value, int n);
/* LDIR: n bytes from src to dst, one at a time (so overlaps behave as the ROM's). */
void game_copy(Game *g, uint16_t dst, uint16_t src, int n);
/* A byte of a ROM's tables (data: main $0000, sub GAME_SUB +, sub2 GAME_SUB2 +). */
uint8_t game_data(const Game *g, unsigned addr);

/* Shorthands for the routines (g is the Game in scope). */
#define RD(a) game_rd(g, (uint16_t)(a))
#define WR(a, v) game_wr(g, (uint16_t)(a), (uint8_t)(v))

/* A CPU's wait while `addr` holds `value`, until another CPU changes it
   (a handshake). The C game runs the other CPU's part first, so the wait
   is over: 1 (and g->stalled set) if it is not. */
int game_wait(Game *g, uint16_t addr, uint8_t value);
/* ... until `addr` holds `value` */
int game_wait_for(Game *g, uint16_t addr, uint8_t value);

/* The Z80's refresh register as main reads it (LD A,R, in $1000's random
   numbers): the count of its opcode fetches, which only an emulation of
   every instruction knows. The tests give the board's through r_hook; the
   C game makes its own. */
uint8_t game_r(Game *g);

/* The C game reached ROM code not yet ported: remember where (the caller
   must leave the state unspecified). */
void game_unported(Game *g, uint16_t addr);

#endif
