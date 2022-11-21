import pytest
import numpy
import time
import _matrix

def valid_res(size, mat_py, mat):
	for i in range(size):
		for j in range(size):
			assert(mat_py[i, j] == pytest.approx(mat[i, j]))
	return 

def time_cost(size):
	mat1_py = numpy.random.random((size, size))
	mat2_py = numpy.random.random((size, size))

	time_bulidin = time.time()
	res_py = numpy.matmul(mat1_py, mat2_py)
	time_bulidin = time.time() - time_bulidin	
	
	#using the class Matrix for the rest of the test
	mat1 = _matrix.Matrix(size,size)
	mat2 = _matrix.Matrix(size,size)
	for it in range(size):
		for jt in range(size):
			mat1[it, jt] = mat1_py[it, jt]
			mat2[it, jt] = mat2_py[it, jt]

	time_naive = time.time()
	res_naive = _matrix.multiply_naive(mat1, mat2)
	time_naive = time.time() - time_naive

	time_tile = time.time()
	res_tile = _matrix.multiply_tile(mat1, mat2, 16)
	time_tile = time.time() - time_tile

	time_mkl = time.time()
	res_mkl = _matrix.multiply_mkl(mat1, mat2)
	time_mkl = time.time() - time_mkl

	return time_bulidin, time_naive, time_tile, time_mkl

def test_performance():
	time_bulidin, time_naive, time_tile, time_mkl = time_cost(1000)

	file = open("performance.txt", "w")
	file.write("##Time cost##\n")
	file.write("-Naive :" + str(time_naive) + "\n")
	file.write("-Tile :" + str(time_tile) + "\n")
	file.write("-Mlk :" + str(time_mkl) + "\n")
	file.write("-Python bulidin :" + str(time_bulidin) + "\n")
	file.close()
	return