#!/bin/sh
. ../../main/include/meta/meta_version.inl
VERSIONSTR=${META_VERSION_STR}

echo "Cov-Measurement Source Package Version ${VERSIONSTR}"
if [ -z $VERSIONSTR ]; then exit; fi
echo "----"
echo "clean up possibly broken previous cov-build"
test -d crystal-facet-uml_${VERSIONSTR} && rm -fr crystal-facet-uml_${VERSIONSTR}
test -d lcov_covts.info && rm -f lcov_covts.info
test -d lcov_covts_filtered.info && rm -f lcov_covts_filtered.info

echo "building binary"
mkdir crystal-facet-uml_${VERSIONSTR}
cd crystal-facet-uml_${VERSIONSTR}
mkdir cmake_build
cd cmake_build
# Release has optimizations that interfere with coverage measurements.
# Only in debug mode, fault injectsions and assert statements have effect.
cmake -DCMAKE_BUILD_TYPE=Debug -DCFU_ADD_GCOV_TARGET=ON ../../../..
# start up to 12 parallel processes to make use of 12-core processors:
make -j12 gcov_crystal-facet-uml
cd ../..

echo "initializing lcov for COVERAGE tests"
lcov --capture --initial --directory ./crystal-facet-uml_${VERSIONSTR}/cmake_build/CMakeFiles/gcov_crystal-facet-uml.dir --output-file lcov_covts.info

echo "running all coverage tests"
cd crystal-facet-uml_${VERSIONSTR}/cmake_build/
{ ./gcov_crystal-facet-uml -C || echo "ERROR == ERROR == ERROR == ERROR" ; } | grep -e '^test ' -e '^  test ' -e 'ERROR '
cd ../..

echo "running gcov/lcov on covts"
lcov --capture --directory ./crystal-facet-uml_${VERSIONSTR}/cmake_build/CMakeFiles/gcov_crystal-facet-uml.dir --output-file lcov_covts.info
lcov --remove lcov_covts.info '*/test_fw/*' '*/test/*' '/usr/*' --output-file lcov_covts_filtered.info
genhtml --prefix `pwd`/crystal-facet-uml_${VERSIONSTR} lcov_covts_filtered.info --title crystal-facet-uml_v${VERSIONSTR}_covts --output-directory crystal-facet-uml_${VERSIONSTR}_covts_coverage

echo "clean up test"
sleep 10
rm -fr crystal-facet-uml_${VERSIONSTR}

zip -r crystal-facet-uml_${VERSIONSTR}_covts_coverage.zip crystal-facet-uml_${VERSIONSTR}_covts_coverage
echo "output written to crystal-facet-uml_${VERSIONSTR}_covts_coverage"


# Copyright 2021-2025 Andreas Warnke
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
