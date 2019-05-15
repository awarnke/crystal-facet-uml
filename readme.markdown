
crystal_facet_uml
=============

crystal_facet_uml creates diagrams to document software architecture.
These diagrams are consistent in element naming and element hierarchies.
Diagrams are exported as png, svg, pdf, ps
to be used in text processing systems as docbook, html, latex.

It is based on glib, gdk, gtk, cairo, pango, sqlite.

Install crystal_facet_uml
-----------

This section describes how to install a binary package.

Debian/Ubuntu/Raspbian:

>       sudo dpkg -i crystal-facet-uml_1.x.0-1_amd64.deb

openSuSE:

>       sudo zypper install crystal-facet-uml-1.x.0-1.1.x86_64.rpm

How to build from source
-----------

Building and installing a source package is described in this section.

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

all:

> mkdir my_build && cd my_build
>
> cmake -DCMAKE_BUILD_TYPE=Release ../crystal_facet_uml/       # adapt the source directory name
>
> make crystal_facet_uml

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
