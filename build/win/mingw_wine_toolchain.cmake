# call cmake providing this toolchain file:
# cmake -DCMAKE_TOOLCHAIN_FILE=../crystal-facet-uml/build/win/mingw_wine_toolchain.cmake ../crystal-facet-uml/build/win

# compiler and linker variables:
SET(CMAKE_SYSTEM_NAME "Windows")
SET(CMAKE_SYSTEM_PROCESSOR "x86_64")
set(CMAKE_FIND_ROOT_PATH "/usr/x86_64-w64-mingw32" "${CMAKE_PREFIX_PATH}/../..")
SET(CMAKE_C_COMPILER "/usr/bin/x86_64-w64-mingw32-gcc")
SET(CMAKE_CXX_COMPILER "/usr/bin/x86_64-w64-mingw32-g++")
SET(CMAKE_RC_COMPILER "/usr/bin/x86_64-w64-mingw32-windres")

# pkg config executable:
set(PKG_CONFIG_EXECUTABLE "/usr/bin/x86_64-w64-mingw32-pkg-config")


# Copyright 2021-2024 Andreas Warnke
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
