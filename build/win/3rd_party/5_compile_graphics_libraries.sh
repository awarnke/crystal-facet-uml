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
export LDFLAGS="-L${PREFIX}/lib -L${PREFIX}/lib64 -L${PREFIX}/bin -lharfbuzz -ljpeg"
#export PKG_CONFIG_PATH="${PREFIX}/lib/pkgconfig:${PREFIX}/lib64/pkgconfig"
export PKG_CONFIG_PATH=
export PKG_CONFIG_LIBDIR="${PREFIX}/lib/pkgconfig:${PREFIX}/lib64/pkgconfig"
export PKG_CONFIG_SYSROOT_DIR="${HOST_ROOT}"
PKG_CONFIG_EXE="/usr/bin/x86_64-w64-mingw32-pkg-config"
# some packages do not produce a package.config file:
#export LIBRARY_PATH="-L/usr/x86_64-w64-mingw32/sys-root/mingw/lib -L${PREFIX}/lib -L${PREFIX}/lib64"

echo `date +'%H:%M'`" building jpeg..."
LOG_FILE=${LOG_DIR}/log_jpeg.txt
echo "      log: ${LOG_FILE}"
cd src/libjpeg-turbo-2*
    cmake -DCMAKE_TOOLCHAIN_FILE=../../../mingw_wine_toolchain.cmake -DCMAKE_INSTALL_PREFIX=${PREFIX} . > ${LOG_FILE} 2>&1
    make >> ${LOG_FILE} 2>&1
    make install >> ${LOG_FILE} 2>&1
cd ../..
echo "      lib: `${PKG_CONFIG_EXE} --libs libturbojpeg`"

echo `date +'%H:%M'`" building pixman..."
LOG_FILE=${LOG_DIR}/log_pixman.txt
echo "      log: ${LOG_FILE}"
cd src/pixman-0*
    ./configure --host=${HOST} --prefix=${PREFIX} > ${LOG_FILE} 2>&1
    make >> ${LOG_FILE} 2>&1
    make install >> ${LOG_FILE} 2>&1
cd ../..
echo "      lib: `${PKG_CONFIG_EXE} --libs pixman-1`"

echo `date +'%H:%M'`" building gdk-pixbuf (libpng)..."
LOG_FILE=${LOG_DIR}/log_gdk-pixbuf.txt
echo "      log: ${LOG_FILE}"
cd src/gdk-pixbuf-2*
    rm -fr builddir  # remove artifacts from previous build
    meson setup . builddir --cross-file ../../cross_file.txt -Dprefix=${PREFIX} > ${LOG_FILE} 2>&1
    cd builddir
        meson compile >> ${LOG_FILE} 2>&1
        meson install >> ${LOG_FILE} 2>&1
    cd ..
cd ../..
echo "      lib: `${PKG_CONFIG_EXE} --libs gdk-pixbuf-2.0`"

echo `date +'%H:%M'`" building freetype..."
LOG_FILE=${LOG_DIR}/log_freetype.txt
echo "      log: ${LOG_FILE}"
cd src/freetype-2*
    ./configure --host=${HOST} --prefix=${PREFIX} > ${LOG_FILE} 2>&1
    make >> ${LOG_FILE} 2>&1
    make install >> ${LOG_FILE} 2>&1
cd ../..
echo "      lib: `${PKG_CONFIG_EXE} --libs freetype2`"

echo `date +'%H:%M'`" building fontconfig..."
echo "      you possibly need to install package gperf."
#if pkg_config is not working, you may need to set a couple of environment variables:
#export FREETYPE_CFLAGS="-I/usr/x86_64-w64-mingw32/include -I${PREFIX}/include"
#export FREETYPE_LIBS="${PREFIX}/lib64"
#export FONTCONFIG_CFLAGS="-I/usr/x86_64-w64-mingw32/include -I${PREFIX}/include"
#export FONTCONFIG_LIBS="${PREFIX}/lib64"
#export CFLAGS="-I/usr/x86_64-w64-mingw32/include -I${PREFIX}/include -I${PREFIX}/include/freetype2"
#export LIBS="-lcairo -lcairo -lcairo -lm  -lcairo -lfreetype -lz -lfontconfig -lpng12 -lpangocairo-1.0 -lpango-1.0 -lcairo -lgobject-2.0 -lgmodule-2.0 -ldl -lglib-2.0 -lfreetype -lz -lfontconfig -lpng"
#export LIBS="-lfreetype"
LOG_FILE=${LOG_DIR}/log_fontconfig.txt
echo "      log: ${LOG_FILE}"
cd src/fontconfig-2*
    #./configure --host=${HOST} --prefix=${PREFIX} --disable-rpath --enable-static > ${LOG_FILE} 2>&1
    ./configure --host=${HOST} --prefix=${PREFIX} > ${LOG_FILE} 2>&1
    make >> ${LOG_FILE} 2>&1
    make install >> ${LOG_FILE} 2>&1
cd ../..
echo "      lib: `${PKG_CONFIG_EXE} --libs fontconfig`"
#export CFLAGS="-I/usr/x86_64-w64-mingw32/include -I${PREFIX}/include"

echo `date +'%H:%M'`" building atk..."
LOG_FILE=${LOG_DIR}/log_atk.txt
echo "      log: ${LOG_FILE}"
cd src/atk-2*
    # turn off introspection support, seems not to be available in my gobject:
    sed -i -e 's/true/false/' meson_options.txt
    rm -fr builddir  # remove artifacts from previous build
    meson setup . builddir --cross-file ../../cross_file.txt -Dprefix=${PREFIX} > ${LOG_FILE} 2>&1
    cd builddir
        meson compile >> ${LOG_FILE} 2>&1
        meson install >> ${LOG_FILE} 2>&1
    cd ..
cd ../..
echo "      lib: `${PKG_CONFIG_EXE} --libs atk`"

echo `date +'%H:%M'`" building croco..."
LOG_FILE=${LOG_DIR}/log_croco.txt
echo "      log: ${LOG_FILE}"
cd src/libcroco-0*
    ./configure --host=${HOST} --prefix=${PREFIX} > ${LOG_FILE} 2>&1
    make >> ${LOG_FILE} 2>&1
    make install >> ${LOG_FILE} 2>&1
cd ../..
echo "      lib: `${PKG_CONFIG_EXE} --libs libcroco-0.6`"

echo `date +'%H:%M'`" building cairo..."
LOG_FILE=${LOG_DIR}/log_cairo.txt
echo "      log: ${LOG_FILE}"
echo "      expected duration: 15 min"
cd src/cairo-1*
    ./configure --host=${HOST} --prefix=${PREFIX} > ${LOG_FILE} 2>&1
    make >> ${LOG_FILE} 2>&1
    make install >> ${LOG_FILE} 2>&1
cd ../..
echo "      lib: `${PKG_CONFIG_EXE} --libs cairo`"

echo `date +'%H:%M'`" building pango (freebidi, harfbuzz) ..."
LOG_FILE=${LOG_DIR}/log_pango.txt
echo "      log: ${LOG_FILE}"
echo "      expected duration: 15 min"
cd src/pango-1*
    # meson setup . builddir --cross-file ../../cross_file.txt -Ddefault_library=static -Dprefix=${PREFIX} > ${LOG_FILE} 2>&1
    rm -fr builddir  # remove artifacts from previous build
    meson setup . builddir --cross-file ../../cross_file.txt -Dprefix=${PREFIX} > ${LOG_FILE} 2>&1
    cd builddir
        # gio tests do not work in my cross build environment:
        meson configure -Dtests=false -Dglib.tests=false >> ${LOG_FILE} 2>&1
        meson compile >> ${LOG_FILE} 2>&1
        meson install >> ${LOG_FILE} 2>&1
    cd ..
cd ../..
echo "      lib: `${PKG_CONFIG_EXE} --libs pango`"

echo `date +'%H:%M'`" building gtk..."
LOG_FILE=${LOG_DIR}/log_gtk.txt
echo "      log: ${LOG_FILE}"
echo "      expected duration: 15 min"
#cd src/gtk+-3*
#    # no xkbdep needed for wine
#    #sed -i -e "s/^\(xkbdep[^)]*\))$/\1, required: false)/" meson.build
#    rm -fr builddir  # remove artifacts from previous build
#    meson setup . builddir --cross-file ../../cross_file.txt -Dprefix=${PREFIX} -Denable-win32-backend=true -Denable-x11-backend=false -Denable-wayland-backend=false -Denable-broadway-backend=false -Denable-mir-backend=false -Denable-quartz-backend=false -Denable-cloudproviders=false -Dintrospection=false -Dbuild-tests=false -Ddemos=false -Dwith-included-immodules=none -Denable-cloudprint-print-backend=no -Denable-cups-print-backend=no -Denable-papi-print-backend=no -Denable-xinerama=no > ${LOG_FILE} 2>&1
#    cd builddir
#        meson compile >> ${LOG_FILE} 2>&1
#        meson install >> ${LOG_FILE} 2>&1
#    cd ..
#cd ../..
cd src/gtk-4*
    # GetLocaleInfoEx is not available in my wine emulation (2022)
    sed -i -e 's/#ifdef G_OS_WIN32/#if 0/' gtk/language-names.c
    rm -fr builddir  # remove artifacts from previous build
    meson setup . builddir --cross-file ../../cross_file.txt -Dprefix=${PREFIX} \
    -Dwin32-backend=true -Dx11-backend=false -Dwayland-backend=false \
    -Dbroadway-backend=false -Dmacos-backend=false \
    -Dmedia-ffmpeg=disabled -Dmedia-gstreamer=disabled \
    -Dprint-cups=disabled \
    -Dsysprof=disabled -Dcloudproviders=disabled -Dvulkan=disabled \
    -Dtracker=disabled -Dcolord=disabled \
    -Dintrospection=disabled -Dmedia-gtk_doc=false -Dman-pages=false \
    -Dbuild-tests=false -Ddemos=false \
    -Dprofile=default -Dinstall-tests=false -Dbuild-examples=false > ${LOG_FILE} 2>&1
    cd builddir
        meson compile >> ${LOG_FILE} 2>&1
        meson install >> ${LOG_FILE} 2>&1
    cd ..
cd ../..
echo "      lib: `${PKG_CONFIG_EXE} --libs gtk4`"

#echo `date +'%H:%M'`" building gail..."
#LOG_FILE=${LOG_DIR}/log_gail.txt
#echo "      log: ${LOG_FILE}"
#cd src/gail-1*
#    # ./configure --host=${HOST} --prefix=${PREFIX} --disable-rpath --enable-relocatable --enable-static-pie > ${LOG_FILE} 2>&1
#    ./configure --host=${HOST} --prefix=${PREFIX} > ${LOG_FILE} 2>&1
#    make >> ${LOG_FILE} 2>&1
#    make install >> ${LOG_FILE} 2>&1
#cd ../..

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
