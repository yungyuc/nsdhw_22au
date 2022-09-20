#!/bin/bash
#
#Test for countline.py
#

if [ $# -lt 1 ]
then
	echo missing the file
elif [ $# -gt 1 ]
then
	echo only one argument is allow
else 
	if [ -f $1 ]
	then
		echo "$(wc -l $1 | tr -dc '0-9') lines in $1"
	else
		echo "file not found"
	fi
fi

