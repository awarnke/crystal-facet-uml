
# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

all: build/compile_commands.json
.PHONY : all

build/compile_commands.json: ../../CMakeLists.txt
	echo "REPLACE -O2 by -O0 in ../../CMakeLists.txt first"
	mkdir -p build
	(cd build && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ../../..)

