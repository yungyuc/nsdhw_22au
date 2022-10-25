import pytest
import _vector

radians = 57.2957795

def test_zeroLength():
    with pytest.raises(Exception) as e_info:
        _vector.angle([0,0],[0,1])
    with pytest.raises(Exception) as e_info:
        _vector.angle([0,1],[0,0])

def test_zeroAngle():
    assert _vector.angle([0,1],[0,5]) == 0

def test_rightAngle():
    assert abs(_vector.angle([0,1],[1,0]) - 90 / radians) < 1e-2

def test_180Angle():
    assert abs(_vector.angle([0,1],[0,-1]) - 180 / radians) < 1e-2

def test_345Angle():
    assert abs(_vector.angle([3,4],[1,0]) - 53.13 / radians) < 1e-2
