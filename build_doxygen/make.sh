#!/bin/sh
echo "== building source code documentation =="
rm -fr ../build_doxygen/doc
doxygen Doxyfile
