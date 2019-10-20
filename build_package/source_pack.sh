#!/bin/sh
VERSIONSTR=1.15.0
echo "Building Archive Version $VERSIONSTR"
echo "based on latest git commit. (Remember to commit your latest changes!)"
echo "----"
echo "clean up possibly broken previous build"
test -e crystal-facet-uml_$VERSIONSTR.orig.tar.gz && rm crystal-facet-uml_$VERSIONSTR.orig.tar.gz

echo "pack archive"
cd ..
git archive --format tar.gz --prefix=crystal-facet-uml-$VERSIONSTR/ --output=build_package/crystal-facet-uml_$VERSIONSTR.orig.tar.gz master
cd build_package

echo "----"
echo "output written to crystal-facet-uml_$VERSIONSTR.orig.tar.gz"
echo "test the archive by calling ./source_test.sh"
