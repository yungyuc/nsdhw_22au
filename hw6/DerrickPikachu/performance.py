#!/usr/bin/python3

import sys
import timeit

import _matrix

def make_matrix():
    mat = _matrix.Matrix(500, 500)
    for i in range(500):
        for j in range(500):
            mat[i, j] = i * 500 + j
    return mat

def main():
    file = open('performace.txt', 'w')
    mat1 = make_matrix()
    mat2 = make_matrix()
    ns = dict(_matrix=_matrix, mat1=mat1, mat2=mat2)
    naive_timer = timeit.Timer('_matrix.multiply_naive(mat1, mat2)', globals=ns)
    naive_time = naive_timer.timeit(1)
    file.writelines(f'naive multiply time: {naive_time}\n')
    tile_size = [16, 32, 64, 128, 256]
    for tsize in tile_size:
        ns = dict(_matrix=_matrix, mat1=mat1, mat2=mat2, tsize=tsize)
        tile_timer = timeit.Timer('_matrix.multiply_tile(mat1, mat2, tsize)', globals=ns)
        tile_time = tile_timer.timeit(1)
        file.writelines(f'tile size: {tsize}, time: {tile_time}\n')


if __name__ == "__main__":
    main()