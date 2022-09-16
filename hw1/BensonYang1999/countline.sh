#!/bin/bash

if [ "$#" -lt 1 ]; then
    echo "missing file name"
elif [ "$#" -gt 1 ]; then
    echo "only one argument is allowed"
else
    filename=$1
    if [ -f $filename ]; then
        n=0
        while read line; do
            n=$((n + 1))
        done <$filename

        echo "$n lines in $filename"
    else
        echo "$filename not found"
    fi
fi
