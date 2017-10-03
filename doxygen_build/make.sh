#!/bin/sh
echo "check that you have the latex package pdflatex installed."
echo "check that you have the latex package xtab installed."
echo "check that you have the latex package multirow installed."
echo "check that you have the latex package sectsty installed."
echo "check that you have the latex package tocloft installed."

echo "== building user doc only =="
rm -fr ../doxygen_build/userdoc
doxygen Doxyfile_UserDocOnly
cd userdoc/latex
make
mv refman.pdf ../../crystal_facet_uml_user_documentation.pdf
cd ../..

echo "== building user doc and sourccode doc =="
rm -fr ../doxygen_build/doc
doxygen Doxyfile
cd doc/latex
make
mv refman.pdf ../../crystal_facet_uml_documentation.pdf
cd ../..
