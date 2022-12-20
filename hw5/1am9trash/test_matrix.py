import unittest
import pytest
import _matrix

class MatrixTest(unittest.TestCase):
    def setUp(self):
        self.l_m = _matrix.Matrix(100, 800)
        self.l_m_copy = _matrix.Matrix(100, 800)
        self.r_m = _matrix.Matrix(800, 500)
        self.r_m_copy = _matrix.Matrix(800, 500)
        for r in range(100):
            for c in range(800):
                self.l_m[r, c] = r * 800 + c
                self.l_m_copy[r, c] = r * 800 + c
        for r in range(800):
            for c in range(500):
                self.r_m[r, c] = r * 500 + c
                self.r_m_copy[r, c] = r * 500 + c
    
    def testSetter(self):
        self.l_m[0, 0] = 100
        self.assertEqual(100, self.l_m[0, 0])

    def testGetter(self):
        self.assertEqual(100, self.l_m.nrow)
        self.assertEqual(800, self.l_m.ncol)
    
    def testEqual(self):
        self.assertTrue(self.l_m == self.l_m_copy)
    
    def testNotEqual(self):
        self.assertTrue(self.l_m != self.r_m)
        buf = self.l_m_copy[99, 799]
        self.l_m_copy[99, 799] = -1
        self.assertTrue(self.l_m != self.l_m_copy)
        self.l_m_copy[99, 799] = buf
    
    def testMultiplyNaive(self):
        mkl_m = _matrix.multiply_mkl(self.l_m, self.r_m)
        naive_m = _matrix.multiply_naive(self.l_m, self.r_m)
        self.assertEqual(naive_m, mkl_m)

    def testMultiplyTile(self):
        mkl_m = _matrix.multiply_mkl(self.l_m, self.r_m)
        tile_m_16 = _matrix.multiply_tile(self.l_m, self.r_m, 16)
        tile_m_32 = _matrix.multiply_tile(self.l_m, self.r_m, 32)
        tile_m_512 = _matrix.multiply_tile(self.l_m, self.r_m, 512)
        tile_m_1024 = _matrix.multiply_tile(self.l_m, self.r_m, 1024)
        self.assertEqual(tile_m_16, mkl_m)
        self.assertEqual(tile_m_32, mkl_m)
        self.assertEqual(tile_m_512, mkl_m)
        self.assertEqual(tile_m_1024, mkl_m)
