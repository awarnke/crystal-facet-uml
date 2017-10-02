#
# spec file for package crystal_facet_uml
#

Name:           crystal_facet_uml
Version:        0.17.0
Release:        0
License:        Apache-2.0
Summary:        Draws UML Diagrams
Url:            https://github.com/awarnke/crystal_facet_uml
Group:          Development/Tools
Source:         crystal_facet_uml-%{version}-src.zip
BuildRequires:  gcc
BuildRequires:  gcc-c++
BuildRequires:  cmake
BuildRequires:  gtk3
BuildRequires:  gtk3-devel
BuildRequires:  unzip
#BuildRequires:  desktop-file-utils
#BuildRequires:  xdg-utils
#PreReq:
#Provides:       crystal_facet_uml
BuildRoot:      %{_tmppath}/%{name}-%{version}-build

%description
crystal_facet_uml draws UML diagrams.

%prep
%setup -q -n %{name}/cmake_build

%build
cmake -DCMAKE_BUILD_TYPE=Release .
make %{?_smp_mflags}

%install
mkdir -p %{buildroot}/usr/bin 
cp crystal_facet_uml %{buildroot}/usr/bin
mkdir -p %{buildroot}/usr/share/applications
cp ../installation_linux/config/crystal_facet_uml.desktop %{buildroot}/usr/share/applications
#desktop-file-install ../installation_linux/config/crystal_facet_uml.desktop
#cp cfu_install_files/crystal_facet_uml.desktop .
#cp ../gui/source/resources/crystal_facet_uml_80x80.png crystal_facet_uml.png
#cp ../gui/source/resources/crystal_facet_uml.png crystal_facet_uml.png
#ls -l crystal_facet_uml.png
#xdg-desktop-icon install --novendor crystal_facet_uml.desktop
#xdg-desktop-icon install --novendor crystal_facet_uml.png
#xdg-icon-resource install --novendor --size 80 crystal_facet_uml.png crystal_facet_uml
mkdir -p %{buildroot}/usr/share/pixmaps
cp ../gui/source/resources/crystal_facet_uml_80x80.png %{buildroot}/usr/share/pixmaps/crystal_facet_uml.png
mkdir -p %{buildroot}/usr/share/man/man1
cp ../installation_linux/man/crystal_facet_uml.1.gz %{buildroot}/usr/share/man/man1

%check
./unittest_crystal_facet_uml -a

%files
%defattr(-,root,root)
%doc ../license.txt ../readme.markdown ../release_notes.txt
/usr/bin/crystal_facet_uml
/usr/share/pixmaps/crystal_facet_uml.png
/usr/share/applications/crystal_facet_uml.desktop
/usr/share/man/man1/crystal_facet_uml.1.gz

%changelog

