
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

On Windows(TM), run crystal_facet_uml.exe by doubleclicking the file.

Or, if running on linux using wine64, you may want to perform the following steps:

> 1) download libgcc_s_seh-1.zip from e.g. https://www.dll-files.com or https://www.dllme.com
> 2) unzip libgcc_s_seh-1.zip -d bin/
> 3) check the hash: sha1sum bin/libgcc_s_seh-1.dll - This may result in e.g.
>    fd268f574257da1cecee399d498d5ed63c5daaf3  bin/libgcc_s_seh-1.dll
>    c83e53e537f4c0bef4a6b4aba179f42bb78fa585  bin/libgcc_s_seh-1.dll
> 4) export XDG_DATA_HOME=".\\\\share\\\\" ; wine64 bin/crystal-facet-uml.exe

