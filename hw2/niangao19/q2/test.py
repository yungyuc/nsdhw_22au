from _vector import ca
import math
import pytest
from pytest import approx
 
# Test for zero-length 2-vector
def test0():
    assert (calculateAngle([2.0, 0.0], [4.0, 0.0])) == 0.0
#Test for right angle (90-deg)
def test45():
    assert (calculateAngle([4.0, 0.0], [1.0, 1.0])) == approx(math.pi/4)
# Test for zero angle
def test3():
    assert (calculateAngle([1.0, 0.0], [0.0, 1.0])) == approx(math.pi/2)

# Test for one other angle
def test4():
    assert (calculateAngle([-1.0, 0.0], [1.0, 0.0])) == approx(math.pi)

