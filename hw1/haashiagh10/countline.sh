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
	filename ="$1"
	if [ -f $1 ];then
		count = $(wc -l $1)
		echo "$count lines in $1"
	else
		echo "file not found"
	fi
fi

