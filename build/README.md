
Build crystal-facet-uml
=============

How to build from source
-----------

Building from source is described in this section.


debian/ubuntu/raspbian:

> sudo apt install gcc cmake make
>
> sudo apt install libgtk-4-dev libsqlite3-dev  # or libgtk-3-dev
>
> sudo apt install devscripts  # for debian build, includes build-essential
>
> sudo apt install yamllint    # for debian script checker, possibly not needed
>
> sudo apt install debsigs     # for signing debian packages, possibly not needed
>
> sudo apt install dblatex xmlto librsvg2-bin   # for user documentation, e.g. rsvg-convert
>
> sudo apt install doxygen     # for source code documentation
>
> sudo apt install inkscape    # for generation of pdf and png icons from svg
>
> sudo apt install lcov        # for reporting test coverage
>
> sudo apt install cargo       # for running qualification tests, includes rustc


> mkdir my_build && cd my_build
>
> cmake -DCMAKE_BUILD_TYPE=Release ../crystal-facet-uml/       # adapt the source directory name
>
> make crystal-facet-uml
>
> sudo make install


openSuSE:

> sudo zypper install gcc cmake
>
> sudo zypper install gtk4-devel  # or gtk3-devel; switch also the comment in CMakeLists.txt
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
>
> sudo zypper install cargo       # for running the rust-based qualification tests


> mkdir my_build && cd my_build
>
> cmake -DCMAKE_BUILD_TYPE=Release ../crystal-facet-uml/       # adapt the source directory name
>
> make crystal-facet-uml
>
> sudo make install

wine:

> see  [./win/README.md](win)


