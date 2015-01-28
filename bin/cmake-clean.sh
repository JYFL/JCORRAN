#!/bin/bash
make clean
find . */ | perl -nle'm{/(CMakeCache.txt|cmake_install.cmake|Makefile|CMakeFiles)$} and print' | while read x; do [ -e "$x" ] && rm -rf $x;done
