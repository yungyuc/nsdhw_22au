import sys
import os
import pytest
from pytest import approx as ap # import approx
import timeit
import _matrix
import unittest
# from timeit import Timer

def create_matrices(size):
    mat1 = _matrix.Matrix(size, size)
    mat2 = _matrix.Matrix(size, size)
    mat3 = _matrix.Matrix(size, size)

    for i in range(size):
        for j in range(size):
            mat1[i, j] = i * size + j + 1
            mat2[i, j] = i * size + j + 1
            mat3[i, j] = 0

    return mat1, mat2

# def test_function():
#     size = 1000
#     tile_size = 16 # 16? 10?
#     m1, m2 = create_matrices(size)

#     matrix_naive = _matrix.multiply_naive(m1, m2)
#     matrix_tile = _matrix.multiply_tile(m1, m2, tile_size)
#     matrix_mkl = _matrix.multiply_mkl(m1, m2)

#     assert matrix_naive == matrix_tile
#     assert matrix_tile == matrix_mkl
def test_basic(self):
    size = 100
    self.assertEqual(0, _matrix.bytes())
    mat1, mat2, mat3, *_ =  self.create_matrices(size)
    self.assertEqual(3 * 8 * size * size, _matrix.bytes())
    base_alloc = _matrix.allocated()
    base_dealloc = _matrix.deallocated()

    self.assertEqual(size, mat1.nrow)
    self.assertEqual(size, mat1.ncol)
    self.assertEqual(size, mat2.nrow)
    self.assertEqual(size, mat2.ncol)
    self.assertEqual(size, mat3.nrow)
    self.assertEqual(size, mat3.ncol)

    self.assertEqual(size + 3, mat1[1, 2])
    self.assertEqual(5, mat1[0, 4])
    self.assertEqual(4 * size, mat1[3, size - 1])
    self.assertEqual(size * size, mat1[size - 1, size - 1])

    for i in range(mat1.nrow):
        for j in range(mat1.ncol):
            self.assertNotEqual(0, mat1[i,j])
            self.assertEqual(mat1[i,j], mat2[i,j])
            self.assertEqual(0, mat3[i,j])

    self.assertEqual(mat1, mat2)
    self.assertTrue(mat1 is not mat2)

    self.assertEqual(3 * 8 * size * size, _matrix.bytes())
    self.assertEqual(base_alloc, _matrix.allocated())
    self.assertEqual(base_dealloc, _matrix.deallocated())

def test_match_naive_mkl(self):
    size = 100
    self.assertEqual(0, _matrix.bytes())
    mat1, mat2, *_ =  self.create_matrices(size)
    self.assertEqual(3 * 8 * size * size, _matrix.bytes())
    base_alloc = _matrix.allocated()
    base_dealloc = _matrix.deallocated()

    result_naive = _matrix.multiply_naive(mat1, mat2)
    result_mkl = _matrix.multiply_mkl(mat1, mat2)
    self.assertEqual(size, result_naive.nrow)
    self.assertEqual(size, result_naive.ncol)
    self.assertEqual(size, result_mkl.nrow)
    self.assertEqual(size, result_mkl.ncol)

    for i in range(result_mkl.nrow):
        for j in range(result_mkl.ncol):
            self.assertEqual(result_mkl[i, j], result_naive[i, j])
            self.assertNotEqual(result_mkl[i, j], mat1[i, j])
            
    self.assertEqual(5 * 8 * size * size, _matrix.bytes())
    self.assertEqual(base_alloc + 2 * 8 * size * size, _matrix.allocated())
    self.assertEqual(base_dealloc, _matrix.deallocated())

def test_zero(self):
    size = 100
    self.assertEqual(0, _matrix.bytes())
    mat1, mat3, *_ =  self.create_matrices(size)
    self.assertEqual(3 * 8 * size * size, _matrix.bytes())
    base_alloc = _matrix.allocated()
    base_dealloc = _matrix.deallocated()

    result_naive = _matrix.multiply_naive(mat1, mat3)
    result_mkl = _matrix.multiply_mkl(mat1, mat3)
    self.assertEqual(size, result_naive.nrow)
    self.assertEqual(size, result_naive.ncol)
    self.assertEqual(size, result_mkl.nrow)
    self.assertEqual(size, result_mkl.ncol)

    for i in range(result_naive.nrow):
        for j in range(result_naive.ncol):
            self.assertEqual(0, result_mkl[i, j])
            self.assertEqual(0, result_naive[i, j])
    self.assertEqual(4 * 8 * size * size, _matrix.bytes())
    self.assertEqual(base_alloc + 2 * 8 * size * size, _matrix.allocated())
    self.assertEqual(base_dealloc, _matrix.deallocated())

def test_memory(self):

    size = 100
    self.assertEqual(0, _matrix.bytes())  
    base_alloc = _matrix.allocated()
    base_dealloc = _matrix.deallocated()
    mat1, mat2, mat3, *_ = self.make_matrices(size)

    self.assertEqual(3 * 8 * size * size, _matrix.bytes())
    # New allocation.
    self.assertEqual(base_alloc + 3 * 8 * size * size, _matrix.allocated())
    # No deallocation.
    self.assertEqual(base_dealloc, _matrix.deallocated())
    mat1 = mat2 = mat3 = None
    # Matrices are deallocated.
    self.assertEqual(0, _matrix.bytes())
    self.assertEqual(base_dealloc + 3 * 8 * size * size, _matrix.deallocated())
    self.assertEqual(base_alloc + 3 * 8 * size * size, _matrix.allocated())