#!/bin/sh
find src/ -type f -a -name 'COPY*' -o -type f -a -name 'LICENSE*' \
| sort \
| tee LICENSES_of_3rd_party.txt \
| sed -e 's/\(.*\)/echo \"\n\n\n===   ===   ===   \1   ===   ===   ===\n\n\";cat \1/' \
| sh \
>> LICENSES_of_3rd_party.txt

