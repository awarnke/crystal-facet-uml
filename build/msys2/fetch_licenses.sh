#!/bin/sh
for file in $( ldd crystal-facet-uml.exe | grep -e ' /ucrt64' | sed -e 's/^[^/]*\(\S*\).*$/\1/' )
do
    echo $file
    pkg=$( pacman -Qo $file | sed -e 's/^\S* \S* \S* \S* \(\S*\).*$/\1/' 2>&1 )
    echo $pkg
    pacman -Qi $pkg
    echo --
done

file2=/ucrt64/bin/gdbus.exe
echo $file2
pkg2=$( pacman -Qo $file2 | sed -e 's/^\S* \S* \S* \S* \(\S*\).*$/\1/' 2>&1 )
echo $pkg2
pacman -Qi $pkg2
echo --

file3=/ucrt64/share/glib-2.0/schemas/org.gtk.gtk4.Settings.FileChooser.gschema.xml
echo $file3
pkg3=$( pacman -Qo $file3 | sed -e 's/^\S* \S* \S* \S* \(\S*\).*$/\1/' 2>&1 )
echo $pkg3
pacman -Qi $pkg3
echo --
