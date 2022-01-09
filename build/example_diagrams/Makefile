.PHONY: all
all: all_export_test mouse_droid/mouse_droid.pdf mouse_droid/mouse_droid.xhtml quality/quality.pdf quality/quality.xhtml self_architecture/self_architecture.pdf self_architecture/self_architecture.xhtml pencil_tests/pencil_tests.xhtml info

ARCDIR=../../architecture/doc
SRCDIR=../../example_diagrams
INSTALLDIR=../../example_diagrams

mouse_droid/mouse_droid.xhtml: $(SRCDIR)/mouse_droid.cfu1
	mkdir -p mouse_droid
	crystal-facet-uml -e $(SRCDIR)/mouse_droid.cfu1 xhtml mouse_droid

mouse_droid/mouse_droid.pdf: $(SRCDIR)/mouse_droid.cfu1
	mkdir -p mouse_droid
	crystal-facet-uml -e $(SRCDIR)/mouse_droid.cfu1 docbook mouse_droid
	[ ! -f /home/pi/Downloads/docbook.xsd ] || xmllint --noout --schema /home/pi/Downloads/docbook.xsd mouse_droid/mouse_droid.xml
	(cd mouse_droid && dblatex mouse_droid.xml)

quality/quality.cfu1: $(SRCDIR)/quality.json
	mkdir -p quality
	crystal-facet-uml -i quality/quality.cfu1 add $(SRCDIR)/quality.json

quality/quality.xhtml: quality/quality.cfu1
	crystal-facet-uml -e quality/quality.cfu1 xhtml quality

quality/quality.pdf: quality/quality.cfu1
	crystal-facet-uml -e quality/quality.cfu1 docbook quality
	[ ! -f /home/pi/Downloads/docbook.xsd ] || xmllint --noout --schema /home/pi/Downloads/docbook.xsd quality/quality.xml
	(cd quality && dblatex quality.xml)

self_architecture/self_architecture.cfu1: $(ARCDIR)/self_architecture.json
	mkdir -p self_architecture
	crystal-facet-uml -i self_architecture/self_architecture.cfu1 add $(ARCDIR)/self_architecture.json

self_architecture/self_architecture.xhtml: self_architecture/self_architecture.cfu1
	crystal-facet-uml -e self_architecture/self_architecture.cfu1 xhtml self_architecture

self_architecture/self_architecture.pdf: self_architecture/self_architecture.cfu1
	crystal-facet-uml -e self_architecture/self_architecture.cfu1 docbook self_architecture
	[ ! -f /home/pi/Downloads/docbook.xsd ] || xmllint --noout --schema /home/pi/Downloads/docbook.xsd self_architecture/self_architecture
	(cd self_architecture && dblatex self_architecture.xml)

pencil_tests/pencil_tests.xhtml: $(SRCDIR)/pencil_tests.cfu1
	mkdir -p pencil_tests
	crystal-facet-uml -e $(SRCDIR)/pencil_tests.cfu1 xhtml pencil_tests

.PHONY: all_export_test
all_export_test: export_test/export_test_08_block.xmi export_test/export_test_11_component.xmi export_test/export_test_15_activity.xmi export_test/export_test_18_usecase.xmi export_test/export_test_10_constraint.xmi export_test/export_test_12_package.xmi export_test/export_test_16_requirement.xmi export_test/export_test_19_deployment.xmi export_test/export_test_11_class.xmi export_test/export_test_14_state.xmi export_test/export_test_17_sequence.xmi export_test/export_test_17_interaction.xmi

export_test/export_test_08_block.xmi: $(SRCDIR)/export_test_08_block.cfu1
	mkdir -p export_test
	crystal-facet-uml -e $(SRCDIR)/export_test_08_block.cfu1 xmi export_test

export_test/export_test_11_component.xmi: $(SRCDIR)/export_test_11_component.cfu1
	mkdir -p export_test
	crystal-facet-uml -e $(SRCDIR)/export_test_11_component.cfu1 xmi export_test

export_test/export_test_15_activity.xmi: $(SRCDIR)/export_test_15_activity.cfu1
	mkdir -p export_test
	crystal-facet-uml -e $(SRCDIR)/export_test_15_activity.cfu1 xmi export_test

export_test/export_test_18_usecase.xmi: $(SRCDIR)/export_test_18_usecase.cfu1
	mkdir -p export_test
	crystal-facet-uml -e $(SRCDIR)/export_test_18_usecase.cfu1 xmi export_test

export_test/export_test_10_constraint.xmi: $(SRCDIR)/export_test_10_constraint.cfu1
	mkdir -p export_test
	crystal-facet-uml -e $(SRCDIR)/export_test_10_constraint.cfu1 xmi export_test

export_test/export_test_12_package.xmi: $(SRCDIR)/export_test_12_package.cfu1
	mkdir -p export_test
	crystal-facet-uml -e $(SRCDIR)/export_test_12_package.cfu1 xmi export_test

export_test/export_test_16_requirement.xmi: $(SRCDIR)/export_test_16_requirement.cfu1
	mkdir -p export_test
	crystal-facet-uml -e $(SRCDIR)/export_test_16_requirement.cfu1 xmi export_test

export_test/export_test_19_deployment.xmi: $(SRCDIR)/export_test_19_deployment.cfu1
	mkdir -p export_test
	crystal-facet-uml -e $(SRCDIR)/export_test_19_deployment.cfu1 xmi export_test

export_test/export_test_11_class.xmi: $(SRCDIR)/export_test_11_class.cfu1
	mkdir -p export_test
	crystal-facet-uml -e $(SRCDIR)/export_test_11_class.cfu1 xmi export_test

export_test/export_test_14_state.xmi: $(SRCDIR)/export_test_14_state.cfu1
	mkdir -p export_test
	crystal-facet-uml -e $(SRCDIR)/export_test_14_state.cfu1 xmi export_test

export_test/export_test_17_sequence.xmi: $(SRCDIR)/export_test_17_sequence.cfu1
	mkdir -p export_test
	crystal-facet-uml -e $(SRCDIR)/export_test_17_sequence.cfu1 xmi export_test

export_test/export_test_17_interaction.xmi: $(SRCDIR)/export_test_17_interaction.cfu1
	mkdir -p export_test
	crystal-facet-uml -e $(SRCDIR)/export_test_17_interaction.cfu1 xmi export_test

.PHONY: info
info:
	@printf "== call \\e[33;1m make install \\e[0m to update the files in folder ../../example_diagrams ==\n"

.PHONY: install
install: all_export_test
	cp -fr export_test $(INSTALLDIR)/

.PHONY: clean
clean:
	rm -fr mouse_droid quality pencil_tests export_test self_architecture
