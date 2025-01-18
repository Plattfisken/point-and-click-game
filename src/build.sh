#!/bin/bash
PROGRAM_NAME="Point & Click Game";
SOURCE_FILE_NAME="main.c";
PKG="$(pkg-config --libs --cflags raylib)"
FLAGS="-g -O0 -Wall -Wextra "
mkdir -p ../build;
clang $SOURCE_FILE_NAME $PKG $FLAGS -o ../build/"$PROGRAM_NAME" && echo "$PROGRAM_NAME";
