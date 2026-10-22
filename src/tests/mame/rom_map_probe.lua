-- MAME probe: the ROM regions as hex lines in the format of
-- `galaga --dump-rom`: "REGION:ADDR BYTE", region by region in the port's
-- order. The CPUs' regions are 64 KB in MAME; only their ROMs are dumped.
-- MAME's :gfx1 has the two 8-byte halves of each tile in $800-$FFF swapped
-- (rearranged at load; the port keeps the file's order): read back in the
-- file's order.
--
--   PROBE_OUT=out.txt mame galaga -autoboot_script rom_map_probe.lua ...

local out_path = os.getenv("PROBE_OUT") or "rom_map_probe.txt"

local regions = manager.machine.memory.regions
local dump = {
    { "maincpu", ":maincpu", 0x4000 },
    { "sub", ":sub", 0x1000 },
    { "sub2", ":sub2", 0x1000 },
    { "gfx1", ":gfx1", 0x1000 },
    { "gfx2", ":gfx2", 0x2000 },
    { "proms", ":proms", 0x220 },
    { "namco", ":namco", 0x200 },
    { "51xx", ":51xx:mcu", 0x400 },
    { "54xx", ":54xx:mcu", 0x400 },
}

local out = assert(io.open(out_path, "w"))
for _, d in ipairs(dump) do
    local r = regions[d[2]]
    for i = 0, d[3] - 1 do
        local at = i
        if d[1] == "gfx1" and i & 0x800 ~= 0 then at = i ~ 8 end
        out:write(string.format("%s:%04X %02X\n", d[1], i, r:read_u8(at)))
    end
end
out:close()
manager.machine:exit()
