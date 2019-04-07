#!/bin/sh
echo "1.) Update the VERSIONSTR in this debian_pack.sh script"
echo "2.) Copy the latest entry in debian_config/debian/changelog"
echo "3.) cd debian_config && dch -i"
echo "4.) vi debian/changelog"
VERSIONSTR=1.12.0
echo "clean old files and directories"
test -e crystal-facet-uml_$VERSIONSTR.orig.tar.gz && rm crystal-facet-uml_$VERSIONSTR.orig.tar.gz
test -d crystal-facet-uml-$VERSIONSTR && rm -r crystal-facet-uml-$VERSIONSTR
echo "pack src archive"
cd ..
git archive --format tar.gz --prefix=crystal-facet-uml-$VERSIONSTR/ --output=package_build/crystal-facet-uml_$VERSIONSTR.orig.tar.gz master
cd package_build
tar -xzf crystal-facet-uml_$VERSIONSTR.orig.tar.gz
cp -r debian_config/debian crystal-facet-uml-$VERSIONSTR/
# note _ is not allowed in debian package names
cd crystal-facet-uml-$VERSIONSTR
debuild -i -us -uc -b
cd ..
echo Test the created archive by installing it:
echo sudo dpkg -i crystal-facet-uml_$VERSIONSTR-1_amd64.deb
