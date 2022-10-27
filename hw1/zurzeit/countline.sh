#!/bin/bash
chmod u+x *.sh # give permission
args=("$@")

if [ 1 -gt $# ]; then
    echo "missing file name"
elif [ $# -gt 1 ]; then
    echo "only one argument is allowed"
else
    fname=${args[0]}
    if test -f "$fname"; then
        lines=$(cat  $fname | wc -l)
        echo "$lines lines in $fname"
    else
        echo "$fname not found"
    fi
fi
