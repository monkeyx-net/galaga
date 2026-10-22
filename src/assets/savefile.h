/*
 * The player's saved files (galaga.hiscore: the high score, which the
 * arcade forgets at power-off; galaga.cfg: the DIP switches). Directory: $GALAGA_SAVE_DIR, else
 * $XDG_DATA_HOME/galaga, else (Windows) %APPDATA%/galaga, else
 * ~/.local/share/galaga. Files are written to a temp file and renamed, so a
 * power-off mid-write keeps the old one.
 */
#ifndef GALAGA_SAVEFILE_H
#define GALAGA_SAVEFILE_H

#include <stddef.h>

/* The full path of a saved file (static buffer). */
const char *save_path(const char *name);
/* Read exactly len bytes; returns 0, or -1 if missing or the wrong size. */
int save_load(const char *name, void *buf, size_t len);
/* Returns 0 on success. */
int save_store(const char *name, const void *buf, size_t len);

#endif
