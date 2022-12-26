#!/bin/sh
cmake ../.. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j --target crystal-facet-uml --config Release
find . -name '*.rs' | xargs rustfmt
cargo run ./crystal-facet-uml ./tmp ; echo "\nERROR: $?"
echo "consider running rustup update"
echo "view source documentation using cargo doc && chromium-browser target/doc/qualification_test/index.html"
