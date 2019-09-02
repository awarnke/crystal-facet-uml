
crystal_facet_uml - build for windows/wine
=============

How to build from source using mingw for wine
-----------

wine on debian/raspbian/ubuntu:

> sudo apt install cmake
>
> sudo apt install wine
>
> sudo apt install gcc-mingw-w64-i686
>
> sudo apt install g++-mingw-w64-i686  # not needed but cmake searches for it



Starting from here, this is a bit of exploratory try-and-error, no final solution yet...

> sudo dpkg --add-architecture i386
>
> sudo apt update
>
> sudo apt install wine32
>
> sudo apt install meson  # to configure the glib sources


Starting from here, this is a bit of exploratory try-and-error, no final solution yet...


> mkdir my_build && cd my_build
>
>
> make crystal_facet_uml

> echo download the sqlite sources (amalgamation) from https://sqlite.org/download.html
>
> echo clone the git repositories
>
> git clone https://github.com/GNOME/atk.git
>
> git clone https://gitlab.freedesktop.org/cairo/cairo.git
>
> git clone https://github.com/GNOME/gdk-pixbuf.git
>
> git clone https://github.com/GNOME/glib.git
>
> git clone https://github.com/GNOME/gtk.git
>
> git clone https://github.com/GNOME/pango.git


Download the gtk-everything archive from the internet
( e.g. gtk+-bundle_3.6.4-20130513_win32.zip (28.8 Mb) from http://www.tarnyko.net/dl/gtk.htm
or sourceforge.net/projects/gtk-win )


download the sqlite sources (amalgamation) from https://sqlite.org/download.html





How to run
-----------

> mv crystal_facet_uml.exe gtk+-bundle_3.6.4-20130513_win32/bin/
> 
> wine crystal_facet_uml.exe

