/* The mouse position, read from Intuition.
 *
 * In the lab (Instance-45, a 640x480 window on the Workbench) the mouse
 * events of OpenGPU's SDL2.module came about 16 px right and 32 px down
 * of the real pointer: the window's borders counted where they should
 * not be. So the game takes the pointer's place from its own Intuition
 * window instead: MouseX/MouseY less the borders, scaled from the inner
 * size to the game's 640x480. Our window is the active one with the
 * game's title, or the active one on a screen of SDL's own (-f).
 * The AmiCradle Team. GPL 2 or later, like Maelstrom. */
#include <exec/types.h>
#include <intuition/intuitionbase.h>
#include <intuition/screens.h>
#include <proto/exec.h>
#include <proto/intuition.h>
#include <string.h>

extern struct IntuitionBase *IntuitionBase;

/* 1 and *x, *y in game units (0..log_w-1, 0..log_h-1), or 0 when the
 * active window is not the game's (the caller keeps SDL's numbers). */
int amiga_pointer_get(const char *title, int log_w, int log_h, int *x, int *y)
{
	struct Window *w;
	ULONG lock;
	int mx, my, iw, ih, ours;

	lock = LockIBase(0);
	w = IntuitionBase->ActiveWindow;
	ours = w && ((w->Title && strcmp((const char *)w->Title, title) == 0) ||
	             (w->WScreen && (w->WScreen->Flags & SCREENTYPE) != WBENCHSCREEN &&
	              w->WScreen->Title && strcmp((const char *)w->WScreen->Title, "SDL") == 0));
	if (!ours) {
		UnlockIBase(lock);
		return 0;
	}
	mx = w->MouseX - w->BorderLeft;
	my = w->MouseY - w->BorderTop;
	iw = w->Width - w->BorderLeft - w->BorderRight;
	ih = w->Height - w->BorderTop - w->BorderBottom;
	UnlockIBase(lock);

	if (iw <= 0 || ih <= 0)
		return 0;
	mx = mx * log_w / iw;
	my = my * log_h / ih;
	*x = mx < 0 ? 0 : mx >= log_w ? log_w - 1 : mx;
	*y = my < 0 ? 0 : my >= log_h ? log_h - 1 : my;
	return 1;
}
