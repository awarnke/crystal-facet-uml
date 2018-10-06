
crystal_facet_uml
=============

crystal_facet_uml is a uml design tool
to create a set of consistent uml diagrams.

These diagrams can be exported to various vector-image formats.

It is based on glib, gdk, gtk, cairo, pango, sqlite, utf8stringbuf, embunit

How to build
-----------

> sudo apt-get install cmake
>
> sudo apt-get install libgtk-3-dev
>
> sudo apt-get install devscripts
>
> cd cmake_build
>
> cmake .
>
> make -j5    # j5 starts 5 jobs in parallel to efficiently use 4 cores

How to run
-----------

> ./crystal_facet_uml

How to install
-----------

> sudo make install

How to use
-----------

> cd doxygen_build
>
> ./make.sh
>
> echo read first chapter of crystal_facet_uml_documentation.pdf

Meta-Information
-----------

> Version: see PROJECT_NUMBER in doxygen_build/doxygen_config
>
> Author+Copyright: 2016-2018 Andreas Warnke
>
> License: Apache 2.0 License
>
> Note: This program contains code from other projects with different licenses (see license.txt).
