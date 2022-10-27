#!/bin/bash

if [ $# -lt 1 ]
then
    echo "missing file name"
elif [ $# -gt 1 ]
then
    echo "only one argument is allowed"
else
    fname=$1
    if test -f "$fname";
    then
        line=$(wc -l < $fname)
        echo "$line lines in $fname"
    else
        echo "$fname not found"
    fi
fi