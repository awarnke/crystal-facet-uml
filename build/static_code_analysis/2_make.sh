#!/bin/sh
SOURCE_ROOT=../..
. ${SOURCE_ROOT}/main/include/meta/meta_version.inl
VERSIONSTR=${META_VERSION_STR}
[ -e Makefile ] && make clean
cmake -DCFU_USE_GTK4=ON ${SOURCE_ROOT}
cov-build --dir cov-int make -j4 crystal-facet-uml
tar czvf crystal-facet-uml_${VERSIONSTR}.tgz cov-int
