import pytest
import math
from _vector import calc_angle


def test_orthogonality():
    a1 = calc_angle(0, 1, 1, 0)
    a2 = calc_angle(0, 1, -1, 0)
    a3 = calc_angle(0, -1, 1, 0)
    a4 = calc_angle(0, -1, -1, 0)

    print(f"angles= {a1}, {a2}, {a3}, {a4}")

    assert a1 == pytest.approx(math.pi / 2)
    assert a2 == pytest.approx(math.pi / 2)
    assert a3 == pytest.approx(math.pi / 2)
    assert a4 == pytest.approx(math.pi / 2)


def test_perpendicular():
    a1 = calc_angle(1, 1, 10, 10)

    print(f"angle= {a1}")

    assert a1 == pytest.approx(0)


def test_zero_vector_1():
    a1 = calc_angle(0, 0, 1, 1)

    print(f"angle= {a1}")

    assert a1 == pytest.approx(-1)


def test_zero_vector_2():
    a1 = calc_angle(1, 1, 0, 0)

    print(f"angle= {a1}")

    assert a1 == pytest.approx(-1)


def test_diagonal():
    a1 = calc_angle(1, 1, 0, 1)

    print(f"angle= {a1}")

    assert a1 == pytest.approx(math.pi/4)
