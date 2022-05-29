#!/bin/sh
cmake ../..
cmake --build . --target crystal-facet-uml --config Release
cargo run ./crystal-facet-uml ; echo "\nERROR: $?"
