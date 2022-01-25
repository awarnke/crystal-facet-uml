#!/bin/sh
mkdir -p bin

echo "==== install:"
echo "sudo zypper install mingw64-cross-gcc mingw-cross-pkgconfig wine"

echo "configuring libiconv..."
cd src/libiconv-1*
    # host is the prefix of the compiler executables
    ./configure --host=x86_64-w64-mingw32 --enable-relocatable --prefix=`pwd`/../../bin
    make
    make install
cd ../..

echo "configuring glib..."
cd src/glib-2*
    # see ../3rd_party/src/glib-2.9.6/docs/reference/glib/html/glib-cross-compiling.html
    echo "glib_cv_long_long_format=I64
    glib_cv_stack_grows=no" > win32.cache
    chmod a-w win32.cache   # prevent configure from changing it
    export CFLAGS=-I../libiconv-1.16/include/
    export LDFLAGS=-L`pwd`/../../bin
    export LIBS=`pwd`/../../bin
    # host is the prefix of the compiler executables
    ./configure --cache-file=win32.cache --host=x86_64-w64-mingw32 --enable-relocatable --with-libiconv=gnu --prefix=`pwd`/../../bin
cd ../..


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
