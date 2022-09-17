#!/bin/bash
'''':
if [ $# -lt 1 ] ; then
    echo "missing file name"
elif [ $# -gt 1 ] ; then
    echo "only one argument is allowed"
else
    fname=$1

    num_lines=`wc --lines < $fname`

    echo "$num_lines lines in "$fname""
fi
