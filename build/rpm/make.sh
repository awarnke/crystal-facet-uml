#!/bin/sh
. ../../main/include/meta/meta_version.inl
VERSIONSTR=${META_VERSION_STR}

echo "prepare sources"
test -e SOURCES && rm -fr SOURCES
mkdir SOURCES
cd ../..
git archive --format tar.gz --prefix=crystal-facet-uml-$VERSIONSTR/ --output=build/rpm/SOURCES/crystal-facet-uml_$VERSIONSTR.orig.tar.gz master
cd build/rpm

echo "prepare signatures"
echo "%_signature gpg
%_gpg_path /home/andi/.gnupg
%_gpg_name Andreas Warnke
%_gpgbin /usr/bin/gpg" > ~/.rpmmacros

echo "do build"
test -e BUILD && rm -fr BUILD
rpmbuild --define "_topdir `pwd`" -bb SPECS/crystal-facet-uml.spec 
