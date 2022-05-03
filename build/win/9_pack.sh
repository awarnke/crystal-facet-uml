#!/bin/sh

PREFIX=`pwd`/root/usr/local
if ! test -e root/usr/local; then
    echo run 3rd_party steps first
    exit -1
fi
. ../../main/include/meta/meta_version.inl
VERSIONSTR=${META_VERSION_STR}
PACK_FILE="crystal-facet-uml_${VERSIONSTR}-1_win64.zip"

echo `date +'%H:%M'`" adding doc files from git repo..."
    cp ../../installation_win/gschemas.compiled ${PREFIX}/share/glib-2.0/schemas/
    crystal-facet-uml -i ${PREFIX}/crystal-facet-uml_mod5g-example.cfu1 add ../../example_diagrams/mouse_droid.json
    cp ../../user_doc/crystal-facet-uml_documentation.pdf ${PREFIX}/
echo "      doc: "`ls ${PREFIX}/crystal-facet-uml_documentation.pdf`

echo `date +'%H:%M'`" packing crystal-facet-uml..."
rm -f ${PACK_FILE}
cd root/usr
    mv local crystal-facet-uml

    zip ../../${PACK_FILE} crystal-facet-uml/bin/*.dll
    zip ../../${PACK_FILE} crystal-facet-uml/bin/glib-compile-schemas.exe
    zip ../../${PACK_FILE} crystal-facet-uml/bin/gdbus.exe
    zip ../../${PACK_FILE} crystal-facet-uml/bin/*crystal-facet-uml*
    zip -r ../../${PACK_FILE} crystal-facet-uml/share/glib-2.0/schemas
    zip ../../${PACK_FILE} crystal-facet-uml/*.*

    mv crystal-facet-uml local
cd ../..
echo "      chk: run zip -Tv ${PACK_FILE}"
echo "      zip: "`ls ${PACK_FILE}`

echo `date +'%H:%M'`" finished."


# Copyright 2022-2022 Andreas Warnke
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
