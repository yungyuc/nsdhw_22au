from _vector import calculates_angle
import math
import pytest
from pytest import approx

# Test for zero-length 2-vector (invalid input)
def testiv():
    assert (calculates_angle([0.0, 0.0], [4.0, 0.0])) == 0.0
# Test for zero angle
def test0():
    assert (calculates_angle([2.0, 0.0], [4.0, 0.0])) == 0.0
#Test for right angle (90-deg)
def test45():
    assert (calculates_angle([4.0, 0.0], [1.0, 1.0])) == approx(math.pi/4)
# Test for zero angle
def test90():
    assert (calculates_angle([1.0, 0.0], [0.0, 1.0])) == approx(math.pi/2)

# Test for one other angle
def test180():
    assert (calculates_angle([-1.0, 0.0], [1.0, 0.0])) == approx(math.pi)

