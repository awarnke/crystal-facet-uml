#!/bin/sh
unzip ../3rd_party/gtk+-bundle_3.6.4-20130513_win64.zip -d crystal-facet-uml
unzip ../3rd_party/sqlite-amalgamation-3360000.zip -d sqlite-amalgamation-3360000
cmake -DCMAKE_TOOLCHAIN_FILE=../mingw_wine_toolchain.cmake ..


# Copyright 2021-2022 Andreas Warnke
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
