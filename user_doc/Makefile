all: crystal_facet_uml_user_documentation.pdf html

crystal_facet_uml_user_documentation.pdf: doc/*.xml
	cd doc &&\
	dblatex --type=pdf crystal_facet_uml_user_documentation.xml &&\
	cd .. &&\
	mv doc/crystal_facet_uml_user_documentation.pdf .

html: doc/*.xml
	mkdir -p html &&\
	xmlto -o html/ xhtml doc/crystal_facet_uml_user_documentation.xml &&\
	echo relocate image files &&\
	cd doc &&\
	cat *.xml | grep -e 'fileref.*png' | sed -e 's/^[^"]*"\([^"]*\)".*/\1/' | xargs cp --target-directory=../html &&\
	cd .. &&\
	find html -name '*.html' | xargs sed -i -e 's/\.\.\/\.\.\/gui\/source\/resources\///g'

# daps exists only on opensuse, not on ubuntu
# pandoc drops the images (at least when i tried to use it)

does-not-work:
	pandoc --output=../crystal_facet_uml_user_documentation/index.html --extract-media=../crystal_facet_uml_user_documentation --self-contained --to=html5 --from=docbook crystal_facet_uml_user_documentation.xml

clean:
	rm -f crystal_facet_uml_user_documentation.pdf &&\
	rm -fr html

