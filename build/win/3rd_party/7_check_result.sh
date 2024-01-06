#!/bin/sh

cd ..
    if ! test -e root/usr/local; then
        echo run steps 3 and 4 first
        exit 1
    fi
    HOST_ROOT=`pwd`/root
    PREFIX=`pwd`/root/usr/local
cd 3rd_party
# host is the prefix of the compiler executables
HOST=x86_64-w64-mingw32

#export PKG_CONFIG_PATH="${PREFIX}/lib/pkgconfig:${PREFIX}/lib64/pkgconfig"
export PKG_CONFIG_PATH=
export PKG_CONFIG_LIBDIR="${PREFIX}/lib/pkgconfig:${PREFIX}/lib64/pkgconfig"
export PKG_CONFIG_SYSROOT_DIR="${HOST_ROOT}"
/usr/bin/x86_64-w64-mingw32-pkg-config --list-all | sort

ls -l ${HOST_ROOT}/usr/local/share/glib-2.0/schemas/gschemas.compiled
ls -l ${HOST_ROOT}/usr/local/bin/libgcc_s_seh-1.dll
ls -l ${HOST_ROOT}/usr/local/bin/libwinpthread-1.dll

# Copyright 2022-2024 Andreas Warnke
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
