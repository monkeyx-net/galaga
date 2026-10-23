Galaga (native SDL2 build)
==========================

Converted by monkeyx.

Work in progress. Run ./galaga, or galaga.exe on Windows (add --windowed
for a window on a desktop). The game is rewritten in C; it reads only the
assets it extracts once from your ROM set on the first run (to assets/
next to the program, or the save folder if that is read-only; or run
./galaga --extract galaga.zip). Without a set it shows the test card (the
ROM status, an input tester and the graphics presets). --emulate runs your
set's own code on an emulated Galaga board instead (checked against MAME).

ROM set
  The game needs your own MAME galaga ROM set. Put galaga.zip (or a
  galaga/ folder) next to the program, set GALAGA_ROM to its path, or
  start with --rom /path/to/galaga.zip. FBNeo sets and old file names
  work too: ROMs are matched by checksum. Only the emulated board
  (--emulate) needs the device ROMs, namco51.zip and namco54.zip, next to
  galaga.zip.
  Check a set with:
  ./galaga --check-rom galaga.zip

Controls (one control panel: the players take turns, as on the cabinet)
  Left/right: left stick / D-pad; keyboard Left Right
  Fire: A or B; keyboard Space or Ctrl
  Coin: Select / 5
  Start: Start / 1 or Enter;  2-player start: L1+Start / 2
  Graphics: Select+L1 / F3 steps the presets, F4 glow, F5 CRT scanlines
  Operator menu (the DIP switches): Select+R1 / F2
  Exit: hold Select+Start for 1 second / Esc
  Every connected pad drives the panel.

Settings and saves
  galaga.cfg holds the DIP switches (difficulty, ships, bonus ship,
  coinage, demo sounds, cabinet, freeze), MAME's defaults until changed;
  edit it or use the operator menu, which the game waits for. The cocktail
  cabinet turns the screen round for player two's turns. galaga.hiscore
  keeps the top five, which the arcade forgets at power-off. Both live in
  the save folder: $GALAGA_SAVE_DIR, else $XDG_DATA_HOME/galaga, else
  %APPDATA%\galaga on Windows, else ~/.local/share/galaga (PortMaster:
  conf/ in the port's folder).

Graphics presets (--gfx <preset>)
  classic, classic-crt, crisp, smooth, hd-crisp, hd-smooth, hd-glow, hd-glow-crt

See NOTICE for the third-party material this project refers to.
