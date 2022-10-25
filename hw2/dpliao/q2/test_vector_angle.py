import pytest
import math
import _vector

def test_zero_length():
    with pytest.raises(ValueError) as Error:
        _vector.degree_of_vector([], [])
def test_zero_angle():
    assert _vector.degree_of_vector([1, 1], [2, 2]) == 0
def test_right_angle():
    assert _vector.degree_of_vector([0, 1], [1, 0]) == math.pi/2
def test_another_angle():
    assert _vector.degree_of_vector([0, 1], [1, 1]) == math.pi/4
