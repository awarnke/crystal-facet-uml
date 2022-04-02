#!/bin/sh

HOST_ROOT=`pwd`/root
PREFIX=`pwd`/root/usr/local
if ! test -e root/usr/local; then
    echo run 3rd_party steps first
    exit -1
fi
# host is the prefix of the compiler executables
HOST=x86_64-w64-mingw32
LOG_DIR=`pwd`

# static-libgcc is disabled - seems not to work with wine or win
# Position independant executable (PIE) is NOT enabled: -fPIE
export CFLAGS="-I/usr/x86_64-w64-mingw32/include -I${PREFIX}/include -I${PREFIX}/share/gettext -I${PREFIX}/include/glib-2.0 -I${PREFIX}/lib/glib-2.0/include -I${PREFIX}/include/libpng16 -I${PREFIX}/include/freetype2"
export CXXFLAGS="${CFLAGS}"
export LDFLAGS="-L${PREFIX}/lib -L${PREFIX}/lib64 -L${PREFIX}/bin -pie"

export PKG_CONFIG_PATH=
export PKG_CONFIG_LIBDIR="${PREFIX}/lib/pkgconfig:${PREFIX}/lib64/pkgconfig"
export PKG_CONFIG_SYSROOT_DIR="${PREFIX}/../.."

echo `date +'%H:%M'`" building crystal-facet-uml..."
LOG_FILE=export PKG_CONFIG_PATH=
export PKG_CONFIG_LIBDIR="${PREFIX}/lib/pkgconfig:${PREFIX}/lib64/pkgconfig"
export PKG_CONFIG_SYSROOT_DIR="${HOST_ROOT}"
PKG_CONFIG_EXE="/usr/bin/x86_64-w64-mingw32-pkg-config"
LOG_DIR=`pwd`
LOG_FILE=${LOG_DIR}/log_crystal-facet-uml.txt
echo "      log: ${LOG_FILE}"
echo "      expected duration: 10 min"
rm -fr builddir  # remove artifacts from previous build
mkdir builddir
cd builddir
    cmake -DCMAKE_TOOLCHAIN_FILE=../mingw_wine_toolchain.cmake -DCMAKE_INSTALL_PREFIX=${PREFIX} \
    -DCMAKE_IMPORT_LIBRARY_PREFIX=${PREFIX} -DCMAKE_SHARED_LIBRARY_PREFIX=${PREFIX} \
    -DCMAKE_SHARED_MODULE_PREFIX=${PREFIX} -DCMAKE_STATIC_LIBRARY_PREFIX=${PREFIX} \
    -DCMAKE_FIND_LIBRARY_PREFIXES=${PREFIX} -DCMAKE_PREFIX_PATH=${PREFIX} .. \
    -DCMAKE_BUILD_TYPE=Debug \
    | tee {LOG_FILE} 2>&1
    make -j4 | tee -a ${LOG_FILE} 2>&1
    make install | tee -a ${LOG_FILE} 2>&1
cd ..
echo "      exe: "`ls ${PREFIX}/bin/crystal-facet-uml*`

echo `date +'%H:%M'`" finished. Please check the log files for errors."


# Copyright 2022-2022 Andreas Warnke
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
