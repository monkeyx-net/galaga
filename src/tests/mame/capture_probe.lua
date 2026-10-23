-- MAME probe: video RAM, RAM and the write-only latches at chosen frames,
-- with scripted input. Only dumps; C does the decoding and comparing.
--
--   PROBE_OUT=<dir> PROBE_FRAMES="900 1500" PROBE_LAST=1800 \
--   PROBE_SCRIPT="1200:coin 1215: 1300:start 1400:left,fire ..." mame galaga -autoboot_script capture_probe.lua
--
-- PROBE_POKE="8a00=05" writes memory every frame from PROBE_POKE_FROM on.
-- PROBE_FIELDS="Cabinet=0" sets switches by MAME's names (the DIP switches).
--
-- Writes into PROBE_OUT, at each PROBE_FRAMES frame:
--   frame_<n>.txt   "video" + the 2 KB at $8000, "ram1" / "ram2" / "ram3" +
--                   the 1 KB at $8800 / $9000 / $9800, "latch" + the last
--                   value written to each of $6820-$6827 and $A000-$A007
--                   (16 bytes, "--" = never)
--   and a MAME snapshot (snap/galaga/NNNN.png, in order)

local out_dir = os.getenv("PROBE_OUT") or "."
local last_frame = tonumber(os.getenv("PROBE_LAST") or "1800")

local snap_frames = {}
for n in string.gmatch(os.getenv("PROBE_FRAMES") or "", "%d+") do snap_frames[tonumber(n)] = true end

-- "frame:input,input frame:..." where input is coin, start, start2, left,
-- right, fire; each entry holds until the next one.
local script = {}
for f, what in string.gmatch(os.getenv("PROBE_SCRIPT") or "", "(%d+):([%w,]*)") do
    table.insert(script, { frame = tonumber(f), inputs = what })
end
local poke_from = tonumber(os.getenv("PROBE_POKE_FROM") or "0")
local pokes = {}
for a, v in string.gmatch(os.getenv("PROBE_POKE") or "", "(%x+)=(%x+)") do
    table.insert(pokes, { addr = tonumber(a, 16), value = tonumber(v, 16) })
end

local fields = {}
for _, port in pairs(manager.machine.ioport.ports) do
    for name, field in pairs(port.fields) do fields[name] = field end
end
local NAMES = { coin = "Coin 1", start = "1 Player Start", start2 = "2 Players Start",
                left = "P1 Left", right = "P1 Right", fire = "P1 Button 1" }

local set_fields = {}
for name, v in string.gmatch(os.getenv("PROBE_FIELDS") or "", "([%w ]+)=(%d+)") do
    if fields[name] then set_fields[#set_fields + 1] = { field = fields[name], value = tonumber(v) } end
end

local space = manager.machine.devices[":maincpu"].spaces["program"]

-- the latches, written by any of the three CPUs
local latch = {}
local taps = {}
for _, cpu in ipairs({ ":maincpu", ":sub", ":sub2" }) do
    local sp = manager.machine.devices[cpu].spaces["program"]
    -- Kept in a global: MAME removes a tap once its handle is garbage-collected.
    taps[#taps + 1] = sp:install_write_tap(0x6820, 0x6827, "capture_misc" .. cpu, function(offset, data, mask)
        latch[offset & 7] = data
    end)
    taps[#taps + 1] = sp:install_write_tap(0xA000, 0xA007, "capture_video" .. cpu, function(offset, data, mask)
        latch[8 + (offset & 7)] = data
    end)
end
_G.capture_taps = taps

local function hex(base, n)
    local row = {}
    for a = base, base + n - 1 do row[#row + 1] = string.format("%02X", space:read_u8(a)) end
    return table.concat(row)
end

local function dump_frame(n)
    local f = assert(io.open(string.format("%s/frame_%d.txt", out_dir, n), "w"))
    f:write("video " .. hex(0x8000, 0x800) .. "\n")
    f:write("ram1 " .. hex(0x8800, 0x400) .. "\n")
    f:write("ram2 " .. hex(0x9000, 0x400) .. "\n")
    f:write("ram3 " .. hex(0x9800, 0x400) .. "\n")
    local l = {}
    for i = 0, 15 do l[#l + 1] = latch[i] and string.format("%02X", latch[i]) or "--" end
    f:write("latch " .. table.concat(l) .. "\n")
    f:close()
end

local frame, held = 0, {}
_G.capture_frames = emu.add_machine_frame_notifier(function()
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
    -- a DIP switch's setting is its field's value under its mask (Cabinet=0: cocktail)
    for _, f in ipairs(set_fields) do f.field.user_value = f.value end
    if frame >= poke_from then
        for _, p in ipairs(pokes) do space:write_u8(p.addr, p.value) end
    end
    if snap_frames[frame] then
        dump_frame(frame)
        manager.machine.video:snapshot()
    end
    if frame >= last_frame then manager.machine:exit() end
end)
