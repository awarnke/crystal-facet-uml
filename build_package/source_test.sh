#!/bin/sh
VERSIONSTR=1.16.0
echo "Test Source Package Version $VERSIONSTR"
echo "based on crystal-facet-uml_$VERSIONSTR.orig.tar.gz"
echo "----"
echo "clean up possibly broken previous test-build"
test -d crystal-facet-uml-$VERSIONSTR && rm -fr crystal-facet-uml-$VERSIONSTR

echo "test archive"
tar -xzf crystal-facet-uml_$VERSIONSTR.orig.tar.gz

echo "building doc"
cd crystal-facet-uml-$VERSIONSTR/build_doxygen
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

echo "runing unit tests"
cd crystal-facet-uml-$VERSIONSTR/cmake_build
./unittest_crystal_facet_uml -a || echo "ERROR == ERROR == ERROR == ERROR"
cd ../..

echo "clean up test"
sleep 10
rm -fr crystal-facet-uml-$VERSIONSTR
