#!/bin/sh
. ../../main/include/meta/meta_version.inl
VERSIONSTR=${META_VERSION_STR}

echo "pack src archive"
cd ../..
git archive --format tar.gz --prefix=crystal-facet-uml-$VERSIONSTR/ --output=build/test_coverage/crystal-facet-uml_$VERSIONSTR.orig.tar.gz master
cd build/test_coverage

echo "Cov-Measurement Source Package Version ${VERSIONSTR}"
if [ -z $VERSIONSTR]; then exit; fi
echo "based on crystal-facet-uml_${VERSIONSTR}.orig.tar.gz"
echo "----"
echo "clean up possibly broken previous cov-build"
test -d crystal-facet-uml-${VERSIONSTR} && rm -fr crystal-facet-uml-${VERSIONSTR}
test -d lcov.info && rm -f lcov.info
test -d lcov_filtered.info && rm -f lcov_filtered.info

echo "extract archive"
tar -xzf crystal-facet-uml_${VERSIONSTR}.orig.tar.gz

echo "building binary"
cd crystal-facet-uml-${VERSIONSTR}
mkdir cmake_build
cd cmake_build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j4 gcov_crystal-facet-uml  # start up to 4 parallel processes to make use of quad-core processors
cd ../..

echo "initializing lcov"
lcov --capture --initialize --directory ./crystal-facet-uml-${VERSIONSTR}/cmake_build/CMakeFiles/gcov_crystal-facet-uml.dir --output-file lcov.info

echo "running unit tests"
cd crystal-facet-uml-${VERSIONSTR}/cmake_build/
./gcov_crystal-facet-uml -a || echo "ERROR == ERROR == ERROR == ERROR"
cd ../..

echo "running lcov"
lcov --capture --directory ./crystal-facet-uml-${VERSIONSTR}/cmake_build/CMakeFiles/gcov_crystal-facet-uml.dir --output-file lcov.info
lcov --remove lcov.info '*/unittest/*' '*/test/*' --output-file lcov_filtered.info
genhtml --prefix `pwd`/crystal-facet-uml-${VERSIONSTR} lcov_filtered.info --title crystal-facet-uml_v${VERSIONSTR} --output-directory crystal-facet-uml-${VERSIONSTR}_lcov

echo "clean up test"
sleep 10
rm -fr crystal-facet-uml-${VERSIONSTR}

echo "output written to crystal-facet-uml-${VERSIONSTR}_lcov"

