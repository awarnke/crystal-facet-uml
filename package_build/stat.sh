#!/bin/sh
echo 'number of files'
find ../data ../gui ../log ../trace ../ctrl ../main ../pencil -name '*.c' -o -name '*.inl' -o -name '*.h' | wc
echo 'number of lines'
find ../data ../gui ../log ../trace ../ctrl ../main ../pencil -name '*.c' -o -name '*.inl' -o -name '*.h' | xargs wc
echo 'number of lines in 3rd party libraries'
find ../utf8stringbuf ../embunit ../sqlite -name '*.c' -o -name '*.inl' -o -name '*.h' | xargs wc
