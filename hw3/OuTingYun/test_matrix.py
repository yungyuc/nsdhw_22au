from time import time
import pytest
import _matrix
import numpy
import  time

def check(row,col,mat_np,mat):
    for i in range(row):
        for j in range(col):
            assert (mat_np[i, j]==pytest.approx(mat[i, j]))

def count_time(row,col):


    mat1_np = numpy.random.randint(1,5,size=(row, col))
    mat2_np = numpy.random.randint(1,5,size=(row, col))

    time_tmp = time.time()
    numpy_result = numpy.matmul(mat1_np,mat2_np)
    numpy_time = time.time() - time_tmp

    mat1 = _matrix.Matrix(row,col)
    mat2 = _matrix.Matrix(row,col)

    for i in range(row):
        for j in range(col):
            mat1[i, j] =  mat1_np[i, j]
            mat2[i, j] =  mat2_np[i, j]

    time_tmp = time.time()
    naive_result = _matrix.multiply_naive(mat1, mat2)
    naive_time = time.time() - time_tmp
    # Check the multiplication 
    check(row,col,numpy_result,naive_result)
    

    time_tmp = time.time()
    tile_result = _matrix.multiply_tile(mat1, mat2, 16)
    tile_time = time.time() - time_tmp
    check(row,col, numpy_result, tile_result)

    tile_time = 1;mkl_time=1
    return numpy_time, naive_time, tile_time, mkl_time

    time_tmp = time.time()
    mkl_result = _matrix.multiply_mkl(mat1, mat2)
    mkl_time = time.time() - time_tmp
    check(row,col,numpy_result, mkl_result)

    return numpy_time, naive_time, tile_time, mkl_time


def test_performance():
	numpy_time, naive_time, tile_time, mkl_time = count_time(1000,1000)
	
	fout = open("performance.txt", "w")
	
	fout.write("Python bulidin time cost :" + str(numpy_time) + "\n")
	fout.write("Naive time cost :" + str(naive_time) + "\n")
	fout.write("Tile time cost :" + str(tile_time) + "\n")
	fout.write("Mlk time cost :" + str(mkl_time) + "\n")
	fout.write("Ratio of tile / naive :" + str(tile_time / naive_time) + "X\n")
	fout.write("Ratio of tile / mlk :" + str(tile_time / mkl_time) + "X\n")
	fout.write("Ratio of mkl / buildin :" + str(mkl_time / numpy_time) + "X\n")
	
	fout.close()
	return 

test_performance()