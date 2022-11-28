#!/bin/bash

''':'
export PYTHONPATH=".:$PYTHONPATH"
export VECLIB_MAXIMUM_THREADS=1
export MKL_NUM_THREADS=1
export NUMEXPR_NUM_THREADS=1
export OMP_NUM_THREADS=1

if [ -f "PRELOAD_MKL" ] ; then
    PRELOAD_MKL=1
fi

test_path="${BASH_SOURCE[0]}"

if [[ (-n "$PRELOAD_MKL") && ("Linux" == "$(uname)") ]] ; then
    # Workaround for cmake + MKL in conda.
    MKL_LIB_DIR=/usr/lib/x86_64-linux-gnu
    MKL_LIBS=$MKL_LIB_DIR/libmkl_def.so
    MKL_LIBS=$MKL_LIBS:$MKL_LIB_DIR/libmkl_avx2.so
    MKL_LIBS=$MKL_LIBS:$MKL_LIB_DIR/libmkl_core.so
    MKL_LIBS=$MKL_LIBS:$MKL_LIB_DIR/libmkl_intel_lp64.so
    MKL_LIBS=$MKL_LIBS:$MKL_LIB_DIR/libmkl_sequential.so
    export LD_PRELOAD=$MKL_LIBS
    echo "INFO: set LD_PRELOAD=$LD_PRELOAD for MKL"
else
    echo "INFO: set PRELOAD_MKL if you see (Linux) MKL linking error"
fi

make test

# Benchmark
echo "INFO: benchmark"
python3 $test_path

exit 0
':'''

import pytest
import math
import _matrix
import numpy as np
import timeit
import sys

def test_constructor():
    m = _matrix.Matrix(5, 3)
    npm = np.zeros((5, 3), dtype=np.float64)
    assert m.nrow == npm.shape[0]
    assert m.ncol == npm.shape[1]
    assert m.size == npm.size

def test_constructor_vector():
    v = [i for i in range(5*3)]
    m = _matrix.Matrix(5, 3, v)
    npm = np.array(v, dtype=np.float64)
    npm.shape = (5, 3)
    assert m.nrow == npm.shape[0]
    assert m.ncol == npm.shape[1]
    assert m.size == npm.size
    for i, j in range(5, 3):
        assert m[i, j] == npm[i, j]

def test_multiply_naive():
    v1 = [i for i in range(1000*1000)]
    v2 = [i-1 for i in range(1000*1000)]
    m1 = _matrix.Matrix(1000, 1000, v1)
    m2 = _matrix.Matrix(1000, 1000, v2)
    npm1 = np.array(v1, dtype=np.float64)
    npm1.shape = (1000, 1000)
    npm2 = np.array(v2, dtype=np.float64)
    npm2.shape = (1000, 1000)
    m3 = _matrix.multiply_naive(m1, m2)
    npm3 = _matrix.Matrix(1000, 1000, np.ravel(np.matmul(npm1, npm2)))
    assert m3 == npm3

def test_multiply_tile():
    v1 = [i for i in range(1000*1000)]
    v2 = [i-1 for i in range(1000*1000)]
    m1 = _matrix.Matrix(1000, 1000, v1)
    m2 = _matrix.Matrix(1000, 1000, v2)
    npm1 = np.array(v1, dtype=np.float64)
    npm1.shape = (1000, 1000)
    npm2 = np.array(v2, dtype=np.float64)
    npm2.shape = (1000, 1000)
    m3 = _matrix.multiply_tile(m1, m2, 16)
    npm3 = _matrix.Matrix(1000, 1000, np.ravel(np.matmul(npm1, npm2)))
    assert m3 == npm3

def test_multiply_mkl():
    v1 = [i for i in range(1000*1000)]
    v2 = [i-1 for i in range(1000*1000)]
    m1 = _matrix.Matrix(1000, 1000, v1)
    m2 = _matrix.Matrix(1000, 1000, v2)
    npm1 = np.array(v1, dtype=np.float64)
    npm1.shape = (1000, 1000)
    npm2 = np.array(v2, dtype=np.float64)
    npm2.shape = (1000, 1000)
    m3 = _matrix.multiply_mkl(m1, m2)
    npm3 = _matrix.Matrix(1000, 1000, np.ravel(np.matmul(npm1, npm2)))
    assert m3 == npm3

class Writer:

    def __init__(self, streams):

        self.streams = streams

    def write(self, msg):

        for stream in self.streams:

            stream.write(msg)


def benchmark():

    setup = '''
import _matrix

size = 1000

mat1 = _matrix.Matrix(size,size)
mat2 = _matrix.Matrix(size,size)

for it in range(size):
    for jt in range(size):
        mat1[it, jt] = it * size + jt + 1
        mat2[it, jt] = it * size + jt + 1
'''

    naive = timeit.Timer('_matrix.multiply_naive(mat1, mat2)', setup=setup)
    tile = timeit.Timer('_matrix.multiply_tile(mat1, mat2, 64)', setup=setup)
    mkl = timeit.Timer('_matrix.multiply_mkl(mat1, mat2)', setup=setup)

    repeat = 5

    with open('performance.txt', 'w') as fobj:

        w = Writer([sys.stdout, fobj])

        w.write(f'Start multiply_naive (repeat={repeat}), take min = ')
        naivesec = minsec = min(naive.repeat(repeat=repeat, number=1))
        w.write(f'{minsec} seconds\n')

        w.write(f'Start multiply_tile (repeat={repeat}), take min = ')
        tilesec = minsec = min(tile.repeat(repeat=repeat, number=1))
        w.write(f'{minsec} seconds\n')

        w.write(f'Start multiply_mkl (repeat={repeat}), take min = ')
        mklsec = minsec = min(mkl.repeat(repeat=repeat, number=1))
        w.write(f'{minsec} seconds\n')

        w.write('MKL speed-up over naive: %g x\n' % (naivesec/mklsec))
        w.write('MKL speed-up over tile: %g x\n' % (tilesec/mklsec))

if __name__ == '__main__':
    benchmark()

