#!/bin/sh
echo "== building sourccode doc =="
rm -fr ../doxygen_build/doc
doxygen Doxyfile
