/* The Amiga port's own helpers (amiga/).
 * Copyright (c) 2026 AmiCradle Team. GPL 2 or later, like Maelstrom. */
#ifndef AMIGA_PORT_H
#define AMIGA_PORT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Set when Right Amiga+Q was pressed: the game leaves at the next chance. */
extern volatile int amiga_quit_requested;

/* Right Amiga+Q becomes SDL_QUIT (a game ends; the title screen quits). */
void amiga_install_quit_key(void);

#ifdef __cplusplus
}
#endif

#endif
