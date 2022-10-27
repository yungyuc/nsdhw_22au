#!/usr/bin/env python3

import sys
import os.path

# Trying to make this python scipt reads the environment variable
# named "PYTHON_BIN" to switch the Python executable for the
# script. But need to exclude "python0"
# As below,
if os.environ["PYTHON_BIN"] == "python0":
	sys.stdout.write("exec: python0: not found")
	exit(1)
"exec" "$PYTHON_BIN" "$0" "$@"

if len(sys.argv) < 2:
    sys.stdout.write('missing file name\n')
elif len(sys.argv) > 2:
    sys.stdout.write('only one argument is allowed\n')
else:
    fname = sys.argv[1]
    if os.path.exists(fname):
        with open(fname) as fobj:
            lines = fobj.readlines()
        sys.stdout.write('{} lines in {}\n'.format(len(lines), fname))
    else:
        sys.stdout.write('{} not found\n'.format(fname))
