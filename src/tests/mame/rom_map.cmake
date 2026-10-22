# The ROM map in MAME against `galaga --dump-rom`: every region, the device
# ROMs' included (MAME needs them to run galaga at all).
#   cmake -DMAME=... -DROMPATH=<dir holding galaga.zip, namco51.zip, namco54.zip> -DGAME=galaga
#         -DROM=<set> -DWORKDIR=dir -DPROBE=rom_map_probe.lua -P rom_map.cmake
foreach(v MAME ROMPATH GAME ROM WORKDIR PROBE)
  if(NOT DEFINED ${v})
    message(FATAL_ERROR "rom_map.cmake: ${v} not set")
  endif()
endforeach()

file(MAKE_DIRECTORY "${WORKDIR}")
set(mame_out "${WORKDIR}/rom_map_mame.txt")
set(ours "${WORKDIR}/rom_map_ours.txt")
file(REMOVE "${mame_out}" "${ours}")
set(ENV{PROBE_OUT} "${mame_out}")
execute_process(
  COMMAND "${MAME}" galaga -rompath "${ROMPATH}"
    -nvram_directory "${WORKDIR}/nvram" -cfg_directory "${WORKDIR}/cfg" -snapshot_directory "${WORKDIR}/snap"
    -video none -sound none -nothrottle -noplugin data -skip_gameinfo -seconds_to_run 60
    -autoboot_script "${PROBE}"
  RESULT_VARIABLE rc OUTPUT_QUIET)
if(NOT rc EQUAL 0 OR NOT EXISTS "${mame_out}")
  message(FATAL_ERROR "MAME exited with ${rc} and no probe output (are namco51.zip and namco54.zip in ${ROMPATH}?)")
endif()

execute_process(COMMAND "${GAME}" --dump-rom "${ROM}" OUTPUT_FILE "${ours}" RESULT_VARIABLE rc)
if(NOT rc EQUAL 0)
  message(FATAL_ERROR "galaga --dump-rom exited with ${rc}")
endif()
execute_process(COMMAND "${CMAKE_COMMAND}" -E compare_files "${ours}" "${mame_out}" RESULT_VARIABLE diff)
if(NOT diff EQUAL 0)
  message(FATAL_ERROR "ROM map differs from MAME's: ${ours} vs ${mame_out}")
endif()
message(STATUS "ROM map matches MAME (main 16 KB, sub and sub2 4 KB, tiles 4 KB, sprites 8 KB, PROMs, device ROMs)")
