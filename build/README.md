
Build crystal-facet-uml
=============

How to build from source
-----------

Building from source is described in this section.


debian/ubuntu/raspberry-os:

> sudo apt install gcc cmake make
>
> sudo apt install libgtk-4-dev libsqlite3-dev
>
> sudo apt install devscripts debhelper  # for debian build, includes build-essential
>
> sudo apt install yamllint    # for debian script checker, possibly not needed
>
> sudo apt install debsigs     # for signing debian packages, possibly not needed
>
> sudo apt install dblatex xmlto librsvg2-bin  # for user documentation, e.g. rsvg-convert
>
> sudo apt install doxygen     # for source code documentation
>
> sudo apt install lcov        # for reporting test coverage
>
> sudo apt install curl        # for installing rust
>
> curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh # for running qualification tests or icon_artist resource generator


> mkdir my_build && cd my_build
>
> cmake -DCMAKE_BUILD_TYPE=Release ../crystal-facet-uml/  # adapt the source directory path
>
> make crystal-facet-uml
>
> sudo make install


openSuSE:

> sudo zypper install gcc cmake
>
> sudo zypper install gtk4-devel sqlite3-devel
>
> sudo zypper install devscripts  # for debian build, includes build-essential
>
> sudo zypper install dblatex xmlto  # for user documentation
>
> sudo zypper install doxygen     # for source code documentation
>
> sudo zypper install fakeroot    # to build debian archives
>
> sudo zypper install debhelper   # to build debian archives
>
> sudo zypper install lcov        # for reporting test coverage
>
> curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh # for running qualification tests or icon_artist resource generator


> mkdir my_build && cd my_build
>
> cmake -DCMAKE_BUILD_TYPE=Release ../crystal-facet-uml/  # adapt the source directory path
>
> make crystal-facet-uml
>
> sudo make install

windows:

> see  [./msys2/README.md](msys2)
