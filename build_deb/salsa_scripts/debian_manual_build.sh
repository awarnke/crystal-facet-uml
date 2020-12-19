#!/bin/sh
# goal of this script is to perform the build steps
# in a similar way as the the salsa/gitlab ci system
echo run this script in the root folder of the cloned git repo https://salsa.debian.org/debian-edu-pkg-team/crystal-facet-uml
test -e debian || exit
echo cleaning up previous downloads and builds
rm ../crystal-facet-uml_*.orig.tar.gz
rm -fr CMakeCache.txt CMakeFiles/ cmake_install.cmake Makefile
rm -r install_manifest.txt
rm -fr architecture CMakeLists.txt ctrl data doxygen_build gui installation_linux io license.txt main pencil readme.markdown release_notes.txt test_fw todo.txt trace tslog universal user_doc utf8stringbuf
rm -f crystal-facet-uml test_crystal-facet-uml
echo fetching sources
debian/rules get-orig-source
(cd .. && tar --strip=1 --one-top-level=crystal-facet-uml -xzf crystal-facet-uml_*.orig.tar.gz)
echo building binaries and package
debuild --diff-ignore='^(\.git.*|.*\.yml)$' --lintian-opts -i --pedantic
yamllint debian/upstream/metadata

