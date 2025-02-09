
crystal-facet-uml - build for windows using msys2
=============

How to build from source
-----------

install msys2.

update the package databases:

> pacman -Syu

install packages using pacman:

> pacman -S git
>
> pacman -S mingw-w64-ucrt-x86_64-sqlite3
>
> pacman -S mingw-w64-ucrt-x86_64-gtk4
>
> pacman -S mingw-w64-ucrt-x86_64-gcc
>
> pacman -S make
>
> pacman -S cmake
>
> pacman -S mingw-w64-ucrt-x86_64-7zip

get crystal-facet-uml:

> git clone https://github.com/awarnke/crystal-facet-uml.git

build (adapt the path to the source code ../crystal-facet-uml):

> mkdir -p tmp
>
> windres ../crystal-facet-uml/build/msys2/crystal-facet-uml.rc -O coff -o tmp/crystal-facet-uml.res
>
> cmake -DCOMPILE_IN_LINKER_FLAG="tmp/crystal-facet-uml.res" -DCOMPILE_IN_SOURCE_FILE="tmp/crystal-facet-uml.res" \\\
>       -DCMAKE_C_FLAGS="-mwindows" -DCMAKE_BUILD_TYPE=Release ../crystal-facet-uml
>
> make --debug=p -j12

deploy:

> make msys2_deploy

see also https://www.gtk.org/docs/installations/windows

run:

> append 'C:/msys64/mingw64/share' to XDG_DATA_HOME and XDG_DATA_DIRS

