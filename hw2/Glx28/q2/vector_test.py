import pytest
import math
from angles import calc_angle


def orthogonality():
    angle1, angle2, angle3, angle4 = (
        calc_angle(0, 1, 1, 0),
        calc_angle(0, 1, -1, 0),
        calc_angle(0, -1, 1, 0),
        calc_angle(0, -1, -1, 0),
    )

    half_pi = math.pi / 2
    rel = 0.0001
    assert math.isclose(half_pi, angle1, rel)
    assert math.isclose(half_pi, angle2, rel)
    assert math.isclose(half_pi, angle3, rel)
    assert math.isclose(half_pi, angle4, rel)


def perpendicular():
    angle1 = calc_angle(1, 1, 10, 10)
    print(f"angle= {angle1}")
    assert math.isclose(0, angle1, rel)


def zero_vector_1():
    angle1 = calc_angle(0, 0, 1, 1)
    print(f"angle= {angle1}")
    assert math.isclose(-1, angle1, rel)


def zero_vector_2():
    angle1 = calc_angle(1, 1, 0, 0)
    print(f"angle= {angle1}")
    assert math.isclose(-1, a1, rel)


def diagonal():
    angle1 = calc_angle(1, 1, 0, 1)
    print(f"angle= {angle1}")
    assert math.isclose(math.pi / 4, angle1, rel_tol)
