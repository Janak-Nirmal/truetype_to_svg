#/bin/sh

if [ "`command -v clang++`" ]; then
  CC=clang++
else
  CC=g++
fi

WARN="-pedantic -Wall"
FREETYPE_FLAGS=`freetype-config --cflags --libs`
$CC $WARN main.cpp -o truetype_to_svg $FREETYPE_FLAGS

