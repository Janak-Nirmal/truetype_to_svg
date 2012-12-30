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
of glyphs.

A good start is to run 'charmap', under ubuntu, and then right click
on interesting looking glyphs. It will give you some rough idea of the font
name. 

Now you can do 'locate ttf | grep ttf$' which will give a list of all 
Truetype fonts (w .ttf extensions) on your system . 

You can match this up with what you did in Charmap, then maybe you can find
the truetype you are looking for. 




Note on points

The display of raw TrueType points can be a bit confusing. Truetype uses
Quadratic Beziers (two points on-curve, and one 'control' point defining
the curve). 

But even though its really using Quadratic beziers , it kind of looks 
like it might be using Cubics (two points on curve, and two control 
points off curve)... but it's not. There are actually invisible on-curve 
points 'implied' to exist halfway between two adjacent off-curve 
Quadratic Bezier control points - that's how Truetype works (and 
supposedly saves data of having extra points since the machine can 
calculate the implied points automatically).

So the 'off-the-curve' aka 'control' points are drawn ... but TrueType 
has 'implied' "on-the-curve" points when two off-curve points come one 
right after the other - and they aren't drawn. It's not cubics though. We
aren't doing cubics. 

I could draw the 'invisibles', but this is a quick example hack not a 
well designed thought out program.


Known Bugs


It screws up on the last segment of each contour so some letters look 'off'. 
This is easily viewable with unicode 67, 68 (C, D)
Also see unicode 48007 and 0x1f01e

There are 'duplicate points'

i.e.
imagine A and B are points

you have QA B LB LC

it should be QA B LC




