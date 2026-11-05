/* Start and end of the Amiga version.
 *
 * amiga_startup() (first thing in main()) notes the screens that are
 * open before SDL starts and sets up a handler that runs after the
 * game's own CleanUp() (atexit order): it closes the empty "SDL" screens
 * SDL2.module 0.6 leaves after a full screen (amiga_screens.c) and, if
 * the game stopped on an error after its start-up, shows the messages
 * in an Intuition requester; started from the Workbench there is no
 * Shell to read them in.
 * The AmiCradle Team. GPL 2 or later, like Maelstrom. */
#include <stdlib.h>
#include <string.h>

void amiga_screens_snapshot(void);
void amiga_screens_close_leftovers(void);
void amiga_message(const char *title, const char *text);

static char errors[1024];
static int report;

void amiga_note_error(const char *text)
{
	size_t have = strlen(errors);
	size_t room = sizeof(errors) - 1 - have;
	size_t len = strlen(text);

	if (len > room)
		len = room;
	memcpy(errors + have, text, len);
	errors[have + len] = '\0';
}

void amiga_report_errors(int on)
{
	report = on;
}

static void amiga_atexit(void)
{
	char *p;

	amiga_screens_close_leftovers();
	if (report && errors[0]) {
		/* The game ends its lines with "\r\n" here and there */
		for (p = errors; *p; ++p)
			if (*p == '\r')
				*p = ' ';
		amiga_message("Maelstrom", errors);
	}
}

void amiga_startup(void)
{
	amiga_screens_snapshot();
	atexit(amiga_atexit);
}
