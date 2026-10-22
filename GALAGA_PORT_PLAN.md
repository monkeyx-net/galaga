# Galaga native port: plan

A C11 / SDL2 port of Galaga (Namco, 1981; MAME `galaga`, Namco rev. B)
that plays like the arcade game, for PortMaster handhelds (aarch64),
desktops (x86_64 Linux, Windows, macOS) and the web. It is converted the
same way as the sibling Galaxian port (`../galaxian`): emulate the board,
check the emulator against MAME 0.289, extract the assets once, then
rewrite the game in C, checked frame by frame against the emulated board.

The repository started on 2026-10-22 as a copy of Galaxian's skeleton
(platform layer, loaders, framebuffer, enhanced graphics, packaging, CI,
MAME harness), itself from the Defender, Joust and Robotron ports. The
hardware (three Z80s, the Namco customs and their MB88xx MCUs, the video,
the WSG and the 54xx's noise) is new.

## 1. Decisions (2026-10-22)

1. **The ROM set is read once** (as Galaxian's decision 1). The binary
   never embeds ROM-derived data; it loads it at run time from an `assets/`
   folder that `galaga --extract <galaga.zip>` writes from the player's
   own ROM set (the PortMaster launcher runs it on the first start). The
   repo, CI and releases carry no ROM-derived data: not tiles, sprites, the
   font, the palette, the colour lookups, the waveforms or tables; the
   extractor's tables name ROM addresses only. Until the game is in C, the
   emulated board runs the ROM's code and reads the ROM set.
2. **MAME is the reference.** Behaviour is established by probing the ROM
   in MAME 0.289 and checked by CTest against the port. Probes are Lua and
   only dump; C or CMake compares. Emulation is written from the hardware's
   behaviour, not from MAME's source.
3. **Shared code is copied, not linked**, from `../galaxian/src` (renamed
   galaxian → galaga, GALAXIAN_ → GALAGA_): `platform/`, `gfx/` (bar
   `galaxian.c`), `assets/` (with a new `rom.c`), `packaging/`, `web/`,
   CI, the MAME CMake and Lua harness; the Z80 core in E1. Fixes found here
   that apply there are listed in section 5, to carry across once agreed.
4. **Controls:** one control panel (the players take turns), as the upright
   cabinet: left/right and fire. Pad: stick or D-pad left/right, A or B
   fire; every pad drives the panel. Select = coin, Start = 1P, L1+Start =
   2P, Select+L1 = graphics presets, hold Select+Start 1 s = exit.
   Keyboard: arrows left/right, Space/Ctrl fire, 5 coin, 1/Enter and 2
   start, F3 presets, F4 glow, F5 scanlines, Esc exit. The DIP switches
   (rank, ships, coinage, bonus, cabinet, demo sound, freeze) come from a
   settings file (`galaga.cfg`) with MAME's defaults; an operator menu
   (F2, Select+R1) changes them while the game waits and saves the file.
5. **Enhanced graphics** as in the Galaxian port (FILTER and HD presets,
   glow, scanlines), cosmetic only, classic by default. The screen is
   portrait (224x288, the monitor turned 90 degrees): integer scaling where
   it fills the screen, else fitted (640x480 handhelds: 1.667x, sharp
   bilinear, pillarboxed); the scanlines run down the screen.
6. **The game in C.** The game logic is rewritten in C (`src/sim/`), from
   the ROM's behaviour (traces, probes, the disassembly MAME writes to
   `build/re`, never committed), routine by routine, each mirroring the
   ROM's arithmetic. The emulated board (`src/emu/`, checked against MAME
   by `mame_emu`) stays as the reference the C game is compared with.
7. **High-score saving** (a port feature: the arcade forgets its table at
   power-off): the top five with initials, `galaga.hiscore` in the save
   folder.
8. **The device ROMs** (`51xx.bin` from `namco51.zip`, `54xx.bin` from
   `namco54.zip`, the MB8843 / MB8844 programs of the 51xx and 54xx) are
   loaded only when present: the emulated board (`--emulate`) runs them
   on an MB88xx core, and the MAME tests need them. The C game does not:
   players need only `galaga.zip`. The loader finds them in `galaga.zip`,
   their own zips next to it, or a folder, by CRC.
9. **The WSG's waveforms are extracted** (`prom-1.1d`, 8 waveforms of 32
   4-bit samples): unlike Galaxian's discrete circuit, Galaga's tones are
   ROM data, so they go to `assets/` with the rest. The 54xx's explosions
   (its noise through MAME's discrete filters) are modelled from MAME's
   output, as Galaxian's sound was.

## 2. Hardware

Confirmed so far (MAME 0.289's view, by `-listxml` and probe):
- ROM set (`rom_load`, `mame_rom_map`), MAME region and offset:
  - `:maincpu` (16 KB): `gg1_1b.3p` ab036c9f ($0000), `gg1_2b.3m` d9232240
    ($1000), `gg1_3.2m` 753ce503 ($2000), `gg1_4b.2l` 499fcc76 ($3000)
  - `:sub` (4 KB): `gg1_5b.3f` bb5caae3; `:sub2` (4 KB): `gg1_7b.2c`
    d016686b
  - `:gfx1` (tiles, 4 KB): `gg1_9.4l` 58b2f47c; `:gfx2` (sprites, 8 KB):
    `gg1_11.4d` ad447c80 ($0000), `gg1_10.4f` dd6f1afc ($1000)
  - `:proms` (544 bytes): `prom-5.5n` 54603c6b (palette, $000, 32 bytes),
    `prom-4.2n` 59b6edab (character lookup, $020), `prom-3.1c` 4a04bb6b
    (sprite lookup, $120)
  - `:namco` (512 bytes): `prom-1.1d` 7a2815b4 (WSG waveforms, $000),
    `prom-2.5c` 77245b66 ($100, not used by the sound)
  - device ROMs: `51xx.bin` c2f57ef8 (`:51xx:mcu`, 1 KB), `54xx.bin`
    ee7357e0 (`:54xx:mcu`, 1 KB)
  - MAME's CPU regions are 64 KB (the ROM at the start). Its `:gfx1` is
    the file with the two 8-byte halves of each tile in $800-$FFF swapped
    (rearranged at load; probe): the port keeps the file's order, and
    `rom_map_probe.lua` reads that half back in it.
- CPUs (`-listxml`): three Z80s (`maincpu`, `sub`, `sub2`) at 3.072 MHz;
  the 51xx's MB8843 and 54xx's MB8844 at 1.536 MHz. Screen: 288x224
  visible of 384x264 at 6.144 MHz (60.606 Hz), turned 90 degrees. Sound:
  the Namco WSG (clock 96 kHz) and a discrete circuit, one channel.
- DIP switches (MAME's ports DSWA, DSWB; defaults): DSWA difficulty 03
  (Easy 03 default, Medium 00, Hard 01, Hardest 02), unused 04, demo
  sounds 08 (on = 0, the default), freeze 10, rack test 20, unused 40,
  cabinet 80 (upright set). DSWB coinage 07 (1C/1C = 7 default), bonus
  life 38 (by the lives setting; 20K, 70K, every 70K = 10 by default with
  3 ships), lives C0 (2 = 00, 3 = 80 default, 4 = 40, 5 = C0). Service
  mode is IN1 bit 7 (off = set).

Confirmed in E1 (`mame_emu`: the three Z80s and the 54xx instruction for
instruction over 100 frames of play, cycles and registers; video RAM,
all RAM, the latches and every pixel, stars included, at 15 frames from
power-on to over a minute of play):
- Memory map (probe of MAME's address maps; the same for the three Z80s):
  ROM $0000-$3FFF (sub and sub2: 4 KB, then 0), DIP switches $6800-$6807
  (bit 0: DSWB bit n, bit 1: DSWA bit n; the rest 0), the WSG $6800-$681F
  (writes), the misc latch $6820-$6827 (bit 0), the watchdog $6830, the
  06xx data $7000-$70FF and control $7100, video RAM $8000-$87FF, RAM
  $8800-$8BFF, $9000-$93FF, $9800-$9BFF, the video latch $A000-$A007 (bit
  0). Unmapped reads give 0.
- Power-on: main runs (AF $0040, IX = IY = $FFFF, the rest 0); sub, sub2
  and both MCUs are held in reset until $6823 = 1, which main writes at
  frame 699 after its RAM test. Released, a CPU starts at the scheduler's
  time (it kept none while held).
- Interrupts: at the vertical blank (line 224, where frames start) main
  and sub get an IRQ (IM 1) if $6820 / $6821 is set, held until 0 is
  written there; sub2 gets an NMI at lines 64 and 192 unless $6822 is set.
- The 06xx: a control write (bits 0-3 chips, 51xx = 0, 54xx = 3; bit 4
  read; bits 5-7 n) starts a clock of period 64 << n CPU cycles at the next
  edge of its 48 kHz clock (edges at k / 48000 s); control $x0 stops it.
  The select lines are high for the first half of each period; main gets
  an NMI at each rise (read mode: from the second). A data write latches
  the byte in each selected chip; a read returns the selected chip's O port
  and latches that too. The 51xx's K port is (read bit << 3) | latch bits
  0-2, the 54xx's K its latch's high nibble and R0 its low. Control and data
  writes take effect at a sync (below).
- The 51xx: R0-R3 = IN0 low, IN0 high, IN1 low, IN1 high (active low); its
  timer counts vertical blanks; it answers on O. The 54xx drives the noise
  (E2).
- The MB88xx (both MCUs, 1.536 MHz / 6): MAME's traces of both, replayed
  through the core with their port reads, match every register. The PC
  counts on into the next page (JMP stays in its page); the external
  interrupt is taken after the instruction under way, in 3 cycles, to
  $002 with ST set; NF follows the pin; the serial unit (SF) is not
  modelled (unread).
- The Z80, beyond Galaxian's core: LDIR / LDDR / CPIR / CPDR going round
  again take X and Y from the instruction's address bits 11 and 13; SCF and
  CCF take them from (Q xor F) or A, Q being F when the last instruction
  set the flags.
- Watchdog (probe: main halted): the 8th vertical blank without a write to
  $6830 resets the board: main from that vertical blank, the latches
  cleared (so the others go back into reset), the 06xx stopped; RAM kept.
- Video (`gfx/galaga.c`; `mame_capture` and `mame_emu`): 224x288 portrait.
  Tiles: rows 2-33 at $040 + (27 - x) x 32 + (y - 2), rows 0-1 and 34-35 at
  $3C0 / $3E0 / $000 / $020 + 29 - x, colour at +$400; a tile's pixel (px,
  py) is bits 3 - py % 4 and 7 - py % 4 of byte 7 - px (+8 when py < 4).
  Sprites (64; $8B80 code and colour, $9380 position, $9B80 flags, + 2i):
  16x16, pixel (px, py) from byte (15 - px, or 24 + 15 - px past 8) + 8
  (py / 4); at x = byte 0 - 17, y = byte 1 + 256 x ($9B81 bit 0) - 40;
  flags bit 0 flips top to bottom, bit 1 left to right, bit 2 doubles
  down, bit 3 across (code bit 0 the lower half, bit 1 the left, each
  turned by its flip). Colour c, pen p: tiles prom-4[4c + p] into palette
  16-31, sprites prom-3[4c + p] into 0-15; 15 transparent. Palette:
  red and green bits weigh 33, 71, 151; blue 81, 174. Tiles over sprites
  over stars. MAME draws the frame at the vertical blank from memory then.
- Stars (the 05xx; identified from MAME's pictures: every star of the
  65535-step cycle, then the one 16-bit register that gives them): a
  shift register taking in at bit 15 the parity of bits 0, 3, 5 and 10;
  256 steps a line along y 16-271, lines from x 223 down to 0. A step is a
  star when bits 0, 3, 5, 10 are clear and 12-15 set; colour red ~bits 6,
  7, green ~bits 8, 1, blue ~bits 2, 4 (0, $47, $97, $DE; blue 0, $51, $AE,
  $FF); shown when $A003 (bit 11 clear) or $A004 (set) equals bit 9, with
  $A005 set. At each vertical blank the origin moves -1, -2, -3, -4, +3,
  +2, +1 or 0 steps by $A000-$A002 (shown or not); at power-on it is 1579
  steps on from where the register holds 1 (MAME's).
- Inputs a MAME probe sets in its frame callback reach the game a frame
  later.

How MAME runs the board, which the board copies to be equal to it
instruction for instruction (each point found where a trace differed):
- Time in attoseconds: the Z80's cycle 325520833333 (1 / 3.072 MHz,
  truncated), the MCU's a hair under 12 of those (1 to 4096 as under fit
  every run so far; the board uses 1); the frame 384 x 264 pixels of
  162760416666; slices of 1/6000 s.
- Each slice runs main, sub, sub2, 51xx, 54xx to its end (a timer's time
  if sooner); a CPU's cycles to run are its time to the end divided by its
  cycle cut to 32 bits (MAME's divisor, which rounds up a hair short of a
  whole cycle); a CPU ending short of the end (its slice aborted, or the
  MCUs on their 12-cycle grid) brings the end back to its time for the
  CPUs after it. A timer that becomes the next due while a CPU runs aborts
  its slice.
- A write that changes another CPU's lines ($6820/$6821 clears, $6823) or
  the 06xx (control, data) syncs: it takes effect at a timer at the
  writer's time, once the others have caught up; the writer's slice ends
  there. Timer callbacks see their own time as now.
- A Z80 stops between machine cycles when its cycles run out: an access
  whose machine cycle would start later waits for its next slice, after
  the other CPUs (the board re-runs the instruction then, replaying the
  accesses already made).
- The 50 Hz sound update (MAME's) and screen line 0 are timers too: they
  cut slices.

Not yet like MAME:
- The 51xx's view of its select line in read mode: MAME lets it see a rise
  or fall up to an instruction later than the 06xx's clock edge in some
  transfers, which the board does not model (`mame_emu` reports the 51xx's
  first different instruction and does not fail on it). What it hands the
  Z80s, and all memory, is the same.
- The flip ($A007, the cocktail table): to do (E3).

### 2.1 Sound (E2, measured from MAME's output)

`mame_emu` checks the board's sound against MAME's recording of the
scripted run (`game.wav` from `mame_capture`): in 1/10 s windows where
MAME is not quiet, 554 of 555 within 3 dB of its level (the test asks 90%).
- The WSG (`emu/wsg.c`; probe: sub2's NMI held off so the game leaves it
  alone, the registers set from main's space, one voice at a time): the
  Pac-Man layout (waveforms at $05/$0A/$0F, voice 0's 20-bit frequency at
  $10-$14 and volume $15, voices 1 and 2 at $16-$1A and $1B-$1F with their
  frequency's low nibble 0), each voice's 20-bit accumulator adding its
  frequency 96000 times a second, its top 5 bits the sample in its
  waveform (prom-1.1d: 8 of 32 nibbles). Each voice gives (sample - 8) x
  volume x 48 in MAME's output units, summed, no filter (the DC stays):
  waveform 0 at volume 1 peaks at +288 and -384, levels linear in volume,
  0x2000 plays 750 Hz. MAME's resampler smooths it into 48 kHz; the board
  averages two steps a sample.
- The 54xx's explosions (`emu/noise54.c`; MAME's output with the WSG's
  volumes forced to 0 by a write tap): the MCU (already exact) drives two
  4-bit levels on its O port, its own noise already gated. A linear fit of
  MAME's output to the board's O port: the low nibble through a band-pass
  at 182 Hz (Q 2.1, peak 4125 a step), the high nibble at 517 Hz (Q 1.6,
  986): 86% of its power, every 1/10 s window within 3 dB. The R2 bits it
  toggles add nothing reliable. The sources are summed.
- Not modelled: MAME's thump at power-on as its filters charge (the first
  1/10 s).

### 2.2 The ROM set's data (X1, `extract`)

- `galaga --extract` writes `gfx/tiles.txt` (the 256 tiles as the player
  sees them), `gfx/sprites.txt` (the 128 sprites), `gfx/palette.txt` (the
  32 colours through MAME's weights, then the tiles' and sprites' colour
  lookups), `gfx/font.txt` (the character codes: digits $00-$09, A-Z
  $0A-$23, a space $24), `sound/waves.txt` (the WSG's 8 waveforms),
  previews (tiles, sprites, and with the device ROMs the title, score table
  and demo screens from the emulated board), `tables.txt` (the programs'
  tables the C game reads: main's, then sub's at +$4000 and sub2's at
  +$5000; none yet) and `manifest.txt` last. `gfx/galaga.c` reads them back
  (`galaga_video_load`) for the game in C. Development builds extract to
  `build/assets` after each build.

### 2.3 The programs (C0-, `src/sim/`)

Read from listings built from the board's executed instructions (a Z80
disassembler over the PCs a 30000-frame scripted run reaches; `build/re`,
never committed). Each of the three Z80s has its own ROM at $0000 and its
own stack: main's at $8B00 then $90A0, sub's $9100, sub2's $9B00.
- main: reset $0000 -> $02C4 (RAM, ROM and video tests from $336C), then
  the state machine at $02D3 (the attract mode, the game) polling flags
  the tasks set. IRQ (IM 1, $0038 -> $0237): the stars' latches from
  $99BE, the watchdog, then the tasks: $9000-$901F a byte each, each one
  set run through the pointer table at $0096; then the 06xx read of the
  51xx's three bytes into $99B5 (control $71). NMI ($0066): the 06xx's
  byte mover (LDI from the queued buffer, then the next queued transfer:
  $9AB9 sends the 54xx's command from $0092).
- sub: $0000 -> $057C (its ROM's sum), IRQ ($0038 -> $0513): $92A0
  counts frames, then 8 tasks at $9020 through the pointers at $003B; the
  aliens' flight scripts are data from $001D.
- sub2 (`sim/sound.c`, all ported): $0000 -> $0085: its ROM's sum handed
  to main at $9101, then everything in the NMI ($0066, lines 64 and 192):
  the sound driver ($00BB). Sounds are asked for by flags at $9AA0-$9AB6
  (main and sub set them), started ($9AC0-$9AD6), played from headers at
  $0734 (first track, tracks, voice), track pointers at $0779 (envelope,
  decay start, waveform, then notes: pitch nibble, octave nibble, length
  in units of $07D7's frames a sound), pitches at $06DA, the beam's glide
  at $06F4 / $0704 by $9211. $9AB7 stops everything, $9AB8 restarts sub2.
  The WSG's registers are kept at $9A60-$9A72 and copied out each NMI.

To confirm by probe (written up here as confirmed):
- **High scores:** a top-five table with initials in RAM.

## 3. Phases

| Phase | Work | Exit | Status |
|---|---|---|---|
| P0 | `git init`, skeleton from Galaxian, loader for the galaga set and the device ROMs, test card, MAME ROM map, packaging, CI, `CLAUDE.md`, README | `enhance`, `rom_load`, `mame_rom_map` pass | done 2026-10-22 |
| E1 | Three Z80s, the MB88xx core (51xx, 54xx), board (shared RAM, latches, 06xx, interrupts, watchdog), video (tiles, sprites, lookups), stars; the game plays via `--emulate` (silent) | `z80`, `mb88`; `mame_emu`: the CPUs cycle for cycle over traced frames; RAM and pictures equal at captured frames (`mame_capture`) | done 2026-10-22 (the 51xx's read-mode select timing aside) |
| E2 | The WSG (waveforms from prom-1.1d), the 54xx's noise modelled from MAME's output | each voice exact; the noise's level and timing as MAME's wav | done 2026-10-22 |
| X1 | Extractor: tiles, sprites, palette, lookups, waveforms, font, tables | `extract` | done 2026-10-22 (tables as the C phases need them) |
| C0-C3 | RE notes; the C game piece by piece (the three CPUs' work), then the default | `sim`, `routines`, `frame`; the C game plays from power-on through game over | under way: the state, the checker, sub2's sound driver |
| E3/E4 | Polish (cocktail flip, devices, web), the DIP switches' menu, high-score saving, release | device checklist; published | to do |

Each phase gets its own commit. Nothing is pushed or published without
asking.

## 4. Tests

- `enhance` - always runs (scalers, presets, glow)
- `rom_load` - label `rom`: the set from the zip, from renamed copies in a
  folder (CRC matching), the device ROMs from their zips or the folder
  and optional, and a clean failure on an empty folder
- `mame_rom_map` - label `mame`: MAME's view of every region (`:maincpu`,
  `:sub`, `:sub2`, `:gfx1`, `:gfx2`, `:proms`, `:namco`, the 51xx's and
  54xx's) equals `galaga --dump-rom`
- `z80` - always runs: every opcode's T-states and machine cycles, where
  writes fall, known flag results, NMI, IM 1, HALT (from Galaxian)
- `mb88` - always runs: the MB88xx opcodes' results, flags (ST, Z, C),
  cycles; paging, JPA, CALL / RTS / RTI; the ports; the interrupt (after
  the instruction under way, 3 cycles, NF following the pin); the timer
- `mame_capture` - label `mame`: a scripted game in MAME
  (`src/tests/mame/script.cmake`: power-on, the attract mode, a coin, a
  start, over a minute of play), dumping video RAM, RAM and the latches
  with a snapshot at 15 frames; each frame drawn from the dump equals
  MAME's snapshot (bar the stars, whose origin is not dumped)
- `mame_emu` - label `mame`: the board replays the script from power-on:
  MAME's traces of frames 1700-1800 (`trace_probe.lua`, a CPU a run): main,
  sub, sub2 and the 54xx instruction for instruction (cycle and registers;
  the 51xx reported only); at every captured frame video RAM, all RAM, the
  latches and the picture, stars included, equal MAME's. `DUMP_DIR` writes
  the board's pictures. The sound against MAME's recording of the run
  (`game.wav`): in 1/10 s windows where MAME is not quiet, at least 90%
  within 3 dB of its level (section 2.1)
- `extract` - label `rom`: two extractions are byte-identical and hold
  every file; the tiles, sprites, palette, lookups, waveforms and tables
  read back are the ROM set's; with the device ROMs, the font reads
  "GALAGA", "SCORE", "HIGH SCORE" and "CREDIT" off the title screen the
  emulated board draws, each letter of GALAGA its tile's pixels in its
  colour; no manifest without a ROM set
- `sim` - label `rom`: a scripted run on the board (30000 frames: the
  attract mode, coins, a one- and a two-player game). Every ported routine
  is checked at every call its CPU makes (by CALL, RST or the interrupt
  for a handler; nested calls too): a model loaded from the board runs
  the C routine; when the ROM's returns, every address that CPU wrote
  holds what it last wrote, every other address what it held at entry
  (bar its stack below the entry SP), and the output registers are the
  CPU's. Skipped: calls an interrupt broke into, and calls that read
  something another CPU changed meanwhile (counted)
- `routines` - label `rom`: ROM routines from random states on the
  emulated Z80 against the C ones
- `frame` - label `rom`: the C game alone equals the board every frame
  through scripted games; it reads only the extracted assets

## 5. For the sibling ports

- The Z80 core: LDIR / LDDR / CPIR / CPDR going round again take X and Y
  from the instruction's address (MAME, after the 2018 findings); SCF and
  CCF take them through Q. Galaxian's ROM may never show either.
- Galaxian's "MAME raises the NMI a cycle before the line starts" may be
  MAME's scheduler: its divisor rounding, its slices, and the Z80 stopping
  between machine cycles (section 2) are worth trying there.
