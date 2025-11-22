
![ScreenShot](documentation/user_manual/1_introduction_banner.svg)

crystal-facet-uml
=============

<!-- What is it? -->
crystal-facet-uml creates sysml/uml diagrams to document system and software architecture.

Like a crystal shows different facets of the same thing, this application shows different views of the same system.

![ScreenShot](documentation/user_manual/screenshot_1.png)

<!-- For Whom? What can one do with it? Why does it help? -->
As software architect, you create a set of diagrams describing
use-cases, requirements, structural views, behavioral and deployment views.

<!-- How does the tool solve the task? What is the data flow? Is it interoperable? input/output formats. required Environment? -->
These views show selected elements and their relationships.
crystal-facet-uml keeps element names and element hierarchies consistent.

This tool runs on your local PC.
It stores the model data in a json-based file
which can be stored in git, branched and merged together with your source code.
crystal-facet-uml exports diagrams in svg, pdf, ps and png formats
which can be used in text processing systems like DocBook, html, LaTeX.

![ScreenShot](documentation/user_manual/screenshot_2.png)

![ScreenShot](documentation/user_manual/screenshot_3.png)

How to use this program is described in the [User Manual](https://andreaswarnke.de/crystal-facet-uml/crystal-facet-uml_documentation.pdf) .

Install crystal-facet-uml
-----------

This section describes how to install a binary package.

Debian/Ubuntu/Raspbian:

>       sudo apt install crystal-facet-uml

openSuSE:

>       sudo zypper addrepo https://download.opensuse.org/repositories/devel:/tools/openSUSE_Tumbleweed devel_tools_tumble
>       # or sudo zypper addrepo https://download.opensuse.org/repositories/devel:/tools/15.5 devel_tools_15.5
>       sudo zypper install crystal-facet-uml

Windows/wine:

Fetch a prebuild binary version from e.g. https://www.heise.de/download/product/crystal-facet-uml/
or https://sourceforge.net/projects/crystal-facet-uml/ and unpack it:

>       unzip crystal-facet-uml_1.x.0-1_win64.zip

How to build from source
-----------

Building from source is described in [./build](build) .

About
-----------

> Version: see META_VERSION_STR in [meta_version.inl](main/include/meta/meta_version.inl)
>
> Author and Copyright: 2016-2025 Andreas Warnke
>
> License: Apache 2.0 License
