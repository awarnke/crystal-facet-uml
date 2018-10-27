PROJECT(crystal_facet_uml)
CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -D_XOPEN_SOURCE=700 -O2 -pedantic -g -Wformat -Werror=format-security -Wall" )

INCLUDE_DIRECTORIES(gui/source)
INCLUDE_DIRECTORIES(gui/include)
INCLUDE_DIRECTORIES(main/source)
INCLUDE_DIRECTORIES(main/include)
INCLUDE_DIRECTORIES(tslog/include)
INCLUDE_DIRECTORIES(trace/source)
INCLUDE_DIRECTORIES(trace/include)
INCLUDE_DIRECTORIES(ctrl/source)
INCLUDE_DIRECTORIES(ctrl/include)
INCLUDE_DIRECTORIES(data/source)
INCLUDE_DIRECTORIES(data/include)
INCLUDE_DIRECTORIES(sqlite/source)
INCLUDE_DIRECTORIES(sqlite/include)
INCLUDE_DIRECTORIES(utf8stringbuf/include)
INCLUDE_DIRECTORIES(pencil/source)
INCLUDE_DIRECTORIES(pencil/include)
INCLUDE_DIRECTORIES(universal/include)

INCLUDE_DIRECTORIES(/usr/include/cairo)
INCLUDE_DIRECTORIES(/usr/include/freetype2)
INCLUDE_DIRECTORIES(/opt/gnome/include/gtk-3.0)
INCLUDE_DIRECTORIES(/opt/gnome/lib/gtk-3.0/include)
INCLUDE_DIRECTORIES(/opt/gnome/include/glib-3.0)
INCLUDE_DIRECTORIES(/opt/gnome/lib/glib-3.0/include)
INCLUDE_DIRECTORIES(/usr/lib/glib-2.0/include)
INCLUDE_DIRECTORIES(/opt/gnome/include/atk-1.0)
INCLUDE_DIRECTORIES(/opt/gnome/include/gtk-3.0/gtk)
INCLUDE_DIRECTORIES(/opt/gnome/include/pango-1.0)
INCLUDE_DIRECTORIES(/usr/include/pango-1.0)
INCLUDE_DIRECTORIES(/usr/include/gio-unix-2.0)
INCLUDE_DIRECTORIES(/usr/include/atk-1.0)
INCLUDE_DIRECTORIES(/usr/lib/glib-3.0/include)
INCLUDE_DIRECTORIES(/usr/lib/gtk-3.0/include)
INCLUDE_DIRECTORIES(/usr/include/gtk-3.0)
INCLUDE_DIRECTORIES(/usr/lib64/gtk-3.0/include)
INCLUDE_DIRECTORIES(/usr/include/glib-2.0)
INCLUDE_DIRECTORIES(/usr/lib64/glib-2.0/include)
INCLUDE_DIRECTORIES(/usr/include/gdk-pixbuf-2.0)
INCLUDE_DIRECTORIES(/usr/lib/arm-linux-gnueabihf/glib-2.0/include)
INCLUDE_DIRECTORIES(/usr/lib/arm-linux-gnueabihf/gtk-3.0/include)
INCLUDE_DIRECTORIES(/usr/lib/x86_64-linux-gnu/glib-2.0/include)
MESSAGE("Note: if include directories are missing, check out pkg-config --cflags glib-2.0")
LINK_DIRECTORIES(/opt/gnome/lib)
LINK_DIRECTORIES(/opt/gnome/lib/gtk-3.0)
LINK_DIRECTORIES(/usr/lib/)
LINK_DIRECTORIES(/usr/lib64/)
SET(crystal_facet_uml_BINARY_DIR ../bin)
SET(crystal_facet_uml_SOURCE_DIR ..)
FILE(GLOB commonSource gui/source/*.c trace/source/*.c ctrl/source/*.c data/source/*.c data/source/storage/*.c data/source/serial/*.c sqlite/source/sqlite3.c utf8stringbuf/source/util/string/*.c pencil/source/*.c pencil/source/util/geometry/*.c)
FILE(GLOB appSource main/source/*.c)
FILE(GLOB utestSource main/unittest/*.c embunit/embUnit/*.c embunit/textui.c data/unittest/*.c ctrl/unittest/*.c pencil/unittest/*.c universal/unittest/*.c)

LIST(APPEND applicationSource ${commonSource} ${appSource})
ADD_EXECUTABLE(crystal_facet_uml ${applicationSource})
TARGET_LINK_LIBRARIES(crystal_facet_uml gtk-3 gdk-3 glib-2.0 cairo pango-1.0 pangocairo-1.0 gobject-2.0 freetype gdk_pixbuf-2.0 pthread dl m)
SET_TARGET_PROPERTIES(crystal_facet_uml PROPERTIES LINKER_LANGUAGE C)

MESSAGE( "Note: to build a release, run               cmake -DCMAKE_BUILD_TYPE=Release .." )
MESSAGE( "Note: to build with asserts and traces, run cmake -DCMAKE_BUILD_TYPE=Debug .." )
MESSAGE( "Note: to build with all 4 cores, run make -j4 afterwards" )

INSTALL(TARGETS crystal_facet_uml DESTINATION "/usr/bin/") # Needed for CPack to work
INSTALL(FILES "${CMAKE_CURRENT_SOURCE_DIR}/installation_linux/config/crystal_facet_uml.desktop" DESTINATION "/usr/share/applications/")
INSTALL(FILES "${CMAKE_CURRENT_SOURCE_DIR}/gui/source/resources/crystal_facet_uml_80x80.png" DESTINATION "/usr/share/pixmaps/" RENAME "crystal_facet_uml.png")
INSTALL(FILES "${CMAKE_CURRENT_SOURCE_DIR}/installation_linux/man/crystal_facet_uml.1.gz" DESTINATION "/usr/share/man/man1/")
MESSAGE("Note: run 'cd installation_files/man && ./make.sh' before running 'make install' or 'make package'")

# now the test cases. be less restrictive with errors.
SET(CMAKE_C_FLAGS "-std=c99 -D_XOPEN_SOURCE=700 -O2 -pedantic -g -Wformat -Wall" )

INCLUDE_DIRECTORIES(embunit)
INCLUDE_DIRECTORIES(data/unittest)
INCLUDE_DIRECTORIES(ctrl/unittest)
INCLUDE_DIRECTORIES(pencil/unittest)
INCLUDE_DIRECTORIES(universal/unittest)

LIST(APPEND unittestSource ${commonSource} ${utestSource})
ADD_EXECUTABLE(unittest_crystal_facet_uml ${unittestSource})
TARGET_LINK_LIBRARIES(unittest_crystal_facet_uml gtk-3 gdk-3 glib-2.0 cairo pango-1.0 pangocairo-1.0 gobject-2.0 freetype gdk_pixbuf-2.0 pthread dl m)
SET_TARGET_PROPERTIES(unittest_crystal_facet_uml PROPERTIES LINKER_LANGUAGE C)
