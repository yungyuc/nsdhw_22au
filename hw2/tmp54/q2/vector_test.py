import pytest
from _vector import Vector

EPSILON = 1e-6

POINTS = [
    (0, 0),
    (1, 2),
    (3, 4),
    (5, 6),
]

VECTORS = [Vector(x, y) for x, y in POINTS]

LENGTH = [
    0.0,
    2.236067,
    5.0,
    7.810249,
]


def approx(n):
    return pytest.approx(n, abs=EPSILON)


class TestVector:
    @pytest.mark.parametrize(
        'vector,expected',
        zip(VECTORS, LENGTH),
    )
    def test_length(self, vector, expected):
        assert vector.length() == approx(expected)

    @pytest.mark.parametrize(
        'vector_a,vector_b,expected',
        [
            (VECTORS[0], VECTORS[1], 0.0),
            (VECTORS[1], VECTORS[2], 11.0),
            (VECTORS[2], VECTORS[3], 39.0),
        ]
    )
    def test_dot(self, vector_a, vector_b, expected):
        assert vector_a.dot(vector_b) == approx(expected)
        assert vector_b.dot(vector_a) == approx(expected)

    @pytest.mark.parametrize(
        'vector_a,vector_b,expected',
        [
            (VECTORS[1], VECTORS[2], 0.179853),
            (VECTORS[2], VECTORS[3], 0.051237),
        ]
    )
    def test_angle_between(self, vector_a, vector_b, expected):
        assert vector_a.angle_between(vector_b) == approx(expected)
        assert vector_b.angle_between(vector_a) == approx(expected)
