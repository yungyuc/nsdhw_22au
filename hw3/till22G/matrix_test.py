import _matrix
import time

def create_matrices(size):

	m1 = _matrix.Matrix(size, size)
	m2 = _matrix.Matrix(size, size)
	m3 = _matrix.Matrix(size, size)

	for i in range (0, size):
		for j in range (0, size):
			m1[i, j] = i * size + j + 1
			m2[i, j] = i * size + j + 1
			m3[i, j] = 0

	return m1, m2, m3

def naive_tiling_match():

	size = 1000
	m1, m2, m3 = create_matrices(size)

	res_naive = _matrix.multiply_naive(m1, m2)
	res_tiling = _matrix.multiply_tile(m1, m2)

	equal = True
	for i in range(res_naive.nrow):
		for j in range(res_naive.ncol):
			if res_naive[i,j] != res_tiling[i,j]:
				equal = False

	return equal

def testing_naive_tiling_equal():
	assert naive_tiling_match() == True


def timing_naive_and_tiling():

	size = 1000
	m1, m2, m3 = create_matrices(size)

	start_time_naive = time.process_time()
	res_naive = _matrix.multiply_naive(m1, m2)
	end_time_naive = time.process_time()
	total_cpu_runtime_naive = end_time_naive - start_time_naive

	print("Naive approach took : " + str(total_cpu_runtime_naive) + " seconds (cpu runtime)")

	start_time_tile = time.process_time()
	res_tiling = _matrix.multiply_tile(m1, m2)
	end_time_tile = time.process_time()
	total_cpu_runtime_tile = end_time_tile - start_time_tile

	print("Tiling approach took : " + str(total_cpu_runtime_tile) + " seconds (cpu runtime)")

	proportion = total_cpu_runtime_tile / total_cpu_runtime_naive

	print("Tiling takes : " + str("%.2f" % (proportion * 100)) + "% of the naive approach")
	return proportion

def test_timing_naive_and_tiling():
	assert timing_naive_and_tiling() < 0.8

timing_naive_and_tiling()
