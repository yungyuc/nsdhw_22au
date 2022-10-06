import pytest
import math
import _vector

def test_for_zero_length_vector():
    v1 = []
    v2 = []
    with pytest.raises(ValueError):
        _vector.cal_angle(v1, v2)

def test_for_zero_angle():
    v1 = [1, 0]
    v2 = [1, 0]
    angle = _vector.cal_angle(v1, v2)
    assert math.isclose(angle, 0) == True

def test_for_right_angle():
    v1 = [1, 0]
    v2 = [0, 1]
    angle = _vector.cal_angle(v1, v2)
    assert math.isclose(angle, (math.pi/180)*90) == True

def test_for_45_degree_angle():
    v1 = [1, 0]
    v2 = [1, 1]
    angle = _vector.cal_angle(v1, v2)
    assert math.isclose(angle, (math.pi/180)*45) == True

def test_for_120_degree_angle():
    v1 = [1, 0]
    v2 = [-1, math.sqrt(3)]
    angle = _vector.cal_angle(v1, v2)
    assert math.isclose(angle, (math.pi/180)*120) == True