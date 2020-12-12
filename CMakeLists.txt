PROJECT(crystal_facet_uml LANGUAGES C)
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

#helper to find include directories and libraries:
find_package(PkgConfig REQUIRED)

# ==== ==== ==== ==== ==== ==== ==== ==== Build Environment Settings ==== ==== ==== ==== ==== ==== ==== ====

set(COMPILE_IN_INCLUDE_DIR "" CACHE FILEPATH "Path to additional include directory")
set(COMPILE_IN_SOURCE_FILE "" CACHE FILEPATH "Path to additional source file, e.g. sqlite3.c")
INCLUDE_DIRECTORIES(${COMPILE_IN_INCLUDE_DIR})

# crystal_facet_uml

INCLUDE_DIRECTORIES(gui/include)
INCLUDE_DIRECTORIES(main/include)
INCLUDE_DIRECTORIES(tslog/include)
INCLUDE_DIRECTORIES(trace/include)
INCLUDE_DIRECTORIES(ctrl/include)
INCLUDE_DIRECTORIES(data/include)
INCLUDE_DIRECTORIES(utf8stringbuf/include)
INCLUDE_DIRECTORIES(pencil/include)
INCLUDE_DIRECTORIES(io/include)
INCLUDE_DIRECTORIES(universal/include)

#sqlite3 dependencies

pkg_search_module(SQLITE3 REQUIRED sqlite3)
INCLUDE_DIRECTORIES(${SQLITE3_INCLUDE_DIRS})
#MESSAGE("SQLITE3: ${SQLITE3_INCLUDE_DIRS}")

#gtk3 dependencies

pkg_search_module(GLIB2 REQUIRED glib-2.0)
INCLUDE_DIRECTORIES(${GLIB2_INCLUDE_DIRS})
#MESSAGE("GLIB2:   ${GLIB2_INCLUDE_DIRS}")

pkg_search_module(GTK3 REQUIRED gtk+-3.0)
INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS})
#MESSAGE("GTK3:    ${GTK3_INCLUDE_DIRS}")
#MESSAGE("GTK3:    ${GTK3_FOUND}")
#MESSAGE("GTK3:    ${GTK3_LIBRARIES}")
#MESSAGE("GTK3:    ${GTK3_LIBRARY_DIRS}")
#MESSAGE("GTK3:    ${GTK3_LDFLAGS}")
#MESSAGE("GTK3:    ${GTK3_LDFLAGS_OTHER}")
#MESSAGE("GTK3:    ${GTK3_CFLAGS}")
#MESSAGE("GTK3:    ${GTK3_CFLAGS_OTHER}")

pkg_search_module(PANGOCAIRO REQUIRED pangocairo)
INCLUDE_DIRECTORIES(${PANGOCAIRO_INCLUDE_DIRS})
#MESSAGE("PANGOCAIRO: ${PANGOCAIRO_INCLUDE_DIRS}")

FILE(GLOB commonSource gui/source/*.c
                       gui/source/sketch_area/*.c
                       trace/source/*.c
                       ctrl/source/*.c
                       data/source/*.c
                       data/source/storage/*.c
                       data/source/set/*.c
                       utf8stringbuf/source/util/string/*.c
                       universal/source/stream/*.c
                       pencil/source/*.c
                       pencil/source/util/geometry/*.c
                       pencil/source/draw/*.c
                       io/source/*.c
                       io/source/image/*.c
                       io/source/json/*.c
                       io/source/xml/*.c
                       io/source/txt/*.c
                       io/source/xmi/*.c
                       io/source/md/*.c)
FILE(GLOB appSource    main/source/*.c)
FILE(GLOB utestSource  main/unittest/*.c
                       data/unittest/*.c
                       ctrl/unittest/*.c
                       gui/unittest/*.c
                       io/unittest/*.c
                       pencil/unittest/*.c
                       universal/unittest/*.c
                       utf8stringbuf/unittest/*.c)

# ==== ==== ==== ==== ==== ==== ==== ==== C-Code crystal_facet_uml Compiler Settings ==== ==== ==== ==== ==== ==== ==== ====

if(NOT CMAKE_BUILD_TYPE)
  message( "Warn: CMAKE_BUILD_TYPE no set!                 setting(CMAKE_BUILD_TYPE \"Release\")")
  set(CMAKE_BUILD_TYPE "Release")
endif(NOT CMAKE_BUILD_TYPE)
MESSAGE( "Note:                                          CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}" )
#MESSAGE( "Note:                                          CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR}" )
#MESSAGE( "Note:                                          CMAKE_BUILD_DIR = ${CMAKE_BUILD_DIR}" )
MESSAGE( "Note: to build a release, run                  cmake -DCMAKE_BUILD_TYPE=Release " ${CMAKE_CURRENT_SOURCE_DIR} )
MESSAGE( "Note: to build with asserts and traces, run    cmake -DCMAKE_BUILD_TYPE=Debug " ${CMAKE_CURRENT_SOURCE_DIR} )
MESSAGE( "Note: then, to build, run                      make -j4" )
MESSAGE( "Note: to install, run                          sudo make install" )

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -D_XOPEN_SOURCE=700 -O2 -pedantic -g -Wall -Werror -Wno-overlength-strings ${GTK3_CFLAGS_OTHER}" )
# Note: The GIMP-generated images are longer that 4095 characters, therefore -Wno-overlength-strings is required here.

# ==== ==== ==== ==== ==== ==== ==== ==== C-Code crystal_facet_uml Linker Settings ==== ==== ==== ==== ==== ==== ==== ====

ADD_EXECUTABLE(crystal_facet_uml ${appSource} ${commonSource} ${COMPILE_IN_SOURCE_FILE})
TARGET_LINK_LIBRARIES(crystal_facet_uml ${GTK3_LIBRARIES} ${GLIB2_LIBRARIES} ${PANGOCAIRO_LIBRARIES} ${SQLITE3_LIBRARIES} m)
SET_TARGET_PROPERTIES(crystal_facet_uml PROPERTIES LINKER_LANGUAGE C)

# ==== ==== ==== ==== ==== ==== ==== ==== C-Code unittest Compiler Settings ==== ==== ==== ==== ==== ==== ==== ====

# for test cases, be less restrictive with errors.
SET(CMAKE_C_FLAGS "-std=c11 -D_XOPEN_SOURCE=700 -O2 -pedantic -g -Wall -Wno-overlength-strings ${GTK3_CFLAGS_OTHER}" )
# Note: The GIMP-generated images are longer that 4095 characters, therefore -Wno-overlength-strings is needed here.

# ==== ==== ==== ==== ==== ==== ==== ==== C-Code unittest Linker Settings ==== ==== ==== ==== ==== ==== ==== ====

ADD_EXECUTABLE(unittest_crystal_facet_uml ${utestSource} ${commonSource} ${COMPILE_IN_SOURCE_FILE})
TARGET_INCLUDE_DIRECTORIES(unittest_crystal_facet_uml PUBLIC test_fw/include)
TARGET_INCLUDE_DIRECTORIES(unittest_crystal_facet_uml PUBLIC data/unittest)
TARGET_INCLUDE_DIRECTORIES(unittest_crystal_facet_uml PUBLIC ctrl/unittest)
TARGET_INCLUDE_DIRECTORIES(unittest_crystal_facet_uml PUBLIC gui/unittest)
TARGET_INCLUDE_DIRECTORIES(unittest_crystal_facet_uml PUBLIC io/unittest)
TARGET_INCLUDE_DIRECTORIES(unittest_crystal_facet_uml PUBLIC pencil/unittest)
TARGET_INCLUDE_DIRECTORIES(unittest_crystal_facet_uml PUBLIC universal/unittest)
TARGET_INCLUDE_DIRECTORIES(unittest_crystal_facet_uml PUBLIC utf8stringbuf/unittest)
TARGET_LINK_LIBRARIES(unittest_crystal_facet_uml ${GTK3_LIBRARIES} ${GLIB2_LIBRARIES} ${PANGOCAIRO_LIBRARIES} ${SQLITE3_LIBRARIES} m)
SET_TARGET_PROPERTIES(unittest_crystal_facet_uml PROPERTIES LINKER_LANGUAGE C)

# ==== ==== ==== ==== ==== ==== ==== ==== Documentation Build Settings ==== ==== ==== ==== ==== ==== ==== ====

MESSAGE( "Note: update the user docs via 'cd user_doc && make' before running make install")

# ==== ==== ==== ==== ==== ==== ==== ==== Install Settings ==== ==== ==== ==== ==== ==== ==== ====

INSTALL(TARGETS crystal_facet_uml DESTINATION "/usr/bin/")
INSTALL(FILES "${CMAKE_CURRENT_SOURCE_DIR}/installation_linux/config/crystal_facet_uml.desktop" DESTINATION "/usr/share/applications/")
INSTALL(FILES "${CMAKE_CURRENT_SOURCE_DIR}/gui/source/resources/crystal_facet_uml_80x80.png" DESTINATION "/usr/share/pixmaps/" RENAME "crystal_facet_uml.png")
INSTALL(FILES "${CMAKE_CURRENT_SOURCE_DIR}/user_doc/crystal_facet_uml.1.gz" DESTINATION "/usr/share/man/man1/")


