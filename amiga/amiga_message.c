/* An error the player sees when started from the Workbench too: an
 * Intuition requester (the game's text has its own line breaks).
 * The AmiCradle Team. GPL 2 or later, like Maelstrom. */
#include <exec/types.h>
#include <intuition/intuition.h>
#include <proto/intuition.h>

void amiga_message(const char *title, const char *text)
{
	struct EasyStruct es = { sizeof(struct EasyStruct), 0, NULL, NULL, NULL };

	es.es_Title = (UBYTE *)title;
	es.es_TextFormat = (UBYTE *)"%s";
	es.es_GadgetFormat = (UBYTE *)"OK";
	EasyRequest(NULL, &es, NULL, (APTR)text);
}
