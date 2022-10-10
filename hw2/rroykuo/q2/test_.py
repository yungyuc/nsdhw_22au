from _vector import *
import pytest
import math


def test_zero_angle():
  assert angle([1,1], [1,1]) == pytest.approx(0)
  
def test_right_angle():
  assert angle([0,1], [1,0]) == pytest.approx(math.pi / 2)
  
def test_zero_lenth():
  with pytest.raises(RuntimeError):
    angle([0,0], [0,0])