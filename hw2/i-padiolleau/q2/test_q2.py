import pytest
from _vector import comp_angle
import math

def test_0():
    assert comp_angle(0, 0, 0, 0) == pytest.approx(-1)
def test_1():
    assert comp_angle(1, 1, 2, 2) == pytest.approx(0)

def test_2():
    assert comp_angle(0, 1, 1, 0) == pytest.approx(math.pi / 2)