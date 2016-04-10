#!/bin/sh
echo 'number of files'
find ../data ../gui ../log ../trace ../ctrl ../main ../pencil -name '*.c' -o -name '*.inl' -o -name '*.h' | wc
echo 'number of lines'
find ../data ../gui ../log ../trace ../ctrl ../main ../pencil -name '*.c' -o -name '*.inl' -o -name '*.h' | xargs wc
