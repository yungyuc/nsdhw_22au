#!/usr/bin/env bash
if [[ -z "$1" ]]; then
    echo "missing file name"
elif [[ ! -z "$2" ]]; then
    echo "only one argument is allowed"
elif [[ ! -f "$1" ]]; then
    echo "$1 not found"
else
    n_lines=$(cat $1 | wc -l | tr -dc '0-9')
    echo "$n_lines lines in $1"
fi 