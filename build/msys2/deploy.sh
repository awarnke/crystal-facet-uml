#!/bin/sh
echo call parameters: \"${0} ${1} ${2}\"
echo called: \"${0} ${1} ${2}\"

. ${1}/main/include/meta/meta_version.inl
VERSIONSTR=${META_VERSION_STR}
echo version: ${VERSIONSTR}

# create a deployment directory
mkdir -p crystal-facet-uml/bin
cp crystal-facet-uml.exe crystal-facet-uml/bin/
cp ${1}/installation/crystal-facet-uml_documentation.pdf crystal-facet-uml/
cp ${1}/installation/win/* crystal-facet-uml/
echo ’ldd crystal-facet-uml.exe | grep -e 'ucrt' | sort’

