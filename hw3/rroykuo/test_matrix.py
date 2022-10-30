import _matrix
import time
import timeit
import pytest

def test_init():
    size = 1000
    mat1 = _matrix.Matrix(size, size)
    mat2 = _matrix.Matrix(size, size)
    for it in range(size):
        for jt in range(size):
            mat1[it, jt] = it * size + jt + 1
            mat2[it, jt] = it * size + jt + 1
                

    assert size == mat1.nrow
    assert size == mat1.ncol
    assert size == mat2.nrow
    assert size == mat2.ncol

    assert 2 == mat1[0,1]
    assert size+2 == mat1[1,1]
    assert size*2 == mat1[1, size-1]
    assert size*size == mat1[size-1, size-1]
    

def test_time():
    size = 1000
    tile_size = 16
    mat1 = _matrix.Matrix(size, size)
    mat2 = _matrix.Matrix(size, size)
    for it in range(size):
        for jt in range(size):
            mat1[it, jt] = it * size + jt + 1
            mat2[it, jt] = it * size + jt + 1

    init = dict(_matrix=_matrix, _mat1 = mat1, _mat2= mat2, tile_size = tile_size)
    time_naive = timeit.timeit("_matrix.multiply_naive(_mat1, _mat2)", number=1, globals=init)
    time_tile = timeit.timeit("_matrix.multiply_tile(_mat1, _mat2, tile_size)", number=1, globals=init)
    time_mkl = timeit.timeit("_matrix.multiply_mkl(_mat1, _mat2)", number=1, globals=init)

    with open("performance.txt", "w") as f:
        f.write(f"multiply_naive: {time_naive:.4f} seconds.\n")
        f.write(f"multiply_tile: {time_tile:.4f} seconds.\n")
        f.write(f"multiply_mkl: {time_mkl:.4f} seconds.\n")