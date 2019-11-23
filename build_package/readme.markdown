
Release crystal_facet_uml
=============

This section is for developers.
It describes packaging and releasing a version of crystal_facet_uml.

Prepare
-----------

Update all version numbers:

>       build_doxygen/Doxyfile : PROJECT_NUMBER
>       build_package/make.sh : VERSIONSTR
>       build_deb/make.sh : VERSIONSTR
>       build_deb/debian/changelog : new entry
>       build_rpm/make.sh : VERSIONSTR
>       build_rpm/SPECS/crystal_facet_uml.spec : Version
>       main/include/meta/meta_info.h : META_INFO_VERSION_STR
>       release_notes.txt : new entry
>       user_doc/doc/crystal_facet_user_documentation.xml : date


Pack
-----------

>       # commit all changes before packing !
>       # build and test source packet
>       ./source_pack.sh
>       ./source_test.sh


Document
-----------

>       # add a version tag to the git revision:
>       git tag -a v1.12.0 8f17811
>       git push origin master --follow-tags



