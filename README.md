truetype_to_svg
===============

Render truetype font as SVG path, using the Freetype library

### Using in your own C++ project

    #include <truetype_to_svg.hpp>

    int main()
    {
        tt2svg::glyph g( "FreeSans.ttf", 67 );
        std::cout << g.outline();
    }

### Build and run:

    # install freetype using your package manager
    # (something like sudo apt-get install libfreetype6-dev)
    # then copy a .ttf file to this directory for convenience
    cp `locate FreeSerif.ttf | tail -1 ` .
    ./build.sh
    ./test1 ./FreeSerif.ttf 66 > /tmp/x.svg 
    firefox /tmp/x.svg

### Details on running examples

When we ran this,

    ./test1 ./FreeSerif.ttf 66 > /tmp/x.svg 

66 = unicode for 'B'. you can use any number here. (note that the first 
part of ASCII is the same as unicode so ASCII codes work too)

You can use hex as well. For example, Cherokee Letter O:

    ./a.out ./FreeSerif.ttf 0x13A3 > /tmp/x.svg

To view the resulting svg file, it is easiest to use firefox: type 
'file:///tmp/x.svg' into your address bar. It is recommended to get the 
Firefox addon that can auto-reload files when they change on disk.

You can also use svg viewers like Inkscape.

### Detail on using in your own projcet

Truetype_to_svg is a 'header library' so you dont need to compile any 
libraries just include the header and use it. You will still need to
link to Freetype however, using your build system. 

It uses freetype to deal with vaguaries and variations of Truetype 
files. It does not use any of the bitmap rendering code - this is a pure 
outline curve renderer to be used for vector output. 

Truetype_to_svg comes with a very permissive license - it is almost the same
BSD-ish license used by Zlib. It does not come with any of the 
restrictions of GPL. Acknowledgment in the product documentation would 
be appreciated but is not required.

To understand the usage, look at the files named 'test*.cpp' that come
with the source code. You can output the bare svg path data, or a bunch
of debugging information.

### Finding cool Unicode points

http://www.fileformat.info/info/unicode/block/index.htm

### Finding A Unicode font

Linux: Run 'charmap', under ubuntu, and then right click on interesting 
looking glyphs. It will give you some rough idea of the font name.

Then do 'locate ttf | grep ttf$' which will give a list of all Truetype 
font files (w .ttf extensions) on your system .

You can match this up with what you did in Charmap, then maybe you can find
the truetype you are looking for. 

### How does Truetype actually work then? How does this program work?

Truetype fonts consist of a group of characters.

Each character has a sequence of 'contours'. The contours that have 
their points in 'clockwise' order are considered 'solid' The contours 
that are anti-clockwise order are considered 'holes'.

Each contour is actually made up of a sequence of points. There are two 
types - 'on the curve' points and 'off the curve' points. The 'off the curve'
points are used to create Bezier Curves between the 'on the curve' points. 
It uses a type of Bezier called 'Quadratic', which consists of two endpoints
and one 'control point'. 

A special note regards two consecutive 'off the curve' points - in 
Truetype these actually 'imply' that there is an 'on the curve' point 
exactly halfway between them. This program does not draw these implied points. 

This program does not read the files directly - it relies on the Freetype
program. Freetype automates a large amount of detail regarding loading
the files and dealing with character sets, glyphs, mapping, etc. 

This does not use any of Freetype's bitmap rendering. It is only for 
vector output. To show a bitmap (like on a screen) there will need to be 
some implementation of the non-zero winding rule. SVG makes this 
incredibly simple with "fill-rule='nonzero'", which is then implemented
by the SVG viewer program.

Please see these sites for more info.

Basic Terms: http://www.freetype.org/freetype2/docs/glyphs/glyphs-3.html
FType + outlines: http://www.freetype.org/freetype2/docs/reference/ft2-outline_processing.html
FType + contours: http://www.freetype.org/freetype2/docs/glyphs/glyphs-6.html
TType contours: https://developer.apple.com/fonts/TTRefMan/RM01/Chap1.html
TType contours2: http://www.truetype-typography.com/ttoutln.htm
Non-zero winding rule: http://en.wikipedia.org/wiki/Nonzero-rule
SVG paths: http://www.w3schools.com/svg/svg_path.asp
SVG paths + nonzero: http://www.w3.org/TR/SVG/painting.html#FillProperties

### Test characters

Unicode 
67, 68 (C, D)
48007 and 0x1f01e
0x2766 0x2767

Bearing is messed up on some glyphs

### Other projects

The "Batik" project does similar stuff. It is Java:

http://xmlgraphics.apache.org/batik/tools/font-converter.html

### Todo

get real name of glyph (like unicode name)

investigate linux's auto-finding of unicode glyphs

auto-find fonts on linux, not require cmdline fontname?

accept U+4034 (hex) input format

### known bugs

will not calculate proper use of '<g>' for varying uses of transform/footer
