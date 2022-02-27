#!/bin/sh
cd ..
    if ! test -e root/usr/local; then
        echo run steps 3 and 4 first
        exit -1
    fi
    HOST_ROOT=`pwd`/root
    PREFIX=`pwd`/root/usr/local
cd 3rd_party
# host is the prefix of the compiler executables
HOST=x86_64-w64-mingw32

#export PKG_CONFIG_PATH="${PREFIX}/lib/pkgconfig:${PREFIX}/lib64/pkgconfig"
export PKG_CONFIG_PATH=
export PKG_CONFIG_LIBDIR="${PREFIX}/lib/pkgconfig:${PREFIX}/lib64/pkgconfig:${HOST_ROOT}${PREFIX}/lib/pkgconfig:${HOST_ROOT}${PREFIX}/lib64/pkgconfig"
export PKG_CONFIG_SYSROOT_DIR="${HOST_ROOT}"
/usr/bin/x86_64-w64-mingw32-pkg-config --list-all | sort

