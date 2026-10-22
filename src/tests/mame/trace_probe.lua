-- MAME probe: an instruction trace of one CPU over chosen frames of a
-- scripted run (needs -debug -debugger none). Only dumps; test_emu compares.
--
--   PROBE_OUT=<dir> TRACE_FROM=100 TRACE_TO=200 TRACE_CPU=sub \
--   PROBE_SCRIPT="300:coin ..." mame galaga -debug -debugger none -autoboot_script trace_probe.lua
--
-- TRACE_CPU is maincpu (the default), sub, sub2, 51xx or 54xx: one a run,
-- as MAME's tracelog writes to the debugger's visible CPU's trace. Writes
-- <dir>/<cpu>.tr, a line per instruction: "<total cycles> <registers>
-- <PC>: <disassembly>". The Z80s' registers: AF BC DE HL SP IX IY; the
-- MB88s': A X Y flags (MAME's CURFLAGS) PA. TRACE_FROM 0 starts at power-on.
-- Traces hold disassembled ROM code: they stay in the build folder, never
-- commit them. Inputs as in capture_probe.lua.

local out_dir = os.getenv("PROBE_OUT") or "."
local from = tonumber(os.getenv("TRACE_FROM") or "100")
local to = tonumber(os.getenv("TRACE_TO") or "200")
local cpu = os.getenv("TRACE_CPU") or "maincpu"
local TAGS = { maincpu = "maincpu", sub = "sub", sub2 = "sub2", ["51xx"] = "51xx:mcu", ["54xx"] = "54xx:mcu" }
local LOG = {
    z80 = '{tracelog "%d %04X %04X %04X %04X %04X %04X %04X ",totalcycles,af,bc,de,hl,sp,ix,iy}',
    mb88 = '{tracelog "%d %X %X %X %X %X ",totalcycles,a,x,y,curflags,pa}',
}

local script = {}
for f, what in string.gmatch(os.getenv("PROBE_SCRIPT") or "", "(%d+):([%w,]*)") do
    table.insert(script, { frame = tonumber(f), inputs = what })
end
local fields = {}
for _, port in pairs(manager.machine.ioport.ports) do
    for name, field in pairs(port.fields) do fields[name] = field end
end
local NAMES = { coin = "Coin 1", start = "1 Player Start", start2 = "2 Players Start",
                left = "P1 Left", right = "P1 Right", fire = "P1 Button 1" }

local dbg = manager.machine.debugger
local frame, held = 0, {}
local function start_trace()
    local kind = (cpu == "51xx" or cpu == "54xx") and "mb88" or "z80"
    dbg.visible_cpu = manager.machine.devices[":" .. TAGS[cpu]]
    dbg:command('trace ' .. out_dir .. '/' .. cpu .. '.tr,' .. TAGS[cpu] .. ',noloop,' .. LOG[kind])
end
local function stop_trace()
    dbg:command("trace off," .. TAGS[cpu])
end
if from == 0 then start_trace() end
_G.trace_frames = emu.add_machine_frame_notifier(function()
    frame = frame + 1
    for _, s in ipairs(script) do
        if s.frame == frame then
            held = {}
            for what in string.gmatch(s.inputs, "%w+") do held[what] = true end
        end
    end
    for what, name in pairs(NAMES) do
        if fields[name] then fields[name]:set_value(held[what] and 1 or 0) end
    end
    if frame == from then
        start_trace()
    elseif frame == to then
        stop_trace()
    elseif frame == to + 2 then
        manager.machine:exit()
    end
end)
