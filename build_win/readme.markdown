
crystal_facet_uml - build for windows/wine
=============

How to build from source using mingw for wine
-----------

wine:

> sudo apt install cmake
>
> sudo apt install wine
>
> sudo apt install gcc-mingw-w64-i686
>
> sudo apt install g++-mingw-w64-i686  # not needed but cmake searches for it

> sudo dpkg --add-architecture i386
>
> sudo apt update
>
> sudo apt install wine32
>
> sudo apt install meson  # to configure the glib sources

Starting from here, this is a bit of exploratory try-and-error, no final solution yet...

> sudo apt install curl
>
> git clone https://github.com/Microsoft/vcpkg
>
> cd vcpkg
>
> ./bootstrap-vcpkg.sh
>
> ./vcpkg install gtk:x64-windows

This part here looks promising:

> mkdir my_build && cd my_build
>
> cmake -DCMAKE_SYSTEM_NAME=Windows -DCMAKE_C_COMPILER=/usr/bin/i686-w64-mingw32-gcc -DCMAKE_CXX_COMPILER=/usr/bin/i686-w64-mingw32-g++ ../crystal-facet-uml/       # adapt the source directory name
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


> mkdir build_glib
>
> cd build_glib/
>
> meson --cross-file ../../crystal-facet-uml-code/build_win/cross_file.txt ../glib



How to run
-----------

> wine crystal_facet_uml.exe

