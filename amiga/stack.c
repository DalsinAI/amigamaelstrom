/* A Shell gives 4 KB of stack; SDL 2 wants more. libnix's startup sets
 * this much up.
 * Copyright (c) 2026 AmiCradle Team. GPL 2 or later, like Maelstrom. */
unsigned long __stack = 256 * 1024;
static const char version[] __attribute__((used)) = "$VER: Maelstrom 3.0.7 Amiga (05.11.2026)";
