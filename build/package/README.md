
Release crystal-facet-uml
=============

This section is for developers.
It describes packaging and releasing a version of crystal-facet-uml.

Prepare
-----------

Update all version numbers:

>       build/deb/debian/changelog : new entry, append "-1" to the version
>       build/rpm/SPECS/crystal-facet-uml.spec : Version
>       main/include/meta/meta_version.inl : META_VERSION_STR
>       ChangeLog : new entry
>       documentation/user_manual/crystal_facet_documentation.xml : date
>       cd build/user_manual/ && make && make install


Pack
-----------

>       git commit -a -m 'version x.y.z'
>       # build and test source packet
>       ./source_pack.sh
>       ./source_test.sh
>       cd ../qualification_test && ./run_q_test.sh


Tag and Push
-----------

>       # add a version tag to the git revision:
>       git tag -a v1.12.0 -m "version 1.12.0" 8f17811
>       git push origin master --follow-tags



