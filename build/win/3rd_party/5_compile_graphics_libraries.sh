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
LOG_DIR=`pwd`

export CFLAGS="-I/usr/x86_64-w64-mingw32/include -I${PREFIX}/include -I${PREFIX}/share/gettext -I${PREFIX}/include/glib-2.0 -I${PREFIX}/lib/glib-2.0/include -I${PREFIX}/include/libpng16 -I${PREFIX}/include/freetype2"
export CXXFLAGS="-I/usr/x86_64-w64-mingw32/include -I${PREFIX}/include"
export LDFLAGS="-L${PREFIX}/lib -L${PREFIX}/lib64"
export PKG_CONFIG_PATH="${PREFIX}/lib/pkgconfig:${PREFIX}/lib64/pkgconfig"
# some packages do not produce a package.config file:
#export LIBRARY_PATH="-L/usr/x86_64-w64-mingw32/sys-root/mingw/lib -L${PREFIX}/lib -L${PREFIX}/lib64"

echo `date +'%H:%M'`" building freetype..."
LOGFILE=${LOG_DIR}/log_freetype.txt
cd src/freetype-2*
    ./configure --host=${HOST} --prefix=${PREFIX} > ${LOGFILE} 2>&1
    make >> ${LOGFILE} 2>&1
    make install >> ${LOGFILE} 2>&1
cd ../..

echo `date +'%H:%M'`" building fontconfig..."
echo "      you possibly need to install a couple of packages like gperf, ..."
#if pkg_config is not working, you may need to set a couple of environment variables:
#export FREETYPE_CFLAGS="-I/usr/x86_64-w64-mingw32/include -I${PREFIX}/include"
#export FREETYPE_LIBS="${PREFIX}/lib64"
#export FONTCONFIG_CFLAGS="-I/usr/x86_64-w64-mingw32/include -I${PREFIX}/include"
#export FONTCONFIG_LIBS="${PREFIX}/lib64"
#export CFLAGS="-I/usr/x86_64-w64-mingw32/include -I${PREFIX}/include -I${PREFIX}/include/freetype2"
#export LIBS="-lcairo -lcairo -lcairo -lm  -lcairo -lfreetype -lz -lfontconfig -lpng12 -lpangocairo-1.0 -lpango-1.0 -lcairo -lgobject-2.0 -lgmodule-2.0 -ldl -lglib-2.0 -lfreetype -lz -lfontconfig -lpng"
#export LIBS="-lfreetype"
LOGFILE=${LOG_DIR}/log_fontconfig.txt
cd src/fontconfig-2*
    ./configure --host=${HOST} --prefix=${PREFIX} --disable-rpath --enable-static > ${LOGFILE} 2>&1
    make >> ${LOGFILE} 2>&1
    make install >> ${LOGFILE} 2>&1
cd ../..
#export CFLAGS="-I/usr/x86_64-w64-mingw32/include -I${PREFIX}/include"

echo `date +'%H:%M'`" building atk..."
LOGFILE=${LOG_DIR}/log_atk.txt
cd src/atk-2*
    # tun off introspection support, seems not to be available in my gobject:
    sed -i -e 's/true/false/' meson_options.txt
    meson setup . builddir --cross-file ../../cross_file.txt > ${LOGFILE} 2>&1
    cd builddir
        meson compile >> ${LOGFILE} 2>&1
        meson install --destdir=${HOST_ROOT} >> ${LOGFILE} 2>&1
    cd ..
cd ../..

echo `date +'%H:%M'`" building croco..."
LOGFILE=${LOG_DIR}/log_croco.txt
cd src/libcroco-0*
    ./configure --host=${HOST} --prefix=${PREFIX} > ${LOGFILE} 2>&1
    make >> ${LOGFILE} 2>&1
    make install >> ${LOGFILE} 2>&1
cd ../..

echo `date +'%H:%M'`" building pixman..."
LOGFILE=${LOG_DIR}/log_pixman.txt
cd src/pixman-0*
    ./configure --host=${HOST} --prefix=${PREFIX} > ${LOGFILE} 2>&1
    make >> ${LOGFILE} 2>&1
    make install >> ${LOGFILE} 2>&1
cd ../..

echo `date +'%H:%M'`" building gdk-pixbuf..."
LOGFILE=${LOG_DIR}/log_gdk-pixbuf.txt
cd src/gdk-pixbuf-2*
    meson setup . builddir --cross-file ../../cross_file.txt > ${LOGFILE} 2>&1
    cd builddir
        meson compile >> ${LOGFILE} 2>&1
        meson install --destdir=${HOST_ROOT} >> ${LOGFILE} 2>&1
    cd ..
cd ../..

echo `date +'%H:%M'`" building cairo..."
echo "      expected duration: 15 min"
LOGFILE=${LOG_DIR}/log_cairo.txt
cd src/cairo-1*
    ./configure --host=${HOST} --prefix=${PREFIX} > ${LOGFILE} 2>&1
    make >> ${LOGFILE} 2>&1
    make install >> ${LOGFILE} 2>&1
cd ../..

echo `date +'%H:%M'`" building pango (freebidi, harfbuzz) ..."
LOGFILE=${LOG_DIR}/log_pango.txt
cd src/pango-1*
    meson setup . builddir --cross-file ../../cross_file.txt -Ddefault_library=static > ${LOGFILE} 2>&1
    cd builddir
        meson compile >> ${LOGFILE} 2>&1
        meson install --destdir=${HOST_ROOT} >> ${LOGFILE} 2>&1
    cd ..
cd ../..

echo `date +'%H:%M'`" building gtk..."
LOGFILE=${LOG_DIR}/log_gtk.txt
cd src/gtk+-3*
    meson setup . builddir --cross-file ../../cross_file.txt > ${LOGFILE} 2>&1
    cd builddir
        meson compile >> ${LOGFILE} 2>&1
        meson install --destdir=${HOST_ROOT} >> ${LOGFILE} 2>&1
    cd ..
cd ../..

echo `date +'%H:%M'`" building gail..."
LOGFILE=${LOG_DIR}/log_gail.txt
cd src/gail-1*
    ./configure --host=${HOST} --prefix=${PREFIX} --disable-rpath --enable-relocatable --enable-static-pie > ${LOGFILE} 2>&1
    make >> ${LOGFILE} 2>&1
    make install >> ${LOGFILE} 2>&1
cd ../..

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
