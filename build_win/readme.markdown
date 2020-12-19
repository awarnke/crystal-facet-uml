
crystal-facet-uml - build for windows/wine
=============

How to prepare for build from source using mingw and wine
-----------

wine on debian/raspbian/ubuntu:

> sudo apt install cmake
>
> sudo apt install wine
>
> sudo dpkg --add-architecture i386 && apt-get update && apt-get install wine32
>
> sudo apt install gcc-mingw-w64-i686
>
> sudo apt install g++-mingw-w64-i686  # not needed but cmake searches for it
>
> echo Download the gtk+-bundle archive to build_win/3rd_party/
>
> echo e.g. gtk+-bundle_3.6.4-20130513_win32.zip (28.8 Mb) from http://www.tarnyko.net/dl/gtk.htm
>
> echo Download the sqlite sources (amalgamation) from https://sqlite.org/download.html to build_win/3rd_party/
>
> echo e.g. sqlite-amalgamation-3290000.zip


How to build and pack
-----------

> cd pack_scripts
>
> ./0_cleanup.sh
>
> ./1_configure.sh
>
> ./2_make.sh
>
> ./3_install.sh
>
> ./4_test.sh
>
> ./5_pack.sh


How to run
-----------

> wine crystal-facet-uml.exe

