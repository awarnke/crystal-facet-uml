PROJECT(crystal-facet-uml LANGUAGES C)
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
SET(CMAKE_VERBOSE_MAKEFILE TRUE)

# pkgconfig is a helper program to find include directories and libraries:
FIND_PACKAGE(PkgConfig REQUIRED)

# ==== ==== ==== ==== ==== ==== ==== ==== Build Environment Settings ==== ==== ==== ==== ==== ==== ==== ====

SET(COMPILE_IN_INCLUDE_DIR "" CACHE FILEPATH "Path to additional include directory")
SET(COMPILE_IN_SOURCE_FILE "" CACHE FILEPATH "Path to additional source file, e.g. sqlite3.c")
INCLUDE_DIRECTORIES(${COMPILE_IN_INCLUDE_DIR})

INCLUDE_DIRECTORIES(../../gui/include)
INCLUDE_DIRECTORIES(../../main/include)
INCLUDE_DIRECTORIES(../../ctrl/include)
INCLUDE_DIRECTORIES(../../data/include)
INCLUDE_DIRECTORIES(../../u8buf/include)
INCLUDE_DIRECTORIES(../../pencil/include)
INCLUDE_DIRECTORIES(../../io/include)
INCLUDE_DIRECTORIES(../../u8stream/include)

SET(THIRD_PARTY_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/root")
MESSAGE("THIRD_PARTY_ROOT: ${THIRD_PARTY_ROOT}")

# pkg config variables
#include(FindPkgConfig)
MESSAGE("PKG_CONFIG_EXECUTABLE:       ${PKG_CONFIG_EXECUTABLE}")
MESSAGE("ENV{PKG_CONFIG_PATH}:        $ENV{PKG_CONFIG_PATH}")
MESSAGE("ENV{PKG_CONFIG_LIBDIR}:      $ENV{PKG_CONFIG_LIBDIR}")
MESSAGE("ENV{PKG_CONFIG_SYSROOT_DIR}: $ENV{PKG_CONFIG_SYSROOT_DIR}")

INCLUDE_DIRECTORIES("${THIRD_PARTY_ROOT}/usr/local/include")
INCLUDE_DIRECTORIES("${THIRD_PARTY_ROOT}/usr/local/include/cairo")
INCLUDE_DIRECTORIES("${THIRD_PARTY_ROOT}/usr/local/include/gtk-4.0")
INCLUDE_DIRECTORIES("${THIRD_PARTY_ROOT}/usr/local/include/pango-1.0")
INCLUDE_DIRECTORIES("${THIRD_PARTY_ROOT}/usr/local/include/harfbuzz")
INCLUDE_DIRECTORIES("${THIRD_PARTY_ROOT}/usr/local/include/gdk-pixbuf-2.0")
INCLUDE_DIRECTORIES("${THIRD_PARTY_ROOT}/usr/local/include/libxml2")
INCLUDE_DIRECTORIES("${THIRD_PARTY_ROOT}/usr/local/include/libxml2/libxml")
INCLUDE_DIRECTORIES("${THIRD_PARTY_ROOT}/usr/local/include/xkbcommon")
INCLUDE_DIRECTORIES("${THIRD_PARTY_ROOT}/usr/local/include/gio-win32-2.0")
INCLUDE_DIRECTORIES("${THIRD_PARTY_ROOT}/usr/local/include/graphene-1.0")
INCLUDE_DIRECTORIES("${THIRD_PARTY_ROOT}/usr/local/include/libcroco-0.6")
INCLUDE_DIRECTORIES("${THIRD_PARTY_ROOT}/usr/local/include/glib-2.0")
INCLUDE_DIRECTORIES("${THIRD_PARTY_ROOT}/usr/local/include/atk-1.0")
INCLUDE_DIRECTORIES("${THIRD_PARTY_ROOT}/usr/local/include/epoxy")
INCLUDE_DIRECTORIES("${THIRD_PARTY_ROOT}/usr/local/include/fontconfig")
INCLUDE_DIRECTORIES("${THIRD_PARTY_ROOT}/usr/local/include/libpng16")
INCLUDE_DIRECTORIES("${THIRD_PARTY_ROOT}/usr/local/include/fribidi")
INCLUDE_DIRECTORIES("${THIRD_PARTY_ROOT}/usr/local/include/pixman-1")
INCLUDE_DIRECTORIES("${THIRD_PARTY_ROOT}/usr/local/include/freetype2")
#INCLUDE_DIRECTORIES("${THIRD_PARTY_ROOT}/usr/local/include/textstyle")

FILE(GLOB commonSource ../../gui/source/*.c
                       ../../gui/source/sketch/*.c
                       ../../ctrl/source/*.c
                       ../../data/source/*.c
                       ../../data/source/storage/*.c
                       ../../data/source/set/*.c
                       ../../u8buf/source/utf8stringbuf/*.c
                       ../../u8stream/source/dir/*.c
                       ../../u8stream/source/u8stream/*.c
                       ../../u8stream/source/trace/*.c
                       ../../pencil/source/*.c
                       ../../pencil/source/geometry/*.c
                       ../../pencil/source/draw/*.c
                       ../../io/source/*.c
                       ../../io/source/image/*.c
                       ../../io/source/json/*.c
                       ../../io/source/xml/*.c
                       ../../io/source/txt/*.c
                       ../../io/source/xhtml/*.c
                       ../../io/source/xmi/*.c
                       ../../io/source/md/*.c)
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
                       ../../u8buf/test/unit/*.c
                       ../../u8stream/test/unit/*.c)

EXECUTE_PROCESS(COMMAND "${PKG_CONFIG_EXECUTABLE}" --libs atk
                OUTPUT_VARIABLE ATK_LIB OUTPUT_STRIP_TRAILING_WHITESPACE)
#FIND_LIBRARY( ATK_LIB atk-1.0 "${THIRD_PARTY_ROOT}/usr/local/")
MESSAGE("atk found at ${ATK_LIB}")
#pkg_search_module(ATK_LIB REQUIRED atk)
#MESSAGE("atk found at ${ATK_LIB_LIBRARIES} / ${ATK_LIB_INCLUDE_DIRS}")

EXECUTE_PROCESS(COMMAND "${PKG_CONFIG_EXECUTABLE}" --libs cairo
                OUTPUT_VARIABLE CAIRO_LIB OUTPUT_STRIP_TRAILING_WHITESPACE)
#FIND_LIBRARY( CAIRO_LIB cairo "${THIRD_PARTY_ROOT}/usr/local/" )
MESSAGE("atk found at ${CAIRO_LIB}")
#pkg_search_module(CAIRO_LIB REQUIRED cairo)
#MESSAGE("cairo found at ${CAIRO_LIB_LIBRARIES} / ${CAIRO_LIB_INCLUDE_DIRS}")

EXECUTE_PROCESS(COMMAND "${PKG_CONFIG_EXECUTABLE}" --libs fontconfig
                OUTPUT_VARIABLE FONTCNF_LIB OUTPUT_STRIP_TRAILING_WHITESPACE)
#FIND_LIBRARY( FONTCNF_LIB fontconfig "${THIRD_PARTY_ROOT}/usr/local/bin/" )
#FIND_LIBRARY( FONTCNF_LIB NAMES "libfontconfig-1.dll" PATHS "${THIRD_PARTY_ROOT}/usr/local/bin" NO_DEFAULT_PATH)
MESSAGE("fontconfig found at ${FONTCNF_LIB}")

set(GAIL_LIB "")
#FIND_LIBRARY( GAIL_LIB gailutil "${THIRD_PARTY_ROOT}/usr/local/" )
MESSAGE("gailutil found at ${GAIL_LIB}")

EXECUTE_PROCESS(COMMAND "${PKG_CONFIG_EXECUTABLE}" --libs gdk-pixbuf-2.0
                OUTPUT_VARIABLE GDKPIX_LIB OUTPUT_STRIP_TRAILING_WHITESPACE)
#FIND_LIBRARY( GDKPIX_LIB gdk_pixbuf-2.0 "${THIRD_PARTY_ROOT}/usr/local/" )
MESSAGE("gdk_pixbuf found at ${GDKPIX_LIB}")

set(GDKWIN_LIB "")
#FIND_LIBRARY( GDKWIN_LIB gdk-win32-3.0 "${THIRD_PARTY_ROOT}/usr/local/" )
MESSAGE("gdk-win32 found at ${GDKWIN_LIB}")

EXECUTE_PROCESS(COMMAND "${PKG_CONFIG_EXECUTABLE}" --libs gio-2.0
                OUTPUT_VARIABLE GIO_LIB OUTPUT_STRIP_TRAILING_WHITESPACE)
#FIND_LIBRARY( GIO_LIB gio-2.0 "${THIRD_PARTY_ROOT}/usr/local/" )
MESSAGE("gio-2.0 found at ${GIO_LIB}")

EXECUTE_PROCESS(COMMAND "${PKG_CONFIG_EXECUTABLE}" --libs glib-2.0
                OUTPUT_VARIABLE GLIB_LIB OUTPUT_STRIP_TRAILING_WHITESPACE)
#FIND_LIBRARY( GLIB_LIB glib-2.0 "${THIRD_PARTY_ROOT}/usr/local/" )
MESSAGE("glib-2.0 found at ${GLIB_LIB}")

EXECUTE_PROCESS(COMMAND "${PKG_CONFIG_EXECUTABLE}" --libs gmodule-2.0
                OUTPUT_VARIABLE GMOD_LIB OUTPUT_STRIP_TRAILING_WHITESPACE)
#FIND_LIBRARY( GMOD_LIB gmodule-2.0 "${THIRD_PARTY_ROOT}/usr/local/" )
MESSAGE("gmodule-2.0 found at ${GMOD_LIB}")

EXECUTE_PROCESS(COMMAND "${PKG_CONFIG_EXECUTABLE}" --libs gobject-2.0
                OUTPUT_VARIABLE GOBJ_LIB OUTPUT_STRIP_TRAILING_WHITESPACE)
#FIND_LIBRARY( GOBJ_LIB gobject-2.0 "${THIRD_PARTY_ROOT}/usr/local/" )
MESSAGE("gobject-2.0 found at ${GOBJ_LIB}")

EXECUTE_PROCESS(COMMAND "${PKG_CONFIG_EXECUTABLE}" --libs gthread-2.0
                OUTPUT_VARIABLE GTHREAD_LIB OUTPUT_STRIP_TRAILING_WHITESPACE)
#FIND_LIBRARY( GTHREAD_LIB gthread-2.0 "${THIRD_PARTY_ROOT}/usr/local/" )
MESSAGE("gthread-2.0 found at ${GTHREAD_LIB}")

if(CFU_USE_GTK4)
    EXECUTE_PROCESS(COMMAND "${PKG_CONFIG_EXECUTABLE}" --libs gtk4
                    OUTPUT_VARIABLE GTK_LIB OUTPUT_STRIP_TRAILING_WHITESPACE)
    #FIND_LIBRARY( GTK_LIB gtk4 "${THIRD_PARTY_ROOT}/usr/local/" )
    MESSAGE("gtk4 found at ${GTK_LIB}")
else(CFU_USE_GTK4)
    EXECUTE_PROCESS(COMMAND "${PKG_CONFIG_EXECUTABLE}" --libs gtk+-3.0
                    OUTPUT_VARIABLE GTK_LIB OUTPUT_STRIP_TRAILING_WHITESPACE)
    #FIND_LIBRARY( GTK_LIB gtk4 "${THIRD_PARTY_ROOT}/usr/local/" )
    MESSAGE("gtk+-3 found at ${GTK_LIB}")
endif(CFU_USE_GTK4)

EXECUTE_PROCESS(COMMAND "${PKG_CONFIG_EXECUTABLE}" --libs pango
                OUTPUT_VARIABLE PANGO_LIB OUTPUT_STRIP_TRAILING_WHITESPACE)
#FIND_LIBRARY( PANGO_LIB pango-1.0 "${THIRD_PARTY_ROOT}/usr/local/" )
MESSAGE("pango found at ${PANGO_LIB}")

EXECUTE_PROCESS(COMMAND "${PKG_CONFIG_EXECUTABLE}" --libs pangocairo
                OUTPUT_VARIABLE PANGOCAIRO_LIB OUTPUT_STRIP_TRAILING_WHITESPACE)
#FIND_LIBRARY( PANGOCAIRO_LIB pangocairo-1.0 "${THIRD_PARTY_ROOT}/usr/local/" )
MESSAGE("pangocairo found at ${PANGOCAIRO_LIB}")

EXECUTE_PROCESS(COMMAND "${PKG_CONFIG_EXECUTABLE}" --libs pangoft2
                OUTPUT_VARIABLE PANGOFT_LIB OUTPUT_STRIP_TRAILING_WHITESPACE)
#FIND_LIBRARY( PANGOFT_LIB pangoft2-1.0 "${THIRD_PARTY_ROOT}/usr/local/" )
MESSAGE("pangoft2 found at ${PANGOFT_LIB}")

EXECUTE_PROCESS(COMMAND "${PKG_CONFIG_EXECUTABLE}" --libs pangowin32
                OUTPUT_VARIABLE PANGOWIN_LIB OUTPUT_STRIP_TRAILING_WHITESPACE)
#FIND_LIBRARY( PANGOWIN_LIB pangowin32-1.0 "${THIRD_PARTY_ROOT}/usr/local/" )
MESSAGE("pangowin32 found at ${PANGOWIN_LIB}")

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

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -D_XOPEN_SOURCE=700 -O2 -pedantic -g -Wall -Wno-overlength-strings -mwindows ${GTK3_CFLAGS_OTHER}" )
# Note: The GIMP-generated images are longer that 4095 characters, therefore -Wno-overlength-strings is required here.
ADD_CUSTOM_COMMAND(
    OUTPUT crystal-facet-uml.res
    COMMAND x86_64-w64-mingw32-windres ${CMAKE_CURRENT_SOURCE_DIR}/crystal-facet-uml.rc -O coff -o crystal-facet-uml.res
)


# ==== ==== ==== ==== ==== ==== ==== ==== C-Code crystal-facet-uml Linker Settings ==== ==== ==== ==== ==== ==== ==== ====

ADD_EXECUTABLE(crystal-facet-uml ${commonSource} ${appSource} ${COMPILE_IN_SOURCE_FILE} crystal-facet-uml.res)
TARGET_LINK_LIBRARIES(crystal-facet-uml ${ATK_LIB} ${CAIRO_LIB} ${FONTCNF_LIB} ${GAIL_LIB} ${GDKPIX_LIB} ${GDKWIN_LIB} ${GIO_LIB} ${GLIB_LIB} ${GMOD_LIB} ${GOBJ_LIB} ${GTHREAD_LIB} ${GTK_LIB} ${PANGO_LIB} ${PANGOCAIRO_LIB} ${PANGOFT_LIB} ${PANGOWIN_LIB} )
SET_TARGET_PROPERTIES(crystal-facet-uml PROPERTIES LINKER_LANGUAGE C)

# ==== ==== ==== ==== ==== ==== ==== ==== Install Settings ==== ==== ==== ==== ==== ==== ==== ====

INSTALL(TARGETS crystal-facet-uml DESTINATION "${THIRD_PARTY_ROOT}/usr/local/bin/")
INSTALL(FILES "${CMAKE_CURRENT_SOURCE_DIR}/../../installation_win/crystal-facet-uml.bat" DESTINATION "${THIRD_PARTY_ROOT}/usr/local/")
INSTALL(FILES "${CMAKE_CURRENT_SOURCE_DIR}/../../installation_win/crystal-facet-uml_readme.txt" DESTINATION "${THIRD_PARTY_ROOT}/usr/local/")

# ==== ==== ==== ==== ==== ==== ==== ==== test Settings ==== ==== ==== ==== ==== ==== ==== ====

ADD_EXECUTABLE(test_crystal-facet-uml ${commonSource} ${utestSource} ${COMPILE_IN_SOURCE_FILE})
TARGET_INCLUDE_DIRECTORIES(test_crystal-facet-uml PUBLIC ../../test_fw/include)
TARGET_INCLUDE_DIRECTORIES(test_crystal-facet-uml PUBLIC ../../data/test)
TARGET_INCLUDE_DIRECTORIES(test_crystal-facet-uml PUBLIC ../../ctrl/test)
TARGET_INCLUDE_DIRECTORIES(test_crystal-facet-uml PUBLIC ../../gui/test)
TARGET_INCLUDE_DIRECTORIES(test_crystal-facet-uml PUBLIC ../../io/test)
TARGET_INCLUDE_DIRECTORIES(test_crystal-facet-uml PUBLIC ../../pencil/test)
TARGET_INCLUDE_DIRECTORIES(test_crystal-facet-uml PUBLIC ../../u8buf/test)
TARGET_INCLUDE_DIRECTORIES(test_crystal-facet-uml PUBLIC ../../u8stream/test)
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

