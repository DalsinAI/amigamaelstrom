/* Right Amiga+Q quits, as on the other Amiga ports: an SDL event filter
 * turns the key into SDL_QUIT, which ends a game (as the close gadget
 * does) and, through amiga_quit_requested, the title screen too.
 * Copyright (c) 2026 AmiCradle Team. GPL 2 or later, like Maelstrom. */
#include "SDL.h"
#include "amiga_port.h"

volatile int amiga_quit_requested = 0;

static int SDLCALL amiga_quit_filter(void *userdata, SDL_Event *event)
{
	(void)userdata;

	if (event->type == SDL_KEYDOWN &&
	    event->key.keysym.sym == SDLK_q &&
	    (event->key.keysym.mod & KMOD_RGUI) != 0) {
		amiga_quit_requested = 1;
		event->type = SDL_QUIT;
	} else if (event->type == SDL_QUIT) {
		amiga_quit_requested = 1;
	}
	return 1;
}

void amiga_install_quit_key(void)
{
	SDL_SetEventFilter(amiga_quit_filter, NULL);
}
