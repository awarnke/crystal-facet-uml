
Build crystal-facet-uml
=============

How to build from source
-----------

Building from source is described in this section.


debian/ubuntu/raspbian:

> sudo apt install gcc
>
> sudo apt install cmake
>
> sudo apt install libgtk-3-dev
>
> sudo apt install libsqlite3-dev
>
> sudo apt install devscripts  # for debian build, includes possibly build-essential
>
> sudo apt install build-essential  # for debian build, possibly outdated?
>
> sudo apt install yamllint    # for debian script checker
>
> sudo apt install debsigs     # for signing debian packages
>
> sudo apt install dblatex     # for user documentation
>
> sudo apt install xmlto       # for user documentation
>
> sudo apt install doxygen     # for source code documentation
>
> sudo apt install inkscape    # for generation of pdf and png icons from svg
>
> sudo apt install lcov        # for reporting test coverage


> mkdir my_build && cd my_build
>
> cmake -DCMAKE_BUILD_TYPE=Release ../crystal-facet-uml/       # adapt the source directory name
>
> make crystal-facet-uml
>
> sudo make install


openSuSE:

> sudo zypper install gcc
>
> sudo zypper install cmake
>
> sudo zypper install gtk3-devel
>
> sudo zypper install sqlite3-devel
>
> sudo zypper install devscripts  # for debian build, includes build-essential
>
> sudo zypper install dblatex     # for user documentation
>
> sudo zypper install xmlto       # for user documentation
>
> sudo zypper install doxygen     # for source code documentation
>
> sudo zypper install fakeroot    # to build debian archives
>
> sudo zypper install debhelper   # to build debian archives
>
> sudo zypper install inkscape    # for generation of pdf and png icons from svg
>
> sudo zypper install lcov        # for reporting test coverage


> mkdir my_build && cd my_build
>
> cmake -DCMAKE_BUILD_TYPE=Release ../crystal-facet-uml/       # adapt the source directory name
>
> make crystal-facet-uml
>
> sudo make install

wine:

> see win/readme.markdown


