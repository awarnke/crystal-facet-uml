#!/bin/sh
VERSIONSTR=1.9.0
echo "Buidling Version $VERSIONSTR"

echo "clean up possibly broken previous build"
test -d crystal-facet-uml-$VERSIONSTR && rm -fr crystal-facet-uml-$VERSIONSTR
test -e crystal-facet-uml_$VERIONSTR.orig.tar.gz && rm crystal-facet-uml_$VERIONSTR.orig.tar.gz

echo "pack archive"
cd ..
git archive --format tar.gz --prefix=crystal-facet-uml-$VERSIONSTR/ --output=package_build/crystal-facet-uml_$VERSIONSTR.orig.tar.gz master
cd package_build

echo "test archive"
tar -xzf crystal-facet-uml_$VERSIONSTR.orig.tar.gz

echo "building doc"
cd crystal-facet-uml-$VERSIONSTR/doxygen_build
./make.sh
cd ../..
echo "building user doc and man page"
cd crystal-facet-uml-$VERSIONSTR/user_doc
make
cd ../..
echo "building binary"
cd crystal-facet-uml-$VERSIONSTR
mkdir cmake_build
cd cmake_build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j4    # start up to 4 parallel processes to make use of quad-core processors
cd ../..
echo "run unit tests"
cd crystal-facet-uml-$VERSIONSTR/cmake_build
./unittest_crystal_facet_uml -a
cd ../..

echo "clean up test"
sleep 10
rm -fr crystal-facet-uml-$VERSIONSTR

