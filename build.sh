#/bin/sh

if [ "`command -v clang++`" ]; then
  CC=clang++
else
  CC=g++
fi

WARN="-pedantic -Wall"
FREETYPE_FLAGS=`freetype-config --cflags --libs`
SOURCE_FILES="test1 test2"

for sourcefile in $SOURCE_FILES;
  do $CC $WARN $sourcefile".cpp" -o $sourcefile $FREETYPE_FLAGS
done


