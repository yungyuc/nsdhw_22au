#!/bin/bash

filename=$1

# n=0
# while read line; do
# n=$((n+1))
# done < $filename

# echo "$n lines in $filename"

if [ -f $filename ]
then
    n=0
    while read line; do
        n=$((n+1))
    done < $filename

    echo "$n lines in $filename"
else
    echo "$filename not found"
fi