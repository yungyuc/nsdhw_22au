from _matrix import Matrix, multiply_naive, multiply_tile, multiply_mkl
import pytest


def test_attributes():
    a = Matrix(2, 2)
    assert a.nrow == 2
    assert a.ncol == 2
    assert a.get_buffer() == [0, 0, 0, 0]


def test_accessor():
    a = Matrix(1, 1)
    assert a[0, 0] == 0
    a[0, 0] = 1
    assert a[0, 0] == 1


def test_comparison():
    a = Matrix(1, 1)
    b = Matrix(1, 1)
    c = Matrix(2, 2)
    assert a == b
    assert a != c

    b[0, 0] = 1
    assert a != b


def setup_matrix():
    a = Matrix(2, 2)
    b = Matrix(2, 2)
    c = Matrix(3, 3)

    a[0, 0], a[0, 1] = 1, 2
    a[1, 0], a[1, 1] = 3, 4

    b[0, 0], b[0, 1] = 1, 0
    b[1, 0], b[1, 1] = 0, 1

    return a, b, c


def test_multiply_naive():
    a, b, c = setup_matrix()

    assert multiply_naive(a, b) == a

    with pytest.raises(Exception):
        multiply_naive(a, c)


def test_multiply_tile():
    a, b, c = setup_matrix()

    assert multiply_tile(a, b, 8) == a

    with pytest.raises(Exception):
        multiply_tile(a, c, 8)


def test_multiply_mkl():
    a, b, c = setup_matrix()

    assert multiply_mkl(a, b) == a

    with pytest.raises(Exception):
        multiply_mkl(a, c)
