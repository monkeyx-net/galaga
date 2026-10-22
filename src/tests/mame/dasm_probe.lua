-- RE helper (not a test): MAME's disassembly of the three Z80s' programs
-- and the 51xx's and 54xx's, for reading the game's code. Writes text files
-- only, to DASM_OUT (default build/re); never commit them, they hold the
-- ROMs' code.
--
--   DASM_OUT=build/re mame galaga -debug -debugger none -video none -sound none \
--     -seconds_to_run 30 -autoboot_script src/tests/mame/dasm_probe.lua
--
-- Writes main.asm ($0000-$3FFF), sub.asm and sub2.asm ($0000-$0FFF),
-- 51xx.asm and 54xx.asm ($000-$3FF). DASM_EXTRA="main:1a55:40,sub:0100:80"
-- also disassembles from those starts (where data ahead misaligns the
-- listing), to <cpu>_<start>.asm.

local out = os.getenv("DASM_OUT") or "build/re"
local dbg = manager.machine.debugger
local jobs = {
    { "main.asm", "0", "4000", "maincpu" },
    { "sub.asm", "0", "1000", "sub" },
    { "sub2.asm", "0", "1000", "sub2" },
    { "51xx.asm", "0", "400", "51xx:mcu" },
    { "54xx.asm", "0", "400", "54xx:mcu" },
}
local cpus = { main = "maincpu", sub = "sub", sub2 = "sub2" }
for cpu, a, n in string.gmatch(os.getenv("DASM_EXTRA") or "", "(%w+):(%x+):(%x+)") do
    jobs[#jobs + 1] = { cpu .. "_" .. a .. ".asm", a, n, cpus[cpu] or cpu }
end
local frame = 0
_G.dasm_frames = emu.add_machine_frame_notifier(function()
    frame = frame + 1
    local j = jobs[frame - 4]
    if j then
        dbg:command("dasm " .. out .. "/" .. j[1] .. "," .. j[2] .. "," .. j[3] .. ",1," .. j[4])
    elseif frame == 5 + #jobs + 30 then
        manager.machine:exit()
    end
end)
