#!/bin/sh
. ../../main/include/meta/meta_version.inl
VERSIONSTR=${META_VERSION_STR}

echo "pack src archive"
cd ../..
git archive --format tar.gz --prefix=crystal-facet-uml_$VERSIONSTR/ --output=build/test_coverage/crystal-facet-uml_$VERSIONSTR.orig.tar.gz master
cd build/test_coverage

echo "Cov-Measurement Source Package Version ${VERSIONSTR}"
if [ -z $VERSIONSTR ]; then exit; fi
echo "based on crystal-facet-uml_${VERSIONSTR}.orig.tar.gz"
echo "----"
echo "clean up possibly broken previous cov-build"
test -d crystal-facet-uml_${VERSIONSTR} && rm -fr crystal-facet-uml_${VERSIONSTR}
test -d lcov_unit.info && rm -f lcov_unit.info
test -d lcov_unit_filtered.info && rm -f lcov_unit_filtered.info
test -d lcov_all.info && rm -f lcov_all.info
test -d lcov_all_filtered.info && rm -f lcov_all_filtered.info

echo "extract archive"
tar -xzf crystal-facet-uml_${VERSIONSTR}.orig.tar.gz

echo "building binary"
cd crystal-facet-uml_${VERSIONSTR}
mkdir cmake_build
cd cmake_build
cmake -DCMAKE_BUILD_TYPE=Debug -DCFU_ADD_GCOV_TARGET=ON ..  # Release has optimizations that interfere with coverage measurements
make -j4 gcov_crystal-facet-uml  # start up to 4 parallel processes to make use of quad-core processors
cd ../..

echo "initializing lcov for unit tests"
lcov --capture --initialize --directory ./crystal-facet-uml_${VERSIONSTR}/cmake_build/CMakeFiles/gcov_crystal-facet-uml.dir --output-file lcov_unit.info

echo "running unit tests"
cd crystal-facet-uml_${VERSIONSTR}/cmake_build/
./gcov_crystal-facet-uml -u > /dev/null || echo "ERROR == ERROR == ERROR == ERROR"
cd ../..

echo "running gcov/lcov on unittest"
lcov --capture --directory ./crystal-facet-uml_${VERSIONSTR}/cmake_build/CMakeFiles/gcov_crystal-facet-uml.dir --output-file lcov_unit.info
lcov --remove lcov_unit.info '*/test_fw/*' '*/test/*' --output-file lcov_unit_filtered.info
genhtml --prefix `pwd`/crystal-facet-uml_${VERSIONSTR} lcov_unit_filtered.info --title crystal-facet-uml_v${VERSIONSTR}_unittest --output-directory crystal-facet-uml_${VERSIONSTR}_unittest_coverage

echo "initializing lcov for all tests"
lcov --capture --initialize --directory ./crystal-facet-uml_${VERSIONSTR}/cmake_build/CMakeFiles/gcov_crystal-facet-uml.dir --output-file lcov_all.info

echo "running all tests"
cd crystal-facet-uml_${VERSIONSTR}/cmake_build/
./gcov_crystal-facet-uml -a > /dev/null || echo "ERROR == ERROR == ERROR == ERROR"
cd ../..

echo "running gcov/lcov on alltests"
lcov --capture --directory ./crystal-facet-uml_${VERSIONSTR}/cmake_build/CMakeFiles/gcov_crystal-facet-uml.dir --output-file lcov_all.info
lcov --remove lcov_all.info '*/test_fw/*' '*/test/*' --output-file lcov_all_filtered.info
genhtml --prefix `pwd`/crystal-facet-uml_${VERSIONSTR} lcov_all_filtered.info --title crystal-facet-uml_v${VERSIONSTR}_alltests --output-directory crystal-facet-uml_${VERSIONSTR}_alltests_coverage

echo "clean up test"
sleep 10
rm -fr crystal-facet-uml_${VERSIONSTR}

zip -r crystal-facet-uml_${VERSIONSTR}_unittest_coverage.zip crystal-facet-uml_${VERSIONSTR}_unittest_coverage
echo "output written to crystal-facet-uml_${VERSIONSTR}_unittest_coverage"
zip -r crystal-facet-uml_${VERSIONSTR}_alltests_coverage.zip crystal-facet-uml_${VERSIONSTR}_alltests_coverage
echo "output written to crystal-facet-uml_${VERSIONSTR}_alltests_coverage"


# Copyright 2021-2023 Andreas Warnke
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

