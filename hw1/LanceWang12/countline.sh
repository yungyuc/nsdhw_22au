#!/bin/bash

if [[ $# < 1 ]]
then
    echo missing file name
elif [[ $# > 1 ]]
then
    echo only one argument is allowed
else
    if test -f $1; then
        num=`wc -l $1 | grep -o "[0-9]."`
        echo $num lines in $1
    else
        echo $1 not found
    fi
fi