#!/bin/sh
cd ..
    mkdir -p root/usr/local
    HOST_ROOT=`pwd`/root
    PREFIX=`pwd`/root/usr/local
cd 3rd_party
# host is the prefix of the compiler executables
HOST=x86_64-w64-mingw32
LOG_DIR=`pwd`

export CFLAGS="-I/usr/x86_64-w64-mingw32/include -I${PREFIX}/include -I${PREFIX}/share/gettext"
export CXXFLAGS="-I/usr/x86_64-w64-mingw32/include -I${PREFIX}/include"
export LDFLAGS="-L${PREFIX}/lib -L${PREFIX}/lib64"
export PKG_CONFIG_PATH="${PREFIX}/lib/pkgconfig:${PREFIX}/lib64/pkgconfig"
#export PATH="${PREFIX}/bin:${PATH}"

echo "possibly some tools need to be installed first:"
echo "sudo zypper install meson ninja mingw64-cross-pkgconf mingw64-cross-gcc"

echo `date +'%H:%M'`" building libiconv..."
LOG_FILE=${LOG_DIR}/log_iconv.txt
echo "      log: ${LOG_FILE}"
cd src/libiconv-1*
    ./configure --host=${HOST} --enable-relocatable --prefix=${PREFIX} --disable-rpath --enable-static-pie > ${LOG_FILE} 2>&1
    make >> ${LOG_FILE} 2>&1
    make install >> ${LOG_FILE} 2>&1
cd ../..

echo `date +'%H:%M'`" building libffi..."
LOG_FILE=${LOG_DIR}/log_ffi.txt
echo "      log: ${LOG_FILE}"
cd src/libffi-3*
    ./configure --host=${HOST} --prefix=${PREFIX} --enable-static > ${LOG_FILE} 2>&1
    make >> ${LOG_FILE} 2>&1
    make install >> ${LOG_FILE} 2>&1
cd ../..

echo `date +'%H:%M'`" building gettext..."
LOG_FILE=${LOG_DIR}/log_gettext.txt
echo "      log: ${LOG_FILE}"
echo "      expected duration: 40 min"
cd src/gettext-0*
    # fix the ruby formatstring problem in this version:
    sed -i -e 's/\&formatstring_ruby,/\&formatstring_php,/' gettext-tools/src/format.c
    ./configure --host=${HOST} --enable-relocatable --prefix=${PREFIX} --disable-rpath --disable-libasprintf --disable-java --disable-native-java --disable-openmp > ${LOG_FILE} 2>&1
    make >> ${LOG_FILE} 2>&1
    make install >> ${LOG_FILE} 2>&1
cd ../..

echo `date +'%H:%M'`" building glib (gio, glib, gobject, gmodule, gthread) ..."
echo "      depending on libffi, pcre, proxy-libintl, zlib"
echo "      you possibly need to install a couple of packages like meson, ninja, mingw64-cross-gcc-c++, ..."
LOG_FILE=${LOG_DIR}/log_glib.txt
echo "      log: ${LOG_FILE}"
echo "      expected duration: 15 min"
cd src/glib-2*
    # fix the preprocessor concatenation problem in this version:
    sed -i -e 's/@guint64_constant@/(val ## ULL)/' glib/glibconfig.h.in
    meson setup . builddir --cross-file ../../cross_file.txt -Dprefix=${PREFIX} > ${LOG_FILE} 2>&1
    cd builddir
        # gio tests do not work in my cross build environment:
        meson configure -Dtests=false >> ${LOG_FILE} 2>&1
        meson compile >> ${LOG_FILE} 2>&1
        meson install --destdir=${HOST_ROOT} >> ${LOG_FILE} 2>&1
        # see ../3rd_party/src/glib-2.9.6/docs/reference/glib/html/glib-cross-compiling.html
    cd ..
cd ../..

echo `date +'%H:%M'`" finished. Please check the log files for errors."


# Thanks to many inspiring webpages, especially of help were:
# 1) http://www.tarnyko.net/repo/gtk3_build_system/gtk+-bundle_3.6.4-20130513_win64.zip
#    folder: src/tarnyko/scripts/ (2013-2022)
# 2) https://zwyuan.github.io/2016/07/17/cross-compile-glib-for-android/ (2016-2022)
# 3) https://mesonbuild.com/Cross-compilation.html (2018-2022)


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
