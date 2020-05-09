#!/bin/sh
unzip ../3rd_party/gtk+-bundle_3.6.4-20130513_win32.zip -d crystal_facet_uml
unzip ../3rd_party/sqlite-amalgamation-3300100.zip -d sqlite-amalgamation-3300100
cmake -DCMAKE_TOOLCHAIN_FILE=../mingw_wine_toolchain.cmake ..
