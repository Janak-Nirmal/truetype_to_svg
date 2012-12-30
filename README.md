truetype_to_svg
===============

Render truetype font as SVG path

Tested on Linux

Build and run:

    cp `where FreeSerif.ttf` .
    clang++ example5.cpp `freetype-config --cflags --libs `
    ./a.out ./FreeSerif.ttf 66 > /tmp/x.svg 

66 = unicode for 'B'. you can use any number here. you can use hex as well.

Then in Firefox type 'file:///tmp/x.svg' into your address bar. 

It is recommended to get the Firefox addon that can auto-reload files
when they change on disk. 

