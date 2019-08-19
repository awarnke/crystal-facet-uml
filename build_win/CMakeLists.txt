CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

# ==== ==== ==== ==== ==== ==== ==== ==== Compiler Settings ==== ==== ==== ==== ==== ==== ==== ====

SET(CMAKE_C_COMPILER "/usr/bin/i686-w64-mingw32-gcc")
SET(CMAKE_CXX_COMPILER "/usr/bin/i686-w64-mingw32-g++")
SET(CMAKE_SYSTEM_NAME "Windows")

# ==== ==== ==== ==== ==== ==== ==== ==== Build Environment Settings ==== ==== ==== ==== ==== ==== ==== ====

PROJECT(crystal_facet_uml)

INCLUDE_DIRECTORIES(../gui/source)
INCLUDE_DIRECTORIES(../gui/include)
INCLUDE_DIRECTORIES(../main/source)
INCLUDE_DIRECTORIES(../main/include)
INCLUDE_DIRECTORIES(../tslog/include)
INCLUDE_DIRECTORIES(../trace/source)
INCLUDE_DIRECTORIES(../trace/include)
INCLUDE_DIRECTORIES(../ctrl/source)
INCLUDE_DIRECTORIES(../ctrl/include)
INCLUDE_DIRECTORIES(../data/source)
INCLUDE_DIRECTORIES(../data/include)
INCLUDE_DIRECTORIES(../utf8stringbuf/include)
INCLUDE_DIRECTORIES(../pencil/source)
INCLUDE_DIRECTORIES(../pencil/include)
INCLUDE_DIRECTORIES(../io/source)
INCLUDE_DIRECTORIES(../io/include)
INCLUDE_DIRECTORIES(../universal/include)

SET(THIRD_PARTY_BASE "${CMAKE_BINARY_DIR}")

INCLUDE_DIRECTORIES("${THIRD_PARTY_BASE}/sqlite-amalgamation-3290000")
INCLUDE_DIRECTORIES("${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/libffi-3.0.12/include")
INCLUDE_DIRECTORIES("${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/glib-2.0/include")
INCLUDE_DIRECTORIES("${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/include")
INCLUDE_DIRECTORIES("${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/include/libpng15")
INCLUDE_DIRECTORIES("${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/include/libxml2")
INCLUDE_DIRECTORIES("${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/include/gail-3.0")
INCLUDE_DIRECTORIES("${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/include/cairo")
INCLUDE_DIRECTORIES("${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/include/gio-win32-2.0")
INCLUDE_DIRECTORIES("${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/include/pango-1.0")
INCLUDE_DIRECTORIES("${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/include/libcroco-0.6")
INCLUDE_DIRECTORIES("${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/include/jasper")
INCLUDE_DIRECTORIES("${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/include/glib-2.0")
INCLUDE_DIRECTORIES("${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/include/lzma")
INCLUDE_DIRECTORIES("${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/include/gdk-pixbuf-2.0")
INCLUDE_DIRECTORIES("${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/include/atk-1.0")
INCLUDE_DIRECTORIES("${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/include/fontconfig")
INCLUDE_DIRECTORIES("${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/include/gtk-3.0")
INCLUDE_DIRECTORIES("${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/include/librsvg-2.0")
INCLUDE_DIRECTORIES("${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/include/pixman-1")
INCLUDE_DIRECTORIES("${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/include/freetype2")

FILE(GLOB commonSource ../gui/source/*.c
                       ../trace/source/*.c
                       ../ctrl/source/*.c
                       ../data/source/*.c
                       ../data/source/storage/*.c
                       ../data/source/set/*.c
                       ../utf8stringbuf/source/util/string/*.c
                       ../pencil/source/*.c
                       ../pencil/source/util/geometry/*.c
                       ../pencil/source/draw/*.c
                       ../io/source/*.c
                       ../io/source/json/*.c
                       ${THIRD_PARTY_BASE}/sqlite-amalgamation-3290000/sqlite3.c)
FILE(GLOB appSource    ../main/source/*.c)
FILE(GLOB utestSource  ../main/unittest/*.c
                       ../data/unittest/*.c
                       ../ctrl/unittest/*.c
                       ../io/unittest/*.c
                       ../pencil/unittest/*.c
                       ../universal/unittest/*.c
                       ../utf8stringbuf/unittest/*.c)

ADD_LIBRARY( atk-1.0 STATIC IMPORTED )
ADD_LIBRARY( cairo STATIC IMPORTED )
ADD_LIBRARY( fontconfig STATIC IMPORTED )
ADD_LIBRARY( gailutil STATIC IMPORTED )
ADD_LIBRARY( gdk_pixbuf-2.0 STATIC IMPORTED )
ADD_LIBRARY( gdk-win32-3.0 STATIC IMPORTED )
ADD_LIBRARY( gio-2.0 STATIC IMPORTED )
ADD_LIBRARY( glib-2.0 STATIC IMPORTED )
ADD_LIBRARY( gmodule-2.0 STATIC IMPORTED )
ADD_LIBRARY( gobject-2.0 STATIC IMPORTED )
ADD_LIBRARY( gthread-2.0 STATIC IMPORTED )
ADD_LIBRARY( gtk-win32-3.0 STATIC IMPORTED )
ADD_LIBRARY( pango-1.0 STATIC IMPORTED )
ADD_LIBRARY( pangocairo-1.0 STATIC IMPORTED )
ADD_LIBRARY( pangoft2-1.0 STATIC IMPORTED )
ADD_LIBRARY( pangowin32-1.0 STATIC IMPORTED )

SET_TARGET_PROPERTIES( atk-1.0 PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/atk-1.0.lib" )
SET_TARGET_PROPERTIES( cairo PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/cairo.lib" )
SET_TARGET_PROPERTIES( fontconfig PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/fontconfig.lib" )
SET_TARGET_PROPERTIES( gailutil PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/gailutil.lib" )
SET_TARGET_PROPERTIES( gdk_pixbuf-2.0 PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/gdk_pixbuf-2.0.lib" )
SET_TARGET_PROPERTIES( gdk-win32-3.0 PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/gdk-win32-3.0.lib" )
SET_TARGET_PROPERTIES( gio-2.0 PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/gio-2.0.lib" )
SET_TARGET_PROPERTIES( glib-2.0 PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/glib-2.0.lib" )
SET_TARGET_PROPERTIES( gmodule-2.0 PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/gmodule-2.0.lib" )
SET_TARGET_PROPERTIES( gobject-2.0 PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/gobject-2.0.lib" )
SET_TARGET_PROPERTIES( gthread-2.0 PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/gthread-2.0.lib" )
SET_TARGET_PROPERTIES( gtk-win32-3.0 PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/gtk-win32-3.0.lib" )
SET_TARGET_PROPERTIES( pango-1.0 PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/pango-1.0.lib" )
SET_TARGET_PROPERTIES( pangocairo-1.0 PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/pangocairo-1.0.lib" )
SET_TARGET_PROPERTIES( pangoft2-1.0 PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/pangoft2-1.0.lib" )
SET_TARGET_PROPERTIES( pangowin32-1.0 PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/pangowin32-1.0.lib" )

#ADD_LIBRARY( atk-1.0 STATIC IMPORTED )
#ADD_LIBRARY( gtk-3 STATIC IMPORTED )
#ADD_LIBRARY( gdk-3 STATIC IMPORTED )
#ADD_LIBRARY( glib-2.0 STATIC IMPORTED )
#ADD_LIBRARY( cairo STATIC IMPORTED )
#ADD_LIBRARY( pango-1.0 STATIC IMPORTED )
#ADD_LIBRARY( pangocairo-1.0 STATIC IMPORTED )
#ADD_LIBRARY( gobject-2.0 STATIC IMPORTED )
#ADD_LIBRARY( freetype STATIC IMPORTED )
#ADD_LIBRARY( gdk_pixbuf-2.0 STATIC IMPORTED )
#ADD_LIBRARY( gio-2.0 STATIC IMPORTED )
#ADD_LIBRARY( cairo-gobject STATIC IMPORTED )
#ADD_LIBRARY( cairo-script-interpreter STATIC IMPORTED )
#ADD_LIBRARY( croco-0.6 STATIC IMPORTED )
#ADD_LIBRARY( ffi STATIC IMPORTED )
#ADD_LIBRARY( fontconfig STATIC IMPORTED )
#ADD_LIBRARY( gailutil-3 STATIC IMPORTED )
#ADD_LIBRARY( gmodule-2.0 STATIC IMPORTED )
#ADD_LIBRARY( gthread-2.0 STATIC IMPORTED )
#ADD_LIBRARY( jasper STATIC IMPORTED )
#ADD_LIBRARY( jpeg STATIC IMPORTED )
#ADD_LIBRARY( lzma STATIC IMPORTED )
#ADD_LIBRARY( pangoft2-1.0 STATIC IMPORTED )
#ADD_LIBRARY( pangowin32-1.0 STATIC IMPORTED )
#ADD_LIBRARY( pixman-1 STATIC IMPORTED )
#ADD_LIBRARY( png15 STATIC IMPORTED )
#ADD_LIBRARY( png STATIC IMPORTED )
#ADD_LIBRARY( rsvg-2 STATIC IMPORTED )
#ADD_LIBRARY( tiff STATIC IMPORTED )
#ADD_LIBRARY( tiffxx STATIC IMPORTED )
#ADD_LIBRARY( xml2 STATIC IMPORTED )
#ADD_LIBRARY( z STATIC IMPORTED )
#SET_TARGET_PROPERTIES( atk-1.0 PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/libatk-1.0.dll.a")
#SET_TARGET_PROPERTIES( cairo PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/libcairo.dll.a")
#SET_TARGET_PROPERTIES( cairo-gobject PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/libcairo-gobject.dll.a")
#SET_TARGET_PROPERTIES( cairo-script-interpreter PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/libcairo-script-interpreter.dll.a")
#SET_TARGET_PROPERTIES( croco-0.6 PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/libcroco-0.6.dll.a")
#SET_TARGET_PROPERTIES( ffi PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/libffi.dll.a")
#SET_TARGET_PROPERTIES( fontconfig PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/libfontconfig.dll.a")
#SET_TARGET_PROPERTIES( freetype PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/libfreetype.dll.a")
#SET_TARGET_PROPERTIES( gailutil-3 PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/libgailutil-3.dll.a")
#SET_TARGET_PROPERTIES( gdk-3 PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/libgdk-3.dll.a")
#SET_TARGET_PROPERTIES( gdk_pixbuf-2.0 PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/libgdk_pixbuf-2.0.dll.a")
#SET_TARGET_PROPERTIES( gio-2.0 PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/libgio-2.0.dll.a")
#SET_TARGET_PROPERTIES( glib-2.0 PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/libglib-2.0.dll.a")
#SET_TARGET_PROPERTIES( gmodule-2.0 PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/libgmodule-2.0.dll.a")
#SET_TARGET_PROPERTIES( gobject-2.0 PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/libgobject-2.0.dll.a")
#SET_TARGET_PROPERTIES( gthread-2.0 PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/libgthread-2.0.dll.a")
#SET_TARGET_PROPERTIES( gtk-3 PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/libgtk-3.dll.a")
#SET_TARGET_PROPERTIES( jasper PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/libjasper.dll.a")
#SET_TARGET_PROPERTIES( jpeg PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/libjpeg.dll.a")
#SET_TARGET_PROPERTIES( lzma PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/liblzma.dll.a")
#SET_TARGET_PROPERTIES( pango-1.0 PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/libpango-1.0.dll.a")
#SET_TARGET_PROPERTIES( pangocairo-1.0 PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/libpangocairo-1.0.dll.a")
#SET_TARGET_PROPERTIES( pangoft2-1.0 PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/libpangoft2-1.0.dll.a")
#SET_TARGET_PROPERTIES( pangowin32-1.0 PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/libpangowin32-1.0.dll.a")
#SET_TARGET_PROPERTIES( pixman-1 PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/libpixman-1.dll.a")
#SET_TARGET_PROPERTIES( png15 PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/libpng15.dll.a")
#SET_TARGET_PROPERTIES( png PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/libpng.dll.a")
#SET_TARGET_PROPERTIES( rsvg-2 PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/librsvg-2.dll.a")
#SET_TARGET_PROPERTIES( tiff PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/libtiff.dll.a")
#SET_TARGET_PROPERTIES( tiffxx PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/libtiffxx.dll.a")
#SET_TARGET_PROPERTIES( xml2 PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/libxml2.dll.a")
#SET_TARGET_PROPERTIES( z PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY_BASE}/gtk+-bundle_3.6.4-20130513_win64/lib/libz.dll.a")


# ==== ==== ==== ==== ==== ==== ==== ==== C-Code crystal_facet_uml Compiler Settings ==== ==== ==== ==== ==== ==== ==== ====

if(NOT CMAKE_BUILD_TYPE)
  message( "Warn: CMAKE_BUILD_TYPE no set!                 setting(CMAKE_BUILD_TYPE \"Release\")")
  set(CMAKE_BUILD_TYPE "Release")
endif(NOT CMAKE_BUILD_TYPE)
MESSAGE( "Note:                                          CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}" )
MESSAGE( "Note:                                          CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR}" )
MESSAGE( "Note:                                          CMAKE_BUILD_DIR = ${CMAKE_BUILD_DIR}" )
MESSAGE( "Note:                                          THIRD_PARTY_BASE = ${THIRD_PARTY_BASE}" )
MESSAGE( "Note: to build a release, run                  cmake -DCMAKE_BUILD_TYPE=Release " ${CMAKE_CURRENT_SOURCE_DIR} )
MESSAGE( "Note: to build with asserts and traces, run    cmake -DCMAKE_BUILD_TYPE=Debug " ${CMAKE_CURRENT_SOURCE_DIR} )
MESSAGE( "Note: then, to build using 4 cores, run        make -j4" )
MESSAGE( "Note: to install, run                          sudo make install" )

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -D_XOPEN_SOURCE=700 -O2 -pedantic -g -Wformat -Werror=format-security -Wall -Wno-overlength-strings" )
# Note: The GIMP-generated images are longer that 4095 characters, therefore -Wno-overlength-strings is required here.

LIST(APPEND applicationSource ${commonSource} ${appSource})

# ==== ==== ==== ==== ==== ==== ==== ==== C-Code crystal_facet_uml Linker Settings ==== ==== ==== ==== ==== ==== ==== ====

ADD_EXECUTABLE(crystal_facet_uml ${applicationSource})
TARGET_LINK_LIBRARIES(crystal_facet_uml atk-1.0 cairo fontconfig gailutil gdk_pixbuf-2.0 gdk-win32-3.0 gio-2.0 glib-2.0 gmodule-2.0 gobject-2.0 gthread-2.0 gtk-win32-3.0 pango-1.0 pangocairo-1.0 pangoft2-1.0 pangowin32-1.0 m )
#TARGET_LINK_LIBRARIES(crystal_facet_uml atk-1.0 gtk-3 gdk-3 glib-2.0 cairo pango-1.0 pangocairo-1.0 gobject-2.0 freetype gdk_pixbuf-2.0 gio-2.0 cairo-gobject cairo-script-interpreter croco-0.6 ffi fontconfig gailutil-3 gmodule-2.0 gthread-2.0 jasper jpeg lzma pangoft2-1.0 pangowin32-1.0 pixman-1 png15 png rsvg-2 tiff tiffxx xml2 z m)
SET_TARGET_PROPERTIES(crystal_facet_uml PROPERTIES LINKER_LANGUAGE C)

# ==== ==== ==== ==== ==== ==== ==== ==== C-Code unittest Compiler Settings ==== ==== ==== ==== ==== ==== ==== ====

INCLUDE_DIRECTORIES(../test_fw/include)
INCLUDE_DIRECTORIES(../data/unittest)
INCLUDE_DIRECTORIES(../ctrl/unittest)
INCLUDE_DIRECTORIES(../io/unittest)
INCLUDE_DIRECTORIES(../pencil/unittest)
INCLUDE_DIRECTORIES(../universal/unittest)
INCLUDE_DIRECTORIES(../utf8stringbuf/unittest)

# now the test cases. be less restrictive with errors.
SET(CMAKE_C_FLAGS "-std=c11 -D_XOPEN_SOURCE=700 -O2 -pedantic -g -Wformat -Wall -Wno-overlength-strings" )
# Note: The GIMP-generated images are longer that 4095 characters, therefore -Wno-overlength-strings is required here.

LIST(APPEND unittestSource ${commonSource} ${utestSource})

# ==== ==== ==== ==== ==== ==== ==== ==== C-Code unittest Linker Settings ==== ==== ==== ==== ==== ==== ==== ====

ADD_EXECUTABLE(unittest_crystal_facet_uml ${unittestSource})
TARGET_LINK_LIBRARIES(unittest_crystal_facet_uml atk-1.0 cairo fontconfig gailutil gdk_pixbuf-2.0 gdk-win32-3.0 gio-2.0 glib-2.0 gmodule-2.0 gobject-2.0 gthread-2.0 gtk-win32-3.0 pango-1.0 pangocairo-1.0 pangoft2-1.0 pangowin32-1.0 m )
#TARGET_LINK_LIBRARIES(unittest_crystal_facet_uml atk-1.0 gtk-3 gdk-3 glib-2.0 cairo pango-1.0 pangocairo-1.0 gobject-2.0 freetype gdk_pixbuf-2.0 gio-2.0 cairo-gobject cairo-script-interpreter croco-0.6 ffi fontconfig gailutil-3 gmodule-2.0 gthread-2.0 jasper jpeg lzma pangoft2-1.0 pangowin32-1.0 pixman-1 png15 png rsvg-2 tiff tiffxx xml2 z m)
SET_TARGET_PROPERTIES(unittest_crystal_facet_uml PROPERTIES LINKER_LANGUAGE C)



