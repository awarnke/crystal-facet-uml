#!/bin/sh

mkdir -p packed_src/
cd packed_src/

if test ! -f libiconv* ; then
    echo "fetching libiconv"
    echo "  version 1.17 date 2022-05-15"
    wget https://ftp.gnu.org/pub/gnu/libiconv/libiconv-1.17.tar.gz
fi

if test ! -f libffi* ; then
    echo "fetching libffi"
    echo "  version 3.4.3 date 19-Sep-2022"
    wget ftp://sourceware.org/pub/libffi/libffi-3.4.3.tar.gz
    # Alternative: https://ftp.gwdg.de/pub/linux/sources.redhat.com/libffi/
fi

if test ! -f gettext* ; then
    echo "fetching gettext"
    echo "  version 0.21 date 2020-07-26"
    wget http://ftp.gnu.org/pub/gnu/gettext/gettext-0.21.tar.xz
fi

if test ! -f glib* ; then
    echo "fetching glib"
    echo "  version 2.72.4 date 2022-Sep-21"
    wget https://download.gnome.org/sources/glib/2.72/glib-2.72.4.tar.xz
    # Alternative may be here: http://www.gtk.org/
fi

if test ! -f libxkbcommon* ; then
    echo "fetching xkbcommon"
    echo "  version 1.4.1 date 21-May-2022"
    wget https://xkbcommon.org/download/libxkbcommon-1.4.1.tar.xz
fi

if test ! -f expat* ; then
    echo "fetching expat"
    echo "  version 2.5.0 date 2022-10-26"
    wget https://github.com/libexpat/libexpat/releases/download/R_2_5_0/expat-2.5.0.tar.xz
fi

if test ! -f libxml2* ; then
    echo "fetching libxml2"
    echo "  version 2.9.9 date 2022-Feb-14"
    wget https://download.gnome.org/sources/libxml2/2.9/libxml2-2.9.9.tar.xz
    #echo "  version 2.10.2 date 2022-Aug-29 ... this version has new python 2.7 dependencies"
    #wget https://download.gnome.org/sources/libxml2/2.10/libxml2-2.10.2.tar.xz
    # Alternative may be here: http://www.xmlsoft.org/downloads.html
fi

if test ! -f libjpeg-turbo* ; then
    echo "fetching libjpeg-turbo"
    echo "  version 2.1.4 date 2022-08-12"
    wget --output-document=libjpeg-turbo-2.1.4.tar.gz https://sourceforge.net/projects/libjpeg-turbo/files/2.1.4/libjpeg-turbo-2.1.4.tar.gz/download
fi

if test ! -f gtk* ; then
    echo "fetching gtk"
    echo "  version 4.6.8 date 2022-Oct-24"
    wget https://download.gnome.org/sources/gtk/4.6/gtk-4.6.8.tar.xz
    # echo "  version 4.7.2 date 2022-Aug-11"
    # wget https://download.gnome.org/sources/gtk/4.7/gtk-4.7.2.tar.xz
    # echo "  version 4.9.1 date 2022-Nov-01"
    # wget https://download.gnome.org/sources/gtk/4.9/gtk-4.9.1.tar.xz
fi

if test ! -f atk* ; then
    echo "fetching atk"
    echo "  version 2.38 date 2022-Mar-24"
    wget https://download.gnome.org/sources/atk/2.38/atk-2.38.0.tar.xz
fi

if test ! -f pango* ; then
    echo "fetching pango"
    echo "  version 1.50.4 date 2022-Feb-09"
    wget https://download.gnome.org/sources/pango/1.50/pango-1.50.4.tar.xz
    # echo "  version 1.90.0 date 2022-Jun-22"
    # wget https://download.gnome.org/sources/pango/1.90/pango-1.90.0.tar.xz
fi

if test ! -f cairo* ; then
    echo "fetching cairo"
    echo "  version 1.17.6 date 2022-Mar-18"
    wget https://download.gnome.org/sources/cairo/1.17/cairo-1.17.6.tar.xz
    # Alternative: wget https://www.cairographics.org/releases/cairo-1.16.0.tar.xz
fi

if test ! -f pixman* ; then
    echo "fetching pixman"
    echo "  version 0.42.2 date 2022-11-02"
    wget https://www.cairographics.org/releases/pixman-0.42.2.tar.gz
fi

if test ! -f fontconfig* ; then
    echo "fetching fontconfig"
    echo "  version 2.14.1 date 2022-10-21"
    wget https://www.freedesktop.org/software/fontconfig/release/fontconfig-2.14.1.tar.xz
fi

if test ! -f gdk-pixbuf* ; then
    echo "fetching gdk-pixbuf"
    echo "  version 2.42.6 date 2021-Apr-09"
    wget https://download.gnome.org/sources/gdk-pixbuf/2.42/gdk-pixbuf-2.42.6.tar.xz
    # echo "  version 2.42.9 date 2022-Aug-09"
    # wget https://download.gnome.org/sources/gdk-pixbuf/2.42/gdk-pixbuf-2.42.9.tar.xz
fi

if test ! -f libcroco* ; then
    echo "fetching libcroco"
    echo "  version 0.6.13 date 2019-Apr-06"
    wget https://download.gnome.org/sources/libcroco/0.6/libcroco-0.6.13.tar.xz
fi

if test ! -f freetype* ; then
    echo "fetching freetype2"
    echo "  version 2.12.1 date 2022-05-01"
    wget --output-document=freetype-2.12.1.tar.xz https://sourceforge.net/projects/freetype/files/freetype2/2.12.1/freetype-2.12.1.tar.xz/download
fi

if test ! -f sqlite* ; then
    echo "fetching sqlite3"
    echo "  version 5.2.5 date 2022-01-12"
    wget https://sqlite.org/2022/sqlite-amalgamation-3370200.zip
fi

cd ..


# Copyright 2022-2023 Andreas Warnke
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
