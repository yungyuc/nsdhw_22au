import pytest
import _matrix as M
import numpy as np
import timeit


def test_int_multiply():
    A = M.Matrix(5,10)
    a_value = np.random.randint(100,size=(5,10))
    A.load(a_value)

    B = M.Matrix(10,3)
    b_value = np.random.randint(100,size=(10,3))
    B.load(b_value)

    result_numpy = M.Matrix(5,3)
    result_numpy.load(np.dot(a_value,b_value))
    result_naive = M.multiply_naive(A,B)
    result_tile = M.multiply_tile(A,B,8)
    result_mkl = M.multiply_mkl(A,B)

    assert(result_numpy==result_naive)
    assert(result_numpy==result_tile)
    assert(result_numpy==result_mkl)

def test_memory():
    alloc_init = M.allocated()
    dealloc_init = M.deallocated()

    A = M.Matrix(5,10)
    size_A = 8*5*10
    assert M.bytes() == size_A
    assert M.allocated() == alloc_init + size_A
    assert M.deallocated() == dealloc_init

    B = M.Matrix(10,3)
    size_B = 8*10*3
    assert M.bytes() == size_A + size_B
    assert M.allocated() == alloc_init + size_A + size_B
    assert M.deallocated() == dealloc_init

    A = None
    assert M.bytes() == size_B
    assert M.allocated() == alloc_init + size_A + size_B
    assert M.deallocated() == dealloc_init + size_A

    B = None
    assert M.bytes() == 0
    assert M.allocated() == alloc_init + size_A + size_B
    assert M.deallocated() == dealloc_init + size_A + size_B