import pytest
from _vector import multiply_naive, multiply_tiling, multiply_mkl

def test_0():
    assert multiply_naive(mat1, mat2) == multiply_tiling(mat1, mat2)

