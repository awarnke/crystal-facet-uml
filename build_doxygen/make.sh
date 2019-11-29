#!/bin/sh
. ../main/include/meta/meta_version.inl
VERSIONSTR=${META_VERSION_STR}

echo "== building source code documentation version ${VERSIONSTR} =="
rm -fr ../build_doxygen/doc
(cat Doxyfile; echo "PROJECT_NUMBER=${VERSIONSTR}") | doxygen -
