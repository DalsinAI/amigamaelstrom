/* The Amiga build's stand-in for netscore.cpp: no world-wide score server
 * (it was at libsdl.org:4444 and needed SDL_net). The scores stay in
 * PROGDIR:Maelstrom-Scores.
 * The AmiCradle Team. GPL 2 or later, like Maelstrom. */

#include "Maelstrom_Globals.h"
#include "netscore.h"

void RegisterHighScore(Scores high)
{
	(void)high;
}

int NetLoadScores(void)
{
	return(-1);
}
