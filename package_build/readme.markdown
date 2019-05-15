
Release crystal_facet_uml
=============

This section is for developers.
It describes packaging and releasing a version of crystal_facet_uml.

Prepare
-----------

Update all version numbers:

>       doxygen_build/Doxyfile : PROJECT_NUMBER
>       main/include/meta/meta_info.h : META_INFO_VERSION_STR
>       user_doc/doc/crystal_facet_user_documentation.xml : date
>       package/build/source_pack.sh : VERSIONSTR
>       package/build/source_test.sh : VERSIONSTR
>       package/build/debian_pack.sh : VERSIONSTR
>       package_build/openSUSE_build_service_config/crystal_facet_uml.spec : Version
>       release_notes.txt : new entry
>       package_build/debian_config/debian/changelog :
>               new entry, e.g. copy latest entry, then call dch -i from folder debian_config


Pack
-----------

>       # build and test source packet
>       ./source_pack.sh
>       ./source_test.sh
>       # build and test for debian, ubuntu, raspbian:
>       ./debian_pack.sh
>       debsigs --sign=origin -k=DA4213C7 crystal-facet-uml_$VERSIONSTR-1_amd64.deb
>       sudo dpkg -i crystal-facet-uml_$VERSIONSTR-1_amd64.deb
>       # build and test for opensuse
>       echo "for the oben suse build service, copy the source package and the spec file to a build directory."
>       echo "create a crystal-facet-uml.changes file there"
>       echo "run the osc (openSUSE build service command-line tool):"
>       osc help signkey
>       osc build --clean --local-package openSUSE_Tumbleweed
>       sudo zypper install /var/tmp/.../crystal-facet-uml-$VERSIONSTR-1.x86_64.rpm
>       echo "Note: the open build server seems to sign the packages, no need to do this locally"


Document
-----------

>       # add a version tag to the git revision:
>       git tag -a v1.12.0 8f17811
>       git push origin master --follow-tags 
>       # write the git revision to release_notes.txt



