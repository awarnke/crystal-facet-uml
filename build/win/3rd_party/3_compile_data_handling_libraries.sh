#!/bin/sh

HOST_ROOT=`cd .. && pwd`/root
PREFIX=${HOST_ROOT}/usr/local
mkdir -p ${PREFIX}
# host is the prefix of the compiler executables
HOST=x86_64-w64-mingw32
LOG_DIR=`pwd`

# Position independant executable (PIE) is NOT enabled: -fPIE
export CFLAGS="-I/usr/x86_64-w64-mingw32/include -I${PREFIX}/include"
export CXXFLAGS="${CFLAGS}"
# export LDFLAGS="-L${PREFIX}/lib -L${PREFIX}/lib64 -L${PREFIX}/bin -pie"  ### < PIE disabled
export LDFLAGS="-L${PREFIX}/lib -L${PREFIX}/lib64 -L${PREFIX}/bin"

# for cross compiling, PKG_CONFIG_PATH shall be empty:
export PKG_CONFIG_PATH=
export PKG_CONFIG_LIBDIR="${PREFIX}/lib/pkgconfig:${PREFIX}/lib64/pkgconfig"
export PKG_CONFIG_SYSROOT_DIR="${HOST_ROOT}"
PKG_CONFIG_EXE="/usr/bin/x86_64-w64-mingw32-pkg-config"

echo `date +'%H:%M'`" building libxml2..."
LOG_FILE=${LOG_DIR}/log_xml.txt
echo "      log: ${LOG_FILE}"
cd src/libxml2-2*
    ./configure --host=${HOST} --prefix=${PREFIX} > ${LOG_FILE} 2>&1
    make >> ${LOG_FILE} 2>&1
    make install >> ${LOG_FILE} 2>&1
cd ../..
echo "      lib: "`${PKG_CONFIG_EXE} --libs libxml-2.0`

echo `date +'%H:%M'`" building expat..."
LOG_FILE=${LOG_DIR}/log_expat.txt
echo "      log: ${LOG_FILE}"
cd src/expat-2*
    ./configure --host=${HOST} --prefix=${PREFIX} --enable-static --enable-wrapper=gcc > ${LOG_FILE} 2>&1
    make >> ${LOG_FILE} 2>&1
    make install >> ${LOG_FILE} 2>&1
cd ../..
echo "      lib: "`${PKG_CONFIG_EXE} --libs expat`

echo `date +'%H:%M'`" copying sqlite3..."
LOG_FILE=${LOG_DIR}/log_sqlite.txt
echo "      log: ${LOG_FILE}"
cd src/sqlite-amalgamation-3*
    mkdir -p ${HOST_ROOT}/home/sqlite3
    cp -v * ${HOST_ROOT}/home/sqlite3 > ${LOG_FILE} 2>&1
cd ../..
echo "      code: "`ls ${HOST_ROOT}/home/sqlite3/sqlite3.c`

echo `date +'%H:%M'`" copying seh, pthread..."
echo "      pre: you possibly need to install mingw64-libgcc_s_seh1, mingw64-libwinpthread1"
LOG_FILE=${LOG_DIR}/log_seh.txt
echo "      log: ${LOG_FILE}"
    cp -v /usr/x86_64-w64-mingw32/sys-root/mingw/bin/libgcc_s_seh-1.dll ${HOST_ROOT}/usr/local/bin/ > ${LOG_FILE} 2>&1
    cp -v /usr/x86_64-w64-mingw32/sys-root/mingw/bin/libwinpthread-1.dll ${HOST_ROOT}/usr/local/bin/ > ${LOG_FILE} 2>&1
echo "      lib: "`ls -l ../root/usr/local/bin/libgcc_s_seh-1.dll`
echo "      lib: "`ls -l ../root/usr/local/bin/libwinpthread-1.dll`

echo `date +'%H:%M'`" finished. Please check the log files for errors."


# Copyright 2022-2025 Andreas Warnke
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
