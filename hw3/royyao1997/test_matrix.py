#!/usr/bin/env python3
import _matrix
import unittest
import random
import math
import time


class MyTest(unittest.TestCase):
    def make_matrices(self, size):

        mat1 = _matrix.Matrix(size, size)
        mat2 = _matrix.Matrix(size, size)
        mat3 = _matrix.Matrix(size, size)

        for it in range(size):
            for jt in range(size):
                mat1[it, jt] = it * size + jt + 1
                mat2[it, jt] = it * size + jt + 1
                mat3[it, jt] = 0

        return mat1, mat2, mat3
    
    def test_match_naive_mkl(self):

        size = 100
        mat1, mat2, *_ = self.make_matrices(size)

        ret_naive = _matrix.multiply_naive(mat1, mat2)
        ret_mkl = _matrix.multiply_mkl(mat1, mat2)

        self.assertEqual(size, ret_naive.nrow)
        self.assertEqual(size, ret_naive.ncol)
        self.assertEqual(size, ret_mkl.nrow)
        self.assertEqual(size, ret_mkl.ncol)

        for i in range(ret_naive.nrow):
            for j in range(ret_naive.ncol):
                self.assertNotEqual(mat1[i,j], ret_mkl[i,j])
                self.assertEqual(ret_naive[i,j], ret_mkl[i,j])

    def test_match_tile_mkl(self):

        size = 100
        mat1, mat2, *_ = self.make_matrices(size)
        ret_mkl = _matrix.multiply_mkl(mat1, mat2)
        ret_tile = _matrix.multiply_tile(mat1, mat2, 4)

        self.assertEqual(size, ret_tile.nrow)
        self.assertEqual(size, ret_tile.ncol)
        self.assertEqual(size, ret_mkl.nrow)
        self.assertEqual(size, ret_mkl.ncol)

        for i in range(ret_tile.nrow):
            for j in range(ret_tile.ncol):
                self.assertNotEqual(mat1[i, j], ret_mkl[i, j])
                self.assertEqual(ret_tile[i, j], ret_mkl[i, j])

    def test_performance(self):
        size = 1000
        mat1, mat2, *_ = self.make_matrices(1000)
        tile = random.randint(10, 100)
        st = time.time()
        ret_naive = _matrix.multiply_naive(mat1, mat2)
        naive_time = time.time() - st

        st = time.time()
        ret_tile = _matrix.multiply_tile(mat1, mat2, tile)
        tile_time = time.time() - st

        st = time.time()
        ret_mkl = _matrix.multiply_mkl(mat1, mat2)
        mkl_time = time.time() - st

        f = open('performance.txt', 'w')
        f.writelines(['tile = ', repr(tile), '\nnaive ', repr(naive_time), ' s.\ntile ',
                      repr(tile_time), ' s\nmkl', repr(mkl_time), ' s.\ntile/naive = ',
                      repr(tile_time / naive_time)])
        f.close()
        self.assertLess(tile_time / naive_time, 0.8)


if __name__ == '__main__':
    unittest.main()