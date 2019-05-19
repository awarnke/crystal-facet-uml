#!/bin/sh
VERSIONSTR=1.13.0
echo "Building Archive Version $VERSIONSTR"
echo "based on latest git commit. (Remember to commit your latest changes!)"
echo "----"
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
#debuild -i -us -uc -b --lintian-opts -i # -us and -uc suppress signing
debuild -i -b --lintian-opts -i # check man dpkg-buildpackage and man lintian for options
cd ..
echo Test the created archive by installing it:
echo sudo dpkg -i crystal-facet-uml_$VERSIONSTR-1_amd64.deb
