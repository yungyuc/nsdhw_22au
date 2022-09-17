#!/bin/bash

if [ $# -eq 0 ]; then
    echo "missing file name"
    exit 1
elif [ $# -gt 1 ]; then
    echo "only one argument is allowed"
    exit 1
elif [ -f $1 ]; then
    echo "$(wc -l $1 | tr -dc '0-9') lines in $1" 
else
    echo "$1 not found"
    exit 1
fi