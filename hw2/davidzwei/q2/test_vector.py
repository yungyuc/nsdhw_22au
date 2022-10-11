import math
import pytest
import _vector
from _vector import py_cal_angle

def test_zero_length():
    with pytest.raises(Exception) as exception:
        v1 = [0.0, 0.0]
        v2 = [0.0, 0.0]
        angle = _vector.py_cal_angle(v1, v2)
        
def test_zero_angle():
    v1 = [0.0, 1.0]
    v2 = [0.0, 2.0]
    angle = _vector.py_cal_angle(v1, v2)
    angle = angle*180/math.pi
    print(angle)
    assert math.isclose(angle, 0.0)

def test_right_angle():
    v1 = [0.0, 1.0]
    v2 = [1.0, 0.0]
    angle = _vector.py_cal_angle(v1, v2)
    angle = angle*180/math.pi
    assert math.isclose(angle, 90.0)

def test_another_angle():
    v1 = [1.0, 0.0]
    v2 = [-1.0, 1.0]
    angle = _vector.py_cal_angle(v1, v2)
    angle = angle*180/math.pi
    assert math.isclose(angle, 135.0)

def test_another_angle_2():
    v1 = [1.0, 0.0]
    v2 = [3.0, 4.0]
    angle = _vector.py_cal_angle(v1, v2)
    angle = angle*180/math.pi
    assert math.isclose(angle, 53.13010235415598)
