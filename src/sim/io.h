/*
 * The 06xx bus and its customs, in C: the 51xx (coins, credits, the control
 * panel: its MB8843 program ported, its loop once a frame as on the board,
 * where the vertical blank clocks its timer) and the 54xx (the noise
 * commands). Main moves bytes to and from them in transfers; here a
 * transfer happens at once.
 */
#ifndef GALAGA_IO_H
#define GALAGA_IO_H

#include <stdint.h>

#include "sim/game.h"

/* an MB88's state, as its program sees it */
typedef struct {
    uint8_t a, x, y, cf, zf, st, sb, tl, th, o, p, pio, vf, sf;
    uint8_t k;          /* its K port: the byte's low bits (in write mode), | 8 in read mode */
    uint8_t select;     /* its interrupt line (the 06xx's select) */
    uint8_t r[4];       /* its R ports as written */
    uint8_t ram[64];
    uint8_t in0, in1;   /* the control panel, active low (the 51xx's R ports) */
    int8_t coinage;     /* command 1 under way: the RAM nibble its next byte fills, or -1 */
} Mcu;

typedef struct Io {
    Mcu m51;
    uint8_t ctrl;       /* the 06xx's control register as main reads it ($7100) */
    uint8_t noise[16];  /* the 54xx's last command (its four bytes) and a count */
    int noises;
} Io;

void io_reset(Io *io);
/* The vertical blank: the 51xx's loop with the control panel of this frame */
void io_frame(Io *io, uint8_t in0, uint8_t in1);
/* A transfer: control `ctrl` (bit 4 read, bits 0-3 the chips), `n` bytes
   from or to main's memory at `mem` */
void io_transfer(Io *io, Game *g, uint8_t ctrl, uint16_t mem, int n);

/* the 51xx alone (the tests): a byte main writes to it, a byte it hands */
void io_51xx_write(Mcu *m, uint8_t v);
uint8_t io_51xx_read(Mcu *m);

#endif
