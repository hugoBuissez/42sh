#!/bin/bash

rm_trailing_whitespaces() {
    echo cleaning: "$1"
    sed -i 's/[ \t]*$//' "$1"
}

if [[ ! -z "$1" && -f "$1" ]]; then
    rm_trailing_whitespaces "$1"
    exit 0
fi

for f in $(find . -type f -not -path '\./\.*' -not -path '\./build/*' -not -path '\./venv/*' -not -path '\./__pycache__/*')
do
    rm_trailing_whitespaces "$f"
done
