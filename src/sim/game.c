#include "sim/game.h"

uint8_t game_rd(const Game *g, uint16_t a)
{
    if (a >= 0x6800 && a <= 0x6807) {
        int n = a & 7;
        return (uint8_t)((g->dswb >> n & 1) | (g->dswa >> n & 1) << 1);
    }
    if (a >= 0x8000 && a <= 0x87FF) return g->video[a & 0x7FF];
    if (a >= 0x8800 && a <= 0x8BFF) return g->ram[0][a & 0x3FF];
    if (a >= 0x9000 && a <= 0x93FF) return g->ram[1][a & 0x3FF];
    if (a >= 0x9800 && a <= 0x9BFF) return g->ram[2][a & 0x3FF];
    return 0;
}

void game_wr(Game *g, uint16_t a, uint8_t v)
{
    if (g->on_write) g->on_write(g->user, a, v);
    if (a >= 0x6800 && a <= 0x681F) g->wsg[a & 0x1F] = v & 15;
    else if (a >= 0x6820 && a <= 0x6827) g->misc[a & 7] = v & 1;
    else if (a >= 0x8000 && a <= 0x87FF) g->video[a & 0x7FF] = v;
    else if (a >= 0x8800 && a <= 0x8BFF) g->ram[0][a & 0x3FF] = v;
    else if (a >= 0x9000 && a <= 0x93FF) g->ram[1][a & 0x3FF] = v;
    else if (a >= 0x9800 && a <= 0x9BFF) g->ram[2][a & 0x3FF] = v;
    else if (a >= 0xA000 && a <= 0xA007) g->vlatch[a & 7] = v & 1;
}

uint16_t game_rd16(const Game *g, uint16_t a) { return (uint16_t)(game_rd(g, a) | game_rd(g, (uint16_t)(a + 1)) << 8); }

void game_wr16(Game *g, uint16_t a, uint16_t v)
{
    game_wr(g, a, (uint8_t)v);
    game_wr(g, (uint16_t)(a + 1), (uint8_t)(v >> 8));
}

void game_fill(Game *g, uint16_t a, uint8_t v, int n)
{
    while (n--) game_wr(g, a++, v);
}

void game_copy(Game *g, uint16_t dst, uint16_t src, int n)
{
    while (n--) game_wr(g, dst++, game_rd(g, src++));
}

uint8_t game_data(const Game *g, unsigned addr)
{
    if (g->data_read && addr < GAME_DATA_SIZE) ((Game *)g)->data_read[addr] = 1;
    return addr < GAME_DATA_SIZE && g->data ? g->data[addr] : 0;
}

void game_unported(Game *g, uint16_t addr)
{
    if (!g->unported) g->unported = addr;
}
