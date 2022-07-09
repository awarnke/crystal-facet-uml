#!/bin/sh
SRCDIR=../../gui/source/resources
BLDDIR=build_dir

echo "cleaning ${BLDDIR}..."
rm -fr ${BLDDIR}

echo "creating Makefile..."
rm -f Makefile

echo "creating all target"
echo ".PHONY: all " >> Makefile
echo -n "all:${BLDDIR} " >> Makefile
ls ${SRCDIR}/*.svg | sed -e 's#^\(../../gui/source/resources/\)\([^.]*\).svg# build_dir/\2.pdf build_dir/\2.png#g' | xargs echo -n >> Makefile
echo " info" >> Makefile
echo "" >> Makefile

echo "creating ${BLDDIR} target"
echo ".PHONY: ${BLDDIR}" >> Makefile
echo "${BLDDIR}:" >> Makefile
printf "\tmkdir -p ${BLDDIR}\n" >> Makefile

echo "creating pdf targets"
ls ${SRCDIR}/*.svg | sed -e 's#^\(../../gui/source/resources/\)\([^.]*\).svg#build_dir/\2.pdf: \1\2.svg\n\tinkscape --export-pdf=build_dir/\2.pdf --file=\1\2.svg --without-gui\n#' >> Makefile

echo "creating png targets"
ls ${SRCDIR}/*.svg | sed -e 's#^\(../../gui/source/resources/\)\([^.]*\).svg#build_dir/\2.png: \1\2.svg\n\tinkscape --export-png=build_dir/\2.png --file=\1\2.svg --without-gui\n#' >> Makefile

echo "creating info target"
echo ".PHONY: info" >> Makefile
echo "info:" >> Makefile
printf "\t@printf \"== call \\\\e[33;1m make install \\\\e[0m to update the files in folder ../../gui/source/resources ==\"\\n" >> Makefile

echo "creating install target"
echo "" >> Makefile
echo ".PHONY: install" >> Makefile
echo "install:" >> Makefile
printf "\tcp build_dir/*.png build_dir/*.pdf ../../gui/source/resources/\n" >> Makefile
echo "" >> Makefile

echo "creating clean target"
echo ".PHONY: clean" >> Makefile
echo "clean:" >> Makefile
printf "\trm -fr ${BLDDIR}\n" >> Makefile

echo "Makefile created."
