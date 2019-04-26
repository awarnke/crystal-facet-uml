
Release crystal_facet_uml
=============

Releasing a version of crystal_facet_uml consists of several steps

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

> ./source_pack.sh
> ./source_test.sh
> ./debian_pack.sh

Document
-----------

> add a version tag to the git revision
> write the git revision to release_notes.txt


