#!/bin/bash
if [ $# -lt 1 ]; then
  echo "missing file name\n"
elif [ $# -gt 1 ]; then
  echo "only one argument is allowed\n"
fi

FILE=$1
if [ -f "$FILE" ]; then
  lines=`wc -l $1 | awk '{ print $1 }'`
  echo "$lines lines in $FILE"
else
  echo "$FILE not found"
fi

