#!/bin/bash
if [ $# -lt 1 ]; then
  echo "missing filename\n"
elif [ $# -gt 1 ]; then
  echo "only one argument is allowed\n"
fi

filename=$1
if [ -f "$filename" ]; then
  lines_cnt=$( cat $filename | wc -l)
  echo "$lines_cnt lines in $filename"
else
  echo "$filename not found"
fi