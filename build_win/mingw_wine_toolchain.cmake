# call cmake:
# cmake -DCMAKE_TOOLCHAIN_FILE=../build_win/mingw_wine_toolchain.cmake ../build_win

SET(CMAKE_C_COMPILER "/usr/bin/i686-w64-mingw32-gcc")
SET(CMAKE_CXX_COMPILER "/usr/bin/i686-w64-mingw32-g++")
SET(CMAKE_SYSTEM_NAME "Windows")
#include(CMakeForceCompiler)
#IF("${GNU_HOST}" STREQUAL "")
#    SET(GNU_HOST i686-w64-mingw32)
#ENDIF()
# Prefix detection only works with compiler id "GNU"
#CMAKE_FORCE_C_COMPILER(i686-w64-mingw32-gcc GNU)
# manually define 'windres':
SET(CMAKE_RC_COMPILER "/usr/bin/i686-w64-mingw32-windres")
