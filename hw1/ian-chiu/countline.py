#!/bin/bash

code="
import sys
import os.path
if len(sys.argv) < 2:
    sys.stdout.write('missing file name')
elif len(sys.argv) > 2:
    sys.stdout.write('only one argument is allowed')
else:
    fname = sys.argv[1]
    if os.path.exists(fname):
        with open(fname) as fobj:
            lines = fobj.readlines()
        sys.stdout.write('{} lines in {}'.format(len(lines), fname))
    else:
        sys.stdout.write('{} not found'.format(fname))
"

if which $PYTHON_BIN > /dev/null; then
   $PYTHON_BIN -c "$code" "$@"
else
    echo "exec: $PYTHON_BIN: not found"
    exit 127
fi
