
![ScreenShot](user_doc/doc/introduction_banner.png)

crystal-facet-uml
=============

<!-- What is it? -->
crystal-facet-uml creates sysml/uml diagrams to document system and software architecture.

Like a crystal shows different facets of the same thing, this application shows different views of the same system.

![ScreenShot](user_doc/doc/screenshot_1.png)

<!-- For Whom? What can one do with it? Why does it help? -->
As software architect, you create a set of diagrams describing
use-cases, requirements, structural views, behavioral and deployment views.

<!-- How does the tool solve the task? What is the data flow? Is it interoperable? input/output formats. required Environment? -->
crystal-facet-uml keeps element names and element hierarchies consistent.
It exports diagrams in svg, pdf, ps and png formats
to be used in text processing systems like docbook, html, latex.
crystal-facet-uml exports the model to xmi format.
This tool runs on your local linux PC and is based on glib, gdk, gtk, cairo, pango, sqlite.

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

Windows/wine:

>       unzip crystal-facet-uml_1.x.0-1_win32.zip

How to build from source
-----------

Building from source is described in [build](build)

How to run
-----------

> ./crystal-facet-uml

About
-----------

> Version: see META_VERSION_STR in main/include/meta/meta_version.inl
>
> Author+Copyright: 2016-2021 Andreas Warnke
>
> License: Apache 2.0 License
