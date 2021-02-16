#!/bin/sh
. ../../main/include/meta/meta_version.inl
VERSIONSTR=${META_VERSION_STR}

echo "Building Source Package Version $VERSIONSTR"
if [ -z ${VERSIONSTR} ]; then exit; fi
echo "based on latest git commit. (Remember to commit your latest changes!)"
echo "----"
echo "clean up possibly broken previous build"
test -e crystal-facet-uml_$VERSIONSTR.orig.tar.gz && rm crystal-facet-uml_$VERSIONSTR.orig.tar.gz

echo "pack archive"
cd ../..
git archive --format tar.gz --prefix=crystal-facet-uml-$VERSIONSTR/ --output=build/package/crystal-facet-uml_$VERSIONSTR.orig.tar.gz master
cd build/package

echo "----"
echo "output written to crystal-facet-uml_$VERSIONSTR.orig.tar.gz"
echo "test the archive by calling ./source_test.sh"
