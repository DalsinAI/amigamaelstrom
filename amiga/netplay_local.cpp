/* The Amiga build's stand-in for netlogic/netplay.cpp: one local player,
 * no network (no SDL_net, no bsdsocket.library).
 *
 * Upstream runs even a one-player game through UDP: every frame the
 * player's keys go out as a SYNC packet on a socket bound to itself and
 * come back before the frame is played. Here SyncNetwork() hands the
 * frame's keys straight to GetSyncBuf(), which is what the loopback
 * packet delivered. -player 1 is still accepted; other players, -server
 * and -deathmatch are refused with a message.
 * The AmiCradle Team. GPL 2 or later, like Maelstrom. */

#include <stdlib.h>

#include "Maelstrom_Globals.h"
#include "netplay.h"
#include "protocol.h"

int gNumPlayers;
int gOurPlayer;
int gDeathMatch;

static unsigned char OutBuf[BUFSIZ];	/* this frame's keys */
static int           OutLen;
static unsigned char SyncBuf[BUFSIZ];	/* last synced frame's keys */
static int           SyncLen;
static int           HaveSync;

int InitNetData(void)
{
	gNumPlayers = 0;
	gOurPlayer = -1;
	gDeathMatch = 0;
	OutLen = 0;
	SyncLen = 0;
	HaveSync = 0;
	return(0);
}

void HaltNetData(void)
{
}

int AddPlayer(const char *player)
{
	if ( (atoi(player) != 1) || strchr(player, '@') || strchr(player, ':') ) {
		error("Network play is not in the Amiga version: "
		      "only '-player 1' (the local player) can be given.\n");
		return(-1);
	}
	gOurPlayer = 0;
	return(0);
}

int SetServer(const char *server)
{
	(void)server;
	error("Network play is not in the Amiga version (-server).\n");
	return(-1);
}

int CheckPlayers(void)
{
	gNumPlayers = 1;
	gOurPlayer = 0;
	if ( gDeathMatch ) {
		error("Warning: No deathmatch in a single player game!\r\n");
		gDeathMatch = 0;
	}
	return(0);
}

void QueueKey(unsigned char Op, unsigned char Type)
{
	/* Drop keys on a full buffer (assumed never to really happen) */
	if ( OutLen >= (BUFSIZ-2) )
		return;
	OutBuf[OutLen++] = Op;
	OutBuf[OutLen++] = Type;
}

int SyncNetwork(void)
{
	memcpy(SyncBuf, OutBuf, OutLen);
	SyncLen = OutLen;
	HaveSync = 1;
	OutLen = 0;
	return(0);
}

int GetSyncBuf(int index, unsigned char **bufptr)
{
	int retlen;

	if ( (index != 0) || !HaveSync ) {
		*bufptr = NULL;
		return(0);
	}
	*bufptr = SyncBuf;
	retlen = SyncLen;
	SyncLen = 0;
	HaveSync = 0;
	return(retlen);
}

int Send_NewGame(int *Wave, int *Lives, int *Turbo)
{
	(void)Wave; (void)Lives; (void)Turbo;
	/* Keys pressed in the menus are not the game's */
	OutLen = 0;
	HaveSync = 0;
	return(0);
}

int Await_NewGame(int *Wave, int *Lives, int *Turbo)
{
	/* Only player 1 plays here, and player 1 sends */
	return(Send_NewGame(Wave, Lives, Turbo));
}
