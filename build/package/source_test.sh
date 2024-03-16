#!/bin/sh
. ../../main/include/meta/meta_version.inl
VERSIONSTR=${META_VERSION_STR}

echo "Testing Source Package Version $VERSIONSTR"
if [ -z ${VERSIONSTR}]; then exit; fi
echo "based on crystal-facet-uml_$VERSIONSTR.orig.tar.gz"
echo "----"
echo "clean up possibly broken previous test-build"
test -d crystal-facet-uml-$VERSIONSTR && rm -fr crystal-facet-uml-$VERSIONSTR
test -d cmake_build && rm -fr cmake_build

echo "test archive"
tar -xzf crystal-facet-uml_$VERSIONSTR.orig.tar.gz

echo "building doc"
cd crystal-facet-uml-$VERSIONSTR/build/source_code_doc
./make.sh
cd ../../..

echo "building user doc and man page"
cd crystal-facet-uml-$VERSIONSTR/build/user_manual
make
cd ../../..

echo "building binary"
mkdir cmake_build
cd cmake_build
cmake -DCMAKE_BUILD_TYPE=Release ../crystal-facet-uml-$VERSIONSTR
make -j4    # start up to 4 parallel processes to make use of quad-core processors
cd ..

echo "running release-relevant tests"
cd cmake_build
./test_crystal-facet-uml -R || echo "ERROR == ERROR == ERROR == ERROR"
cd ..

echo "clean up test"
sleep 10
rm -fr crystal-facet-uml-$VERSIONSTR
rm -fr cmake_build
