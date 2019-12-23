#!/bin/sh
unzip gtk+-bundle_3.6.4-20130513_win32.zip -d crystal_facet_uml
unzip sqlite-amalgamation-3300100.zip -d sqlite-amalgamation-3300100
cmake -DCMAKE_TOOLCHAIN_FILE=../crystal_facet_uml/build_win/mingw_wine_toolchain.cmake ../crystal_facet_uml/build_win
