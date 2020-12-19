#
# spec file for package crystal-facet-uml
#
# Copyright (c) 2019 SUSE LLC
# Copyright (c) 2017-2020 Andreas Warnke cfu@andreaswarnke.de
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
Version:        1.26.1
Release:        0
Summary:        Draws UML/SysML Diagrams
License:        Apache-2.0
Group:          Development/Tools/Doc Generators
URL:            https://github.com/awarnke/crystal_facet_uml
Source:         crystal-facet-uml_%{version}.orig.tar.gz
BuildRequires:  cmake
BuildRequires:  gcc
BuildRequires:  gcc-c++
BuildRequires:  gtk3-devel
BuildRequires:  sqlite3-devel
BuildRequires:  tar

%description
crystal_facet_uml creates sysml/uml diagrams to document system and software architecture.
As software architect, you can create a set of diagrams describing
use-cases, requirements, structural views, behavioral and deployment views.
crystal_facet_uml keeps element names and element hierarchies consistent.
The tool exports diagrams as svg, pdf, ps and png formats
to be used in text processing systems as docbook, html, latex.
crystal_facet_uml exports the model to xmi format.
It runs on your local linux PC and is based on glib, gdk, gtk, cairo, pango, sqlite.
crystal_facet_uml draws UML diagrams.

%prep
%setup -q -n crystal-facet-uml-%{version}

%build
%cmake \
  -DCMAKE_BUILD_TYPE=Release
%make_jobs
# %cmake_build -- Works with openSuSE_TumbleWeed but not with openSuSE_Leap_15.0

%install
%cmake_install

%check
./build/crystal_facet_uml -v
./build/unittest_crystal_facet_uml -a

%files
%license ./license.txt
%doc ./readme.markdown ./release_notes.txt ./user_doc/crystal_facet_uml_user_documentation.pdf
%{_bindir}/crystal_facet_uml
%{_datadir}/pixmaps/crystal_facet_uml.png
%{_datadir}/applications/crystal_facet_uml.desktop
%{_datadir}/metainfo/crystal_facet_uml/crystal-facet-uml.metainfo.xml
%{_mandir}/man1/crystal_facet_uml.1%{?ext_man}

%changelog
