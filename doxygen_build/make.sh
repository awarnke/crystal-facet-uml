#!/bin/sh
doxygen doxygen_config
echo "check that you have the latex package pdflatex installed."
echo "check that you have the latex package xtab installed."
echo "check that you have the latex package multirow installed."
echo "check that you have the latex package sectsty installed."
echo "check that you have the latex package tocloft installed."
cd doc/latex
make
