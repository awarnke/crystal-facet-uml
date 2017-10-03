#!/bin/sh
echo 'number of files'
find ../ctrl ../data ../gui ../main ../pencil ../trace ../tslog ../universal -name '*.c' -o -name '*.inl' -o -name '*.h' | wc
echo 'number of lines'
find ../ctrl ../data ../gui ../main ../pencil ../trace ../tslog ../universal -name '*.c' -o -name '*.inl' -o -name '*.h' | xargs wc
echo 'number of lines in 3rd party libraries'
find ../embunit ../sqlite -name ../utf8stringbuf '*.c' -o -name '*.inl' -o -name '*.h' | xargs wc
