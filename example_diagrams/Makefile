all: mouse_droid/mouse_droid.pdf mouse_droid/mouse_droid.xhtml quality/quality.pdf quality/quality.xhtml pencil_tests/pencil_tests.xhtml

mouse_droid/mouse_droid.xhtml: mouse_droid.cfu1
	mkdir -p mouse_droid
	crystal_facet_uml -e mouse_droid.cfu1 xhtml mouse_droid

mouse_droid/mouse_droid.pdf: mouse_droid.cfu1
	mkdir -p mouse_droid
	crystal_facet_uml -e mouse_droid.cfu1 docbook mouse_droid
	cd mouse_droid && dblatex mouse_droid.xml && cd ..

quality/quality.xhtml: quality.cfu1
	mkdir -p quality
	crystal_facet_uml -e quality.cfu1 xhtml quality

quality/quality.pdf: quality.cfu1
	mkdir -p quality
	crystal_facet_uml -e quality.cfu1 docbook quality
	cd quality && dblatex quality.xml && cd ..

pencil_tests/pencil_tests.xhtml: pencil_tests.cfu1
	mkdir -p pencil_tests
	crystal_facet_uml -e pencil_tests.cfu1 xhtml pencil_tests

clean:
	rm -fr mouse_droid/ quality/ pencil_tests/
