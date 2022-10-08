import _vector
from _vector import get_angle
import pytest
import math

def test_0_degree():
    assert get_angle([1.0, 2.0], [3.0, 6.0]) == 0.0
def test_45_degree():
    assert get_angle([0.0, 9.0], [5.0, 5.0]) == pytest.approx(math.pi/4)
def test_90_degree():
    assert get_angle([0.0, 4.0], [3.0, 0.0]) == pytest.approx(math.pi/2)
def test_180_degree():
    assert get_angle([1.0, 3.0], [-2.0, -6.0]) == pytest.approx(math.pi)
def test_270_degree():
    assert get_angle([1.0, 2.0], [2.0, -1.0]) == pytest.approx(math.pi/2)

# test error situation
def test_error1():
    with pytest.raises(Exception):
        assert get_angle([0.0, 0.0], [2.0, 1.0]) == 0.0
def test_error2():
    with pytest.raises(Exception):
        assert get_angle([5.0, 2.0], [0.0, 0.0]) == 0.0
