#!/bin/bash

if [ "$#" -lt 1 ]; then
  echo "missing file name"
elif [ "$#" -gt 1 ]; then
  echo "only one argument is allowed"
elif [ -f $1 ]; then
  lines=$(cat  $1 | wc -l) 
  echo "$lines lines in $1 \n"
else
  echo "$1 not found"
fi
