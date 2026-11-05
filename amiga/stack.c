/* A Shell gives 4 KB of stack; libnix's startup sets this much up instead.
 * The AmiCradle Team. GPL 2 or later, like Maelstrom. */
unsigned long __stack = 128 * 1024;
static const char version[] __attribute__((used)) = "$VER: Maelstrom 3.0.7 Amiga (05.11.2026)";
