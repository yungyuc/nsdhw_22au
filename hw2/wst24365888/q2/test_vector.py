from _vector import Vector, get_angle
import math


def test_vector_constructor():
    try:
        v = Vector(1, 2)
        assert True
    except:
        assert False


def test_get_angle_0():
    v1 = Vector(1, 0)
    v2 = Vector(1, 0)

    assert abs(get_angle(v1, v2) - math.radians(0)) < 1e-6


def test_get_angle_90():
    v1 = Vector(1, 0)
    v2 = Vector(0, 1)

    assert abs(get_angle(v1, v2) - math.radians(90)) < 1e-6


def test_get_angle_180():
    v1 = Vector(1, 0)
    v2 = Vector(-1, 0)

    assert abs(get_angle(v1, v2) - math.radians(180)) < 1e-6


def test_get_angle_270():
    v1 = Vector(1, 0)
    v2 = Vector(0, -1)

    assert abs(get_angle(v1, v2) - abs(math.radians(-90))) < 1e-6
