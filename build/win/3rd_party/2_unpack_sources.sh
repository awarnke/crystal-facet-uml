#!/bin/sh
mkdir -p src
cd src
ls -1 ../*.tar.xz | xargs -n 1 tar --xz -xf
ls -1 ../*.tar.gz | xargs -n 1 tar -xzf
cd ..



