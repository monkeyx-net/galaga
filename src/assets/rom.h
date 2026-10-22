/*
 * The user's Galaga ROM set (MAME `galaga`, Namco rev. B), read from a
 * folder or a zip, and the device ROMs of the 51xx and 54xx customs (MAME
 * `namco51`, `namco54`). Files are matched by CRC-32, so any naming works
 * (MAME's current names, older sets, FBNeo).
 *
 * The regions are MAME's: the three Z80s' programs (main 16 KB, sub and
 * sub2 4 KB), the tiles (gfx1) and sprites (gfx2), the colour PROMs
 * (palette, then the tile and sprite lookups) and the sound PROMs (the
 * WSG's waveforms, then prom-2). The device ROMs are the MB8843 / MB8844
 * programs; only the emulated board needs them (plan decision 8), so they
 * are optional: found in the set itself, in namco51.zip / namco54.zip next
 * to it, or in a folder.
 */
#ifndef GALAGA_ROM_H
#define GALAGA_ROM_H

#include <stddef.h>
#include <stdint.h>

#define ROM_MAIN_SIZE 0x4000  /* main CPU, $0000-$3FFF */
#define ROM_SUB_SIZE 0x1000   /* sub CPU, $0000-$0FFF */
#define ROM_SUB2_SIZE 0x1000  /* sub2 CPU, $0000-$0FFF */
#define ROM_GFX1_SIZE 0x1000  /* 256 8x8 tiles, 2 bits a pixel */
#define ROM_GFX2_SIZE 0x2000  /* 128 16x16 sprites, 2 bits a pixel */
#define ROM_PROMS_SIZE 0x220  /* palette $000, tile lookup $020, sprite lookup $120 */
#define ROM_NAMCO_SIZE 0x200  /* WSG waveforms $000, prom-2 $100 */
#define ROM_MCU_SIZE 0x400    /* each of the 51xx's and 54xx's programs */

typedef struct {
    uint8_t main[ROM_MAIN_SIZE];
    uint8_t sub[ROM_SUB_SIZE];
    uint8_t sub2[ROM_SUB2_SIZE];
    uint8_t gfx1[ROM_GFX1_SIZE];
    uint8_t gfx2[ROM_GFX2_SIZE];
    uint8_t proms[ROM_PROMS_SIZE];
    uint8_t namco[ROM_NAMCO_SIZE];
    uint8_t mcu51[ROM_MCU_SIZE];
    uint8_t mcu54[ROM_MCU_SIZE];
    int loaded;
    int has_51xx, has_54xx; /* the device ROMs were found */
} RomSet;

/* Load the set from a folder or a zip, and the device ROMs where found.
   Returns 0, or -1 with a message in err (the galaga files are required). */
int rom_load(RomSet *rom, const char *src, char *err, int errlen);
/* Byte at a main CPU address in its program ROM ($0000-$3FFF); -1 elsewhere. */
int rom_byte(const RomSet *rom, unsigned addr);

/* The regions by MAME's names (maincpu sub sub2 gfx1 gfx2 proms namco 51xx
   54xx), in that order: ROM_REGIONS of them. */
#define ROM_REGIONS 9
const char *rom_region_name(int i);
/* Region i's bytes and size; NULL for a device ROM that was not found. */
const uint8_t *rom_region(const RomSet *rom, int i, unsigned *size);

/* Where the ROM set is: $GALAGA_ROM, else galaga.zip or a galaga/ folder next to
   the program or in the current folder (development builds: also in the
   source tree's roms/). Returns 0 if found. */
int rom_locate(const char *argv0, char *out, size_t outlen);

#endif
