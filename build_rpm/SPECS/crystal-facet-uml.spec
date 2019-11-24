#
# spec file for package: crystal-facet-uml
#

Name:           crystal-facet-uml
#Name of the contained application: crystal_facet_uml
Version:        1.16.0
Release:        1
License:        Apache-2.0
Summary:        Draws UML/SysML Diagrams
Url:            https://github.com/awarnke/crystal_facet_uml
Group:          Development/Tools/Doc Generators
Source:         crystal-facet-uml_%{version}.orig.tar.gz
Requires:       gtk3
Requires:       sqlite3
BuildRequires:  gcc
BuildRequires:  gcc-c++
BuildRequires:  cmake
BuildRequires:  gtk3-devel
BuildRequires:  tar
BuildRequires:  sqlite3-devel
Provides:       crystal-facet-uml1
BuildRoot:      %{_tmppath}/%{name}-%{version}-build

%description
crystal_facet_uml creates sysml/uml diagrams to document system and software architecture.
As software architect, you can create a set of diagrams describing
use-cases, requirements, structural views, behavioral and deployment views.
crystal_facet_uml keeps element names and element hierarchies consistent.
The tool exports diagrams as svg, pdf, ps and png formats
to be used in text processing systems as docbook, html, latex.
It runs on your local linux PC and is based on glib, gdk, gtk, cairo, pango, sqlite.
crystal_facet_uml draws UML diagrams.

%prep
%setup -q -n crystal-facet-uml-%{version}

%build
cmake -DCMAKE_BUILD_TYPE=Release .
make %{?_smp_mflags}

%install
mkdir -p %{buildroot}/usr/bin
cp crystal_facet_uml %{buildroot}/usr/bin
mkdir -p %{buildroot}/usr/share/applications
cp ./installation_linux/config/crystal_facet_uml.desktop %{buildroot}/usr/share/applications
mkdir -p %{buildroot}/usr/share/pixmaps
cp ./gui/source/resources/crystal_facet_uml_80x80.png %{buildroot}/usr/share/pixmaps/crystal_facet_uml.png
mkdir -p %{buildroot}/usr/share/man/man1
cp ./user_doc/crystal_facet_uml.1.gz %{buildroot}/usr/share/man/man1
mkdir -p %{buildroot}/usr/share/doc/packages/crystal-facet-uml/
cp ./user_doc/crystal_facet_uml_user_documentation.pdf %{buildroot}/usr/share/doc/packages/crystal-facet-uml/

%check
./crystal_facet_uml -v
./unittest_crystal_facet_uml -a

%files
%defattr(-,root,root)
%doc ./license.txt ./readme.markdown ./release_notes.txt
/usr/bin/crystal_facet_uml
/usr/share/pixmaps/crystal_facet_uml.png
/usr/share/applications/crystal_facet_uml.desktop
/usr/share/man/man1/crystal_facet_uml.1.gz
/usr/share/doc/packages/crystal-facet-uml/crystal_facet_uml_user_documentation.pdf

%changelog

