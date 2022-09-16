#!/bin/bash

if [ "$#" -eq 0 ]
then
    echo "missing filename\n"
    exit 1
elif [ "$#" -gt 1 ]
then
    echo "only one argument is allowed\n"
    exit 1
else
    if [ -f $1 ]
    then echo $(wc --line < $1)
    else
        echo "$1 not found"
    fi
fi