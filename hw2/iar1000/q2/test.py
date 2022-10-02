import pytest
import math
from angles import calc_angle

def test_orthogonality():
	a1 = calc_angle(0,1,1,0)
	a2 = calc_angle(0,1,-1,0)
	a3 = calc_angle(0,-1,1,0)
	a4 = calc_angle(0,-1,-1,0)

	print(f"angles= {a1}, {a2}, {a3}, {a4}")

	assert math.isclose((math.pi/2),a1,rel_tol=0.0001)
	assert math.isclose((math.pi/2),a2,rel_tol=0.0001)
	assert math.isclose((math.pi/2),a3,rel_tol=0.0001)
	assert math.isclose((math.pi/2),a4,rel_tol=0.0001)

def test_perpendicular():
	a1 = calc_angle(1,1,10,10)

	print(f"angle= {a1}")

	assert math.isclose(0,a1,rel_tol=0.0001)

def test_zero_vector_1():
	a1 = calc_angle(0,0,1,1)

	print(f"angle= {a1}")

	assert math.isclose(-1,a1,rel_tol=0.0001)

def test_zero_vector_2():
	a1 = calc_angle(1,1,0,0)

	print(f"angle= {a1}")

	assert math.isclose(-1,a1,rel_tol=0.0001)


def test_diagonal():
	a1 = calc_angle(1,1,0,1)

	print(f"angle= {a1}")

	assert math.isclose(math.pi/4,a1,rel_tol=0.0001)

