
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
>
> mkdir my_build && cd my_build
>
> echo Download the gtk+-bundle archive
>
> echo e.g. gtk+-bundle_3.6.4-20130513_win32.zip (28.8 Mb) from http://www.tarnyko.net/dl/gtk.htm
>
> unzip gtk+-bundle_3.6.4-20130513_win32.zip -d gtk+-bundle_3.6.4-20130513_win32
>
> echo Download the sqlite sources (amalgamation) from https://sqlite.org/download.html
>
> unzip sqlite-amalgamation-3290000.zip -d sqlite-amalgamation-3290000
>
> cmake -DCMAKE_TOOLCHAIN_FILE=../crystal_facet_uml/build_win/mingw_wine_toolchain.cmake ../crystal_facet_uml/build_win
>
> make crystal_facet_uml


How to run
-----------

> mv crystal_facet_uml.exe gtk+-bundle_3.6.4-20130513_win32/bin
>
> cd gtk+-bundle_3.6.4-20130513_win32/bin
>
> wine crystal_facet_uml.exe


How to pack
-----------

> rm -fr crystal_facet_uml
>
> unzip gtk+-bundle_3.6.4-20130513_win32.zip -d crystal_facet_uml
>
> cp crystal_facet_uml.exe crystal_facet_uml/bin
>
> cp ../crystal_facet_uml/installation_win/crystal_facet_uml.bat crystal_facet_uml
>
> cp ../crystal_facet_uml/installation_win/crystal_facet_uml_readme.txt crystal_facet_uml
>
> zip -r crystal-facet-uml_1.15.0_win32.zip crystal_facet_uml
