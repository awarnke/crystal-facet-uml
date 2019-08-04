
crystal_facet_uml
=============

<!-- What is it? -->
crystal_facet_uml creates sysml/uml diagrams to document system and software architecture.

<!-- For Whom? What can one do with it? Why does it help? -->
As software architect, you create a set of diagrams describing 
use-cases, requirements, structural views, behavioral and deployment views.

<!-- How does the tool solve the task? What is the data flow? Is it interoperable? input/output formats. required Environment? -->
crystal_facet_uml keeps element names and element hierarchies consistent.
It exports diagrams in svg, pdf, ps and png formats
to be used in text processing systems like docbook, html, latex.
This tool runs on your local linux PC and is based on glib, gdk, gtk, cairo, pango, sqlite.

Install crystal_facet_uml
-----------

This section describes how to install a binary package.

Debian/Ubuntu/Raspbian:

>       sudo dpkg -i crystal-facet-uml_1.x.0-1_amd64.deb

openSuSE:

>       sudo zypper install crystal-facet-uml-1.x.0-1.1.x86_64.rpm

How to build from source
-----------

Building from source is described in this section.


debian/ubuntu/raspbian:

> sudo apt install libgtk-3-dev
>
> sudo apt install libsqlite3-dev
>
> sudo apt install cmake
>
> sudo apt install devscripts  # for debian build, includes build-essential
>
> sudo apt install debsigs     # for signing debian packages
>
> sudo apt install dblatex     # for user documentation
>
> sudo apt install xmlto       # for user documentation
>
> sudo apt install doxygen     # for source code documentation


> mkdir my_build && cd my_build
>
> cmake -DCMAKE_BUILD_TYPE=Release ../crystal_facet_uml/       # adapt the source directory name
>
> make crystal_facet_uml


openSuSE:

> sudo zypper install gtk3-devel
>
> sudo zypper install sqlite3-devel
>
> sudo zypper install cmake
>
> sudo zypper install devscripts  # for debian build, includes build-essential
>
> sudo zypper install dblatex     # for user documentation
>
> sudo zypper install xmlto       # for user documentation
>
> sudo zypper install doxygen     # for source code documentation


> mkdir my_build && cd my_build
>
> cmake -DCMAKE_BUILD_TYPE=Release ../crystal_facet_uml/       # adapt the source directory name
>
> make crystal_facet_uml


wine:

> sudo apt install cmake
>
> sudo apt install wine
>
> sudo apt install gcc-mingw-w64-i686
>
> sudo apt install g++-mingw-w64-i686  # not needed but cmake searches for it

> sudo dpkg --add-architecture i386
>
> sudo apt update
>
> sudo apt install wine32

Starting from here, this is a bit of exploratory try-and-error, no final solution yet...

> sudo apt install curl
>
> git clone https://github.com/Microsoft/vcpkg
>
> cd vcpkg
>
> ./bootstrap-vcpkg.sh 
>
> ./vcpkg install gtk:x64-windows


> mkdir my_build && cd my_build
>
> cmake -DCMAKE_SYSTEM_NAME=Windows -DCMAKE_C_COMPILER=/usr/bin/i686-w64-mingw32-gcc -DCMAKE_CXX_COMPILER=/usr/bin/i686-w64-mingw32-g++ ../crystal-facet-uml/       # adapt the source directory name
>
> make crystal_facet_uml

> echo download the sqlite sources (amalgamation) from https://sqlite.org/download.html
> echo clone the git repositories
> git clone https://github.com/GNOME/atk.git
> git clone https://gitlab.freedesktop.org/cairo/cairo.git
> git clone https://github.com/GNOME/gdk-pixbuf.git
> git clone https://github.com/GNOME/glib.git
> git clone https://github.com/GNOME/gtk.git
> git clone https://github.com/GNOME/pango.git


How to run
-----------

> ./crystal_facet_uml

How to install
-----------

> sudo make install

How to use
-----------

> cd ../crystal_facet_uml/user_doc
>
> make
>
> evince crystal_facet_uml_user_documentation.pdf &

Meta-Information
-----------

> Version: see META_INFO_VERSION_STR in main/include/meta/meta_info.h
>
> Author+Copyright: 2016-2019 Andreas Warnke
>
> License: Apache 2.0 License
