#!/bin/sh
PREFIX=`pwd`/../root/usr/local
export PKG_CONFIG_PATH="${PREFIX}/lib/pkgconfig:${PREFIX}/lib64/pkgconfig"
/usr/bin/x86_64-w64-mingw32-pkg-config --list-all | sort

