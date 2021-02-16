#!/bin/sh
. ../../../main/include/meta/meta_version.inl
VERSIONSTR=${META_VERSION_STR}
zip -r crystal-facet-uml_${VERSIONSTR}-1_win32.zip crystal-facet-uml
