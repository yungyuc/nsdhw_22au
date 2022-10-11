import _vector as vector
import math
import pytest


def test_zero_angle():
    x = [1, 0]
    y = [1, 0]
    print(vector.angle(x, y))
    print(math.pi / 180 * 0.0)
    assert math.isclose(vector.angle(x, y),  math.pi / 180 * 0.0)


def test_zero_length_vector():
    with pytest.raises(Exception) as err:
        vector.angle([ 0, 0],[ 0, 1])


def test_right_angle_90():
    x = [0, 1]
    y = [1, 0]
    assert math.isclose(vector.angle(x, y),  math.pi / 180 * 90 )


def test_another_angle_45():
    x = [1, 0]
    y = [1, 1]
    print(vector.angle(x, y))
    print( math.pi / 180 * 45)
    assert math.isclose(vector.angle(x, y),  math.pi / 180 * 45)
