truetype_to_svg
===============

Render truetype font as SVG path

Tested on Linux

### Build and run:

    cp `locate FreeSerif.ttf` .
    ./build.sh
    ./truetype_to_svg ./FreeSerif.ttf 66 > /tmp/x.svg 

66 = unicode for 'B'. you can use any number here. 
(note that the first part of ASCII is the same as unicode so ASCII codes work too)

You can use hex as well. For example, Cherokee Letter O:

    ./a.out ./FreeSerif.ttf 0x13A3 > /tmp/x.svg

Then in Firefox type 'file:///tmp/x.svg' into your address bar. 

It is recommended to get the Firefox addon that can auto-reload files
when they change on disk. 


### Finding cool Unicode points

http://www.fileformat.info/info/unicode/block/index.htm

### Finding A Unicode font

Linux: Run 'charmap', under ubuntu, and then right click on interesting 
looking glyphs. It will give you some rough idea of the font name.

Then do 'locate ttf | grep ttf$' which will give a list of all Truetype 
font files (w .ttf extensions) on your system .

You can match this up with what you did in Charmap, then maybe you can find
the truetype you are looking for. 

### Note on implied points

The points that are not touching the curve are called 'control points'. 
The points that are touching the curve are called 'on curve points'. If 
there are two consecutive control points, there is an 'implied' point
'on the curve'.

This program does not draw the 'implied points'. 

Note that these are not Cubic Bezier curves, even though it may look 
that way with two 'control points' consecutive. They aren't really 
consecutive - as stated earlier, there is an 'implied' point between 
them. Truetype uses Quadratic Beziers (one control point, two endpoints).

### Test characters

Unicode 
67, 68 (C, D)
48007 and 0x1f01e
0x2766 0x2777

Bearing is messed up on some glyphs

### Other projects

The "Batik" project does similar stuff. It is Java:

http://xmlgraphics.apache.org/batik/tools/font-converter.html

### Todo

get real name of glyph (like unicode name)

investigate linux's auto-finding of unicode glyphs

auto-find fonts on linux, not require cmdline fontname?

accept U+4034 (hex) input format

