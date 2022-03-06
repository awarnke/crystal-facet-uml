PROJECT(crystal-facet-uml LANGUAGES C)
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
SET(CMAKE_VERBOSE_MAKEFILE TRUE)

# ==== ==== ==== ==== ==== ==== ==== ==== Build Environment Settings ==== ==== ==== ==== ==== ==== ==== ====

INCLUDE_DIRECTORIES(../../gui/include)
INCLUDE_DIRECTORIES(../../main/include)
INCLUDE_DIRECTORIES(../../tslog/include)
INCLUDE_DIRECTORIES(../../trace/include)
INCLUDE_DIRECTORIES(../../ctrl/include)
INCLUDE_DIRECTORIES(../../data/include)
INCLUDE_DIRECTORIES(../../utf8stringbuf/include)
INCLUDE_DIRECTORIES(../../pencil/include)
INCLUDE_DIRECTORIES(../../io/include)
INCLUDE_DIRECTORIES(../../universal/include)

SET(THIRD_PARTY_ROOT "${-DCMAKE_PREFIX_PATH}/root")
SET(SQLITE_BASE "${-DCMAKE_PREFIX_PATH}/home/sqlite3")
MESSAGE("THIRD_PARTY_ROOT: ${THIRD_PARTY_ROOT}")

set(ENV{PKG_CONFIG_PATH} "")
set(ENV{PKG_CONFIG_LIBDIR} "${THIRD_PARTY_ROOT}/usr/local/lib/pkgconfig:${THIRD_PARTY_ROOT}/usr/local/lib64/pkgconfig:${THIRD_PARTY_ROOT}${THIRD_PARTY_ROOT}/usr/local/lib/pkgconfig:${THIRD_PARTY_ROOT}${THIRD_PARTY_ROOT}/usr/local/lib64/pkgconfig")
set(ENV{PKG_CONFIG_SYSROOT_DIR} ${THIRD_PARTY_ROOT})
set(PKG_CONFIG_EXECUTABLE /usr/bin/x86_64-w64-mingw32-pkg-config)
include(FindPkgConfig)

INCLUDE_DIRECTORIES("${SQLITE_BASE}")

FILE(GLOB commonSource ../../gui/source/*.c
                       ../../gui/source/sketch_area/*.c
                       ../../trace/source/*.c
                       ../../ctrl/source/*.c
                       ../../data/source/*.c
                       ../../data/source/storage/*.c
                       ../../data/source/set/*.c
                       ../../utf8stringbuf/source/util/string/*.c
                       ../../universal/source/*.c
                       ../../universal/source/stream/*.c
                       ../../pencil/source/*.c
                       ../../pencil/source/util/geometry/*.c
                       ../../pencil/source/draw/*.c
                       ../../io/source/*.c
                       ../../io/source/image/*.c
                       ../../io/source/json/*.c
                       ../../io/source/xml/*.c
                       ../../io/source/txt/*.c
                       ../../io/source/xhtml/*.c
                       ../../io/source/xmi/*.c
                       ../../io/source/md/*.c
                       ${SQLITE_BASE}/sqlite3.c)
FILE(GLOB appSource    ../../main/source/*.c)
FILE(GLOB utestSource  ../../main/test/*.c
                       ../../data/test/unit/*.c
                       ../../data/test/integration/*.c
                       ../../ctrl/test/unit/*.c
                       ../../ctrl/test/integration/*.c
                       ../../gui/test/unit/*.c
                       ../../io/test/unit/*.c
                       ../../io/test/integration/*.c
                       ../../pencil/test/unit/*.c
                       ../../pencil/test/integration/*.c
                       ../../universal/test/unit/*.c
                       ../../utf8stringbuf/test/unit/*.c)

FIND_LIBRARY( ATK_LIB atk-1.0 "${THIRD_PARTY_ROOT}/usr/local/")
FIND_LIBRARY( CAIRO_LIB cairo "${THIRD_PARTY_ROOT}/usr/local/" )
FIND_LIBRARY( FONTCNF_LIB fontconfig "${THIRD_PARTY_ROOT}/usr/local/" )
FIND_LIBRARY( GAIL_LIB gailutil "${THIRD_PARTY_ROOT}/usr/local/" )
FIND_LIBRARY( GDKPIX_LIB gdk_pixbuf-2.0 "${THIRD_PARTY_ROOT}/usr/local/" )
FIND_LIBRARY( GDKWIN_LIB gdk-win32-3.0 "${THIRD_PARTY_ROOT}/usr/local/" )
FIND_LIBRARY( GIO_LIB gio-2.0 "${THIRD_PARTY_ROOT}/usr/local/" )
FIND_LIBRARY( GLIB_LIB glib-2.0 "${THIRD_PARTY_ROOT}/usr/local/" )
FIND_LIBRARY( GMOD_LIB gmodule-2.0 "${THIRD_PARTY_ROOT}/usr/local/" )
FIND_LIBRARY( GOBJ_LIB gobject-2.0 "${THIRD_PARTY_ROOT}/usr/local/" )
FIND_LIBRARY( GTHREAD_LIB gthread-2.0 "${THIRD_PARTY_ROOT}/usr/local/" )
FIND_LIBRARY( GTK_LIB gtk-win32-3.0 "${THIRD_PARTY_ROOT}/usr/local/" )
FIND_LIBRARY( PANGO_LIB pango-1.0 "${THIRD_PARTY_ROOT}/usr/local/" )
FIND_LIBRARY( PANGOCAIRO_LIB pangocairo-1.0 "${THIRD_PARTY_ROOT}/usr/local/" )
FIND_LIBRARY( PANGOFT_LIB pangoft2-1.0 "${THIRD_PARTY_ROOT}/usr/local/" )
FIND_LIBRARY( PANGOWIN_LIB pangowin32-1.0 "${THIRD_PARTY_ROOT}/usr/local/" )
MESSAGE("gtk found at ${GTK_LIB}")

# ==== ==== ==== ==== ==== ==== ==== ==== Check if CMAKE_BUILD_TYPE is set ==== ==== ==== ==== ==== ==== ==== ====

IF(NOT CMAKE_BUILD_TYPE)
  MESSAGE( "Warn: CMAKE_BUILD_TYPE no set!                 setting(CMAKE_BUILD_TYPE \"Release\")")
  SET(CMAKE_BUILD_TYPE "Release")
ENDIF(NOT CMAKE_BUILD_TYPE)
MESSAGE( "Note:                                          CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}" )
MESSAGE( "Note:                                          CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR}" )
MESSAGE( "Note:                                          CMAKE_BUILD_DIR = ${CMAKE_BUILD_DIR}" )
MESSAGE( "Note:                                          THIRD_PARTY_ROOT = ${THIRD_PARTY_ROOT}" )
MESSAGE( "Note: to build a release, run                  cmake -DCMAKE_BUILD_TYPE=Release " ${CMAKE_CURRENT_SOURCE_DIR} )
MESSAGE( "Note: to build with asserts and traces, run    cmake -DCMAKE_BUILD_TYPE=Debug " ${CMAKE_CURRENT_SOURCE_DIR} )

# ==== ==== ==== ==== ==== ==== ==== ==== Compiler Settings ==== ==== ==== ==== ==== ==== ==== ====

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -D_XOPEN_SOURCE=700 -O2 -pedantic -g -Wall -Wno-overlength-strings ${GTK3_CFLAGS_OTHER}" )
# Note: The GIMP-generated images are longer that 4095 characters, therefore -Wno-overlength-strings is required here.

# ==== ==== ==== ==== ==== ==== ==== ==== C-Code crystal-facet-uml Linker Settings ==== ==== ==== ==== ==== ==== ==== ====

ADD_EXECUTABLE(crystal-facet-uml ${commonSource} ${appSource})
TARGET_LINK_LIBRARIES(crystal-facet-uml ${ATK_LIB} ${CAIRO_LIB} ${FONTCNF_LIB} ${GAIL_LIB} ${GDKPIX_LIB} ${GDKWIN_LIB} ${GIO_LIB} ${GLIB_LIB} ${GMOD_LIB} ${GOBJ_LIB} ${GTHREAD_LIB} ${GTK_LIB} ${PANGO_LIB} ${PANGOCAIRO_LIB} ${PANGOFT_LIB} ${PANGOWIN_LIB} )
SET_TARGET_PROPERTIES(crystal-facet-uml PROPERTIES LINKER_LANGUAGE C)

# ==== ==== ==== ==== ==== ==== ==== ==== test Settings ==== ==== ==== ==== ==== ==== ==== ====

ADD_EXECUTABLE(test_crystal-facet-uml ${commonSource} ${utestSource})
TARGET_INCLUDE_DIRECTORIES(test_crystal-facet-uml PUBLIC ../../test_fw/include)
TARGET_INCLUDE_DIRECTORIES(test_crystal-facet-uml PUBLIC ../../data/test)
TARGET_INCLUDE_DIRECTORIES(test_crystal-facet-uml PUBLIC ../../ctrl/test)
TARGET_INCLUDE_DIRECTORIES(test_crystal-facet-uml PUBLIC ../../gui/test)
TARGET_INCLUDE_DIRECTORIES(test_crystal-facet-uml PUBLIC ../../io/test)
TARGET_INCLUDE_DIRECTORIES(test_crystal-facet-uml PUBLIC ../../pencil/test)
TARGET_INCLUDE_DIRECTORIES(test_crystal-facet-uml PUBLIC ../../universal/test)
TARGET_INCLUDE_DIRECTORIES(test_crystal-facet-uml PUBLIC ../../utf8stringbuf/test)
TARGET_LINK_LIBRARIES(test_crystal-facet-uml ${ATK_LIB} ${CAIRO_LIB} ${FONTCNF_LIB} ${GAIL_LIB} ${GDKPIX_LIB} ${GDKWIN_LIB} ${GIO_LIB} ${GLIB_LIB} ${GMOD_LIB} ${GOBJ_LIB} ${GTHREAD_LIB} ${GTK_LIB} ${PANGO_LIB} ${PANGOCAIRO_LIB} ${PANGOFT_LIB} ${PANGOWIN_LIB} )
SET_TARGET_PROPERTIES(test_crystal-facet-uml PROPERTIES LINKER_LANGUAGE C)


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

