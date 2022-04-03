#!/bin/sh

OUTFILE="LICENSES_of_3rd_party.txt"

find src/ -type f -a -name "COPY*" -o -type f -a -name "LICENSE*" \
| sort \
> ${OUTFILE}

# echo "The standard C library (libc) is compiled-in using the -static-libgcc option" \
# >> ${OUTFILE}

find src/ -type f -a -name "COPY*" -o -type f -a -name "LICENSE*" \
| sort \
| sed -e 's/\(.*\)/echo \"\n\n\n===   ===   ===   \1   ===   ===   ===\n\n\";cat \1/' \
| sh \
>> ${OUTFILE}

cp ${OUTFILE} ../root/usr/local
