#!/bin/sh
PREFIX=`pwd`/../root/usr/local
export PKG_CONFIG_PATH="${PREFIX}/lib/pkgconfig:${PREFIX}/lib64/pkgconfig"
#export PKG_CONFIG_LIBDIR="${PREFIX}/lib/pkgconfig"
export PKG_CONFIG_SYSROOT_DIR="/usr/x86_64-w64-mingw32/sys-root/mingw"
/usr/bin/x86_64-w64-mingw32-pkg-config --list-all | sort

