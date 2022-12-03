#!/bin/sh
cmake ../.. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j --target crystal-facet-uml --config Release
find . -name '*.rs' | xargs rustfmt
cargo run ./crystal-facet-uml ./tmp ; echo "\nERROR: $?"
