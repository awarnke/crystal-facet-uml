#!/bin/sh
echo "clean up previous build"
rm crystal_facet_uml.zip
rm -fr crystal_facet_uml

echo "pack archive"
cd ..
git archive --format zip --prefix=crystal_facet_uml/ --output package_build/crystal_facet_uml-x.y.z-src.zip master
cd package_build

echo "test archive"
unzip crystal_facet_uml-x.y.z-src.zip

echo "building doc"
cd crystal_facet_uml/doxygen_build
./make.sh
cd ../..
echo "building man page"
cd crystal_facet_uml/installation_linux/man
./make.sh
cd ../../..
echo "building binary"
cd crystal_facet_uml/cmake_build
cmake -DCMAKE_BUILD_TYPE=Release .
make
cd ../..
echo "run unit tests"
cd crystal_facet_uml/cmake_build
./unittest_crystal_facet_uml -a
cd ../..

echo "clean up test"
sleep 10
rm -fr crystal_facet_uml

echo "binary packages are created by sudo make package"
