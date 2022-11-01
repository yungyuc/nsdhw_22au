import timeit
import _matrix
import pytest

def get_matrices(size):
    mat1 = _matrix.Matrix(size, size)
    mat2 = _matrix.Matrix(size, size)

    for i in range(size):
        for j in range(size):
            mat1[i, j] = i * size + j + 1
            mat2[i, j] = i * size + j + 1

    return mat1, mat2

def test_matrix_multiplication():
    size = 1000
    tile_size = 16
    mat1, mat2 = get_matrices(size)

    matrix_naive = _matrix.multiply_naive(mat1, mat2)
    matrix_tile = _matrix.multiply_tile(mat1, mat2, tile_size)
    matrix_mkl = _matrix.multiply_mkl(mat1, mat2)

    assert matrix_naive == matrix_tile and matrix_tile == matrix_mkl


def test_performance():
    size = 1000
    tile_size = 16
    mat1, mat2 = get_matrices(size)

    init = dict(_matrix=_matrix, _m1 = mat1, _m2= mat2, tile_size = tile_size)
    time_naive = timeit.timeit("_matrix.multiply_naive(_m1, _m2)", number=1, globals=init)
    time_tile = timeit.timeit("_matrix.multiply_tile(_m1, _m2, tile_size)", number=1, globals=init)
    time_mkl = timeit.timeit("_matrix.multiply_mkl(_m1, _m2)", number=1, globals=init)

    with open("performance.txt", "w") as f:
        print("multiply_naive: {time_naive:.4f} seconds.\n", file=f)
        print(f"multiply_tile: {time_tile:.4f} seconds.\n",file=f)
        print(f"multiply_mkl: {time_mkl:.4f} seconds.\n",file=f)