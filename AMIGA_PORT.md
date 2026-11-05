Maelstrom for AmigaOS 3.x
=========================

A port of Maelstrom 3.0.7, Andrew Welch's Asteroids-style shooter for the
Macintosh (Ambrosia Software, 1992-94), which Sam Lantinga ported to SDL
and released under the GPL (https://www.libsdl.org/projects/Maelstrom/),
for AmigaOS 3.x on a 68040 or 68060 with an FPU and an RTG screen, on
OpenGPU's SDL 2.

Base: upstream's tag release-3.0.7 of https://github.com/libsdl-org/Maelstrom
(commit de89be3f31bd3ff419dcc56a820fed3c87fbd8ae, "Maelstrom-3.0.7", the
last SDL 2 release; 4.x moved to SDL 3). Branch team2/maelstrom-amiga.

Status: lab-tested (Instance-45, an emulated 68040 with an FPU, RTG,
5 November 2026): the menu draws in its window, P starts a game, the
ship turns, thrusts and fires and the rocks move smoothly, Esc ends the
game, a mouse click on the About key opens "The Story", and Q quits
back to the Shell. The first run leaves about 1.2 MB of Fast RAM in use
(SDL2.module stays loaded); a second run gives back all but 88 bytes.
Pictures: docs/screenshots/.


What you need
-------------

- A 68040 or 68060 with an FPU (not an LC040 or EC040), AmigaOS 3.x, an RTG
  card (or the AmigaChrome lab).
- OpenGPU 0.6 or later: opengpu.library and LIBS:OpenGPU/SDL2.module
  (installed by OpenUp).
- About 2.5 MB of disk and about 8 MB of memory.


How to play
-----------

Double-click Maelstrom in its drawer, or from a Shell:

    cd Maelstrom
    Maelstrom [-fullscreen] [-volume 0-8] [-gamma 0-8]

It opens a 640x480 window on the Workbench; -fullscreen opens a 640x480
screen of its own instead. -speedtest times the sprite drawing and quits.

Main menu (keys, or click the key pictures):

- P: play. L: play from a chosen wave (with lives and "turbo").
- C: change the game keys. Z: clear the high scores. A: about.
- 0 to 8: sound volume. X: a surprise.
- Q or Esc: quit (at once).

In the game (upstream's keys; C changes them):

- Left / Right: turn. Up: thrust. Tab: fire. Space: shield.
- P: pause (an Amiga keyboard has no Pause key). Esc: end the game.
- F3: save a picture of the screen (ScreenShot0.bmp in the current
  drawer).
- Joystick: left/right turn, forward thrusts, button 1 fires, button 2
  shields.

High scores (Maelstrom-Scores) and the settings (Maelstrom-data: keys,
volume, gamma) are kept in the game's drawer.


Building
--------

GFOpenAmigaGCC 16.2.0-open4 or later and the OpenGPU developer kit:

    make -f amiga/Makefile.amigaos -j2 dist \
         STOVE=/path/to/openamigagcc-16.2.0-open4 \
         NDK=/path/to/OpenNDK/Include_H SDL2KIT=/path/to/opengpu-sdk

The defaults are CradleLab's paths. Output: build/amigaos/Maelstrom
(stripped; Maelstrom.debug beside it keeps the symbols); 'dist' makes
dist/Maelstrom/ (the program, the data, ReadMe, upstream's README, the
licences, with GlowIcons) and dist/Maelstrom.info. The icons are made from
amiga/icons/icons.json (see "about" there); the art is upstream's
icon.bmp (amiga/tools/icon_art.py).


What was changed
----------------

- amiga/Makefile.amigaos: upstream's Makefile.am source lists, configure
  not run; links the kit's libSDL2.a only.
- Network play and the world-wide score server are cut (SDL_net).
  Upstream sends even a one-player game's keys through a UDP socket bound
  to itself; amiga/netplay_local.cpp, in place of netlogic/netplay.cpp,
  hands each frame's keys straight to the player (same frame, as the
  loopback did). '-player 1' is still accepted; other players, -server,
  -deathmatch and -netscores are refused or do nothing
  (amiga/netscore_none.cpp in place of netscore.cpp). Maelstrom-netd is
  not built.
- load.h: the data, the scores and the settings are in PROGDIR:. A path
  after a "NAME:" gets no '/' ("PROGDIR:/x" would be the parent drawer).
  SetExePath() copied the fallback directory with sizeof(a pointer),
  too short for "PROGDIR:"; it is duplicated now.
- maclib/Mac_Resource.cpp: the resource file search split a path only at
  '/' and tried "%PROGDIR:Maelstrom Sounds" (Executor's name), which makes
  AmigaDOS ask for a volume "%PROGDIR"; the directory part now ends at ':'
  too.
- screenlib/SDL_FrameBuf.cpp (the picture):
  - The game draws in 8 bits with a palette; upstream converts the whole
    640x480 picture to 32 bits and uploads it to the texture on every
    update. Here a 32-bit copy is kept for the run and only the dirty
    rectangles the game already tracks are converted and sent
    (SDL_UpdateTexture with a rectangle; their bounding box when there are
    more than 16). After a palette change or during a fade everything is
    sent.
  - The texture is SDL_PIXELFORMAT_RGB888, asked for explicitly (OpenGPU's
    SDL 2 reports a 24-bit window).
  - Fades take 16 steps instead of 32 (each step is a whole picture).
  - The window is created with its title ("Maelstrom"), so the pointer
    code can find it.
- Mouse: the menu and the dialogs take the pointer's place from the
  game's Intuition window, less its borders, scaled to 640x480
  (amiga/amiga_pointer.c, from the Tennix port, where SDL2.module's mouse
  events came about 16 px right and 32 px down of the pointer). This is
  done in FrameBuf::PollEvent()/WaitEvent(), which the menu and dialogs
  use. There too, an "exposed" window event shows the picture again
  (the menu only waits for events and never redrew a window that had
  been covered).
- main.cpp: a window by default, -fullscreen gives a screen
  (SDL_WINDOW_FULLSCREEN, not _DESKTOP); the Alt+Return full-screen toggle
  is off. From the Workbench (argc 0) no arguments are read. Esc quits from
  the menu as Q does, and quitting no longer waits for a sound and a
  1-second fade.
- init.cpp: the window opens and the Ambrosia splash shows before the
  sounds are loaded and converted, instead of a blank wait; the splash
  then stays its usual 5 seconds (a key skips it), then the title with
  the loading bar while the sprites load. signal(SIGSEGV) is not set
  (libnix).
- controls.cpp: Pause is P by default (no Pause key on an Amiga); a
  held key's repeats are ignored in the game (a held P paused and
  unpaused).
- myerror.cpp, amiga/amiga_main.c, amiga/amiga_message.c: after the
  start-up, an error that stops the game is also shown in an Intuition
  requester (from the Workbench there is no Shell); the empty "SDL"
  screens SDL2.module 0.6 leaves after a full screen are closed at exit
  (amiga/amiga_screens.c, from the Seven Kingdoms port).
- screenlib/SDL_FrameBuf.cpp, LoadImage(): the search for a free colour
  for the sprites' see-through key cleared only 256 bytes of a 256-int
  table and read one past its end; the whole table is cleared now.
- buttonlist.h: "struct button" for the class's own typedef, which GCC 16
  refuses; now "button".
- amiga/stack.c: 128 KB of stack and a $VER string.
  amiga/pthread_stubs.c: what libstdc++'s exception runtime asks of POSIX
  threads (from the Tennix port).

Big-endian: the data is Macintosh resource forks (Maelstrom_Fonts,
_Sounds, _Sprites) and big-endian icon files (Images/*.cicn); upstream
reads every field through SDL_SwapBE16/32 or byte by byte, and the
structs it reads whole (resource header, map, type and reference
entries) have the same size and layout with m68k's 2-byte alignment.
The NFNT fonts are swapped only when their magic is wrong, so not here.
The 'snd ' samples are 8-bit. Images/*.bmp go through SDL_LoadBMP. The
scores file is written big-endian on every machine; Maelstrom-data
(settings) is raw structs read back by the same program.


Cut
---

- Network play (-player N@host, -server, -deathmatch) and Maelstrom-netd.
- The world-wide score server (-netscores).
- The Alt+Return full-screen toggle (start with -fullscreen).


Licences
--------

- Maelstrom's code: GPL 2 or later (COPYING.GPL), Andrew Welch and Sam
  Lantinga.
- The artwork and animations: Creative Commons Attribution 3.0
  (COPYING.txt). The sounds and fonts are part of the same GPL release
  (Ambrosia released Maelstrom 3.0 as "a totally free version", press
  release of 7 December 1999) and are distributed only with the game.
  Upstream asks that Docs/MaelstromGPL_press_release.txt and
  Docs/Ambrosia_Software,_Inc._FAQ.txt go with every copy; they are in
  the drawer's Docs.
- SDL 2: OpenGPU's kit (its Licences/).


Known limits / to check in the lab
----------------------------------

- Speed: sprites, the converted dirty rectangles and the texture upload
  per frame; the game runs at a fixed 30 frames a second (FRAME_DELAY 2),
  so a slow machine plays it slower. -speedtest prints a number.
- The dirty-rectangle update: no stale pieces of picture (dialogs,
  explosions, the status line, the star field after a new wave).
- Sound: 11025 Hz 8-bit, mixed by the game in SDL's audio callback.
- The pointer in the menu and dialogs, windowed and with -fullscreen.
- Quitting from -fullscreen: no "SDL" screens must be left.
