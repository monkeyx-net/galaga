# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

A native C11 / SDL2 port of Galaga (Namco, 1981; MAME `galaga`, Namco
rev. B) for Linux handhelds (aarch64: ArkOS, ROCKNIX, muOS, Knulli), x86_64
(Steam Deck, desktops), Windows, macOS and the web. **The plan, decisions,
hardware notes and test plan live in
[GALAGA_PORT_PLAN.md](GALAGA_PORT_PLAN.md)**; read it before larger
changes and keep its phase table and hardware section up to date.

The repo started as a copy of the sibling Galaxian port's skeleton
(`../galaxian`: platform layer, loaders, framebuffer, enhanced graphics,
packaging, CI, MAME harness), itself from the Defender port. Improvements
to that shared code may be worth carrying across (the plan lists some),
but ask first.

## Ground Rules

- **The game is being rewritten in C** (plan decision 6): game logic from
  the ROM's behaviour, checked frame by frame against the emulated board
  (`src/emu/`). Until it reaches parity the emulated board runs the game.
  MAME is the reference for the emulation: changes to `emu/` must keep the
  `mame` tests passing. Write all emulation from the hardware's behaviour,
  not from MAME's source.
- **The ROM set is read once** (plan decision 1): `galaga --extract`
  writes tiles, sprites, font, palette, the colour lookups, the WSG
  waveforms and tables to `assets/`, which the C game reads instead of the
  ROM. No ROM-derived data in the repo, the binary or the packages; the
  extractor's tables name ROM addresses only. The 54xx's noise is a model
  of MAME's output: nothing is extracted for it.
- **The device ROMs** (`51xx.bin` in `namco51.zip`, `54xx.bin` in
  `namco54.zip`) are needed only by `--emulate` and the MAME tests; the C
  game and players need only `galaga.zip`.
- **All new code and tooling is C** (built with CMake). The one exception
  is MAME probe scripts, which must be Lua; they only dump to text files,
  and C or CMake does the comparing.
- New files go in `src/`, `CMakeLists.txt`, `.github/` or
  `GALAGA_PORT_PLAN.md`. Ask before adding files elsewhere.
- The user's ROM sets live in `roms/` (git-ignored); never commit ROMs or
  zips, never send them anywhere, and never overwrite or replace the files
  there.

## Commands

```bash
cmake -S . -B build && cmake --build build -j     # build
ctest --test-dir build                            # all tests
ctest --test-dir build -L rom                     # tests needing the ROM set (roms/galaga.zip)
ctest --test-dir build -L mame                    # tests needing MAME + the ROM sets (galaga, namco51, namco54)
build/galaga --windowed                           # play (dev builds find build/assets, roms/galaga.zip)
build/galaga --windowed --emulate                 # the ROM's own code on the emulated board
build/galaga --windowed --test-card               # the test card (ROM status, input tester)
build/galaga --check-rom roms/galaga.zip          # verify a ROM set (and the device ROMs next to it)
build/galaga --dump-rom roms/galaga.zip [region] # MAME's regions as hex lines (maincpu sub sub2 gfx1 gfx2 proms namco 51xx 54xx)
build/galaga --extract roms/galaga.zip [dir]      # assets from the ROM set (dev builds: build/assets after each build)
SDL_VIDEODRIVER=dummy build/galaga --frames 60 --screenshot out.png   # headless screenshot
cmake --build build --target package_linux package_portmaster   # release archives
cmake -S . -B build-san -DGALAGA_SANITIZE=ON      # ASan/UBSan build
emcmake cmake -S . -B build-web && cmake --build build-web --target serve   # web build on http://localhost:8000/
```

Release builds: `-DGALAGA_DEV_ASSETS=OFF` (no source-tree fallbacks).
Until the game is in C, the program runs the ROM on the emulated board
(with its sound) when it finds the set and the device ROMs, else the test
card. Commands for phases not yet done (see the plan's table) arrive with
them.

## Layout (`src/`)

- `main.c` - SDL setup, the fixed 60 Hz loop running a board frame per
  tick (`--emulate`; the C game later), the graphics pipeline, the test
  card (`--test-card`, or no ROM set or device ROMs), `--frames`,
  `--screenshot`, `--rom`, `--check-rom`, `--dump-rom`; later the DIP
  switches (`galaga.cfg`), the high scores (`galaga.hiscore`)
- `platform/` - the only code that includes SDL: `video.c` (224x288
  portrait framebuffer, integer scaling or fitted with sharp bilinear,
  scanlines down the screen), `input.c` (Galaga's control panel; pads and
  keyboard), `audio.c` (streams generated samples)
- `gfx/` - draws into the framebuffer in arcade coordinates (`fb.scale` > 1
  for HD): `fb.c`, `blit.c`, `scale.c` (Scale2x/3x, xBR), `enhance.c`
  (graphics presets, HD image registry, whole-frame filter, glow),
  `debugfont.c` (3x5 font for diagnostics, not the ROM's), `galaga.c`
  (a frame from the video RAM, sprite RAM and latches: tiles, sprites
  with their flips and doubling, the PROM colours, the 05xx's stars from
  its shift register)
- `assets/` - `rom.c` (the galaga set and the device ROMs from zips or a
  folder, matched by CRC), `zip.c`, `inflate.c`, `png.c`, `assets.c`
  (asset folder lookup), `savefile.c` (the save folder), `extract.c`
  (tiles, sprites, palette and lookups, font, waveforms, tables, previews;
  `manifest.txt` written last)
- `emu/` - the emulated board: `z80.c` (the CPU, each memory access at
  its T-state), `mb88.c` (the MB8843/MB8844 core of the 51xx and 54xx),
  `board.c` (three Z80s, the shared RAM and video RAM, the latches, the
  06xx bus to the 51xx and 54xx, interrupts, the watchdog, the starfield's
  origin, the sound's writes timed in the frame; MAME's scheduler:
  attosecond time, slices, syncs, Z80s stopping between machine cycles),
  `wsg.c` (the Namco WSG), `noise54.c` (the 54xx's noise through two
  band-passes, modelled from MAME's output; 792 samples a frame at 48 kHz)
- `sim/` (C0-C3, under way) - the game in C: `main_run.c`, `sub_run.c`,
  `sub2_run.c` (each Z80 program whole, every instruction a case of one
  resumable function, its cycles, its accesses' machine cycles and F
  exact), `m51_run.c`, `m54_run.c` (the 51xx's and 54xx's MB88 programs
  the same way), all generated from the ROMs' code and read; `cpu.h`
  (registers, flags, the stops between machine cycles), `mcu.h` (the MB88
  state and ports), `game.c` (the board's memory as the ROMs lay it out,
  the ROMs' tables), `sim.c` (the board's scheduler and devices as
  `board.c` has them, a frame at a time)
- `tests/` - C tests, MAME probes (`tests/mame/`)
- `packaging/` - release layout script, PortMaster launcher and metadata
- `web/` - page shell for the Emscripten build

## Conventions

- C11, `-Wall -Wextra`, no warnings. Builds must also pass in Ubuntu 20.04
  (GCC 9, SDL 2.0.10, the PortMaster base): use only the SDL 2.0.10 API.
- The board is deterministic: one frame per tick, no wall clock.
- Enhanced graphics are cosmetic: classic output stays the arcade frame.
- Match the surrounding code: comment density, naming, idiom.
- Each phase gets its own commit; nothing is pushed or published without
  asking.

## Testing

Every change keeps `ctest` green. Tests arrive with their phase (plan
section 4).

- `enhance` - always runs (scalers, presets, glow)
- `z80` - always runs: every opcode's T-states and machine cycles, known
  flag results, NMI, IM 1, HALT
- `mb88` - always runs: the MB88xx opcodes' results, flags and cycles
- `rom_load` - label `rom`: loads the set from the zip, from renamed copies
  in a folder (CRC matching), the device ROMs optional, and fails cleanly
  on an empty folder
- `extract` - label `rom`: two extractions are byte-identical; what is
  read back (graphics, colours, waveforms, tables) is the ROM set's; the
  font reads the title screen the emulated board draws; no manifest
  without a ROM set
- `mcu` - label `rom`: the C 51xx and 54xx programs step for step against
  `emu/mb88.c` with the device ROMs, under random inputs
- `frame` - label `rom`: the C game against the board from power-on, the
  tests' player in an attract cycle and a game: memory, the latches and
  the WSG equal at every frame (`TEST_FRAME_TRACE` to debug a difference)
- `mame_rom_map` - label `mame`: MAME's view of every region equals
  `galaga --dump-rom`
- `mame_capture` - label `mame`: runs the scripted game in
  `src/tests/mame/script.cmake`, dumping video RAM, RAM and the latches
  with a snapshot at 15 frames; each frame drawn from its dump equals
  MAME's (bar the stars)
- `mame_emu` - label `mame`: the board replays that script from power-on:
  MAME's traces of frames 1700-1800 (main, sub, sub2, 54xx instruction for
  instruction; the 51xx reported only, plan section 2); video RAM, all
  RAM, the latches and every pixel, stars included, at every captured
  frame; the sound against MAME's recording (levels within 3 dB)
- To debug a difference: `test_emu` with `DUMP_DIR=dir` (its pictures);
  `trace_probe.lua` with `TRACE_CPU`, `TRACE_FROM`/`TRACE_TO` (needs
  `-debug -debugger none`) against `Board.step_hook`; `dasm_probe.lua` for
  the five programs' disassembly. Traces, dumps and disassembly stay in
  `build/`. MAME tests share `build/mame` (one at a time); tests that run
  the game use their own save dir; never write to the user's home folder.

## Reference

- ROM: MAME `galaga` (0.289 names); CRCs in `src/assets/rom.c`. Device
  ROMs: `namco51` (51xx.bin c2f57ef8), `namco54` (54xx.bin ee7357e0).
- Hardware: Namco Galaga board (three Z80s at 3.072 MHz sharing RAM; the
  06xx, 51xx, 54xx and 05xx customs; tiles, 64 sprites, starfield; the
  Namco WSG and the 54xx's noise). Notes go in the plan as they are
  confirmed.
- MAME 0.289 runs it headless; see `src/tests/mame/mame_common.cmake` for
  the options.
