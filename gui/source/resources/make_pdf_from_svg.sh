#!/bin/sh
#find . -name '*.svg' -print | sed -e 's/^\.\/\([^.]*\).svg/inkscape --export-pdf=\1.pdf --file=\1.svg --without-gui ;/' 
find . -name '*.svg' -print | sed -e 's/^\.\/\([^.]*\).svg/inkscape --export-pdf=\1.pdf --file=\1.svg --without-gui; echo finished: \1\n/' | /bin/sh
