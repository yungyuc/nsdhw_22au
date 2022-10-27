#!/bin/bash

if [ $# -lt 1 ] ; then
  echo "missing file name"
elif [ $# -gt 1 ] ; then
  echo "only one argument is allowed"
else
  if [ ! -f $1 ] ; then
    echo "file not found"
  else
    LINE_COUNT=$(wc -l $1 | tr -dc '0-9')
    echo "$LINE_COUNT lines in $1"
  fi
fi