#!/bin/sh
echo "creating Makefile..."
rm -f Makefile

echo "creating all target"
echo -n "all:" >> Makefile
find . -name '*.svg' -print | sed -e 's/^\.\/\([^.]*\).svg/ \1.pdf \1.png/' | xargs echo -n >> Makefile
echo "" >> Makefile
echo "" >> Makefile

echo "creating pdf targets"
find . -name '*.svg' -print | sed -e 's/^\.\/\([^.]*\).svg/\1.pdf: \1.svg\n\tinkscape --export-pdf=\1.pdf --file=\1.svg --without-gui\n/' >> Makefile

echo "creating png targets"
find . -name '*.svg' -print | sed -e 's/^\.\/\([^.]*\).svg/\1.png: \1.svg\n\tinkscape --export-png=\1.png --file=\1.svg --without-gui\n/' >> Makefile

echo "Makefile created."

echo "running make..."
make
