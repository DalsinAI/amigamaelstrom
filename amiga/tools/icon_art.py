#!/usr/bin/env python3
"""Make amiga/icons/art/maelstrom.png from upstream's icon.bmp (48x48,
8-bit): the black around the picture (reached from the edges) becomes
see-through, everything else is kept as it is.
  python3 amiga/tools/icon_art.py icon.bmp amiga/icons/art/maelstrom.png
The AmiCradle Team. GPL 2 or later, like Maelstrom."""
import sys
from PIL import Image

src, dst = sys.argv[1], sys.argv[2]
im = Image.open(src).convert("RGBA")
w, h = im.size
px = im.load()
seen = set()
todo = [(x, 0) for x in range(w)] + [(x, h - 1) for x in range(w)] + \
       [(0, y) for y in range(h)] + [(w - 1, y) for y in range(h)]
while todo:
    x, y = todo.pop()
    if (x, y) in seen or not (0 <= x < w and 0 <= y < h):
        continue
    seen.add((x, y))
    r, g, b, a = px[x, y]
    if (r, g, b) != (0, 0, 0):
        continue
    px[x, y] = (0, 0, 0, 0)
    todo += [(x + 1, y), (x - 1, y), (x, y + 1), (x, y - 1)]
im.save(dst)
