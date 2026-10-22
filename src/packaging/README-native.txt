Galaga (native SDL2 build)
==========================

Converted by monkeyx.

Work in progress. Run ./galaga, or galaga.exe on Windows (add --windowed
for a window on a desktop). This build shows the test card: the ROM
status, an input tester and the graphics presets. The emulated Galaga
board and the game rewritten in C follow.

ROM set
  The game needs your own MAME galaga ROM set. Put galaga.zip (or a
  galaga/ folder) next to the program, set GALAGA_ROM to its path, or
  start with --rom /path/to/galaga.zip. FBNeo sets and old file names
  work too: ROMs are matched by checksum. The emulated board also needs
  the device ROMs, namco51.zip and namco54.zip, next to galaga.zip.
  Check a set with:
  ./galaga --check-rom galaga.zip

Controls (one control panel: the players take turns, as on the cabinet)
  Left/right: left stick / D-pad; keyboard Left Right
  Fire: A or B; keyboard Space or Ctrl
  Coin: Select / 5
  Start: Start / 1 or Enter;  2-player start: L1+Start / 2
  Graphics: Select+L1 / F3 steps the presets, F4 glow, F5 CRT scanlines
  Exit: hold Select+Start for 1 second / Esc
  Every connected pad drives the panel.

Graphics presets (--gfx <preset>)
  classic, classic-crt, crisp, smooth, hd-crisp, hd-smooth, hd-glow, hd-glow-crt

See NOTICE for the third-party material this project refers to.
