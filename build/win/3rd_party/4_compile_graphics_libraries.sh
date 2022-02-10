#!/bin/sh
cd ..
if ! test -e root/usr/local; then
    echo run step 3 first
    exit -1
fi
HOST_ROOT=`pwd`/root
PREFIX=`pwd`/root/usr/local
cd 3rd_party
LOG_DIR=`pwd`

# host is the prefix of the compiler executables

echo "building freetype..."
LOGFILE=${LOG_DIR}/log_freetype.txt
cd src/freetype-2*
    ./configure --host=x86_64-w64-mingw32 --enable-relocatable --prefix=${PREFIX} --disable-rpath --enable-static-pie > ${LOGFILE} 2>&1
    make >> ${LOGFILE} 2>&1
    make install >> ${LOGFILE} 2>&1
cd ../..

echo "building expat..."
LOGFILE=${LOG_DIR}/log_expat.txt
cd src/expat-2*
    ./configure --host=x86_64-w64-mingw32 --enable-relocatable --prefix=${PREFIX} --disable-rpath --enable-static-pie > ${LOGFILE} 2>&1
    make >> ${LOGFILE} 2>&1
    make install >> ${LOGFILE} 2>&1
cd ../..

export CFLAGS="-I/usr/x86_64-w64-mingw32/include -I${PREFIX}/include"
export CXXFLAGS="-I/usr/x86_64-w64-mingw32/include -I${PREFIX}/include"
export LDFLAGS="-L${PREFIX}/lib -L${PREFIX}/lib64"

echo "building fontconfig..."
echo "you possibly need to install a couple of packages like gperf, ..."
export FREETYPE_CFLAGS="-I/usr/x86_64-w64-mingw32/include -I${PREFIX}/include"
export FREETYPE_LIBS="${PREFIX}/lib64"
LOGFILE=${LOG_DIR}/log_fontconfig.txt
cd src/fontconfig-2*
    ./configure --host=x86_64-w64-mingw32 --prefix=${PREFIX} --disable-rpath > ${LOGFILE} 2>&1
    make >> ${LOGFILE} 2>&1
    make install >> ${LOGFILE} 2>&1
cd ../..

echo "building atk..."
LOGFILE=${LOG_DIR}/log_atk.txt
cd src/atk-2*
    ./configure --host=x86_64-w64-mingw32 --prefix=${PREFIX} --disable-rpath --enable-relocatable --enable-static-pie > ${LOGFILE} 2>&1
    make >> ${LOGFILE} 2>&1
    make install >> ${LOGFILE} 2>&1
cd ../..

echo "building cairo..."
LOGFILE=${LOG_DIR}/log_cairo.txt
cd src/cairo-1*
    ./configure --host=x86_64-w64-mingw32 --prefix=${PREFIX} --disable-rpath --enable-relocatable --enable-static-pie > ${LOGFILE} 2>&1
    make >> ${LOGFILE} 2>&1
    make install >> ${LOGFILE} 2>&1
cd ../..

export PKG_CONFIG_PATH="${PREFIX}/lib/pkgconfig"

echo "building pango..."
LOGFILE=${LOG_DIR}/log_pango.txt
cd src/pango-1*
    meson setup . builddir --cross-file ../../cross_file.txt > ${LOGFILE} 2>&1
    cd builddir
        meson compile >> ${LOGFILE} 2>&1
        meson install --destdir=${HOST_ROOT} >> ${LOGFILE} 2>&1
    cd ..
cd ../..

echo "building gtk..."
LOGFILE=${LOG_DIR}/log_gtk.txt
cd src/gtk+-3*
    meson setup . builddir --cross-file ../../cross_file.txt > ${LOGFILE} 2>&1
    cd builddir
        meson compile >> ${LOGFILE} 2>&1
        meson install --destdir=${HOST_ROOT} >> ${LOGFILE} 2>&1
    cd ..
cd ../..


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
