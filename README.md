# Galaga - native SDL2 port (work in progress)

A C11 / SDL2 port of the 1981 Namco arcade game for Linux handhelds
(PortMaster: ArkOS, ROCKNIX, muOS, Knulli), desktops (Linux, Windows, macOS) and the
web, checked against the original ROM running in MAME.

**Status:** the game runs in C, equal frame for frame (pictures and sound)
to an emulated Galaga board that is itself checked against MAME. Next: the
DIP switch menu, high-score saving, cocktail flip and release. See
[GALAGA_PORT_PLAN.md](GALAGA_PORT_PLAN.md).

You need your own MAME `galaga` ROM set (`galaga.zip`); none is included.
The first run extracts what the game needs from it to `assets/` next to
the program. The emulated board (`--emulate`) and the MAME tests also need
the device ROMs `namco51.zip` and `namco54.zip` next to it.

Converted by monkeyx.

```bash
cmake -S . -B build && cmake --build build -j
build/galaga --windowed --rom /path/to/galaga.zip
```

## Controls

One control panel, as on the upright cabinet: the players take turns, and
every connected pad drives the panel.

| Action | Pad | Keyboard |
|---|---|---|
| Left / right | left stick or D-pad | Left Right |
| Fire | A or B | Space or Ctrl |
| Coin | Select | 5 |
| 1-player start | Start | 1 or Enter |
| 2-player start | L1 + Start | 2 |
| Next graphics preset | Select + L1 | F3 |
| Glow on / off | | F4 |
| CRT scanlines on / off | | F5 |
| Quit | hold Select + Start for 1 s | Esc |

## Building

Needs CMake 3.13+, a C11 compiler and SDL 2.0.10+.

```bash
ctest --test-dir build              # tests (ROM and MAME tests need roms/galaga.zip, namco51.zip, namco54.zip and mame)
cmake --build build --target package_linux package_portmaster
emcmake cmake -S . -B build-web && cmake --build build-web --target serve
```

## License

Apache 2.0 for the source; see [NOTICE](NOTICE) for the third-party material
this project refers to. No ROM data is included or distributed.
