#!/bin/sh
find src/ -name 'COPYING' -o -name 'LICENSE' \
| sort \
| tee LICENSES_of_3rd_party.txt \
| sed -e 's/\(.*\)/echo \"\n\n\n===   ===   ===   \1   ===   ===   ===\n\n\";cat \1/' \
| sh \
>> LICENSES_of_3rd_party.txt

