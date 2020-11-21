all: all_export_test mouse_droid/mouse_droid.pdf mouse_droid/mouse_droid.xhtml quality/quality.pdf quality/quality.xhtml pencil_tests/pencil_tests.xhtml

mouse_droid/mouse_droid.xhtml: mouse_droid.cfu1
	mkdir -p mouse_droid
	crystal_facet_uml -e mouse_droid.cfu1 xhtml mouse_droid

mouse_droid/mouse_droid.pdf: mouse_droid.cfu1
	mkdir -p mouse_droid
	crystal_facet_uml -e mouse_droid.cfu1 docbook mouse_droid
	[ ! -f /home/pi/Downloads/docbook.xsd ] || xmllint --noout --schema /home/pi/Downloads/docbook.xsd mouse_droid/mouse_droid.xml
	(cd mouse_droid && dblatex mouse_droid.xml)

quality/quality.xhtml: quality.cfu1
	mkdir -p quality
	crystal_facet_uml -e quality.cfu1 xhtml quality

quality/quality.pdf: quality.cfu1
	mkdir -p quality
	crystal_facet_uml -e quality.cfu1 docbook quality
	[ ! -f /home/pi/Downloads/docbook.xsd ] || xmllint --noout --schema /home/pi/Downloads/docbook.xsd quality/quality.xml
	(cd quality && dblatex quality.xml)

pencil_tests/pencil_tests.xhtml: pencil_tests.cfu1
	mkdir -p pencil_tests
	crystal_facet_uml -e pencil_tests.cfu1 xhtml pencil_tests

all_export_test: export_test/export_test_08_block.xmi export_test/export_test_11_component.xmi export_test/export_test_15_activity.xmi export_test/export_test_18_usecase.xmi export_test/export_test_10_constraint.xmi export_test/export_test_12_package.xmi export_test/export_test_16_requirement.xmi export_test/export_test_19_deployment.xmi export_test/export_test_11_class.xmi export_test/export_test_14_state.xmi export_test/export_test_17_sequence.xmi export_test/export_test_17_interaction.xmi

export_test/export_test_08_block.xmi: export_test_08_block.cfu1
	mkdir -p export_test
	crystal_facet_uml -e export_test_08_block.cfu1 xmi export_test

export_test/export_test_11_component.xmi: export_test_11_component.cfu1
	mkdir -p export_test
	crystal_facet_uml -e export_test_11_component.cfu1 xmi export_test

export_test/export_test_15_activity.xmi: export_test_15_activity.cfu1
	mkdir -p export_test
	crystal_facet_uml -e export_test_15_activity.cfu1 xmi export_test

export_test/export_test_18_usecase.xmi: export_test_18_usecase.cfu1
	mkdir -p export_test
	crystal_facet_uml -e export_test_18_usecase.cfu1 xmi export_test

export_test/export_test_10_constraint.xmi: export_test_10_constraint.cfu1
	mkdir -p export_test
	crystal_facet_uml -e export_test_10_constraint.cfu1 xmi export_test

export_test/export_test_12_package.xmi: export_test_12_package.cfu1
	mkdir -p export_test
	crystal_facet_uml -e export_test_12_package.cfu1 xmi export_test

export_test/export_test_16_requirement.xmi: export_test_16_requirement.cfu1
	mkdir -p export_test
	crystal_facet_uml -e export_test_16_requirement.cfu1 xmi export_test

export_test/export_test_19_deployment.xmi: export_test_19_deployment.cfu1
	mkdir -p export_test
	crystal_facet_uml -e export_test_19_deployment.cfu1 xmi export_test

export_test/export_test_11_class.xmi: export_test_11_class.cfu1
	mkdir -p export_test
	crystal_facet_uml -e export_test_11_class.cfu1 xmi export_test

export_test/export_test_14_state.xmi: export_test_14_state.cfu1
	mkdir -p export_test
	crystal_facet_uml -e export_test_14_state.cfu1 xmi export_test

export_test/export_test_17_sequence.xmi: export_test_17_sequence.cfu1
	mkdir -p export_test
	crystal_facet_uml -e export_test_17_sequence.cfu1 xmi export_test

export_test/export_test_17_interaction.xmi: export_test_17_interaction.cfu1
	mkdir -p export_test
	crystal_facet_uml -e export_test_17_interaction.cfu1 xmi export_test

clean:
	rm -fr mouse_droid/ quality/ pencil_tests/ export_test/
