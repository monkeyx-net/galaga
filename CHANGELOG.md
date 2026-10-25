# Galaga — native SDL2 port — Changelog

---
## Changelog

### v0.1.2 (2026-10-25)

#### Port

- **Screen scaling and rotation** — `--scale auto|integer|fit|stretch`
  (auto as before: an integer scale when it fills the screen, else fitted
  with sharp bilinear) and `--rotate 0|90|270` for a landscape device held
  on its side, the joystick turned with the picture; F6, F7 or the
  operator menu change them, saved to `galaga.cfg`. Turned, 640x480 gets an
  exact 2x and 1280x720 an exact 3x.
- **Window size** — `--windowed` opens at the largest integer scale that
  fits the desktop.

### v0.1.1 (2026-10-23)

#### Game

- **The emulated board** — three Z80s, the 51xx and 54xx MCUs (MB88xx),
  the 06xx bus, video, starfield and MAME's scheduler, checked against
  MAME 0.289 instruction for instruction (`--emulate`); the Namco WSG and
  the 54xx's noise modelled from MAME's output.
- **Assets extracted once** — tiles, sprites, palette, font, waveforms and
  the programs' tables from the player's `galaga.zip` (`--extract`, or on
  the first run; to the save folder when the program's is read-only).
- **The game in C by default** — the three Z80 programs and the two MCU
  programs rewritten in C on the board's scheduler: memory, latches and
  sound equal to the emulated board's at every frame through a game. It
  needs only `galaga.zip`, not the device ROMs.
- **Cocktail table** — player two's turns turned round as MAME draws
  them.

#### Port

- **Operator menu** — F2 (Select+R1 on a pad) opens the DIP switches over
  the game, which waits: difficulty, ships, bonus ship, coinage, demo
  sounds, cabinet, freeze; saved to `galaga.cfg`.
- **High scores kept** — the top five with initials in `galaga.hiscore`.
- **Controls page** — shown over an idle attract mode.
- **Lower CPU use** — the main loop sleeps until the next frame is due
  instead of redrawing and presenting again (no spinning where vsync does
  not block, no extra frames on 120/144 Hz displays); the graphics
  presets' scalers (Scale2x/3x, xBR) read their neighbours without bounds
  checks inside the picture, write flat areas at once and skip the
  premultiply pass for the opaque game frame. Pictures unchanged, bit for
  bit.

### v0.1.0 (unreleased)

#### Port

- **Skeleton** — the platform layer, loaders, framebuffer, enhanced
  graphics, packaging, CI and MAME test harness from the sibling Galaxian
  port, for a 224x288 portrait screen (integer scaling, or fitted with
  sharp bilinear on screens such as 640x480; scanlines down the screen).
- **ROM set** — the MAME `galaga` set from a zip or folder, matched by
  CRC, and the 51xx and 54xx device ROMs (`namco51.zip`, `namco54.zip`)
  where found; `--check-rom`, `--dump-rom` (checked against MAME by
  `mame_rom_map`).
- **Test card** — ROM status, an input tester for Galaga's panel, the
  graphics presets.
