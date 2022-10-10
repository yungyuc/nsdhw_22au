import _vector
from _vector import calculateAngle
import pytest
from pytest import approx
import math
import pybind11

def test_0_d():
    assert calculateAngle([1.0, 2.0], [3.0, 6.0]) == 0.0

def test_45_d():
  assert (calculateAngle([4.0, 0.0], [1.0, 1.0])) == approx(math.pi/4)

def test_90_d():
  assert (calculateAngle([1.0, 0.0], [0.0, 1.0])) == approx(math.pi/2)

def test_180_d():
  assert (calculateAngle([3.0, 0.0], [-3.0, 0.0])) == approx(math.pi)

def test_err1():
  with pytest.raises(Exception):
    assert (calculateAngle([0.0, 0.0], [2.0, 6.0])) == 0.0

def test_err2():
  with pytest.raises(Exception):
    assert (calculateAngle([-6.0, -2.0], [0.0, 0.0])) == 0.0