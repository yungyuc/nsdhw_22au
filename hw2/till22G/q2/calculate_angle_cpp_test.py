

import numpy as np
from _vector import calculate_angle_cpp
import math

def test_zero_vectors() -> float:
	x = np.array([])
	y = np.array([])
	tmp = calculate_angle_cpp(x, y)
	assert math.isclose(tmp, 100, rel_tol=1e-5)

def test_opposite_vectors() -> float:
	x = np.array([1, 0])
	y = np.array([-1, 0])
	tmp = calculate_angle_cpp(x, y)
	assert math.isclose(tmp, math.pi, rel_tol=1e-5)

def test_right_angle() -> float:
	x = np.array([0, 1])
	y = np.array([1, 0])
	tmp = calculate_angle_cpp(x, y)
	assert math.isclose(tmp, (math.pi/2), rel_tol=1e-5)

def test_same_vectors() -> float:
	x = np.array([1, 0])
	y = np.array([1, 0])
	tmp = calculate_angle_cpp(x, y)
	assert math.isclose(tmp, 0, rel_tol=1e-5)

def test_same_vectors_2() -> float:
	x = np.array([0, 1])
	y = np.array([0, 1])
	tmp = calculate_angle_cpp(x, y)
	assert math.isclose(tmp, 0, rel_tol=1e-5)

def test_different_vectors() -> float:
	x = np.array([-2, 5])
	y = np.array([4, 3])
	tmp = calculate_angle_cpp(x, y)
	assert math.isclose(tmp, 1.3078 , rel_tol=1e-5)

def test_wrong_dimension_vectors() -> float:
	x = np.array([3])
	y = np.array([8])
	tmp = calculate_angle_cpp(x, y)
	assert math.isclose(tmp, 100 , rel_tol=1e-5)

def test_wrong_dimension_vectors_2() -> float:
	x = np.array([3, 8, 1])
	y = np.array([8, 2, 8, 4])
	tmp = calculate_angle_cpp(x, y)
	assert math.isclose(tmp, 100 , rel_tol=1e-5)

