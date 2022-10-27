#!/bin/bash
if [ $# -lt 1 ]; then # less than 
	echo missing file name
elif [ $# -gt 1 ]; then  # greater than 
	echo only one argument is allowed
else
	fname="$1"
	if [ -e $fname ];then # -e check if it exists
		cnt=$(cat $fname | wc -l)
		echo "$cnt lines in $fname"
	else
		echo "$fname not found"
	fi
fi 
