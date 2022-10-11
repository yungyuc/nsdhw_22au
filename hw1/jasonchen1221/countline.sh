#!/bin/bash

if [ "$#" -eq 0 ]; then
	echo "missing file name"
elif [ "$#" -gt 1 ]; then
	echo "only one argument is allowed"
elif [ -e $1 ]; then			# check the file is existed or not
	lineCnt=$(cat $1 | wc -l)	# wc -l : count lines
	echo "$lineCnt lines in $1"
else
	echo "$1 not found"
fi

