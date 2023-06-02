#!/bin/sh

OUTFILE="LICENSES_of_3rd_party.txt"

find src/ -type f -a -name "COPY*" -o -type f -a -name "LICENSE*" \
| sort \
> ${OUTFILE}

# echo "The standard C library (libc) is compiled-in using the -static-libgcc option" \
# >> ${OUTFILE}
echo "\nlibgcc_s_seh-1 is GPL 3.0, see https://opensuse.pkgs.org/15.3/opensuse-oss-aarch64/mingw64-libgcc_s_seh1-9.2.0-bp153.1.72.noarch.rpm.html" \
>> ${OUTFILE}

find src/ -type f -a -name "COPY*" -o -type f -a -name "LICENSE*" \
| sort \
| sed -e 's/\(.*\)/echo \"\n\n\n===   ===   ===   \1   ===   ===   ===\n\n\";cat \1/' \
| sh \
>> ${OUTFILE}

cp ${OUTFILE} ../root/usr/local


# Copyright 2022-2023 Andreas Warnke
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
