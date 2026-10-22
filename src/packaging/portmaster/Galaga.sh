#!/bin/bash
# PortMaster launcher for the native Galaga (SDL2).

XDG_DATA_HOME=${XDG_DATA_HOME:-$HOME/.local/share}

if [ -d "/opt/system/Tools/PortMaster/" ]; then
  controlfolder="/opt/system/Tools/PortMaster"
elif [ -d "/opt/tools/PortMaster/" ]; then
  controlfolder="/opt/tools/PortMaster"
elif [ -d "$XDG_DATA_HOME/PortMaster/" ]; then
  controlfolder="$XDG_DATA_HOME/PortMaster"
else
  controlfolder="/roms/ports/PortMaster"
fi

source $controlfolder/control.txt

[ -f "${controlfolder}/mod_${CFW_NAME}.txt" ] && source "${controlfolder}/mod_${CFW_NAME}.txt"
get_controls

GAMEDIR=/$directory/ports/galaga

> "$GAMEDIR/log.txt" && exec > >(tee "$GAMEDIR/log.txt") 2>&1

cd $GAMEDIR

# settings and scores live with the port; the player's own ROM set sits beside it
mkdir -p "$GAMEDIR/conf"
export GALAGA_SAVE_DIR="$GAMEDIR/conf"
export GALAGA_ASSETS="$GAMEDIR/assets"
for rom in "$GAMEDIR/galaga.zip" "$GAMEDIR/galaga"; do
  [ -e "$rom" ] && export GALAGA_ROM="$rom" && break
done

# first run: extract the tiles, sprites, font and tables from the ROM set into assets/
if [ ! -f "$GALAGA_ASSETS/manifest.txt" ] && [ -n "$GALAGA_ROM" ]; then
  ./galaga.${DEVICE_ARCH} --extract "$GALAGA_ROM" "$GALAGA_ASSETS"
fi

# gptokeyb only provides the exit hotkey; the game reads the pad itself
$GPTOKEYB "galaga.${DEVICE_ARCH}" &

SDL_GAMECONTROLLERCONFIG="$sdl_controllerconfig" ./galaga.${DEVICE_ARCH}

pm_finish
