#!/bin/sh
echo "copying files to be packed"
rm -fr crystal_facet_uml
mkdir crystal_facet_uml
cp -r ../architecture crystal_facet_uml/
cp -r ../data crystal_facet_uml/
cp -r ../embunit crystal_facet_uml/
cp -r ../gui crystal_facet_uml/
cp -r ../tslog crystal_facet_uml/
cp ../readme.txt crystal_facet_uml/
cp ../release_notes.txt crystal_facet_uml/
cp -r ../trace crystal_facet_uml/
cp -r ../utf8stringbuf crystal_facet_uml/
mkdir crystal_facet_uml/cmake_build
cp ../cmake_build/CMakeLists.txt crystal_facet_uml/cmake_build/
cp -r ../ctrl crystal_facet_uml/
mkdir crystal_facet_uml/doxygen_build
cp ../doxygen_build/doxygen_config crystal_facet_uml/doxygen_build/
cp ../license.txt crystal_facet_uml/
cp -r ../main crystal_facet_uml/
cp -r ../pencil crystal_facet_uml/
cp -r ../sqlite crystal_facet_uml/
cp -r ../universal crystal_facet_uml/
cp -r ../user_doc crystal_facet_uml/

echo "building doc and binary"
cd crystal_facet_uml/doxygen_build
doxygen doxygen_config
cd ../..
cd crystal_facet_uml/cmake_build
cmake .
make
rm -fr CMakeFiles
rm -f CMakeCache.txt
rm -f cmake_install.cmake
cd ../..

echo "zipping src package"
zip -r crystal_facet_uml.zip crystal_facet_uml/
rm -fr crystal_facet_uml

echo "binary packages are created by sudo make package"
