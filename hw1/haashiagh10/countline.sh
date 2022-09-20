#!/bin/bash

if [ $# -eq 0 ] ; then
    echo "missing file name"
elif [ $# -eq 1 ] ; then
    if [ -f $1 ] ; then
        numLine="$(wc -l $1 | tr -dc '0-9')"
        echo "$numLine"
    else
        echo "$1 not found"
    fi
else
    echo "only one argument is allowed"
fi