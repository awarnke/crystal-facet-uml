.PHONY: all
all: html crystal-facet-uml_documentation.pdf crystal-facet-uml.1.gz info

SOURCE=../../documentation/user_manual

crystal-facet-uml_documentation.pdf: ${SOURCE}/*.xml ${SOURCE}/2_feature_gallery_export/*.pdf ${SOURCE}/*.svg ${SOURCE}/*.png ${SOURCE}/4_stereotypes/*.* ${SOURCE}/5_modeling_hints_export/*.svg
	echo "you may need to install sudo zypper install rsvg-convert or sudo apt install librsvg2-bin" &&\
	echo "PLEASE UPDATE DIAGRAMS IN ../../user_doc/doc/2_feature_gallery_export/ MANUALLY" &&\
	dblatex -c use_rsvg-convert.conf --type=pdf ${SOURCE}/crystal-facet-uml_documentation.xml --output=./crystal-facet-uml_documentation.pdf

html: ${SOURCE}/*.xml ${SOURCE}/2_feature_gallery_export/*.png ${SOURCE}/*.png ${SOURCE}/*.svg ${SOURCE}/4_stereotypes/*.*
	echo "PLEASE UPDATE DIAGRAMS IN ${SOURCE}/2_feature_gallery_export/ MANUALLY" &&\
	xmlto --stringparam html.stylesheet=html.css -o ./html/ xhtml ${SOURCE}/crystal-facet-uml_documentation.xml &&\
	echo relocate image files &&\
	cat ${SOURCE}/*.xml | grep -e 'fileref.*png' | sed -e 's#^[^"]*"\([^"]*\)".*#${SOURCE}/\1#' | xargs cp --target-directory=./html &&\
	cat ${SOURCE}/*.xml | grep -e 'fileref.*svg' | sed -e 's#^[^"]*"\([^"]*\)".*#${SOURCE}/\1#' | xargs cp --target-directory=./html &&\
	cp ${SOURCE}/html.css ./html &&\
	cp ${SOURCE}/4_stereotypes/*.svg ./html &&\
	find ./html -name '*.html' | xargs sed -i -e 's/\.\.\/\.\.\/gui\/source\/resources\///g' &&\
	find ./html -name '*.html' | xargs sed -i -e 's/2_feature_gallery_export\///g'
	find ./html -name '*.html' | xargs sed -i -e 's/5_modeling_hints_export\///g'

MAN_STYLESHEET_ROOT=`find /usr/share/xml/docbook/stylesheet -name manpages`
#MAN_STYLESHEET_0="/usr/share/xml/docbook/stylesheet/nwalsh/1.78.1/manpages/docbook.xsl"
#MAN_STYLESHEET_1="/usr/share/xml/docbook/stylesheet/suse/manpages/docbook.xsl"
#MAN_STYLESHEET_2="/usr/share/xml/docbook/stylesheet/docbook-xsl/manpages/docbook.xsl"
MANPAGE_FILE="crystal-facet-uml.1"

crystal-facet-uml.1.gz: ${SOURCE}/manpage.xml ${SOURCE}/1_non_tech_intro_para.xml ${SOURCE}/6_command_line_example_para.xml
	rm -f ${MANPAGE_FILE}.gz ${MAPGAPE_FILE}i &&\
	if [ -e "${MAN_STYLESHEET_ROOT}/docbook.xsl" ] ; then\
	  echo "using ${MAN_STYLESHEET_ROOT}/docbook.xsl ...\n" &&\
	  xsltproc --nonet --param man.charmap.use.subset "0" --xinclude "${MAN_STYLESHEET_ROOT}/docbook.xsl" ${SOURCE}/manpage.xml &&\
	  gzip --quiet ${MANPAGE_FILE} ;\
	else \
	  echo "ERROR: find an appropriate xsl file for the manapge at /usr/share/xml/docbook/stylesheet" ;\
	fi &&\
	echo "to view the output, gunzip --keep ${MANPAGE_FILE}.gz && man --local-file ${MANPAGE_FILE}"

.PHONY: info
info:
	@printf "== call \\e[33;1m make install \\e[0m to update the files in folder ../../installation ==\n"

.PHONY: clean
clean:
	rm -f crystal-facet-uml_documentation.pdf &&\
	rm -fr html &&\
	rm -f ${MANPAGE_FILE}.gz

.PHONY: install
install: crystal-facet-uml_documentation.pdf crystal-facet-uml.1.gz
	cp crystal-facet-uml_documentation.pdf ../../installation/ &&\
	cp ${MANPAGE_FILE}.gz ../../installation/linux/


# Copyright 2021-2025 Andreas Warnke
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
