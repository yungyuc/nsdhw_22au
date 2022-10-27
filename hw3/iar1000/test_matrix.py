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
'''

import _matrix
import math
import numpy as np
import pytest
import timeit


class TestMatrix:
    def get_matrix(self, size, factor):
        mat = _matrix.Matrix(size, size)

        for i in range(size):
            for j in range(size):
                mat[i, j] = (i * size + j) * factor

        return mat

    # from validation.py
    def test_basic(self):
        size = 100
        mat1 = self.get_matrix(size, 1)
        mat2 = self.get_matrix(size, 1)
        mat3 = self.get_matrix(size, 0)

        assert size == mat1.nrow
        assert size == mat1.ncol
        assert size == mat2.nrow
        assert size == mat2.ncol
        assert size == mat3.nrow
        assert size == mat3.ncol

        assert 2 == mat1[0, 1]
        assert size+2 == mat1[1, 1]
        assert size*2 == mat1[1, size-1]
        assert size*size == mat1[size-1, size-1]

        for i in range(mat1.nrow):
            for j in range(mat1.ncol):
                assert mat1[i, j] == mat2[i, j]
                assert 0 == mat3[i, j]

        assert mat1 == mat2
        assert mat1 is not mat2


    def test_zero(self):
        size = 100
        mat1 = self.get_matrix(size, 0)
        mat2 = self.get_matrix(size, 1)

        mat3_naive = _matrix.multiply_naive(mat1, mat2)
        mat3_mkl = _matrix.multiply_mkl(mat1, mat2)

        for i in range(size):
            for j in range(size):
                assert 0 == mat3_naive[i, j]
                assert 0 == mat3_mkl[i, j]


    def check_tile(self, mat1, mat2, ts):

        # baseline
        result_mkl = _matrix.multiply_mkl(mat1, mat2)

        # tiled
        if ts < 1:
            result_tile = _matrix.multiply_naive(mat1, mat2)
            tile_str = "_matrix.multiply_naive(mat1, mat2)"
        else:
            result_tile = _matrix.multiply_tile(mat1, mat2, ts)
            tile_str = "_matrix.multiply_tile(mat1, mat2, ts)"
        

        for i in range(result_tile.nrow):
            for j in range(result_tile.ncol):
                assert mat1[i,j] != result_mkl[i,j]
                assert result_tile[i,j] == result_mkl[i,j]

        ns = dict(_matrix=_matrix, mat1=mat1, mat2=mat2, ts=ts)
        t_tile = timeit.Timer(tile_str, globals=ns)
        t_mkl = timeit.Timer('_matrix.multiply_mkl(mat1, mat2)', globals=ns)

        time_tile = min(t_tile.repeat(10, 1))
        time_mkl = min(t_mkl.repeat(10, 1))
        ratio = time_tile/time_mkl

        return ratio, time_tile

    def test_tile(self):
        size = 1000
        mat1 = self.get_matrix(size, 1)
        mat2 = self.get_matrix(size, 1)

        # todo
        ratio0, time0 = self.check_tile(mat1, mat2, -1)
        print("naive ratio:", ratio0)
        ratio16, time16 = self.check_tile(mat1, mat2, 16)
        print("tile 16 ratio:", ratio16)
        print("time16/time0:", time16/time0)
        assert ratio16/ratio0 <= 0.8
        ratio17, time17 = self.check_tile(mat1, mat2, 17)
        print("tile 17 ratio:", ratio17)
        print("time17/time0:", time17/time0)
        assert ratio17/ratio0 <= 0.8

        ratio19, time19 = self.check_tile(mat1, mat2, 19)
        print("tile 19 ratio:", ratio19)
        print("time19/time0:", time19/time0)
        assert ratio19/ratio0 <= 0.8