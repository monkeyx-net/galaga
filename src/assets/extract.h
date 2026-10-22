/*
 * Extractor: reads the user's ROM set once and writes what the game draws
 * and plays under <out>/ (GALAGA_PORT_PLAN.md, decision 1). After that the
 * game in C needs only <out>/, never the ROM set. Nothing here is compiled
 * in: the data comes from the player's set. The 54xx's noise is generated
 * (emu/noise54.c), so nothing is extracted for it.
 *
 *   gfx/tiles.txt       the 256 tiles of gg1_9, as the player sees them:
 *                       "tile NN" then 8 rows of 8 pens (0-3)
 *   gfx/sprites.txt     the 128 sprites of gg1_11 and gg1_10: "sprite NN"
 *                       then 16 rows of 16 pens
 *   gfx/palette.txt     the 32 colours of prom-5 (RRGGBB, through the
 *                       board's resistor weights as MAME has them), then
 *                       the tiles' and the sprites' colour lookups (prom-4,
 *                       prom-3: 256 entries each, 15 transparent)
 *   gfx/font.txt        the game's characters: "NN c", NN the tile
 *   gfx/<name>.png      previews, as the player sees them (not read by the
 *                       game): tiles, sprites, and with the device ROMs the
 *                       attract mode's screens from the emulated board
 *   sound/waves.txt     the WSG's 8 waveforms of 32 samples (prom-1.1d)
 *   tables.txt          the program ROMs' tables the game in C reads:
 *                       "# what" then "ADDR: bytes" lines, 16 bytes each,
 *                       for each range listed in extract.c (addresses only
 *                       there; main's program, then sub's at +$4000 and
 *                       sub2's at +$5000)
 *   manifest.txt        written last: the folder is complete
 */
#ifndef GALAGA_EXTRACT_H
#define GALAGA_EXTRACT_H

#include "assets/rom.h"

#define EXTRACT_MANIFEST "manifest.txt"
#define EXTRACT_VERSION 1

/* The programs' address space in tables.txt: main, sub, sub2 */
#define EXTRACT_SUB_BASE 0x4000
#define EXTRACT_SUB2_BASE 0x5000
#define EXTRACT_SPACE 0x6000

/* Write everything to out_dir (created if needed). Returns 0, or -1 with a
   message in err; without manifest.txt the folder counts as missing. */
int extract_all(const RomSet *rom, const char *out_dir, char *err, int errlen);

/* 1 if the byte at addr (main, or sub / sub2 from their bases) is in a
   table the extractor writes. */
int extract_is_table(unsigned addr);
/* Read tables.txt under dir into data (indexed as above, the rest 0).
   Returns 0, or -1 if missing or incomplete. */
int extract_load_tables(const char *dir, uint8_t *data, unsigned size);
/* Read sound/waves.txt under dir: 256 samples, waveform-major. 0 or -1. */
int extract_load_waves(const char *dir, uint8_t *waves);

/* The game's characters in tile order from code 0: the digits, the
   alphabet and a space ($00-$24). */
#define EXTRACT_FONT_CHARS "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ "

#endif
