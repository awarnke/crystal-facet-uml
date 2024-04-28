#!/bin/sh
echo 'number of files'
find ../../ctrl ../../data ../../gui ../../main ../../pencil ../../io ../../test_fw ../../u8stream -name '*.c' -o -name '*.inl' -o -name '*.h' | wc -l
echo 'number of lines'
find ../../ctrl ../../data ../../gui ../../main ../../pencil ../../io ../../test_fw ../../u8stream -name '*.c' -o -name '*.inl' -o -name '*.h' | sort | xargs wc -l
