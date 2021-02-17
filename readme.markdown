
crystal-facet-uml
=============

<!-- What is it? -->
crystal-facet-uml creates sysml/uml diagrams to document system and software architecture.

<!-- For Whom? What can one do with it? Why does it help? -->
As software architect, you create a set of diagrams describing
use-cases, requirements, structural views, behavioral and deployment views.

<!-- How does the tool solve the task? What is the data flow? Is it interoperable? input/output formats. required Environment? -->
crystal-facet-uml keeps element names and element hierarchies consistent.
It exports diagrams in svg, pdf, ps and png formats
to be used in text processing systems like docbook, html, latex.
crystal-facet-uml exports the model to xmi format.
This tool runs on your local linux PC and is based on glib, gdk, gtk, cairo, pango, sqlite.

![ScreenShot](user_doc/doc/screenshot_1.png)

![ScreenShot](user_doc/doc/screenshot_2.png)

![ScreenShot](user_doc/doc/screenshot_3.png)


[User Manual](user_doc/crystal-facet-uml_documentation.pdf)

Install crystal-facet-uml
-----------

This section describes how to install a binary package.

Debian/Ubuntu/Raspbian:

>       sudo dpkg -i crystal-facet-uml_1.x.0-1_amd64.deb

openSuSE:

>       sudo zypper install crystal-facet-uml-1.x.0-1.1.x86_64.rpm

wine:

>       unzip crystal-facet-uml_1.x.0-1_win32.zip

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


> mkdir my_build && cd my_build
>
> cmake -DCMAKE_BUILD_TYPE=Release ../crystal-facet-uml/       # adapt the source directory name
>
> make crystal-facet-uml


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


> mkdir my_build && cd my_build
>
> cmake -DCMAKE_BUILD_TYPE=Release ../crystal-facet-uml/       # adapt the source directory name
>
> make crystal-facet-uml


wine:

> see build/win/readme.markdown


How to run
-----------

> ./crystal-facet-uml

How to install
-----------

> sudo make install

How to use
-----------

See user documentation:

> cd ../crystal-facet-uml/user_doc
>
> make
>
> evince crystal-facet-uml_documentation.pdf &

Meta-Information
-----------

> Version: see META_VERSION_STR in main/include/meta/meta_version.inl
>
> Author+Copyright: 2016-2021 Andreas Warnke
>
> License: Apache 2.0 License
