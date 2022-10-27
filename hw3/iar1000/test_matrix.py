import _matrix
import math
import pytest
import timeit
import numpy as np


class TestMatrix:
    def get_matrix(self, size, factor):
        mat = _matrix.Matrix(size, size)

        counter = 1
        for i in range(size):
            for j in range(size):
                mat[i, j] = counter * factor
                counter = counter + 1

        return mat

    # from validation.py
    def test_basic(self):
        size = 100
        mat1 = self.get_matrix(size, 1)
        mat2 = self.get_matrix(size, 1)
        mat3 = self.get_matrix(size, 0)

        assert size == mat1.nrow, "size error"
        assert size == mat1.ncol, "size error"
        assert size == mat2.nrow, "size error"
        assert size == mat2.ncol, "size error"
        assert size == mat3.nrow, "size error"
        assert size == mat3.ncol, "size error"

        print(f"mat values: {mat1[0, 0]}, {mat1[1, 0]}, {mat1[size-1, size-1]}")
        assert 1 == mat1[0, 0]
        assert size + 1 == mat1[1, 0]
        assert size * size == mat1[size-1, size-1]

        for i in range(mat1.nrow):
            for j in range(mat1.ncol):
                assert np.isclose(0, mat3[i, j])
                assert np.isclose(mat1[i, j], mat2[i, j])

        assert mat1 == mat2, "same matrix error"
        assert mat1 is not mat2, "same matrix reference error"


    def test_zero(self):
        size = 100
        mat1 = self.get_matrix(size, 0)
        mat2 = self.get_matrix(size, 1)

        print("start naive")
        mat3_naive = _matrix.multiply_naive(mat1, mat2)
        print("works naive")
        mat3_mkl = _matrix.multiply_mkl(mat1, mat2)
        print("works mkl")

        for i in range(size):
            for j in range(size):
                assert np.isclose(0, mat3_naive[i, j])
                assert np.isclose(0, mat3_mkl[i, j])


    def check_tile(self, mat1, mat2, ts):

        print("check tiling starts")
        # baseline
        result_mkl = _matrix.multiply_mkl(mat1, mat2)

        print("mkl 1")
        # tiled
        if ts < 1:
            result_tile = _matrix.multiply_naive(mat1, mat2)
            tile_str = "_matrix.multiply_naive(mat1, mat2)"
        else:
            result_tile = _matrix.multiply_tile(mat1, mat2, ts)
            tile_str = "_matrix.multiply_tile(mat1, mat2, ts)"
        

        print("check tile here")
        for i in range(result_tile.nrow):
            for j in range(result_tile.ncol):
                assert mat1[i,j] != result_mkl[i,j]
                assert result_tile[i,j] == result_mkl[i,j]

        print("check tile there")
        ns = dict(_matrix=_matrix, mat1=mat1, mat2=mat2, ts=ts)
        t_tile = timeit.Timer(tile_str, globals=ns)
        t_mkl = timeit.Timer('_matrix.multiply_mkl(mat1, mat2)', globals=ns)

        print("check tile what")
        time_tile = min(t_tile.repeat(10, 1))
        time_mkl = min(t_mkl.repeat(10, 1))
        ratio = time_tile/time_mkl

        return ratio, time_tile

    def test_tile(self):
        size = 1000
        mat1 = self.get_matrix(size, 1)
        mat2 = self.get_matrix(size, 1)

        # todo
        ratio0, time0 = self.check_tile(mat1, mat2, 0)
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
    mkl = timeit.Timer('_matrix.multiply_mkl(mat1, mat2)', setup=setup)
    repeat = 10

    with open('performance.txt', 'w') as fobj:

        w = Writer([sys.stdout, fobj])

        naivesec = minsec = min(naive.repeat(repeat=repeat, number=1))
        w.write(f'multiply_naive (repeat={repeat}) in min = {minsec} seconds\n')

        mklsec = minsec = min(mkl.repeat(repeat=repeat, number=1))
        w.write(f'multiply_mkl (repeat={repeat}) in min = {minsec} seconds\n')

        w.write('speed-up: %g x\n' % (naivesec/mklsec))

if __name__ == '__main__':
    benchmark()