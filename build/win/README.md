
crystal-facet-uml - build for windows/wine
=============

How to prepare for build from source using mingw and wine
-----------

wine on debian/raspbian/ubuntu:

> sudo apt install cmake meson
>
> sudo apt install wine
>
> sudo apt install gcc-mingw-w64-x86-64 mingw-w64-tools
>
> sudo apt install wget

wine on opensuse:

> sudo zypper install cmake meson ninja
>
> sudo zypper install wine  # includes wine64
>
> sudo zypper install mingw64-gcc mingw64-cross-pkgconf mingw64-cross-gcc

How to build and pack
-----------

> cd 3rd_party
>
> 0_clean.sh
>
> 1_fetch_sources.sh
>
> 2_unpack_sources.sh
>
> 3_compile_data_handling_libraries.sh
>
> 4_compile_platform_abstration.sh
>
> 5_compile_graphics_libraries.sh
>
> 6_collect_licenses.sh
>
> 7_check_result.sh
>
> cd ..
>
> 8_compile_crystal_facet_uml.sh
>
> 9_pack.sh

How to run
-----------

On Windows(TM), run crystal_facet_uml.exe by doubleclicking the file.

Or, if running on linux using wine64, start crystal_facet_uml.exe by calling

> XDG_DATA_HOME=".\\\\share\\\\" wine64 bin/crystal-facet-uml.exe
