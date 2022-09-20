#!/bin/bash

if [ $# -lt 1 ] ; then
    echo "missing file name"
elif [ $# -gt 1 ] ; then
    echo "only one argument is allowed"
else 
    if ! [ -f $1 ] ; then
        echo "$1 not found"
    else
        line_count="$(wc -l $1 | tr -dc '0-9')"
        echo "$line_count"
    fi
fi

