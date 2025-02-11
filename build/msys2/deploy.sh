#!/bin/sh
echo call parameters: \"SOURCE_DIR BUILD_DIR\"
echo SELF:       \"${0}\"
echo SOURCE_DIR: \"${1}\"
echo BUILD_DIR:  \"${2}\"

. ${1}/main/include/meta/meta_version.inl
VERSIONSTR=${META_VERSION_STR}
echo version: ${VERSIONSTR}

# create a deployment directory
mkdir -p crystal-facet-uml
cp ${1}/installation/crystal-facet-uml_documentation.pdf crystal-facet-uml/
cp ${1}/installation/win/* crystal-facet-uml/
cp ${1}/documentation/examples/mouse_droid.cfuJ crystal-facet-uml/crystal-facet-uml_mod5g-example.cfuJ
cp ${1}/LICENSE.txt crystal-facet-uml/crystal-facet-uml_LICENSE.txt
${1}/build/msys2/fetch_licenses.sh | tee crystal-facet-uml/LICENSES_of_3rd_party.txt

mkdir -p crystal-facet-uml/bin
cp crystal-facet-uml.exe crystal-facet-uml/bin/
cp test_crystal-facet-uml.exe crystal-facet-uml/bin/
ldd crystal-facet-uml.exe | grep -e ' /ucrt64' | sed -e 's/^[^/]*\(\S*\).*$/\1/' | xargs cp -t crystal-facet-uml/bin/
cp /ucrt64/bin/gdbus.exe crystal-facet-uml/bin/

mkdir -p crystal-facet-uml/share/glib-2.0/schemas
cp /ucrt64/share/glib-2.0/schemas/gschemas.compiled crystal-facet-uml/share/glib-2.0/schemas/

# zip
7z a crystal-facet-uml_${VERSIONSTR}-1_win64.zip crystal-facet-uml
