/*
 * The Galaga board's picture, as the player sees it: 224x288 portrait (the
 * monitor turned 90 degrees). Drawn at the vertical blank from the video
 * RAM, the sprite RAM and the latches:
 * - Tiles: a 28x36 map of 8x8 tiles, two bits a pixel (gg1_9). Rows 2-33
 *   are $040 + (27 - x) x 32 + (y - 2) in the video RAM, rows 0-1 and 34-35
 *   $3C0 / $3E0 / $000 / $020 + 29 - x (two cells each side unseen); the
 *   colour byte is $400 on. A tile's pixel (px, py) is bit 3 - py % 4 (and
 *   7 - py % 4) of byte 7 - px (+8 for py < 4).
 * - Sprites: 64 of 16x16, two bits a pixel (gg1_11 / gg1_10, 64 bytes
 *   each), at $8B80 (code, colour), $9380 (position), $9B80 (flags) + 2i.
 *   Sprite pixel (px, py) comes from byte (15 - px, or 24 + 15 - px past 8)
 *   + 8 (py / 4), bits 7 - py % 4 and 3 - py % 4. It is placed at x = byte
 *   0 - 17, y = byte 1 + 256 x (flags byte 1 bit 0) - 40; flags bit 0
 *   flips it top to bottom, bit 1 left to right.
 * - Colours: a tile's colour c and pen p give prom-4[4c + p] (low nibble),
 *   palette entry 16 on; a sprite's, prom-3[4c + p], entry 0 on; 15 is
 *   transparent. The palette (prom-5) weighs red bits 0-2 and green bits
 *   3-5 33, 71, 151, blue bits 6-7 81, 174.
 * - Stars (the 05xx): a 16-bit shift register (each step: in at bit 15 the
 *   parity of bits 0, 3, 5, 10) gives a sequence of 65535 steps, 256 a line
 *   along y 16-271, lines from x 223 down to 0, from the frame's origin
 *   on. A step is a star when its bits 0, 3, 5 and 10 are clear and 12-15
 *   set: of colour red ~bits 6, 7, green ~bits 8, 1, blue ~bits 2, 4 (low
 *   bit first, each 0, $47, $97, $DE; blue 0, $51, $AE, $FF), shown when
 *   $A003 (bit 11 clear) or $A004 (set) equals bit 9 and $A005 is set. At
 *   each vertical blank the origin moves by -1, -2, -3, -4, +3, +2, +1 or 0
 *   steps by $A000-$A002 (bits 0-2), the stars shown or not.
 * - Tiles are drawn over the sprites, the sprites over the stars.
 * - $A007 (the cocktail table, set for player two's turns) turns the tiles'
 *   picture round; the sprites stay where the game puts them (it places
 *   them for the turned screen), each turned round in its place (both its
 *   flips inverted).
 */
#ifndef GALAGA_GFX_GALAGA_H
#define GALAGA_GFX_GALAGA_H

#include <stdint.h>

#include "assets/rom.h"

#define GALAGA_W 224
#define GALAGA_H 288
#define GALAGA_TILES 256
#define GALAGA_SPRITES 128
#define GALAGA_STAR_PERIOD 65535
#define GALAGA_STARS 256

typedef struct {
    uint8_t tiles[GALAGA_TILES][8][8];       /* pens, [py][px] */
    uint8_t sprites[GALAGA_SPRITES][16][16]; /* pens, [py][px] */
    uint8_t tile_lookup[256], sprite_lookup[256];
    uint32_t palette[32];                    /* 0xRRGGBB */
    struct {
        uint16_t step;                       /* its step in the sequence */
        uint8_t latch, want;                 /* shown when $A003 + latch equals want */
        uint32_t rgb;
    } stars[GALAGA_STARS];
    uint32_t screen[GALAGA_H][GALAGA_W];     /* the last picture drawn */
    uint8_t drawn[GALAGA_H][GALAGA_W];       /* a tile or sprite drew the pixel */
} GalagaVideo;

/* The board's memory a picture is drawn from. */
typedef struct {
    const uint8_t *video;   /* $8000-$87FF */
    const uint8_t *ram[3];  /* $8800, $9000, $9800 (1 KB each) */
    const uint8_t *vlatch;  /* $A000-$A007, bit 0 each */
    int star_origin;        /* the starfield's step at the frame's start */
} GalagaFrame;

/* Decode the graphics and PROMs from the ROM set. */
void galaga_video_init(GalagaVideo *v, const RomSet *rom);
/* The same from the extracted assets in dir (gfx/tiles.txt, sprites.txt,
   palette.txt: assets/extract.h). Returns 0, or -1 if missing or bad. */
int galaga_video_load(GalagaVideo *v, const char *dir);
/* Draw a frame into v->screen. */
void galaga_draw(GalagaVideo *v, const GalagaFrame *f);
/* The starfield's origin after a vertical blank, from the speed in $A000-$A002. */
int galaga_star_advance(int origin, const uint8_t *vlatch);

#endif
