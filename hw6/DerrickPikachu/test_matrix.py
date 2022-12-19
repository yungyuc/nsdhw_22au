import unittest

import _matrix


class MatrixTest(unittest.TestCase):
    def setUp(self):
        self.mat1 = _matrix.Matrix(500, 500)
        self.mat2 = _matrix.Matrix(500, 500)
        for r in range(500):
            for c in range(500):
                self.mat1[r, c] = r * 500 + c
                self.mat2[r, c] = r * 500 + c
    
    def testGetter(self):
        self.assertEqual(500, self.mat1.nrow)
        self.assertEqual(500, self.mat1.ncol)
        self.assertEqual(500, self.mat2.nrow)
        self.assertEqual(500, self.mat2.ncol)
    
    def testEqual(self):
        self.assertTrue(self.mat1 == self.mat2)
    
    def testNotEqual(self):
        mat3 = _matrix.Matrix(500, 500)
        for r in range(500):
            for c in range(500):
                mat3[r, c] = r + c * 500
        self.assertTrue(self.mat1 != mat3)
    
    def testAccessor(self):
        self.mat1[200, 200] = 5487
        self.assertEqual(self.mat1[200, 200], 5487)
    
    def testMultiplyNaive(self):
        naive_res = _matrix.multiply_naive(self.mat1, self.mat2)
        mkl_res = _matrix.multiply_mkl(self.mat1, self.mat2)
        self.assertEqual(naive_res, mkl_res)
    
    def testMultiplyTile(self):
        tile_res = _matrix.multiply_tile(self.mat1, self.mat2, 64)
        mkl_res = _matrix.multiply_mkl(self.mat1, self.mat2)
        self.assertEqual(tile_res, mkl_res)
