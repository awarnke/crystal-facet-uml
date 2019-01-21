
crystal_facet_uml
=============

crystal_facet_uml is a tool to create diagrams for documenting software architecture.
These diagrams are consistent in element naming and element hierarchies.

It is based on glib, gdk, gtk, cairo, pango, sqlite, embunit

How to build
-----------

> sudo apt install cmake
>
> sudo apt install libgtk-3-dev
>
> sudo apt install devscripts  # for debian build, includes build-essential
>
> sudo apt install dblatex  # for user documentation
>
> sudo apt install xmlto  # for user documentation
>
> mkdir cmake_build && cd cmake_build
>
> cmake -DCMAKE_BUILD_TYPE=Release ../crystal-facet-uml-code/
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
> evince chapter of crystal_facet_uml_documentation.pdf &

Meta-Information
-----------

> Version: see META_INFO_VERSION_STR in main/include/meta/meta_info.h
>
> Author+Copyright: 2016-2019 Andreas Warnke
>
> License: Apache 2.0 License
>
> Note: This program contains code from other projects with different licenses (see license.txt).
