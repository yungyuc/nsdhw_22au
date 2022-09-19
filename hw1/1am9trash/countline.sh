#!/bin/bash

if [ $# -lt 1 ] ; then
    echo "missing file name"
elif [ $# -gt 1 ] ; then
    echo "only one argument is allowed"
else
    if test -f "$1"; then
        line_cnt=$(wc -l < "$1")
        echo "${line_cnt} lines in $1"
    else
        echo "$1 not found"
    fi
fi
