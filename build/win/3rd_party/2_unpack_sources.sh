#!/bin/sh

mkdir -p src
cd src
    ls -1 ../packed_src/*.tar.xz | xargs -n 1 tar --xz --verbose -xf
    ls -1 ../packed_src/*.tar.gz | xargs -n 1 tar --verbose -xzf
    ls -1 ../packed_src/*.zip | xargs -n 1 unzip
cd ..

cd ..
    sed -e 's:@root@:'`pwd`'/root:g' 3rd_party/cross_file.txt.in > 3rd_party/cross_file.txt
cd 3rd_party


# Copyright 2022-2022 Andreas Warnke
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
