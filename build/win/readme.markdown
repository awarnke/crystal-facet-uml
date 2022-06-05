
crystal-facet-uml - build for windows/wine
=============

How to prepare for build from source using mingw and wine
-----------

wine on debian/raspbian/ubuntu:

> sudo apt install cmake
>
> sudo apt install wine
>
> sudo apt install gcc-mingw-w64-x86_64

wine on opensuse:

> sudo zypper install cmake
>
> sudo zypper install wine
>
> zypper install mingw64-gcc
>

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
> 3_compile_platform_abstration.sh
>
> 4_compile_data_handling_libraries.sh
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

Run on Windows(TM)

