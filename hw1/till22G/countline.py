#! /bin/bash

bin=$PYTHON_BIN

if [ -z "$(command -v $bin)" ]; then   
    echo  "exec: $bin: not found"
fi

wc -l "$1"
