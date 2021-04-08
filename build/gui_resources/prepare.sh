#!/bin/sh
SRCDIR=../../gui/source/resources

echo "creating Makefile..."
rm -f Makefile

echo "creating all target"
echo ".PHONY: all " >> Makefile
echo -n "all:" >> Makefile
ls ${SRCDIR}/*.svg | sed -e 's#^\(../../gui/source/resources/\)\([^.]*\).svg# \2.pdf \2.png#g' | xargs echo -n >> Makefile
echo " info" >> Makefile
echo "" >> Makefile

echo "creating pdf targets"
ls ${SRCDIR}/*.svg | sed -e 's#^\(../../gui/source/resources/\)\([^.]*\).svg#\2.pdf: \1\2.svg\n\tinkscape --export-pdf=\2.pdf --file=\1\2.svg --without-gui\n#' >> Makefile

echo "creating png targets"
ls ${SRCDIR}/*.svg | sed -e 's#^\(../../gui/source/resources/\)\([^.]*\).svg#\2.png: \1\2.svg\n\tinkscape --export-png=\2.png --file=\1\2.svg --without-gui\n#' >> Makefile

echo "creating info target"
echo ".PHONY: info" >> Makefile
echo "info:" >> Makefile
printf "\t@printf \"== call \\\\e[33;1m make install \\\\e[0m to update the files in folder ../../gui/source/resources ==\"\\n" >> Makefile

echo "creating install target"
echo "" >> Makefile
echo ".PHONY: install" >> Makefile
echo "install:" >> Makefile
printf "\tcp *.png *.pdf ../../gui/source/resources/" >> Makefile
echo "" >> Makefile

echo "Makefile created."
