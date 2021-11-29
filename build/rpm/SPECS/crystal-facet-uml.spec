#
# spec file for package crystal-facet-uml
#
# Copyright (c) 2019-2020 SUSE LLC
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
Version:        1.34.1
Release:        0
Summary:        Draws UML/SysML Diagrams
License:        Apache-2.0
Group:          Development/Tools/Doc Generators
URL:            https://github.com/awarnke/crystal-facet-uml
Source:         crystal-facet-uml_%{version}.orig.tar.gz
BuildRequires:  cmake
BuildRequires:  gcc
BuildRequires:  gtk3-devel
BuildRequires:  sqlite3-devel
BuildRequires:  tar
BuildRequires:  xorg-x11-fonts-core

%description
crystal-facet-uml creates sysml/uml diagrams to document system and software architecture.
As software architect, you can create a set of diagrams describing
use-cases, requirements, structural views, behavioral and deployment views.
crystal-facet-uml keeps element names and element hierarchies consistent.
The tool exports diagrams as svg, pdf, ps and png formats
to be used in text processing systems as docbook, html, latex.
crystal-facet-uml exports the model to xmi format.
It runs on your local linux PC and is based on glib, gdk, gtk, cairo, pango, sqlite.

%prep
%setup -q -n crystal-facet-uml-%{version}

%build
%cmake \
  -DCMAKE_BUILD_TYPE=Release
%if 0%{?cmake_build:1}
# cmake_build works with openSuSE_TumbleWeed and openSuSE_Leap_15.1
%cmake_build
%else
# fallback for old openSUSE_Leap_42.3:
%make_jobs
%endif

%install
%cmake_install

%check
./build/crystal-facet-uml -v
./build/test_crystal-facet-uml -a

%files
%license license.txt
%doc readme.markdown ChangeLog user_doc/crystal-facet-uml_documentation.pdf
%{_bindir}/crystal-facet-uml
%{_datadir}/pixmaps/crystal-facet-uml.png
%{_datadir}/applications/crystal-facet-uml.desktop
%{_datadir}/metainfo/crystal-facet-uml.metainfo.xml
%{_mandir}/man1/crystal-facet-uml.1%{?ext_man}

%changelog
