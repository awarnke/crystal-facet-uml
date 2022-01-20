#!/bin/sh
mkdir -p my_usr

# host is the prefix of the compiler executables

echo "configuring libiconv..."
cd src/libiconv-1*
    ./configure --host=x86_64-w64-mingw32 --enable-relocatable --prefix=`pwd`/../../my_usr --disable-rpath --enable-static-pie
    make
    make install
cd ../..

echo "finished libiconv. Cancel now in case of errors."
sleep 3

echo "configuring libffi..."
cd src/libffi-3*
    ./configure --host=x86_64-w64-mingw32 --prefix=`pwd`/../../my_usr --enable-static
    make
    make install
cd ../..

echo "finished libffi. Cancel now in case of errors."
sleep 3

echo "configuring gettext..."
cd src/gettext-0*
    ./configure --host=x86_64-w64-mingw32 --enable-relocatable --prefix=`pwd`/../../my_usr --disable-rpath --disable-libasprintf --disable-java --disable-native-java --disable-openmp --disable-curses
    make
    make install
cd ../..

echo "finished gettext. Cancel now in case of errors."
sleep 3

echo "configuring glib..."
cd src/glib-2*
    # see ../3rd_party/src/glib-2.9.6/docs/reference/glib/html/glib-cross-compiling.html
    echo "glib_cv_long_long_format=I64
glib_cv_stack_grows=no" > win32.cache
    chmod a-w win32.cache   # prevent configure from changing it
    export CFLAGS=-I`pwd`/../../my_usr/include/
    export LDFLAGS=-L`pwd`/../../my_usr/lib
    ./configure --cache-file=win32.cache --host=x86_64-w64-mingw32 --enable-relocatable --with-libiconv=gnu --prefix=`pwd`/../../my_usr
cd ../..


# Thanks to many inspiring webpages, especially of help were:
# https://zwyuan.github.io/2016/07/17/cross-compile-glib-for-android/ (Jan 2022)


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
