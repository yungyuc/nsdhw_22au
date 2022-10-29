import sys
from typing import List, TextIO
from _matrix import Matrix, multiply_naive, multiply_tile, multiply_mkl
import numpy as np
import timeit


def make_matrix(nrow: int, ncol: int, values: List[float] = []) -> Matrix:
    if len(values) == 0:
        values = np.zeros(nrow * ncol, dtype=np.float64)
    return Matrix(nrow, ncol, values)


def test_matrix():
    mat_1 = make_matrix(2, 2, [1, 2, 3, 4])

    assert mat_1.nrow == 2
    assert mat_1.ncol == 2
    assert mat_1.size() == 4

    for i in range(2):
        for j in range(2):
            assert mat_1[i, j] == i * 2 + j + 1

    mat_2 = make_matrix(100, 100)

    assert mat_2.nrow == 100
    assert mat_2.ncol == 100
    assert mat_2.size() == 10000


def test_multiply_naive():
    mat_1 = make_matrix(2, 2, [1, 2, 3, 4])
    mat_2 = make_matrix(2, 2, [1, 2, 3, 4])

    result = multiply_naive(mat_1, mat_2)

    assert result.nrow == 2
    assert result.ncol == 2
    assert result.size() == 4

    assert result[0, 0] == 7
    assert result[0, 1] == 10
    assert result[1, 0] == 15
    assert result[1, 1] == 22


def test_multiply_tile():
    mat_1 = make_matrix(2, 2, [1, 2, 3, 4])
    mat_2 = make_matrix(2, 2, [1, 2, 3, 4])

    result = multiply_tile(mat_1, mat_2, 32)

    assert result.nrow == 2
    assert result.ncol == 2

    assert result[0, 0] == 7
    assert result[0, 1] == 10
    assert result[1, 0] == 15
    assert result[1, 1] == 22


def test_multiply_mkl():
    mat_1 = make_matrix(2, 2, [1, 2, 3, 4])
    mat_2 = make_matrix(2, 2, [1, 2, 3, 4])

    result = multiply_mkl(mat_1, mat_2)

    assert result.nrow == 2
    assert result.ncol == 2

    assert result[0, 0] == 7
    assert result[0, 1] == 10
    assert result[1, 0] == 15
    assert result[1, 1] == 22


def test_benchmark():
    mat_1 = make_matrix(1000, 1000)
    mat_2 = make_matrix(1000, 1000)

    with open("performance.txt", "w") as f:
        w = Writer([sys.stdout, f])
        sys.stdout.write("\n")

        w.write("----- Performance Report -----\n")

        repeat_time: int = 5

        naive_timer = timeit.Timer(lambda: multiply_naive(mat_1, mat_2))
        min_naive_time = min(naive_timer.repeat(repeat=repeat_time, number=1))
        w.write(
            "naive: " + str(min_naive_time) + "\n")

        mkl_timer = timeit.Timer(lambda: multiply_mkl(mat_1, mat_2))
        w.write(
            "mkl: " + str(min(mkl_timer.repeat(repeat=repeat_time, number=1))) + "\n")

        w.write("\n")

        w.write("| tile size \t| time (s) \t\t| speedup (%) \t\t\t|\n")
        for tile_size in [8, 16, 24, 32, 64]:
            tile_timer = timeit.Timer(
                lambda: multiply_tile(mat_1, mat_2, tile_size))
            min_tile_time = min(tile_timer.repeat(
                repeat=repeat_time, number=1))
            w.write("| " + str(tile_size) + "\t\t| " + str(min_tile_time) +
                    "\t| " + str((min_naive_time - min_tile_time) / min_naive_time * 100) + "\t\t|\n")

        w.write("\n")


def test_eq():
    mat_1 = make_matrix(3, 3, [1, 2, 3, 4, 5, 6, 7, 8, 9])
    mat_2 = make_matrix(3, 3, [1, 2, 3, 4, 5, 6, 7, 8, 9])

    result_naive = multiply_naive(mat_1, mat_2)
    result_tile = multiply_tile(mat_1, mat_2, 32)
    result_mkl = multiply_mkl(mat_1, mat_2)

    # defined by &Matrix::operator==
    assert result_naive == result_tile
    assert result_naive == result_mkl

    with open("performance.txt", "a") as f:
        w = Writer([sys.stdout, f])
        sys.stdout.write("\n")

        w.write("----- Equality Report -----\n")
        w.write("naive == tile: " + str(result_naive == result_tile) + "\n")
        w.write("naive == mkl: " + str(result_naive == result_mkl) + "\n")


class Writer:
    def __init__(self, streams: List[TextIO]):
        self.streams = streams

    def write(self, msg: str):
        for stream in self.streams:
            stream.write(msg)
