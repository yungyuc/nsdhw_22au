import pytest
import math
import _vector
from _vector import get_angle
from pytest import approx as ap
class Testclass:
    def test_zero(self):
        v1 = [0, 1]
        v2 = [0, 1]
        assert get_angle(v1, v2) == ap(0.0)
    def test_ninety(self):
        v1 = [0, 1]
        v2 = [1, 0]
        assert get_angle(v1, v2)*180/math.pi == ap(90.0)
    def test_fourtyfive(self):
        v1 = [0, 1]
        v2 = [1, 1]
        assert  get_angle(v1, v2)*180/math.pi == ap(45)
