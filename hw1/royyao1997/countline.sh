#!/bin/bash

if [ "&#" -lt 1]; then
    echo "missing file name"


elif [ "&#" -gt 1]; then
    echo "only one argument is allowed"


else
	filename=$1
	if [[ -f "$filename" ]] ; then
		lines=$(wc -l $filename | tr -dc '0-9')
		echo "$lines lines in $filename\n"
	else
		echo -e "$filename not found"
	fi
fi