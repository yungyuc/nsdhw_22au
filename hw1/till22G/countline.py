#! /bin/bash

bin=$PYTHON_BIN
#bin=python3

if [ -z "$(command -v $bin)" ]; then   
    echo  "exec: $bin: not found"
fi

#if [ "$PYTHON_BIN" == "python0" ]; then
#    echo "exec: python0: not found"
#fi

wc -l "$1"
