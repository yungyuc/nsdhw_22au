#!/bin/bash

if [ $# -lt 1 ]; then
  echo 'missing file name'
elif [ $# -gt 1 ]; then
  echo 'only one argument is allowed'
else
  fname=$1
  if [ -f $fname ]; then
    cnt=0
    while read _; do
      cnt=$((cnt + 1))
    done < $fname
    echo "$cnt lines in $fname"
  else
    echo "$fname not found"
  fi
fi
