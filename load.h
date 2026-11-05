
#ifndef _load_h
#define _load_h

#include <stdlib.h>
#include <string.h>
#ifdef WIN32  // For access() prototype
#include <io.h>
#define F_OK	0
#define access	_access
#else
#ifdef macintosh
static inline char *strdup(const char *str)
{
	char *newstr;
	
	newstr = (char *)malloc(strlen(str)+1);
	if ( newstr ) {
		strcpy(newstr, str);
	}
	return(newstr);
}
#endif
#if defined(unix) || defined(__MACH__) || defined(__BEOS__)
#include <unistd.h>
#endif
#endif /* WIN32 */

#include "SDL_FrameBuf.h"

/* Pathing stuff for the different operating systems */
#if defined(unix) || defined(__MACH__)
#define DIR_SEP	"/"
#define CUR_DIR	"."
#elif defined(WIN32)
#define DIR_SEP	"/"
#define CUR_DIR	"."
#elif defined(__BEOS__)
#define DIR_SEP	"/"
#define CUR_DIR	"."
#elif defined(macintosh)
#define DIR_SEP	":"
#define CUR_DIR	":"
#elif defined(__amigaos__)
/* AmigaOS: the data, the scores and the settings are in the program's
   own drawer. "PROGDIR:" + "/" would be the parent drawer, so a directory
   ending in ':' gets no separator (see JoinPath() below). */
#define DIR_SEP	"/"
#define CUR_DIR	"PROGDIR:"
#define LIBDIR	"PROGDIR:"
#else
#error Unspecified platform!
#endif /* Choose your platform */

#ifndef LIBDIR
#if defined(unix) || defined(__MACH__)
#define LIBDIR	"/usr/local/lib/Maelstrom"
#else
#define LIBDIR	CUR_DIR
#endif
#endif /* !defined(LIBDIR) */

/* "dir" DIR_SEP "file", except after a device or assign name ("PROGDIR:") */
static inline char *JoinPath(const char *directory, const char *filename)
{
	size_t dirlen = strlen(directory);
	size_t pathlen = dirlen+1+strlen(filename)+1;
	char *path = new char[pathlen];

	if ( (dirlen > 0) && (directory[dirlen-1] == ':') ) {
		SDL_snprintf(path, pathlen, "%s%s", directory, filename);
	} else if (SDL_strcmp(directory, DIR_SEP) == 0) {
		SDL_snprintf(path, pathlen, DIR_SEP "%s", filename);
	} else {
		SDL_snprintf(path, pathlen, "%s" DIR_SEP "%s", directory, filename);
	}
	return(path);
}

class LibPath {

private:
	static char *exepath;

public:
	static void SetExePath(const char *exe) {
		char *exep;

		exepath = SDL_strdup(exe);
		for ( exep = exepath+strlen(exe); exep > exepath; --exep ) {
			if ( (*exep == *DIR_SEP) || (*exep == '\\') ) {
				break;
			}
		}
		if ( exep > exepath ) {
			*exep = '\0';
		} else {
			SDL_free(exepath);
			exepath = SDL_strdup(CUR_DIR);
		}
	}

public:
	LibPath() {
		path = NULL;
	}
	LibPath(const char *file) {
		path = NULL;
		Path(file);
	}
	~LibPath() {
		if ( path ) delete[] path;
	}

	const char *Path(const char *filename) {
		const char *directory;

		directory = SDL_getenv("MAELSTROM_LIB");
		if ( directory == NULL ) {
			directory = LIBDIR;
#if !defined(macintosh) && !defined(__amigaos__)
			if ( access(directory, F_OK) < 0 ) {
				directory = exepath;
			}
#endif
		}

		if ( path != NULL )
			delete[] path;

#ifdef __MACOSX__
		size_t pathlen = strlen(directory)+strlen("/../Resources/")+strlen(filename)+1;
		path = new char[pathlen];
		SDL_snprintf(path, pathlen, "%s/../Resources/%s", directory, filename);
		if ( access(path, F_OK) == 0 ) {
			return(path);
		}
		delete[] path;
#endif

		path = JoinPath(directory, filename);
		return(path);
	}
	const char *Path(void) {
		return(path);
	}

private:
	char *path;
};


class SavePath {

public:
	SavePath() {
		path = NULL;
	}
	SavePath(const char *file) {
		path = NULL;
		Path(file);
	}
	~SavePath() {
		if ( path ) delete[] path;
	}

	const char *Path(const char *filename) {
#ifdef __amigaos__
		char *directory = SDL_strdup(CUR_DIR);
#else
		char *directory = SDL_GetPrefPath("Ambrosia Software", "Maelstrom");
#endif
		if (!directory) {
			directory = SDL_strdup(CUR_DIR);
		}

		if ( path != NULL )
			delete[] path;

		path = JoinPath(directory, filename);
		SDL_free(directory);

		return(path);
	}
	const char *Path(void) {
		return(path);
	}

private:
	char *path;
};

/* Functions exported from load.cc */
extern SDL_Surface *Load_Icon(char **xpm);
extern SDL_Surface *Load_Title(FrameBuf *screen, int title_id);
extern SDL_Surface *GetCIcon(FrameBuf *screen, short cicn_id);

#endif /* _load_h */
