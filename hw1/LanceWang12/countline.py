#!/bin/bash
""":"

if [ $PYTHON_BIN == python0 ]
then
    exec $PYTHON_BIN countline.py
    exit 1
elif [ $PYTHON_BIN == python2 ]
then
    exec python2 countline.py
else
    exec python3 countline.py
fi

exit 0
"""

import os
import sys

if len(sys.argv) < 1:
    sys.stdout.write('missing file name\n')
elif len(sys.argv) > 1:
    sys.stdout.write('only one argument is allowed\n')
else:
    fname = sys.argv[0]
    if os.path.exists(fname):
        with open(fname) as fobj:
            lines = fobj.readlines()
        sys.stdout.write('{} lines in {}\n'.format(len(lines), fname))
    else:
        sys.stdout.write('{} not found\n'.format(fname))
