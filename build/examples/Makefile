.PHONY: all
all: all_export_test mouse_droid/mouse_droid.pdf mouse_droid/mouse_droid.html quality/quality.pdf quality/quality.html self_architecture/self_architecture.pdf self_architecture/self_architecture.html pencil_tests/pencil_tests.html info

ARCDIR=../../documentation/architecture
SRCDIR=../../documentation/examples
INSTALLDIR=../../documentation/examples

mouse_droid/mouse_droid.html: $(SRCDIR)/mouse_droid.cfuJ
	mkdir -p mouse_droid
	crystal-facet-uml -e $(SRCDIR)/mouse_droid.cfuJ html mouse_droid

mouse_droid/mouse_droid.pdf: $(SRCDIR)/mouse_droid.cfuJ
	mkdir -p mouse_droid
	crystal-facet-uml -e $(SRCDIR)/mouse_droid.cfuJ docbook mouse_droid
	[ ! -f /home/andi/Downloads/docbook.xsd ] || xmllint --noout --schema /home/andi/Downloads/docbook.xsd mouse_droid/mouse_droid.xml
	(cd mouse_droid && dblatex -p ../xsl_params.xsl --type=pdf mouse_droid.xml)

quality/quality.html: $(SRCDIR)/quality.cfuJ
	mkdir -p quality
	crystal-facet-uml -e $(SRCDIR)/quality.cfuJ html quality

quality/quality.pdf: $(SRCDIR)/quality.cfuJ
	mkdir -p quality
	crystal-facet-uml -e $(SRCDIR)/quality.cfuJ docbook quality
	[ ! -f /home/andi/Downloads/docbook.xsd ] || xmllint --noout --schema /home/andi/Downloads/docbook.xsd quality/quality.xml
	(cd quality && dblatex -p ../xsl_params.xsl --type=pdf quality.xml)

self_architecture/self_architecture.html: $(ARCDIR)/self_architecture.cfuJ
	mkdir -p self_architecture
	crystal-facet-uml -e $(ARCDIR)/self_architecture.cfuJ html self_architecture

self_architecture/self_architecture.pdf: $(ARCDIR)/self_architecture.cfuJ
	mkdir -p self_architecture
	crystal-facet-uml -e $(ARCDIR)/self_architecture.cfuJ docbook self_architecture
	[ ! -f /home/andi/Downloads/docbook.xsd ] || xmllint --noout --schema /home/andi/Downloads/docbook.xsd self_architecture/self_architecture.xml
	(cd self_architecture && dblatex -p ../xsl_params.xsl --type=pdf self_architecture.xml)

pencil_tests/pencil_tests.html: $(SRCDIR)/pencil_tests.cfuJ
	mkdir -p pencil_tests
	crystal-facet-uml -e $(SRCDIR)/pencil_tests.cfuJ html pencil_tests

.PHONY: all_export_test
all_export_test: export_test/export_test_08_block.xmi export_test/export_test_10_constraint.xmi export_test/export_test_11_class.xmi export_test/export_test_11_component.xmi export_test/ export_test/export_test_12_package.xmi export_test/export_test_12_profile.xmi export_test/export_test_14_state.xmi export_test/export_test_15_activity.xmi export_test/export_test_16_requirement.xmi export_test/export_test_17_sequence.xmi export_test/export_test_17_interaction.xmi export_test/export_test_18_usecase.xmi export_test/export_test_19_deployment.xmi

export_test/export_test_08_block.xmi: $(SRCDIR)/export_test_08_block.cfuJ
	mkdir -p export_test
	crystal-facet-uml -e $(SRCDIR)/export_test_08_block.cfuJ xmi export_test

export_test/export_test_11_class.xmi: $(SRCDIR)/export_test_11_class.cfuJ
	mkdir -p export_test
	crystal-facet-uml -e $(SRCDIR)/export_test_11_class.cfuJ xmi export_test

export_test/export_test_10_constraint.xmi: $(SRCDIR)/export_test_10_constraint.cfuJ
	mkdir -p export_test
	crystal-facet-uml -e $(SRCDIR)/export_test_10_constraint.cfuJ xmi export_test

export_test/export_test_11_component.xmi: $(SRCDIR)/export_test_11_component.cfuJ
	mkdir -p export_test
	crystal-facet-uml -e $(SRCDIR)/export_test_11_component.cfuJ xmi export_test

export_test/export_test_12_package.xmi: $(SRCDIR)/export_test_12_package.cfuJ
	mkdir -p export_test
	crystal-facet-uml -e $(SRCDIR)/export_test_12_package.cfuJ xmi export_test

export_test/export_test_12_profile.xmi: $(SRCDIR)/export_test_12_profile.cfuJ
	mkdir -p export_test
	crystal-facet-uml -e $(SRCDIR)/export_test_12_profile.cfuJ xmi export_test

export_test/export_test_14_state.xmi: $(SRCDIR)/export_test_14_state.cfuJ
	mkdir -p export_test
	crystal-facet-uml -e $(SRCDIR)/export_test_14_state.cfuJ xmi export_test

export_test/export_test_15_activity.xmi: $(SRCDIR)/export_test_15_activity.cfuJ
	mkdir -p export_test
	crystal-facet-uml -e $(SRCDIR)/export_test_15_activity.cfuJ xmi export_test

export_test/export_test_16_requirement.xmi: $(SRCDIR)/export_test_16_requirement.cfuJ
	mkdir -p export_test
	crystal-facet-uml -e $(SRCDIR)/export_test_16_requirement.cfuJ xmi export_test

export_test/export_test_17_sequence.xmi: $(SRCDIR)/export_test_17_sequence.cfuJ
	mkdir -p export_test
	crystal-facet-uml -e $(SRCDIR)/export_test_17_sequence.cfuJ xmi export_test

export_test/export_test_17_interaction.xmi: $(SRCDIR)/export_test_17_interaction.cfuJ
	mkdir -p export_test
	crystal-facet-uml -e $(SRCDIR)/export_test_17_interaction.cfuJ xmi export_test

export_test/export_test_18_usecase.xmi: $(SRCDIR)/export_test_18_usecase.cfuJ
	mkdir -p export_test
	crystal-facet-uml -e $(SRCDIR)/export_test_18_usecase.cfuJ xmi export_test

export_test/export_test_19_deployment.xmi: $(SRCDIR)/export_test_19_deployment.cfuJ
	mkdir -p export_test
	crystal-facet-uml -e $(SRCDIR)/export_test_19_deployment.cfuJ xmi export_test

.PHONY: info
info:
	@printf "== call \\e[33;1m make install \\e[0m to update the files in folder ../../documentation/examples ==\n"

.PHONY: install
install: all_export_test
	cp -frv export_test $(INSTALLDIR)/

.PHONY: clean
clean:
	rm -fr mouse_droid quality pencil_tests export_test self_architecture


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
