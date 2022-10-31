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

#if test ! -f libxkbcommon* ; then
#    echo "fetching xkbcommon"
#    echo "  version 0.4.3 date 19-Aug-2014"
#    wget https://xkbcommon.org/download/libxkbcommon-0.4.3.tar.xz
#fi

if test ! -f xkbcommon* ; then
    echo "fetching xkbcommon"
    echo "  version 1.4.1 date 21-May-2022"
    wget https://xkbcommon.org/download/libxkbcommon-1.4.1.tar.xz
fi

if test ! -f expat* ; then
    echo "fetching expat"
    echo "  version 0.40.0 date 2020-04-19"
    wget https://github.com/libexpat/libexpat/releases/download/R_2_4_4/expat-2.4.4.tar.xz
fi

if test ! -f libxml2* ; then
    echo "fetching libxml2"
    echo "  version 2.9.9 date 2022-Feb-14"
    wget https://download.gnome.org/sources/libxml2/2.9/libxml2-2.9.9.tar.xz
    # Alternative may be here: http://www.xmlsoft.org/downloads.html
fi

if test ! -f libjpeg-turbo* ; then
    echo "fetching libjpeg-turbo"
    echo "  version 2.1.3 date 2022-02-25"
    wget --output-document=libjpeg-turbo-2.1.3.tar.gz https://sourceforge.net/projects/libjpeg-turbo/files/2.1.3/libjpeg-turbo-2.1.3.tar.gz/download
fi

#if test ! -f gtk* ; then
#    echo "fetching gtk"
#    echo "  version 3.92 date 2017-Oct-18"
#    wget https://download.gnome.org/sources/gtk%2B/3.92/gtk%2B-3.92.1.tar.xz
#    # Note: there is an alternative download at https://download-fallback.gnome.org
#fi

#if test ! -f gtk* ; then
#    echo "fetching gtk"
#    echo "  version 3.94 date 2018-Jun-26"
#    wget https://download.gnome.org/sources/gtk%2B/3.94/gtk%2B-3.94.0.tar.xz
#    # Note: there is an alternative download at https://download-fallback.gnome.org
#fi

if test ! -f gtk* ; then
    echo "fetching gtk"
    echo "  version 4.6.2 date 2022-Mar-19"
    wget https://download.gnome.org/sources/gtk/4.6/gtk-4.6.2.tar.xz
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
fi

if test ! -f cairo* ; then
    echo "fetching cairo"
    echo "  version 1.16.0 date 2018-10-19"
    wget https://www.cairographics.org/releases/cairo-1.16.0.tar.xz
fi

if test ! -f pixman* ; then
    echo "fetching pixman"
    echo "  version 0.40.0 date 2020-04-19"
    wget https://www.cairographics.org/releases/pixman-0.40.0.tar.gz
fi

if test ! -f fontconfig* ; then
    echo "fetching fontconfig"
    echo "  version 2.13.96 date 2022-02-04"
    wget https://www.freedesktop.org/software/fontconfig/release/fontconfig-2.13.96.tar.xz
fi

if test ! -f gdk-pixbuf* ; then
    echo "fetching gdk-pixbuf"
    echo "  version 2.42.6 date 2021-Apr-09"
    wget https://download.gnome.org/sources/gdk-pixbuf/2.42/gdk-pixbuf-2.42.6.tar.xz
fi

#if test ! -f gail* ; then
#    echo "fetching gail"
#    echo "  version 1.22.3 date 2019-May-16"
#    wget https://download.gnome.org/sources/gail/1.22/gail-1.22.3.tar.gz
#fi

if test ! -f libcroco* ; then
    echo "fetching libcroco"
    echo "  version 0.6.13 date 2019-Apr-06"
    wget https://download.gnome.org/sources/libcroco/0.6/libcroco-0.6.13.tar.xz
fi

if test ! -f freetype* ; then
    echo "fetching freetype2"
    echo "  version 2.11.1 date 2021-12-02"
    wget --output-document=freetype-2.11.1.tar.xz https://sourceforge.net/projects/freetype/files/freetype2/2.11.1/freetype-2.11.1.tar.xz/download
fi

#if test ! -f xz* ; then
#    echo "fetching lzma"
#    echo "  version 5.2.5 date 2020-03-17"
#    wget https://tukaani.org/xz/xz-5.2.5.tar.gz
#fi

if test ! -f sqlite* ; then
    echo "fetching sqlite3"
    echo "  version 5.2.5 date 2022-01-12"
    wget https://sqlite.org/2022/sqlite-amalgamation-3370200.zip
fi

cd ..


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
