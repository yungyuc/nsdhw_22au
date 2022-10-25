import pytest
import math
from _vector import angle

def test_zero_len():
    with pytest.raises(RuntimeError) as excinfo:
        vector_angle = angle([0.0, 0.0], [0.0, 0.0])

def test_zero_angle():
    vector_angle = angle([1.0, 1.0], [1.0, 1.0])
    assert pytest.approx(vector_angle) == pytest.approx(0.0)

def test_right_angle():
    vector_angle = angle([1.0, 0.0], [0.0, 1.0])
    assert pytest.approx(vector_angle) == pytest.approx(math.radians(90))

def test_any_angle():
    vector_angle = angle([3.14, 2.5], [4.7, 0.2])
    assert pytest.approx(vector_angle) == pytest.approx(0.629879)

test_zero_len()