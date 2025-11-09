#
# spec file for package crystal-facet-uml
#
# Copyright (c) 2019-2025 SUSE LLC
# Copyright (c) 2017-2025 Andreas Warnke cfu@andreaswarnke.de
#
# All modifications and additions to the file contributed by third parties
# remain the property of their copyright owners, unless otherwise agreed
# upon. The license for this file, and modifications and additions to the
# file, is the same license as for the pristine package itself (unless the
# license for the pristine package is not an Open Source License, in which
# case the license is the MIT License). An "Open Source License" is a
# license that conforms to the Open Source Definition (Version 1.9)
# published by the Open Source Initiative.

# Please submit bugfixes or comments via https://bugs.opensuse.org/
#


Name:           crystal-facet-uml
Version:        1.67.1
Release:        0
Summary:        Draws UML/SysML Diagrams
License:        Apache-2.0
Group:          Development/Tools/Doc Generators
URL:            https://github.com/awarnke/crystal-facet-uml
Source:         crystal-facet-uml_%{version}.orig.tar.gz
BuildRequires:  cmake
BuildRequires:  gcc
BuildRequires:  gtk4-devel
BuildRequires:  sqlite3-devel
BuildRequires:  tar
BuildRequires:  lato-fonts

%description
crystal-facet-uml creates sysml/uml diagrams to document system and software architecture.
As software architect, you can create a set of diagrams describing
use-cases, requirements, structural views, behavioral and deployment views.
crystal-facet-uml keeps element names and element hierarchies consistent.
The tool exports diagrams in svg, pdf, ps and png formats
to be used in text processing systems like DocBook, html, LaTeX.
crystal-facet-uml exports the model to json and xmi format; json can also be imported.
The json-based data file can be stored to git, branched and merged together with your source code.
It runs on your local linux PC and is based on gtk (incl glib, gdk, cairo, pango) and sqlite.

%prep
%autosetup

%build
%cmake
%cmake_build

%install
%cmake_install

%check
./build/crystal-facet-uml -v
./build/test_crystal-facet-uml -a

%files
%license LICENSE.txt
%doc ChangeLog installation/crystal-facet-uml_documentation.pdf
%{_bindir}/crystal-facet-uml
%{_datadir}/icons/hicolor/48x48/apps/crystal-facet-uml.png
%{_datadir}/icons/hicolor/scalable/apps/crystal-facet-uml.svg
%{_datadir}/applications/de.arrows2boxes.crystal-facet-uml.desktop
%{_datadir}/metainfo/de.arrows2boxes.crystal-facet-uml.metainfo.xml
%{_mandir}/man1/crystal-facet-uml.1%{?ext_man}

%changelog
