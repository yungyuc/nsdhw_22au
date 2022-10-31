import _vector
import pytest
import math

def test_zeroLength():
    with pytest.raises(Exception) as err:
        _vector.angle([0,0],[0,1])
    with pytest.raises(Exception) as err:
        _vector.angle([0,1],[0,0])

def test_90_degree():
    a = [1, 0]
    b = [0, 1]
    assert math.isclose(_vector.angle(a,b), math.pi / 180 * 90 ) == True
def test_0_degree():
    a = [1, 0]
    b = [1, 0]
    assert math.isclose(_vector.angle(a,b), 0) == True
def test_45_degree():
    a = [1, 0]
    b = [1, 1]
    assert math.isclose(_vector.angle(a,b), math.pi / 180 * 45) == True