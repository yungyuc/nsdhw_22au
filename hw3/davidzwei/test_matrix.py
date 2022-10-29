import math
import pytest
import timeit
import random
import _matrix


def make_matrices(size):
    mat1 = _matrix.Matrix(size,size)
    mat2 = _matrix.Matrix(size,size)

    for it in range(size):
        for jt in range(size):
            mat1[it, jt] = it * size + jt + 1
            mat2[it, jt] = it * size + jt + 1

    return mat1, mat2

def test_function():
    size = 1000
    tile_size = 16
    m1, m2 = make_matrices(size)
    
    matrix_naive = _matrix.multiply_naive(m1, m2)
    matrix_tile = _matrix.multiply_tile(m1, m2, tile_size)
    matrix_mkl = _matrix.multiply_mkl(m1, m2)
    
    assert matrix_naive == matrix_tile
    assert matrix_tile == matrix_mkl


def test_time():
    size = 1000
    tile_size = 16
    m1, m2 = make_matrices(size)
    
    init = dict(_matrix=_matrix, _m1 = m1, _m2= m2, tile_size = tile_size)
    time_naive = timeit.timeit("_matrix.multiply_naive(_m1, _m2)", number=1, globals=init)
    time_tile = timeit.timeit("_matrix.multiply_tile(_m1, _m2, tile_size)", number=1, globals=init)
    time_mkl = timeit.timeit("_matrix.multiply_mkl(_m1, _m2)", number=1, globals=init)

    with open("performance.txt", "w") as f:
        f.write(f"multiply_naive: {time_naive:.4f} seconds.\n")
        f.write(f"multiply_tile: {time_tile:.4f} seconds.\n")
        f.write(f"multiply_mkl: {time_mkl:.4f} seconds.\n")
