#
# spec file for package crystal-facet-uml
#

Name:           crystal-facet-uml
#Name of the contained application: crystal_facet_uml
Version:        1.12.0
Release:        1
License:        Apache-2.0
Summary:        Draws UML Diagrams
Url:            https://github.com/awarnke/crystal_facet_uml
Group:          Development/Tools/Doc Generators
Source:         crystal-facet-uml_%{version}.orig.tar.gz
BuildRequires:  gcc
BuildRequires:  gcc-c++
BuildRequires:  cmake
BuildRequires:  gtk3
BuildRequires:  gtk3-devel
BuildRequires:  tar
BuildRequires:  sqlite3-devel
#BuildRequires:  desktop-file-utils
#BuildRequires:  xdg-utils
#PreReq:
#Provides:       crystal-facet-uml
BuildRoot:      %{_tmppath}/%{name}-%{version}-build

%description
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
#desktop-file-install ../installation_linux/config/crystal_facet_uml.desktop
#cp cfu_install_files/crystal_facet_uml.desktop .
#cp ./gui/source/resources/crystal_facet_uml_80x80.png crystal_facet_uml.png
#cp ./gui/source/resources/crystal_facet_uml.png crystal_facet_uml.png
#ls -l crystal_facet_uml.png
#xdg-desktop-icon install --novendor crystal_facet_uml.desktop
#xdg-desktop-icon install --novendor crystal_facet_uml.png
#xdg-icon-resource install --novendor --size 80 crystal_facet_uml.png crystal_facet_uml
mkdir -p %{buildroot}/usr/share/pixmaps
cp ./gui/source/resources/crystal_facet_uml_80x80.png %{buildroot}/usr/share/pixmaps/crystal_facet_uml.png
mkdir -p %{buildroot}/usr/share/man/man1
cp ./user_doc/crystal_facet_uml.1.gz %{buildroot}/usr/share/man/man1
mkdir -p %{buildroot}/usr/share/doc/packages/crystal-facet-uml/
cp ./user_doc/crystal_facet_uml_user_documentation.pdf %{buildroot}/usr/share/doc/packages/crystal-facet-uml/

%check
./crystal_facet_uml -v
./unittest_crystal_facet_uml -a
# manual check: sudo zypper install crystal_facet_uml-1.10.0-1.x86_64.rpm

%files
%defattr(-,root,root)
%doc ./license.txt ./readme.markdown ./release_notes.txt
/usr/bin/crystal_facet_uml
/usr/share/pixmaps/crystal_facet_uml.png
/usr/share/applications/crystal_facet_uml.desktop
/usr/share/man/man1/crystal_facet_uml.1.gz
/usr/share/doc/packages/crystal-facet-uml/crystal_facet_uml_user_documentation.pdf

%changelog
