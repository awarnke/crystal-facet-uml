
Release crystal-facet-uml
=============

This section is for developers.
It describes packaging and releasing a version of crystal-facet-uml.

Pack
-----------

>       # commit all changes before packing !
>       # build and test for opensuse
>       echo "for the oben suse build service, copy the source package and the spec file to a build directory."
>       echo "create a crystal-facet-uml.changes file there"
>       echo "run the osc (openSUSE build service command-line tool):"
>       osc help signkey
>       osc build --clean --local-package openSUSE_Tumbleweed
>       sudo zypper install /var/tmp/.../crystal-facet-uml-$VERSIONSTR-1.x86_64.rpm
>       echo "Note: the open build server seems to sign the packages, no need to do this locally"
