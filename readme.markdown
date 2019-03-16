
crystal_facet_uml
=============

crystal_facet_uml is a tool to create diagrams for documenting software architecture.
These diagrams are consistent in element naming and element hierarchies.

It is based on glib, gdk, gtk, cairo, pango, sqlite

How to build
-----------

debian/ubuntu/raspbian:

> sudo apt install libgtk-3-dev
>
> sudo apt install libsqlite3-dev
>
> sudo apt install cmake
>
> sudo apt install devscripts  # for debian build, includes build-essential
>
> sudo apt install dblatex     # for user documentation
>
> sudo apt install xmlto       # for user documentation

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

all:

> mkdir cmake_build && cd cmake_build
>
> cmake -DCMAKE_BUILD_TYPE=Release ../crystal-facet-uml-code/  # or
> cmake -DCMAKE_BUILD_TYPE=Release ../crystal_facet_uml/       # depending on the source directory
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
