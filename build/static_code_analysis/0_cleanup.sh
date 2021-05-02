#!/bin/sh
[ -e Makefile ] && make clean
rm -fr crystal-facet-uml_* cov-int
rm -fr CMakeCache.txt CMakeFiles/ cmake_install.cmake Makefile
