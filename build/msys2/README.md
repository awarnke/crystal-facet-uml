
crystal-facet-uml - build for windows using msys2
=============

How to build from source
-----------

install msys2.

update the package databases:

> pacman -Syu

install packages using pacman:

> pacman -S git
> pacman -S mingw-w64-ucrt-x86_64-sqlite3
> pacman -S mingw-w64-ucrt-x86_64-gtk4
> pacman -S mingw-w64-ucrt-x86_64-gcc
> pacman -S make
> pacman -S cmake
> pacman -S mingw-w64-ucrt-x86_64-7zip

get crystal-facet-uml:

> git clone https://github.com/awarnke/crystal-facet-uml.git

build:

> windres ../crystal-facet-uml/build/msys2/crystal-facet-uml.rc -O coff -o crystal-facet-uml.res
> cmake -DCOMPILE_IN_SOURCE_FILE="crystal-facet-uml.res" -DCMAKE_BUILD_TYPE=Release ../crystal-facet-uml
> make

deploy:

> ldd crystal-facet-uml.exe | grep -e 'ucrt' | sort
> echo https://www.gtk.org/docs/installations/windows


An information - maybe for later if needed:

> Note that 'C:/msys64/mingw64/share' is not in the search path
> set by the XDG_DATA_HOME and XDG_DATA_DIRS
> environment variables, so applications may not
> be able to find it until you set them. The
> directories currently searched are:
>
> - C:\msys64\home\andre\.local\share
> - C:\msys64\ucrt64\share\
> - C:\msys64\usr\local\share\
> - C:\msys64\usr\share\
