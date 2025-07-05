CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
PROJECT(crystal-facet-uml LANGUAGES C)

# pkgconfig is a helper program to find include directories and libraries:
FIND_PACKAGE(PkgConfig REQUIRED)

# ==== ==== ==== ==== ==== ==== ==== ==== Build Environment Settings ==== ==== ==== ==== ==== ==== ==== ====

SET(COMPILE_IN_INCLUDE_DIR "" CACHE FILEPATH "Path to additional include directory")
SET(COMPILE_IN_SOURCE_FILE "" CACHE FILEPATH "Path to additional source file, e.g. sqlite3.c")
SET(COMPILE_IN_LINKER_FLAG "" CACHE FILEPATH "Additional linker flag, e.g. win_app_icon.res")
INCLUDE_DIRECTORIES(${COMPILE_IN_INCLUDE_DIR})

# crystal-facet-uml

INCLUDE_DIRECTORIES(gui/include)
INCLUDE_DIRECTORIES(main/include)
INCLUDE_DIRECTORIES(ctrl/include)
INCLUDE_DIRECTORIES(data/include)
INCLUDE_DIRECTORIES(pencil/include)
INCLUDE_DIRECTORIES(io/include)
INCLUDE_DIRECTORIES(u8stream/include)

#sqlite3 dependencies

PKG_SEARCH_MODULE(SQLITE3 REQUIRED sqlite3)
INCLUDE_DIRECTORIES(${SQLITE3_INCLUDE_DIRS})
#MESSAGE("SQLITE3: ${SQLITE3_INCLUDE_DIRS}")

#gtk dependencies
#Note: PKG_SEARCH_MODULE finds only the first successful match
PKG_CHECK_MODULES(GLIB2 REQUIRED glib-2.0)
INCLUDE_DIRECTORIES(${GLIB2_INCLUDE_DIRS})
MESSAGE("GLIB2:   ${GLIB2_INCLUDE_DIRS}")
MESSAGE("GLIB2:   ${GLIB2_LIBRARY_DIRS}")
MESSAGE("GLIB2:   ${GLIB2_CFLAGS}")

# for DEBUG mode, some headers may not be found automatically:
#EXECUTE_PROCESS(COMMAND pkg-config --cflags glib-2.0
#                OUTPUT_VARIABLE GLIB_MORE_CFLAGS)
#SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GLIB_MORE_CFLAGS}" )
#MESSAGE("GLIB_MORE_CFLAGS:   ${GLIB_MORE_CFLAGS}")

#search gtk4
# Note: PKG_CHECK_MODULES searched for all occurerences of the package
PKG_CHECK_MODULES(GTK REQUIRED gtk4)
INCLUDE_DIRECTORIES(${GTK_INCLUDE_DIRS})
MESSAGE("GTK:    ${GTK_INCLUDE_DIRS}")
MESSAGE("GTK:    ${GTK_FOUND}")
MESSAGE("GTK:    ${GTK_LIBRARIES}")
MESSAGE("GTK:    ${GTK_LIBRARY_DIRS}")
MESSAGE("GTK:    ${GTK_LDFLAGS}")
MESSAGE("GTK:    ${GTK_LDFLAGS_OTHER}")
MESSAGE("GTK:    ${GTK_CFLAGS}")
MESSAGE("GTK:    ${GTK_CFLAGS_OTHER}")

# for DEBUG mode, some headers may not be found automatically:
#EXECUTE_PROCESS(COMMAND pkg-config --cflags graphene-1.0
#                OUTPUT_VARIABLE GRAPHENE_MORE_CFLAGS)
#SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GRAPHENE_MORE_CFLAGS}" )
#MESSAGE("GRAPHENE_MORE_CFLAGS:   ${GRAPHENE_MORE_CFLAGS}")

PKG_SEARCH_MODULE(PANGOCAIRO REQUIRED pangocairo)
INCLUDE_DIRECTORIES(${PANGOCAIRO_INCLUDE_DIRS})
#MESSAGE("PANGOCAIRO: ${PANGOCAIRO_INCLUDE_DIRS}")

FILE(GLOB commonSource gui/source/*.c
                       gui/source/attributes/*.c
                       gui/source/sketch/*.c
                       ctrl/source/*.c
                       ctrl/source/consistency/*.c
                       data/source/*.c
                       data/source/entity/*.c
                       data/source/storage/*.c
                       data/source/set/*.c
                       u8stream/source/utf8stringbuf/*.c
                       u8stream/source/u8dir/*.c
                       u8stream/source/u8stream/*.c
                       u8stream/source/u8/*.c
                       pencil/source/*.c
                       pencil/source/geometry/*.c
                       pencil/source/layout/*.c
                       pencil/source/draw/*.c
                       io/source/*.c
                       io/source/image/*.c
                       io/source/json/*.c
                       io/source/document/*.c
                       io/source/xmi/*.c
                       io/source/format/*.c
                       main/source/main_commands.c
                       main/source/main_execute.c)
FILE(GLOB appSource    main/source/main.c)
FILE(GLOB utestSource  main/test/*.c
                       data/test/unit/*.c
                       data/test/integration/*.c
                       ctrl/test/unit/*.c
                       ctrl/test/integration/*.c
                       gui/test/unit/*.c
                       io/test/unit/*.c
                       io/test/integration/*.c
                       pencil/test/unit/*.c
                       pencil/test/test_data/*.c
                       pencil/test/integration/*.c
                       u8stream/test/unit/*.c
                       u8stream/test/integration/*.c)

# ==== ==== ==== ==== ==== ==== ==== ==== Check if CMAKE_BUILD_TYPE is set ==== ==== ==== ==== ==== ==== ==== ====

IF(NOT CMAKE_BUILD_TYPE)
  MESSAGE( "Warn: CMAKE_BUILD_TYPE no set!                 setting(CMAKE_BUILD_TYPE \"Release\")")
  SET(CMAKE_BUILD_TYPE "Release")
ENDIF(NOT CMAKE_BUILD_TYPE)
MESSAGE( "Note:                                          CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}" )
MESSAGE( "Note: to build a release, run                  cmake -DCMAKE_BUILD_TYPE=Release " ${CMAKE_CURRENT_SOURCE_DIR} )
MESSAGE( "Note: to build with asserts and traces, run    cmake -DCMAKE_BUILD_TYPE=Debug " ${CMAKE_CURRENT_SOURCE_DIR} )

# ==== ==== ==== ==== ==== ==== ==== ==== Common Settings ==== ==== ==== ==== ==== ==== ==== ====

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -D_XOPEN_SOURCE=700 -O2 -pedantic -g -Wall" )

# ==== ==== ==== ==== ==== ==== ==== ==== EXE Target Settings ==== ==== ==== ==== ==== ==== ==== ====

ADD_EXECUTABLE(crystal-facet-uml ${appSource} ${commonSource} ${COMPILE_IN_SOURCE_FILE})
TARGET_LINK_LIBRARIES(crystal-facet-uml ${GTK_LIBRARIES} ${GLIB2_LIBRARIES} ${PANGOCAIRO_LIBRARIES} ${SQLITE3_LIBRARIES} m)
SET_TARGET_PROPERTIES(crystal-facet-uml PROPERTIES LINKER_LANGUAGE C)
SET_TARGET_PROPERTIES(crystal-facet-uml PROPERTIES LINK_FLAGS "${COMPILE_IN_LINKER_FLAG}")

# ==== ==== ==== ==== ==== ==== ==== ==== test Target Settings ==== ==== ==== ==== ==== ==== ==== ====

ADD_EXECUTABLE(test_crystal-facet-uml ${utestSource} ${commonSource} ${COMPILE_IN_SOURCE_FILE})
TARGET_INCLUDE_DIRECTORIES(test_crystal-facet-uml PUBLIC test_fw/include)
TARGET_INCLUDE_DIRECTORIES(test_crystal-facet-uml PUBLIC data/test)
TARGET_INCLUDE_DIRECTORIES(test_crystal-facet-uml PUBLIC ctrl/test)
TARGET_INCLUDE_DIRECTORIES(test_crystal-facet-uml PUBLIC gui/test)
TARGET_INCLUDE_DIRECTORIES(test_crystal-facet-uml PUBLIC io/test)
TARGET_INCLUDE_DIRECTORIES(test_crystal-facet-uml PUBLIC pencil/test)
TARGET_INCLUDE_DIRECTORIES(test_crystal-facet-uml PUBLIC u8stream/test)
TARGET_LINK_LIBRARIES(test_crystal-facet-uml ${GTK_LIBRARIES} ${GLIB2_LIBRARIES} ${PANGOCAIRO_LIBRARIES} ${SQLITE3_LIBRARIES} m)
SET_TARGET_PROPERTIES(test_crystal-facet-uml PROPERTIES LINKER_LANGUAGE C)
SET_TARGET_PROPERTIES(test_crystal-facet-uml PROPERTIES LINK_FLAGS "${COMPILE_IN_LINKER_FLAG}")
SET_TARGET_PROPERTIES(test_crystal-facet-uml PROPERTIES COMPILE_FLAGS "-Wno-overlength-strings")

# ==== ==== ==== ==== ==== ==== ==== ==== Install Settings ==== ==== ==== ==== ==== ==== ==== ====

INSTALL(TARGETS crystal-facet-uml DESTINATION "/usr/bin/")
INSTALL(FILES "${CMAKE_CURRENT_SOURCE_DIR}/installation/linux/crystal-facet-uml.desktop" DESTINATION "/usr/share/applications/")
INSTALL(FILES "${CMAKE_CURRENT_SOURCE_DIR}/installation/linux/crystal-facet-uml.metainfo.xml" DESTINATION "/usr/share/metainfo/")
INSTALL(FILES "${CMAKE_CURRENT_SOURCE_DIR}/gui/source/resources/crystal_facet_uml_80x80.png" DESTINATION "/usr/share/pixmaps/" RENAME "crystal-facet-uml.png")
INSTALL(FILES "${CMAKE_CURRENT_SOURCE_DIR}/installation/linux/crystal-facet-uml.1.gz" DESTINATION "/usr/share/man/man1/")

# ==== ==== ==== ==== ==== ==== ==== ==== gcov Target Settings ==== ==== ==== ==== ==== ==== ==== ====

if(CFU_ADD_GCOV_TARGET)
    if(CMAKE_COMPILER_IS_GNUCC)
        ADD_EXECUTABLE(gcov_crystal-facet-uml ${utestSource} ${commonSource} ${COMPILE_IN_SOURCE_FILE})
        TARGET_INCLUDE_DIRECTORIES(gcov_crystal-facet-uml PUBLIC test_fw/include)
        TARGET_INCLUDE_DIRECTORIES(gcov_crystal-facet-uml PUBLIC data/test)
        TARGET_INCLUDE_DIRECTORIES(gcov_crystal-facet-uml PUBLIC ctrl/test)
        TARGET_INCLUDE_DIRECTORIES(gcov_crystal-facet-uml PUBLIC gui/test)
        TARGET_INCLUDE_DIRECTORIES(gcov_crystal-facet-uml PUBLIC io/test)
        TARGET_INCLUDE_DIRECTORIES(gcov_crystal-facet-uml PUBLIC pencil/test)
        TARGET_INCLUDE_DIRECTORIES(gcov_crystal-facet-uml PUBLIC u8stream/test)
        TARGET_LINK_LIBRARIES(gcov_crystal-facet-uml ${GTK_LIBRARIES} ${GLIB2_LIBRARIES} ${PANGOCAIRO_LIBRARIES} ${SQLITE3_LIBRARIES} m)
        SET_TARGET_PROPERTIES(gcov_crystal-facet-uml PROPERTIES LINKER_LANGUAGE C)
        SET_TARGET_PROPERTIES(gcov_crystal-facet-uml PROPERTIES EXCLUDE_FROM_ALL TRUE)
        target_compile_options(gcov_crystal-facet-uml -Wno-overlength-strings -fprofile-arcs -ftest-coverage -O0)
        target_link_options(gcov_crystal-facet-uml -fprofile-arc)
    endif(CMAKE_COMPILER_IS_GNUCC)
endif(CFU_ADD_GCOV_TARGET)

# ==== ==== ==== ==== ==== ==== ==== ==== run the msys2 deploy script ==== ==== ==== ==== ==== ==== ==== ====

ADD_CUSTOM_TARGET(msys2_deploy
                  ${CMAKE_CURRENT_SOURCE_DIR}/build/msys2/deploy.sh
                  ${CMAKE_CURRENT_SOURCE_DIR}
                  ${CMAKE_CURRENT_BINARY_DIR})


# Copyright 2018-2025 Andreas Warnke
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
