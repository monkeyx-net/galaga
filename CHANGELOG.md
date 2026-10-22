# Galaga — native SDL2 port — Changelog

---
## Changelog

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
