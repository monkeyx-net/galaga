# The scripted run the MAME tests capture and test_emu replays (inputs in
# capture_probe.lua's PROBE_SCRIPT format): power-on (the RAM test takes
# 700 frames), the attract mode, a coin, a 1-player start, then over a
# minute of play, moving and firing.
set(capture_script "1050:coin 1060: 1150:start 1160:")
set(moves "left,fire" "right" "fire" "left" "right,fire" "fire" "left" "right")
set(f 1400)
set(k 0)
while(f LESS 5200)
  math(EXPR m "${k} % 8")
  list(GET moves ${m} move)
  # hold each move a while, then let go
  math(EXPR hold "15 + (${k} * 7) % 25")
  math(EXPR off "${f} + ${hold}")
  string(APPEND capture_script " ${f}:${move} ${off}:")
  math(EXPR f "${off} + 5")
  math(EXPR k "${k} + 1")
endwhile()
# frames dumped and snapshotted (the picture is drawn at the vertical
# blank from what memory holds then, so every one is checked)
set(capture_frames 750 900 1100 1300 1500 1700 2000 2400 2800 3200 3600 4000 4400 4800 5200)
# frames whose instructions are traced on every CPU: play, the aliens diving
set(trace_from 1700)
set(trace_to 1800)
