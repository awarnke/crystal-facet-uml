.PHONY: all
all: mouse_droid/mouse_droid.pdf mouse_droid/mouse_droid.html quality/quality.pdf quality/quality.html self_architecture/self_architecture.pdf self_architecture/self_architecture.html

ARCDIR=../../documentation/architecture
SRCDIR=../../documentation/examples

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

.PHONY: clean
clean:
	rm -fr mouse_droid quality self_architecture


# Copyright 2021-2026 Andreas Warnke
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
