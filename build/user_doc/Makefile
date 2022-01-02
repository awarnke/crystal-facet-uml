.PHONY: all
all: crystal-facet-uml_documentation.pdf html crystal-facet-uml.1.gz info

SOURCE="../../user_doc"

crystal-facet-uml_documentation.pdf: ../../user_doc/doc/*.xml ../../user_doc/doc/2_feature_gallery_export/*.pdf ../../user_doc/doc/*.pdf ../../user_doc/doc/*.png
	dblatex --type=pdf ${SOURCE}/doc/crystal-facet-uml_documentation.xml --output=./crystal-facet-uml_documentation.pdf

html: ../../user_doc/doc/*.xml ../../user_doc/doc/2_feature_gallery_export/*.png ../../user_doc/doc/*.png
	xmlto --stringparam html.stylesheet=html.css -o ./html/ xhtml ${SOURCE}/doc/crystal-facet-uml_documentation.xml &&\
	echo relocate image files &&\
	cat ${SOURCE}/doc/*.xml | grep -e 'fileref.*png' | sed -e 's/^[^"]*"\([^"]*\)".*/..\/..\/user_doc\/doc\/\1/' | xargs cp --target-directory=./html &&\
	cp ${SOURCE}/doc/html.css ./html &&\
	find ./html -name '*.html' | xargs sed -i -e 's/\.\.\/\.\.\/gui\/source\/resources\///g' &&\
	find ./html -name '*.html' | xargs sed -i -e 's/2_feature_gallery_export\///g'

# xmlto chosen:
# daps exists only on opensuse, not on ubuntu
# pandoc is not fit with xinclude and drops the images (at least when i tried to use it in 2018)

MAN_STYLESHEET_ROOT=`find /usr/share/xml/docbook/stylesheet -name manpages`
#MAN_STYLESHEET_0="/usr/share/xml/docbook/stylesheet/nwalsh/1.78.1/manpages/docbook.xsl"
#MAN_STYLESHEET_1="/usr/share/xml/docbook/stylesheet/suse/manpages/docbook.xsl"
#MAN_STYLESHEET_2="/usr/share/xml/docbook/stylesheet/docbook-xsl/manpages/docbook.xsl"
MANPAGE_FILE="crystal-facet-uml.1"

crystal-facet-uml.1.gz: ../../user_doc/doc/manpage.xml ../../user_doc/doc/1_non_tech_intro_para.xml ../../user_doc/doc/3_command_line_example_para.xml
	rm -f ${MANPAGE_FILE}.gz ${MAPGAPE_FILE}i &&\
	if [ -e "${MAN_STYLESHEET_ROOT}/docbook.xsl" ] ; then\
	  echo "using ${MAN_STYLESHEET_ROOT}/docbook.xsl ...\n" &&\
	  xsltproc --nonet --param man.charmap.use.subset "0" --xinclude "${MAN_STYLESHEET_ROOT}/docbook.xsl" ${SOURCE}/doc/manpage.xml &&\
	  gzip --quiet ${MANPAGE_FILE} ;\
	else \
	  echo "ERROR: find an appropriate xsl file for the manapge at /usr/share/xml/docbook/stylesheet" ;\
	fi &&\
	echo "to view the output, gunzip --keep ${MANPAGE_FILE}.gz && man --local-file ${MANPAGE_FILE}"

.PHONY: info
info:
	@printf "== call \\e[33;1m make install \\e[0m to update the files in folder ../../user_doc ==\n"

.PHONY: clean
clean:
	rm -f crystal-facet-uml_documentation.pdf &&\
	rm -fr html &&\
	rm -f ${MANPAGE_FILE}.gz

.PHONY: install
install: crystal-facet-uml_documentation.pdf crystal-facet-uml.1.gz
	cp crystal-facet-uml_documentation.pdf ../../user_doc/ &&\
	cp ${MANPAGE_FILE}.gz ../../user_doc/
