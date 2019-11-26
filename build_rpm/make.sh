#!/bin/sh
VERSIONSTR=1.16.1

echo "prepare sources"
test -e SOURCES && rm -fr SOURCES
mkdir SOURCES
cd ..
git archive --format tar.gz --prefix=crystal-facet-uml-$VERSIONSTR/ --output=build_rpm/SOURCES/crystal-facet-uml_$VERSIONSTR.orig.tar.gz master
cd build_rpm

echo "prepare signatures"
echo "%_signature gpg
%_gpg_path /home/andi/.gnupg
%_gpg_name Andreas Warnke
%_gpgbin /usr/bin/gpg" > ~/.rpmmacros

echo "do build"
test -e BUILD && rm -fr BUILD
rpmbuild --define "_topdir `pwd`" -bb SPECS/crystal-facet-uml.spec --sign
