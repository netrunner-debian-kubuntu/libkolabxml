#!/bin/bash

# Rebuilds the entire foo in one go. One shot, one kill.

rm -rf build/
mkdir -p build
cd build
mingw32-cmake ..
make
