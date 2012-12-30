truetype_to_svg
===============

Render truetype font as SVG path

Tested on Linux

Build and run:

    cp `where FreeSerif.ttf` .
    clang++ example5.cpp `freetype-config --cflags --libs `
    ./a.out ./FreeSerif.ttf 66 > /tmp/x.svg 

66 = unicode for 'B'. you can use any number here. 
(note that the first part of ASCII is the same as unicode so ASCII codes work too)

You can use hex as well. For example, Cherokee Letter O:

    ./a.out ./FreeSerif.ttf 0x13A3 > /tmp/x.svg

Then in Firefox type 'file:///tmp/x.svg' into your address bar. 

It is recommended to get the Firefox addon that can auto-reload files
when they change on disk. 


Finding A Unicode font

It's a bit tricky, this doesn't use the tricks of linux to automatically 
find fonts to fill a requested code. Different fonts have different amounts
of symbols. 

A start is to do 'locate ttf | grep ttf$' which will give a list of all 
Truetype fonts (w .ttf extensions) on your system . There seems to be 
some good stuff under /usr/share/fonts/truetype/unfonts-core

Known Bugs


It screws up on the last segment of each contour so some letters look 'off'. 
This is easily viewable with unicode 67, 68 (C, D)
Also see unicode 48007
