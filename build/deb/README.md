
Release crystal_facet_uml
=============

This section is for developers.
It describes packaging and releasing a version of crystal_facet_uml.

Pack
-----------

>       # commit all changes before packing !
>       # build and test for debian, ubuntu, raspbian:
>       ./make.sh
>       debsigs --sign=origin -k=DA4213C7 crystal-facet-uml_$VERSIONSTR-1_amd64.deb
>       sudo dpkg -i crystal-facet-uml_$VERSIONSTR-1_amd64.deb
