CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

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
SET(SQLITE_BASE "${CMAKE_BINARY_DIR}/sqlite-amalgamation-3290000")
SET(GTK_BUNDLE_BASE "${CMAKE_BINARY_DIR}/gtk+-bundle_3.6.4-20130513_win64")
#SET(GTK_BUNDLE_BASE "${CMAKE_BINARY_DIR}/gtk+-bundle_3.6.4-20130513_win32")

INCLUDE_DIRECTORIES("${SQLITE_BASE}")
INCLUDE_DIRECTORIES("${GTK_BUNDLE_BASE}/lib/libffi-3.0.12/include")
INCLUDE_DIRECTORIES("${GTK_BUNDLE_BASE}/lib/glib-2.0/include")
INCLUDE_DIRECTORIES("${GTK_BUNDLE_BASE}/include")
INCLUDE_DIRECTORIES("${GTK_BUNDLE_BASE}/include/libpng15")
INCLUDE_DIRECTORIES("${GTK_BUNDLE_BASE}/include/libxml2")
INCLUDE_DIRECTORIES("${GTK_BUNDLE_BASE}/include/gail-3.0")
INCLUDE_DIRECTORIES("${GTK_BUNDLE_BASE}/include/cairo")
INCLUDE_DIRECTORIES("${GTK_BUNDLE_BASE}/include/gio-win32-2.0")
INCLUDE_DIRECTORIES("${GTK_BUNDLE_BASE}/include/pango-1.0")
INCLUDE_DIRECTORIES("${GTK_BUNDLE_BASE}/include/libcroco-0.6")
INCLUDE_DIRECTORIES("${GTK_BUNDLE_BASE}/include/jasper")
INCLUDE_DIRECTORIES("${GTK_BUNDLE_BASE}/include/glib-2.0")
INCLUDE_DIRECTORIES("${GTK_BUNDLE_BASE}/include/lzma")
INCLUDE_DIRECTORIES("${GTK_BUNDLE_BASE}/include/gdk-pixbuf-2.0")
INCLUDE_DIRECTORIES("${GTK_BUNDLE_BASE}/include/atk-1.0")
INCLUDE_DIRECTORIES("${GTK_BUNDLE_BASE}/include/fontconfig")
INCLUDE_DIRECTORIES("${GTK_BUNDLE_BASE}/include/gtk-3.0")
INCLUDE_DIRECTORIES("${GTK_BUNDLE_BASE}/include/librsvg-2.0")
INCLUDE_DIRECTORIES("${GTK_BUNDLE_BASE}/include/pixman-1")
INCLUDE_DIRECTORIES("${GTK_BUNDLE_BASE}/include/freetype2")

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
                       ${SQLITE_BASE}/sqlite3.c)
FILE(GLOB appSource    ../main/source/*.c)
FILE(GLOB utestSource  ../main/unittest/*.c
                       ../data/unittest/*.c
                       ../ctrl/unittest/*.c
                       ../io/unittest/*.c
                       ../pencil/unittest/*.c
                       ../universal/unittest/*.c
                       ../utf8stringbuf/unittest/*.c)

ADD_LIBRARY( atk-1.0 SHARED IMPORTED )
ADD_LIBRARY( cairo SHARED IMPORTED )
ADD_LIBRARY( fontconfig SHARED IMPORTED )
ADD_LIBRARY( gailutil SHARED IMPORTED )
ADD_LIBRARY( gdk_pixbuf-2.0 SHARED IMPORTED )
ADD_LIBRARY( gdk-win32-3.0 SHARED IMPORTED )
ADD_LIBRARY( gio-2.0 SHARED IMPORTED )
ADD_LIBRARY( glib-2.0 SHARED IMPORTED )
ADD_LIBRARY( gmodule-2.0 SHARED IMPORTED )
ADD_LIBRARY( gobject-2.0 SHARED IMPORTED )
ADD_LIBRARY( gthread-2.0 SHARED IMPORTED )
ADD_LIBRARY( gtk-win32-3.0 SHARED IMPORTED )
ADD_LIBRARY( pango-1.0 SHARED IMPORTED )
ADD_LIBRARY( pangocairo-1.0 SHARED IMPORTED )
ADD_LIBRARY( pangoft2-1.0 SHARED IMPORTED )
ADD_LIBRARY( pangowin32-1.0 SHARED IMPORTED )
LINK_DIRECTORIES("${GTK_BUNDLE_BASE}/bin/")
LINK_DIRECTORIES("${GTK_BUNDLE_BASE}/lib/")
SET_TARGET_PROPERTIES( atk-1.0 PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/atk-1.0.lib" )
SET_TARGET_PROPERTIES( cairo PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/cairo.lib" )
SET_TARGET_PROPERTIES( fontconfig PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/fontconfig.lib" )
SET_TARGET_PROPERTIES( gailutil PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/gailutil.lib" )
SET_TARGET_PROPERTIES( gdk_pixbuf-2.0 PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/gdk_pixbuf-2.0.lib" )
SET_TARGET_PROPERTIES( gdk-win32-3.0 PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/gdk-win32-3.0.lib" )
SET_TARGET_PROPERTIES( gio-2.0 PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/gio-2.0.lib" )
SET_TARGET_PROPERTIES( glib-2.0 PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/glib-2.0.lib" )
SET_TARGET_PROPERTIES( gmodule-2.0 PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/gmodule-2.0.lib" )
SET_TARGET_PROPERTIES( gobject-2.0 PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/gobject-2.0.lib" )
SET_TARGET_PROPERTIES( gthread-2.0 PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/gthread-2.0.lib" )
SET_TARGET_PROPERTIES( gtk-win32-3.0 PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/gtk-win32-3.0.lib" )
SET_TARGET_PROPERTIES( pango-1.0 PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/pango-1.0.lib" )
SET_TARGET_PROPERTIES( pangocairo-1.0 PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/pangocairo-1.0.lib" )
SET_TARGET_PROPERTIES( pangoft2-1.0 PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/pangoft2-1.0.lib" )
SET_TARGET_PROPERTIES( pangowin32-1.0 PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/pangowin32-1.0.lib" )

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
#SET_TARGET_PROPERTIES( atk-1.0 PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/libatk-1.0.dll.a")
#SET_TARGET_PROPERTIES( cairo PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/libcairo.dll.a")
#SET_TARGET_PROPERTIES( cairo-gobject PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/libcairo-gobject.dll.a")
#SET_TARGET_PROPERTIES( cairo-script-interpreter PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/libcairo-script-interpreter.dll.a")
#SET_TARGET_PROPERTIES( croco-0.6 PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/libcroco-0.6.dll.a")
#SET_TARGET_PROPERTIES( ffi PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/libffi.dll.a")
#SET_TARGET_PROPERTIES( fontconfig PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/libfontconfig.dll.a")
#SET_TARGET_PROPERTIES( freetype PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/libfreetype.dll.a")
#SET_TARGET_PROPERTIES( gailutil-3 PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/libgailutil-3.dll.a")
#SET_TARGET_PROPERTIES( gdk-3 PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/libgdk-3.dll.a")
#SET_TARGET_PROPERTIES( gdk_pixbuf-2.0 PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/libgdk_pixbuf-2.0.dll.a")
#SET_TARGET_PROPERTIES( gio-2.0 PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/libgio-2.0.dll.a")
#SET_TARGET_PROPERTIES( glib-2.0 PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/libglib-2.0.dll.a")
#SET_TARGET_PROPERTIES( gmodule-2.0 PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/libgmodule-2.0.dll.a")
#SET_TARGET_PROPERTIES( gobject-2.0 PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/libgobject-2.0.dll.a")
#SET_TARGET_PROPERTIES( gthread-2.0 PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/libgthread-2.0.dll.a")
#SET_TARGET_PROPERTIES( gtk-3 PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/libgtk-3.dll.a")
#SET_TARGET_PROPERTIES( jasper PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/libjasper.dll.a")
#SET_TARGET_PROPERTIES( jpeg PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/libjpeg.dll.a")
#SET_TARGET_PROPERTIES( lzma PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/liblzma.dll.a")
#SET_TARGET_PROPERTIES( pango-1.0 PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/libpango-1.0.dll.a")
#SET_TARGET_PROPERTIES( pangocairo-1.0 PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/libpangocairo-1.0.dll.a")
#SET_TARGET_PROPERTIES( pangoft2-1.0 PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/libpangoft2-1.0.dll.a")
#SET_TARGET_PROPERTIES( pangowin32-1.0 PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/libpangowin32-1.0.dll.a")
#SET_TARGET_PROPERTIES( pixman-1 PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/libpixman-1.dll.a")
#SET_TARGET_PROPERTIES( png15 PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/libpng15.dll.a")
#SET_TARGET_PROPERTIES( png PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/libpng.dll.a")
#SET_TARGET_PROPERTIES( rsvg-2 PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/librsvg-2.dll.a")
#SET_TARGET_PROPERTIES( tiff PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/libtiff.dll.a")
#SET_TARGET_PROPERTIES( tiffxx PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/libtiffxx.dll.a")
#SET_TARGET_PROPERTIES( xml2 PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/libxml2.dll.a")
#SET_TARGET_PROPERTIES( z PROPERTIES IMPORTED_LOCATION "${GTK_BUNDLE_BASE}/lib/libz.dll.a")


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



