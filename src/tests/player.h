/*
 * The tests' player: an attract cycle untouched, then games one after
 * another (one player, then two, by turns): coins and a start once the
 * attract mode is on; in a game the ship steered under the nearest enemy
 * and away from what is about to reach its row, firing. It reads the
 * game's memory through peek; its inputs reach the game a frame later, as a
 * probe's would.
 */
#ifndef GALAGA_PLAYER_H
#define GALAGA_PLAYER_H

#include <stdint.h>

typedef uint8_t (*PlayerPeek)(void *ctx, uint16_t addr);

typedef struct {
    long attract;   /* frames before the first coin */
    int games;      /* games started */
    int in_game;
    int game_over;  /* set when a game has ended (the caller clears it) */
    long next_coin; /* the frame of the next game's first coin, or 0 */
    uint8_t state;  /* $9201 last seen */
    int verbose;    /* the states and the progress to stderr */
} Player;

/* The inputs (active low) for after frame `frame` */
void player_step(Player *p, long frame, PlayerPeek peek, void *ctx, uint8_t *in0, uint8_t *in1);

#endif
