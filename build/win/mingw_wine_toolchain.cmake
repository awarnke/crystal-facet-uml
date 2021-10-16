# call cmake providing this toolchain file:
# cmake -DCMAKE_TOOLCHAIN_FILE=../crystal-facet-uml/build_win/mingw_wine_toolchain.cmake ../crystal-facet-uml/build_win

SET(CMAKE_SYSTEM_NAME "Windows")
SET(CMAKE_C_COMPILER "/usr/bin/x86_64-w64-mingw32-gcc")
SET(CMAKE_CXX_COMPILER "/usr/bin/x86_64-w64-mingw32-g++")
SET(CMAKE_RC_COMPILER "/usr/bin/x86_64-w64-mingw32-windres")
