#!/bin/sh

HOST_ROOT=`cd .. && pwd`/root
PREFIX=${HOST_ROOT}/usr/local
mkdir -p ${PREFIX}
# host is the prefix of the compiler executables
HOST=x86_64-w64-mingw32
LOG_DIR=`pwd`

export CFLAGS="-I/usr/x86_64-w64-mingw32/include -I${PREFIX}/include -I${PREFIX}/share/gettext"
export CXXFLAGS="${CFLAGS}"
export LDFLAGS="-L${PREFIX}/lib -L${PREFIX}/lib64 -L${PREFIX}/bin"

# for cross compiling, PKG_CONFIG_PATH shall be empty:
export PKG_CONFIG_PATH=
export PKG_CONFIG_LIBDIR="${PREFIX}/lib/pkgconfig:${PREFIX}/lib64/pkgconfig"
export PKG_CONFIG_SYSROOT_DIR="${HOST_ROOT}"
PKG_CONFIG_EXE="/usr/bin/x86_64-w64-mingw32-pkg-config"

echo "      pre: possibly some tools need to be installed first, see ../readme.markdown"

echo `date +'%H:%M'`" building libiconv..."
LOG_FILE=${LOG_DIR}/log_iconv.txt
echo "      log: ${LOG_FILE}"
cd src/libiconv-1*
    ./configure --host=${HOST} --prefix=${PREFIX} > ${LOG_FILE} 2>&1
    make >> ${LOG_FILE} 2>&1
    make install >> ${LOG_FILE} 2>&1
cd ../..
echo "      lib: "`ls ${PREFIX}/bin/libiconv*`

echo `date +'%H:%M'`" building libffi..."
LOG_FILE=${LOG_DIR}/log_ffi.txt
echo "      log: ${LOG_FILE}"
cd src/libffi-3*
    ./configure --host=${HOST} --prefix=${PREFIX} > ${LOG_FILE} 2>&1
    make >> ${LOG_FILE} 2>&1
    make install >> ${LOG_FILE} 2>&1
cd ../..
echo "      lib: "`${PKG_CONFIG_EXE} --libs libffi`

echo `date +'%H:%M'`" building gettext (libintl) ..."
LOG_FILE=${LOG_DIR}/log_gettext.txt
echo "      log: ${LOG_FILE}"
echo "      t  : expected duration: 20 min"
cd src/gettext-0*
    # fix the ruby formatstring problem in version 0.21:
    sed -i -e 's/\&formatstring_ruby,/\&formatstring_php,/' gettext-tools/src/format.c
    ./configure --host=${HOST} --prefix=${PREFIX} --disable-libasprintf --disable-java \
    --disable-native-java --disable-openmp > ${LOG_FILE} 2>&1
    make -j4 >> ${LOG_FILE} 2>&1
    make install >> ${LOG_FILE} 2>&1
cd ../..
echo "      lib: "`ls ${PREFIX}/bin/libgettextlib*`

echo `date +'%H:%M'`" building glib (gio, glib, gobject, gmodule, gthread) ..."
echo "      i  : depending on libffi, pcre, proxy-libintl, zlib"
echo "      pre: you possibly need to install a couple of packages like meson, ninja, mingw64-cross-gcc-c++, ..."
LOG_FILE=${LOG_DIR}/log_glib.txt
echo "      log: ${LOG_FILE}"
echo "      t  : expected duration: 15 min"
cd src/glib-2*
    # fix the preprocessor concatenation problem in version 2.71.0 and 2.72.4:
    sed -i -e 's/@guint64_constant@/(val ## ULL)/' glib/glibconfig.h.in
    rm -fr builddir  # remove artifacts from previous build
    # Note: -Db_pie is needed to be able to run without msvcrt.dll
    meson setup . builddir --cross-file ../../cross_file.txt -Dprefix=${PREFIX} \
    -Dtests=false -Db_pie=false > ${LOG_FILE} 2>&1
    cd builddir
        # gio tests do not work in my cross build environment:
        # meson configure -Dtests=false >> ${LOG_FILE} 2>&1
        # meson configure -Db_pie=true
        meson compile >> ${LOG_FILE} 2>&1
        meson install >> ${LOG_FILE} 2>&1
        # see ../3rd_party/src/glib-2.9.6/docs/reference/glib/html/glib-cross-compiling.html
    cd ..
cd ../..
echo "      lib: "`${PKG_CONFIG_EXE} --libs glib-2.0`

echo `date +'%H:%M'`" building libxml2..."
LOG_FILE=${LOG_DIR}/log_xml.txt
echo "      log: ${LOG_FILE}"
cd src/libxml2-2*
    ./configure --host=${HOST} --prefix=${PREFIX} > ${LOG_FILE} 2>&1
    make >> ${LOG_FILE} 2>&1
    make install >> ${LOG_FILE} 2>&1
cd ../..
echo "      lib: "`${PKG_CONFIG_EXE} --libs libxml-2.0`

echo `date +'%H:%M'`" building xkbcommon ..."
LOG_FILE=${LOG_DIR}/log_xkbcommon.txt
echo "      log: ${LOG_FILE}"
cd src/libxkbcommon*
    # fix the linux function calls for windows cross-compile:
    # find test -type f -name '*.c' | xargs sed -i -e 's/\mkdir(\([a-z]*\), 0777)/mkdir(\1)/'
    # sed -i -e 's/\mkdtemp(/mkdir(/' test/context.c
    # drop the tests from the build ( https://github.com/xkbcommon/libxkbcommon/issues/306 ):
    sed -i '525,769d' meson.build
    rm -fr builddir  # remove artifacts from previous build
    meson setup . builddir --cross-file ../../cross_file.txt -Dprefix=${PREFIX} \
    -Denable-x11=false -Denable-wayland=false -Denable-tools=false -Denable-docs=false > ${LOG_FILE} 2>&1
    cd builddir
        # meson configure -Denable-xkbregistry=false >> ${LOG_FILE} 2>&1
        # set windows target ( https://github.com/xkbcommon/libxkbcommon/issues/305 ):
        meson configure -Dbuild.c_args=-D_MSC_VER=1 >> ${LOG_FILE} 2>&1
        meson compile >> ${LOG_FILE} 2>&1
        meson install >> ${LOG_FILE} 2>&1
    cd ..
cd ../..
echo "      lib: "`${PKG_CONFIG_EXE} --libs xkbcommon`

echo `date +'%H:%M'`" finished. Please check the log files for errors."


# Thanks to many inspiring webpages, especially of help were:
# 1) http://www.tarnyko.net/repo/gtk3_build_system/gtk+-bundle_3.6.4-20130513_win64.zip
#    folder: src/tarnyko/scripts/ (2013-2022)
# 2) https://zwyuan.github.io/2016/07/17/cross-compile-glib-for-android/ (2016-2022)
# 3) https://mesonbuild.com/Cross-compilation.html (2018-2022)
# 4) https://autotools.info/pkgconfig/cross-compiling.html

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
