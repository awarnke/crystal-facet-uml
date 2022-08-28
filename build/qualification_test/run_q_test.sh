#!/bin/sh
cmake ../.. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j --target crystal-facet-uml --config Release
cargo run ./crystal-facet-uml ; echo "\nERROR: $?"
