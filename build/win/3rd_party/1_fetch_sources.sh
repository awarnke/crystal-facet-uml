#!/bin/sh
mkdir -p packed_src/
cd packed_src/

if test ! -f gtk* ; then
    echo "fetching gtk"
    wget https://download.gnome.org/sources/gtk%2B/3.94/gtk%2B-3.94.0.tar.xz
    # Note: there is an alternative download at https://download-fallback.gnome.org
fi

if test ! -f atk* ; then
    echo "fetching atk"
    wget https://download.gnome.org/sources/atk/2.9/atk-2.9.4.tar.xz
fi

if test ! -f pango* ; then
    echo "fetching pango"
    wget https://download.gnome.org/sources/pango/1.50/pango-1.50.3.tar.xz
fi

if test ! -f rcairo* ; then
    echo "fetching cairo"
    wget https://www.cairographics.org/releases/rcairo-1.17.4.tar.gz
fi

if test ! -f pixman* ; then
    echo "fetching pixman"
    wget https://www.cairographics.org/releases/pixman-0.40.0.tar.gz
fi

if test ! -f fontconfig* ; then
    echo "fetching fontconfig"
    wget https://www.freedesktop.org/software/fontconfig/release/fontconfig-2.13.94.tar.xz
fi

if test ! -f glib* ; then
    echo "fetching glib"
    wget https://download.gnome.org/sources/glib/2.9/glib-2.9.6.tar.gz
    # Alternative may be here: http://www.gtk.org/
fi

if test ! -f libiconv* ; then
    echo "fetching libiconv"
    wget https://ftp.gnu.org/pub/gnu/libiconv/libiconv-1.16.tar.gz
fi

if test ! -f gdk-pixbuf* ; then
    echo "fetching gdk-pixbuf"
    wget https://download.gnome.org/sources/gdk-pixbuf/2.42/gdk-pixbuf-2.42.6.tar.xz
fi

if test ! -f gail* ; then
    echo "fetching gail"
    wget https://download.gnome.org/sources/gail/1.9/gail-1.9.4.tar.gz
fi

if test ! -f libcroco* ; then
    echo "fetching libcroco"
    wget https://download.gnome.org/sources/libcroco/0.6/libcroco-0.6.9.tar.xz
fi

if test ! -f freetype* ; then
    echo "fetching freetype2"
    wget --output-document=freetype-2.11.1.tar.xz https://sourceforge.net/projects/freetype/files/freetype2/2.11.1/freetype-2.11.1.tar.xz/download
fi

if test ! -f xz* ; then
    echo "fetching lzma"
    wget https://tukaani.org/xz/xz-5.2.5.tar.gz
fi

if test ! -f sqlite* ; then
    echo "fetching sqlite3"
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
