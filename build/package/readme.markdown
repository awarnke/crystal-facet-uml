
Release crystal-facet-uml
=============

This section is for developers.
It describes packaging and releasing a version of crystal-facet-uml.

Prepare
-----------

Update all version numbers:

>       build/deb/debian/changelog : new entry
>       build/rpm/SPECS/crystal-facet-uml.spec : Version
>       main/include/meta/meta_version.inl : META_VERSION_STR
>       ChangeLog : new entry
>       user_doc/doc/crystal_facet_documentation.xml : date


Pack
-----------

>       git commit -a -m 'version x.y.z'
>       # build and test source packet
>       ./source_pack.sh
>       ./source_test.sh


Tag and Push
-----------

>       # add a version tag to the git revision:
>       git tag -a v1.12.0 8f17811
>       git push origin master --follow-tags



