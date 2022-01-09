#!/bin/sh
echo "fetching gtk"
wget https://download.gnome.org/sources/gtk%2B/3.94/gtk%2B-3.94.0.tar.xz

echo "fetching atk" 
wget https://download.gnome.org/sources/atk/2.9/atk-2.9.4.tar.xz

echo "fetching cairo"
wget https://www.cairographics.org/releases/rcairo-1.17.4.tar.gz

echo "fetching pixman"
wget https://www.cairographics.org/releases/pixman-0.40.0.tar.gz

echo "fetching glib"
wget https://download.gnome.org/sources/glib/2.9/glib-2.9.6.tar.gz 

echo "fetching gdk-pixbuf"
wget https://download.gnome.org/sources/gdk-pixbuf/2.42/gdk-pixbuf-2.42.6.tar.xz

echo "fetching gail"
wget https://download.gnome.org/sources/gail/1.9/gail-1.9.4.tar.gz 

echo "fetching libcroco"
wget https://download.gnome.org/sources/libcroco/0.6/libcroco-0.6.9.tar.xz 

echo "fetching freetype2"
wget --output-document=freetype-2.11.1.tar.xz https://sourceforge.net/projects/freetype/files/freetype2/2.11.1/freetype-2.11.1.tar.xz/download




