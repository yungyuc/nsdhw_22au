#!/bin/bash

if [ $# -eq 0 ]; then
    echo -e "missing file name"
elif [ $# -gt 1 ]; then
    echo -e "only one argument is allowed"
else 
    if [ ! -f $1 ]; then
        echo -e "$1 not found" 
    else
        echo -e "number of lines in $1 is $(cat $1 | wc -l)"
    fi
fi