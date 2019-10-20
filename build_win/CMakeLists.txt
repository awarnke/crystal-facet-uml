CMAKE_MINIMUM_REQUIRED(VERSION 2.4)
SET(CMAKE_VERBOSE_MAKEFILE TRUE)

# ==== ==== ==== ==== ==== ==== ==== ==== Build Environment Settings ==== ==== ==== ==== ==== ==== ==== ====

PROJECT(crystal_facet_uml)

INCLUDE_DIRECTORIES(../gui/include)
INCLUDE_DIRECTORIES(../main/include)
INCLUDE_DIRECTORIES(../tslog/include)
INCLUDE_DIRECTORIES(../trace/include)
INCLUDE_DIRECTORIES(../ctrl/include)
INCLUDE_DIRECTORIES(../data/include)
INCLUDE_DIRECTORIES(../utf8stringbuf/include)
INCLUDE_DIRECTORIES(../pencil/include)
INCLUDE_DIRECTORIES(../io/include)
INCLUDE_DIRECTORIES(../universal/include)

SET(THIRD_PARTY_BASE "${CMAKE_BINARY_DIR}")
SET(SQLITE_BASE "${CMAKE_BINARY_DIR}/sqlite-amalgamation-3290000")
#SET(GTK_BUNDLE_BASE "${CMAKE_BINARY_DIR}/gtk+-bundle_3.6.4-20130513_win64")
SET(GTK_BUNDLE_BASE "${CMAKE_BINARY_DIR}/gtk+-bundle_3.6.4-20130513_win32")

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
                       ../gui/source/sketch_area/*.c
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

FIND_LIBRARY( ATK_LIB atk-1.0 "${GTK_BUNDLE_BASE}/lib/")
FIND_LIBRARY( CAIRO_LIB cairo "${GTK_BUNDLE_BASE}/lib/" )
FIND_LIBRARY( FONTCNF_LIB fontconfig "${GTK_BUNDLE_BASE}/lib/" )
FIND_LIBRARY( GAIL_LIB gailutil "${GTK_BUNDLE_BASE}/lib/" )
FIND_LIBRARY( GDKPIX_LIB gdk_pixbuf-2.0 "${GTK_BUNDLE_BASE}/lib/" )
FIND_LIBRARY( GDKWIN_LIB gdk-win32-3.0 "${GTK_BUNDLE_BASE}/lib/" )
FIND_LIBRARY( GIO_LIB gio-2.0 "${GTK_BUNDLE_BASE}/lib/" )
FIND_LIBRARY( GLIB_LIB glib-2.0 "${GTK_BUNDLE_BASE}/lib/" )
FIND_LIBRARY( GMOD_LIB gmodule-2.0 "${GTK_BUNDLE_BASE}/lib/" )
FIND_LIBRARY( GOBJ_LIB gobject-2.0 "${GTK_BUNDLE_BASE}/lib/" )
FIND_LIBRARY( GTHREAD_LIB gthread-2.0 "${GTK_BUNDLE_BASE}/lib/" )
FIND_LIBRARY( GTK_LIB gtk-win32-3.0 "${GTK_BUNDLE_BASE}/lib/" )
FIND_LIBRARY( PANGO_LIB pango-1.0 "${GTK_BUNDLE_BASE}/lib/" )
FIND_LIBRARY( PANGOCAIRO_LIB pangocairo-1.0 "${GTK_BUNDLE_BASE}/lib/" )
FIND_LIBRARY( PANGOFT_LIB pangoft2-1.0 "${GTK_BUNDLE_BASE}/lib/" )
FIND_LIBRARY( PANGOWIN_LIB pangowin32-1.0 "${GTK_BUNDLE_BASE}/lib/" )
MESSAGE("gtk found at ${GTK_LIB}")

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
TARGET_LINK_LIBRARIES(crystal_facet_uml ${ATK_LIB} ${CAIRO_LIB} ${FONTCNF_LIB} ${GAIL_LIB} ${GDKPIX_LIB} ${GDKWIN_LIB} ${GIO_LIB} ${GLIB_LIB} ${GMOD_LIB} ${GOBJ_LIB} ${GTHREAD_LIB} ${GTK_LIB} ${PANGO_LIB} ${PANGOCAIRO_LIB} ${PANGOFT_LIB} ${PANGOWIN_LIB} )
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
TARGET_LINK_LIBRARIES(unittest_crystal_facet_uml ${ATK_LIB} ${CAIRO_LIB} ${FONTCNF_LIB} ${GAIL_LIB} ${GDKPIX_LIB} ${GDKWIN_LIB} ${GIO_LIB} ${GLIB_LIB} ${GMOD_LIB} ${GOBJ_LIB} ${GTHREAD_LIB} ${GTK_LIB} ${PANGO_LIB} ${PANGOCAIRO_LIB} ${PANGOFT_LIB} ${PANGOWIN_LIB} )
SET_TARGET_PROPERTIES(unittest_crystal_facet_uml PROPERTIES LINKER_LANGUAGE C)



