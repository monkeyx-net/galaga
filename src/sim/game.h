/*
 * The C game's state: the board's memory as the ROMs lay it out (video RAM
 * $8000-$87FF, RAM $8800-$8BFF, $9000-$93FF, $9800-$9BFF), the latches
 * the programs write ($6820-$6827, $A000-$A007), the WSG's registers and
 * the switches. The three Z80 programs share it, as on the board, at the
 * ROMs' addresses (game_rd / game_wr map them as the board does), so the
 * whole state compares byte for byte with the board's.
 *
 * `data` is what the programs read of their ROMs, at its ROM addresses:
 * main's at $0000-$3FFF, sub's at $4000 + its address, sub2's at $5000 +
 * its (assets/extract.h); the game fills it from assets/, the tests from
 * the ROM set.
 */
#ifndef GALAGA_GAME_H
#define GALAGA_GAME_H

#include <stdint.h>

#define GAME_DATA_SIZE 0x6000
#define GAME_SUB 0x4000  /* sub's ROM in data */
#define GAME_SUB2 0x5000 /* sub2's */

typedef struct Game {
    uint8_t video[0x800];    /* $8000: tile codes, then colours */
    uint8_t ram[3][0x400];   /* $8800, $9000, $9800 */
    uint8_t misc[8];         /* $6820-$6827 (bit 0 each) */
    uint8_t vlatch[8];       /* $A000-$A007 (bit 0 each) */
    uint8_t wsg[0x20];       /* $6800-$681F (low nibbles) */
    uint8_t in0, in1, dswa, dswb; /* the switches (active low) */
    const uint8_t *data;     /* GAME_DATA_SIZE bytes: the ROMs */
    uint16_t unported;       /* the ROM address the C game last reached unported, or 0 */
    uint8_t *data_read;      /* optional, for tools: a flag per data byte the game reads */
    /* the scheduler's (sim/sim.c): the writes with an effect beyond the
       memory (the latches $6820-$6827, the watchdog, the 06xx) after them,
       the reads of the 06xx ($7000, $7100) */
    void (*bus_write)(void *bus, uint16_t addr, uint8_t value);
    uint8_t (*bus_read)(void *bus, uint16_t addr);
    void *bus;
    /* optional, for tests: called before each write */
    void (*on_write)(void *user, uint16_t addr, uint8_t value);
    void *user;
} Game;

/* A byte as the CPUs read it (the DIP switches at $6800-$6807, the 06xx
   through the bus; 0 where nothing answers). */
uint8_t game_rd(const Game *g, uint16_t addr);
/* A byte as the CPUs write it (the latches keep bit 0, the WSG a nibble). */
void game_wr(Game *g, uint16_t addr, uint8_t value);
/* A byte of a ROM (data: main $0000, sub GAME_SUB +, sub2 GAME_SUB2 +). */
uint8_t game_data(const Game *g, unsigned addr);

/* The C game reached ROM code not ported: remember where (the caller must
   leave the state unspecified). */
void game_unported(Game *g, uint16_t addr);

#endif
