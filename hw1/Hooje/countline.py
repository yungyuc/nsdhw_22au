#!/bin/bash

#we need ''' ''' for python 
#so we have to change first ''' to become '''' and add a : to do nothing
#then we add # to second ''' in order to maintain the structure of ''' ''' 


'''':
exec $PYTHON_BIN "$0" "$@"
#'''



import sys
import os.path


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
