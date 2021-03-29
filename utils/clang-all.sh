#!/bin/bash

cfi() {
    clang-format -style=file -i "$1"
}

for f in $(find -depth -type f -name '*.[ch]')
do
    cfi $f
done
