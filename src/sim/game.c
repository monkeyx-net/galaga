#include "sim/game.h"

uint8_t game_rd(const Game *g, uint16_t a)
{
    if (a >= 0x6800 && a <= 0x6807) {
        int n = a & 7;
        return (uint8_t)((g->dswb >> n & 1) | (g->dswa >> n & 1) << 1);
    }
    if (a >= 0x7000 && a <= 0x7100 && g->bus_read) return g->bus_read(g->bus, a);
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
    if (g->bus_write && ((a >= 0x6800 && a <= 0x6837) || (a >= 0x7000 && a <= 0x7100))) g->bus_write(g->bus, a, v);
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
