all: crystal_facet_uml_user_documentation.pdf crystal_facet_uml_user_documentation

crystal_facet_uml_user_documentation.pdf: doc/*.xml
	cd doc &&\
	dblatex --type=pdf crystal_facet_uml_user_documentation.xml &&\
	cd .. &&\
	mv doc/crystal_facet_uml_user_documentation.pdf .

crystal_facet_uml_user_documentation: doc/*.xml
	mkdir crystal_facet_uml_user_documentation &&\
	cd doc &&\
	pandoc --output=../crystal_facet_uml_user_documentation/index.html --extract-media=../crystal_facet_uml_user_documentation --self-contained --to=html5 --from=docbook crystal_facet_uml_user_documentation.xml
	cd ..

without-images:
	xmlto -o ../crystal_facet_uml_user_documentation/ xhtml-nochunks crystal_facet_uml_user_documentation.xml

clean:
	rm -f crystal_facet_uml_user_documentation.pdf &&\
	rm -fr crystal_facet_uml_user_documentation

