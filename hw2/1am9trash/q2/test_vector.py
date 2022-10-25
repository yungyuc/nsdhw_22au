import pytest
import _vector

eps = 1e-4

def deg_to_ang(deg):
    return deg / 57.295779

def test_0_length():
    with pytest.raises(Exception) as err:
        _vector.cal_angle([0, 0], [0, 1])
        _vector.cal_angle([0, 1], [0, 0])

def test_0_angle():
    assert abs(_vector.cal_angle([0, 1], [0, 1]) - deg_to_ang(0)) < eps

def test_45_Angle():
    assert abs(_vector.cal_angle([1, 0], [1, 1]) - deg_to_ang(45)) < eps
    
def test_90_angle():
    assert abs(_vector.cal_angle([1, 0], [0, 1]) - deg_to_ang(90)) < eps
