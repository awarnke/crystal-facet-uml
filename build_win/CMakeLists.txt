PROJECT(crystal_facet_uml)
CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

# ==== ==== ==== ==== ==== ==== ==== ==== Build Environment Settings ==== ==== ==== ==== ==== ==== ==== ====

MESSAGE( "call cmake with parameters:" )
MESSAGE( "-DCMAKE_SYSTEM_NAME=Windows -DCMAKE_C_COMPILER=/usr/bin/i686-w64-mingw32-gcc -DCMAKE_CXX_COMPILER=/usr/bin/i686-w64-mingw32-g++" )

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

INCLUDE_DIRECTORIES("${THIRD_PARTY_BASE}/cairo")
INCLUDE_DIRECTORIES("${THIRD_PARTY_BASE}/gtk")
INCLUDE_DIRECTORIES("${THIRD_PARTY_BASE}/glib")
INCLUDE_DIRECTORIES("${THIRD_PARTY_BASE}/build_glib")
INCLUDE_DIRECTORIES("${THIRD_PARTY_BASE}/atk")
INCLUDE_DIRECTORIES("${THIRD_PARTY_BASE}/pango")
INCLUDE_DIRECTORIES("${THIRD_PARTY_BASE}/gdk-pixbuf")
INCLUDE_DIRECTORIES("${THIRD_PARTY_BASE}/glib/glib")
INCLUDE_DIRECTORIES("${THIRD_PARTY_BASE}/sqlite-amalgamation-3290000")

#SET(crystal_facet_uml_BINARY_DIR ./bin)
#SET(crystal_facet_uml_SOURCE_DIR .)
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
                       ../io/source/json/*.c)
FILE(GLOB appSource    ../main/source/*.c)
FILE(GLOB utestSource  ../main/unittest/*.c
                       ../data/unittest/*.c
                       ../ctrl/unittest/*.c
                       ../io/unittest/*.c
                       ../pencil/unittest/*.c
                       ../universal/unittest/*.c
                       ../utf8stringbuf/unittest/*.c)

# ==== ==== ==== ==== ==== ==== ==== ==== C-Code crystal_facet_uml Compiler Settings ==== ==== ==== ==== ==== ==== ==== ====

if(NOT CMAKE_BUILD_TYPE)
  message( "Warn: CMAKE_BUILD_TYPE no set!                 setting(CMAKE_BUILD_TYPE \"Release\")")
  set(CMAKE_BUILD_TYPE "Release")
endif(NOT CMAKE_BUILD_TYPE)
MESSAGE( "Note:                                          CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}" )
MESSAGE( "Note:                                          CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR}" )
MESSAGE( "Note:                                          CMAKE_BUILD_DIR = ${CMAKE_BUILD_DIR}" )
MESSAGE( "Note: to build a release, run                  cmake -DCMAKE_BUILD_TYPE=Release " ${CMAKE_CURRENT_SOURCE_DIR} )
MESSAGE( "Note: to build with asserts and traces, run    cmake -DCMAKE_BUILD_TYPE=Debug " ${CMAKE_CURRENT_SOURCE_DIR} )
MESSAGE( "Note: then, to build using 4 cores, run        make -j4" )
MESSAGE( "Note: to install, run                          sudo make install" )

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -D_XOPEN_SOURCE=700 -O2 -pedantic -g -Wformat -Werror=format-security -Wall -Wno-overlength-strings" )
# Note: The GIMP-generated images are longer that 4095 characters, therefore -Wno-overlength-strings is required here.

LIST(APPEND applicationSource ${commonSource} ${appSource})

# ==== ==== ==== ==== ==== ==== ==== ==== C-Code crystal_facet_uml Linker Settings ==== ==== ==== ==== ==== ==== ==== ====

ADD_EXECUTABLE(crystal_facet_uml ${applicationSource})
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
SET_TARGET_PROPERTIES(unittest_crystal_facet_uml PROPERTIES LINKER_LANGUAGE C)

# ==== ==== ==== ==== ==== ==== ==== ==== Documentation Build Settings ==== ==== ==== ==== ==== ==== ==== ====

MESSAGE( "Note: run 'cd user_doc && make' before running make install")



