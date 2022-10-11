#!/bin/bash

if [ $# -lt 1 ]; then
    echo missing file name
elif [ $# -gt 1 ]; then
    echo only one argument is allowed
else
    fname="$1"
    if [ -e $fname ]; then
        sentence=($(wc -l $fname))
        line_counts=${sentence[0]}
        echo $line_counts lines in $fname
    else
        echo $fname not found
    fi
fi
