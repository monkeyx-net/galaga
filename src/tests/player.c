#include "tests/player.h"

#include <stdio.h>

#include "emu/board.h"

void player_step(Player *p, long f, PlayerPeek peek, void *ctx, uint8_t *in0_out, uint8_t *in1_out)
{
    uint8_t in0 = 0xFF, in1 = 0xFF, state = peek(ctx, 0x9201);
    if (p->verbose && state != p->state) fprintf(stderr, "frame %ld: state %d (stage %d)\n", f, state, peek(ctx, 0x9821));
    p->state = state;
    if (state == 3) p->in_game = 1;
    if (p->in_game && state == 1) { /* the game is over */
        p->in_game = 0;
        p->game_over = 1;
        goto out;
    }
    if (!p->games && f < p->attract) goto out;
    if (state == 1 && !p->next_coin) p->next_coin = f + 60; /* the attract mode: a game next */
    if (p->next_coin) {
        int two = p->games & 1;
        long t = f - p->next_coin;
        if ((t >= 0 && t < 8) || (two && t >= 30 && t < 38)) in1 &= (uint8_t)~IN1_COIN1;
        if (t >= 100 && t < 108) in1 &= (uint8_t)~(two ? IN1_START2 : IN1_START1);
        if (t >= 108) {
            p->next_coin = 0;
            p->games++;
        }
        goto out;
    }
    /* only while the ship is in play: a button held through the game's end
       reads as credits (the 51xx still sends the switches) */
    if (state != 3 || !peek(ctx, 0x9014)) goto out;
    {
        /* the sprites from $8800 (states: $80 free, 4 and 5 exploding); $60-$67
           the ship and its shots, $68-$77 the bombs. X grows to the right, Y
           (nine bits) down to the ship's row at $129. */
        int ship = peek(ctx, 0x9362), aim = -1, dist = 256, threat = -1, near = 256;
        for (int l = 0; l < 0x78; l += 2) {
            if (l >= 0x60 && l < 0x68) continue;
            uint8_t st = peek(ctx, (uint16_t)(0x8800 + l));
            int x = peek(ctx, (uint16_t)(0x9300 + l));
            int y = (peek(ctx, (uint16_t)(0x9B01 + l)) & 1) << 8 | peek(ctx, (uint16_t)(0x9301 + l));
            if (st == 0x80 || (st & 0xFE) == 4 || !x) continue;
            int d = x > ship ? x - ship : ship - x;
            if (y >= 0xF0 && y < 0x138 && d < 0x18 && d < near) {
                near = d;
                threat = x;
            }
            if (l < 0x60 && d < dist) {
                dist = d;
                aim = x;
            }
        }
        if (threat >= 0) { /* away from it, towards the middle at the edges */
            if ((threat >= ship && ship > 0x30) || ship > 0xC0) in0 &= (uint8_t)~IN0_LEFT1;
            else in0 &= (uint8_t)~IN0_RIGHT1;
        } else if (aim >= 0 && aim + 2 < ship) {
            in0 &= (uint8_t)~IN0_LEFT1;
        } else if (aim >= 0 && aim > ship + 2) {
            in0 &= (uint8_t)~IN0_RIGHT1;
        }
        if ((f & 3) < 2) in1 &= (uint8_t)~IN1_FIRE1;
        if (p->verbose && f % 100 == 0) {
            int alive = 0;
            for (int l = 0; l < 0x60; l += 2) alive += peek(ctx, (uint16_t)(0x8800 + l)) != 0x80;
            fprintf(stderr, "  %ld: ship %02X aim %d threat %d alive %d hits %d lives %d\n", f, ship, aim, threat, alive,
                    peek(ctx, 0x9844) | peek(ctx, 0x9845) << 8, peek(ctx, 0x9820));
        }
    }
out:
    *in0_out = in0;
    *in1_out = in1;
}
