Maelstrom for AmigaOS 3.x
=========================

A port of Maelstrom 3.0.7 (https://github.com/libsdl-org/Maelstrom, the
Maelstrom-3 branch at commit 063c388fb6e4b7365d005cd25f56844ef94aca51), Sam
Lantinga's GPL port of Andrew Welch's Macintosh asteroids game, for
AmigaOS 3.2 on a 68040 or 68060 with an FPU and an RTG screen, on
GFOpenGPU's SDL 2.

Why the Maelstrom-3 branch: it is upstream's SDL 2 version (3.0.7, "Updated
for SDL 2.0"). The main branch (Maelstrom 4) needs SDL 3.4, SDL_net 3,
PhysicsFS and its own vendored copies, which GFOpenGPU does not have.

Status: runs in the AmiCradle lab: the title screen and its high scores, a
game (the rocks, flying, turning, the shields, the score and the status
line), Esc ending it, game over, the high score name entry and the new
score in the table; Esc on the title screen and Right Amiga+Q in a game
quit cleanly to the Workbench, and the settings and scores were written. The lab is muted, so the sound was not heard.


What you need
-------------

- A 68040 or 68060 with an FPU (not an LC040 or EC040), AmigaOS 3.2, an RTG
  card (or the AmiCradle lab), and about 8 MB of free memory.
- GFOpenGPU 0.8 or later: LIBS:GFOpenGPU/SDL2.module (installed by OpenUp).
- The game's data is included (see Licences.txt).


How to play
-----------

Run Install-Maelstrom (or copy the Maelstrom drawer where you want it),
then double-click Maelstrom, or from a Shell:

    cd Maelstrom
    Maelstrom [-fullscreen] [-gamma 0-8] [-volume 0-8]

- It opens a 640x480 window on the Workbench; -fullscreen asks for a full
  screen instead (Alt+Return switches while it runs).
- On the title screen: P plays, C sets the controls, Z clears the high
  scores, A is the about screen, L starts at a chosen wave, 0-8 set the
  volume, Q or Esc quits.
- In a game, upstream's keys: cursor up thrusts, left and right turn, Tab
  fires, Space raises the shields, Pause pauses, Esc ends the game. C on
  the title screen changes them.
- Right Amiga+Q quits at once from anywhere.
- The settings and the high scores are in the drawer's User drawer
  (Maelstrom-data, Maelstrom-Scores); an update by Install-Maelstrom keeps
  it.


Building
--------

GFOpenAmigaGCC 16.2.0-open6 (the os32-gcc16-open6 stove, with the GFOpenGPU
SDK 0.8 in it):

    make -f amiga/Makefile.amigaos -j4 STOVE=/path/to/os32-gcc16-open6

STOVE defaults to ~/AmiCradle/stoves/os32-gcc16-open6. Output:
build/amigaos/Maelstrom/ (the program, the data, ReadMe, ReadMe-Maelstrom,
the licences and Ambrosia's notes, an empty User drawer, with GlowIcons),
build/amigaos/Maelstrom.info and build/amigaos/Install-Maelstrom with its
icon. The icons are made from amiga/icons/icons.json and Install-Maelstrom
from amiga/installer.json with amicradle's scripts/port_icons.py and
port_installer.py (see "about" in icons.json).


What was changed
----------------

- amiga/Makefile.amigaos: upstream's Makefile.am lists (the game, netlogic,
  screenlib, maclib), linked with the stove's libSDL2_net.a, libSDL2.a and
  libnix's libsocket.a (bsdsocket.library, opened only for network play).
  No RSA checksum (as upstream's configure without ~/RSA). amiga/stack.c: a
  256 KB stack and the $VER string.
- load.h: the data is read from PROGDIR: (LIBDIR); the settings and high
  scores go to PROGDIR:User (made if it is not there) instead of SDL's
  preferences path. "PROGDIR:" and a name are joined without a "/".
- maclib/Mac_Resource.cpp: a "PROGDIR:name" path is split at the colon, and
  a name joined to a volume gets no "/" ("PROGDIR:/x" is the parent
  drawer's x on AmigaOS).
- main.cpp: a window by default (upstream: a full screen) and -fullscreen;
  Esc on the title screen quits as Q does; the title loop leaves when Right
  Amiga+Q or the close gadget asked to.
- amiga/amiga_quit.c: an SDL event filter that turns Right Amiga+Q into
  SDL_QUIT (a game then ends as with its close gadget, and the title screen
  quits).
- The data files are big-endian Mac resources and the game reads them
  through SDL_SwapBE*, so no byte-order changes were needed.


Known limits
------------

- Network play (-player, -server; README.network) is built in but was not
  tried; it needs a TCP/IP stack (bsdsocket.library).
- Full screen was not tried.
- The frame rate on a real 68040 was not measured; the lab's CPU is faster.
- The sound was not heard (muted lab).


Licence
-------

The program is under the GNU GPL, version 2 or later (COPYING.GPL); the
artwork, animations and sounds are CC BY 3.0 (COPYING.txt). See
Licences.txt. The port's own files are GPL 2 or later, Copyright (c) 2026
AmiCradle Team.
