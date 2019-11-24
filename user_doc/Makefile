all: crystal_facet_uml_user_documentation.pdf html crystal_facet_uml.1.gz

crystal_facet_uml_user_documentation.pdf: doc/*.xml doc/2_feature_gallery_export/*.pdf doc/*.pdf doc/*.png
	cd doc &&\
	dblatex --type=pdf crystal_facet_uml_user_documentation.xml &&\
	cd .. &&\
	mv doc/crystal_facet_uml_user_documentation.pdf .

html: doc/*.xml doc/2_feature_gallery_export/*.png doc/*.png
	xmlto --stringparam html.stylesheet=html.css -o html/ xhtml doc/crystal_facet_uml_user_documentation.xml &&\
	echo relocate image files &&\
	cd doc &&\
	cat *.xml | grep -e 'fileref.*png' | sed -e 's/^[^"]*"\([^"]*\)".*/\1/' | xargs cp --target-directory=../html &&\
	cp html.css ../html &&\
	cd .. &&\
	find html -name '*.html' | xargs sed -i -e 's/\.\.\/\.\.\/gui\/source\/resources\///g' &&\
	find html -name '*.html' | xargs sed -i -e 's/2_feature_gallery_export\///g'

# daps exists only on opensuse, not on ubuntu
# pandoc is not fit with xinclude and drops the images (at least when i tried to use it in 2018)

MAN_STYLESHEET_0="/usr/share/xml/docbook/stylesheet/nwalsh/1.78.1/manpages/docbook.xsl"
MAN_STYLESHEET_1="/usr/share/xml/docbook/stylesheet/suse/manpages/docbook.xsl"
MAN_STYLESHEET_2="/usr/share/xml/docbook/stylesheet/docbook-xsl/manpages/docbook.xsl"
MANPAGE_FILE="crystal_facet_uml.1"

crystal_facet_uml.1.gz: doc/manpage.xml doc/1_non_tech_intro_para.xml
	rm -f ${MANPAGE_FILE}.gz ${MAPGAPE_FILE}i &&\
	if [ -e ${MAN_STYLESHEET_0} ] ; then\
	  xsltproc --nonet --param man.charmap.use.subset "0" --xinclude ${MAN_STYLESHEET_0} doc/manpage.xml &&\
	  gzip --quiet ${MANPAGE_FILE} ;\
	elif [ -e ${MAN_STYLESHEET_1} ] ; then\
	  xsltproc --nonet --param man.charmap.use.subset "0" --xinclude ${MAN_STYLESHEET_1} doc/manpage.xml &&\
	  gzip --quiet ${MANPAGE_FILE} ;\
	elif [ -e ${MAN_STYLESHEET_2} ] ; then\
	  xsltproc --nonet --param man.charmap.use.subset "0" --xinclude ${MAN_STYLESHEET_2} doc/manpage.xml &&\
	  gzip --quiet ${MANPAGE_FILE} ;\
	else \
	  echo "ERROR: find an appropriate xsl file for the manapge at /usr/share/xml/docbook/stylesheet" ;\
	fi &&\
        echo "to view the output, gunzip --keep ${MANPAGE_FILE}.gz && man --local-file ${MANPAGE_FILE}"

clean:
	rm -f crystal_facet_uml_user_documentation.pdf &&\
	rm -f crystal_facet_uml_installation_win.pdf &&\
	rm -fr html &&\
	rm -f ${MANPAGE_FILE}.gz

