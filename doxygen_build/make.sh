#!/bin/sh
echo "== building user doc and sourccode doc =="
rm -fr ../doxygen_build/doc
doxygen doxygen_config
echo "check that you have the latex package pdflatex installed."
echo "check that you have the latex package xtab installed."
echo "check that you have the latex package multirow installed."
echo "check that you have the latex package sectsty installed."
echo "check that you have the latex package tocloft installed."
cd doc/latex
make
mv refman.pdf ../../crystal_facet_uml_documentation.pdf
cd ../..
