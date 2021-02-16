#!/bin/sh
echo 'number of files'
find ../../ctrl ../../data ../../gui ../../main ../../pencil ../../io ../../test_fw ../../trace ../../tslog ../../universal ../../utf8stringbuf -name '*.c' -o -name '*.inl' -o -name '*.h' | wc
echo 'number of lines'
find ../../ctrl ../../data ../../gui ../../main ../../pencil ../../io ../../test_fw ../../trace ../../tslog ../../universal ../../utf8stringbuf -name '*.c' -o -name '*.inl' -o -name '*.h' | sort | xargs wc
