#/bin/sh

if [ "`command -v clang++`" ]; then
  CC=clang++
else
  CC=g++
fi

$CC truetype_to_svg.cpp -o truetype_to_svg `freetype-config --cflags --libs`

