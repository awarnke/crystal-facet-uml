#!/bin/sh
echo This script generates fresh debian package information files 
echo to compare these to the existing files in debian_config.
cd crystal-facet-uml-1.7.0
cmake .
rm -fr CMakeCache.txt CMakeFiles/ cmake_install.cmake
dh_make --copyright=apache --email=cfu@andreaswarnke.de --single
