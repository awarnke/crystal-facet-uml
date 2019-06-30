#!/bin/sh
VERSIONSTR=1.14.0
echo "Building Archive Version $VERSIONSTR"
echo "based on latest git commit. (Remember to commit your latest changes!)"
echo "----"
echo "clean up possibly broken previous build"
test -e crystal-facet-uml_$VERIONSTR.orig.tar.gz && rm crystal-facet-uml_$VERIONSTR.orig.tar.gz

echo "pack archive"
cd ..
git archive --format tar.gz --prefix=crystal-facet-uml-$VERSIONSTR/ --output=package_build/crystal-facet-uml_$VERSIONSTR.orig.tar.gz master
cd package_build

echo "----"
echo "output written to crystal-facet-uml_$VERIONSTR.orig.tar.gz"
echo "test the archive by calling ./source_test.sh"
