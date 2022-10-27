from _vector import angle
import pytest
import math

def test_zero_length():
    v1 = [0.0,0.0]
    v2 = [0.0,0.0]
    assert math.isclose(angle(v1,v2),-1)

def test_zero_angle():
    v1 = [1.0,1.0]
    v2 = [12.0,12.0]
    assert math.isclose(angle(v1,v2),0)

def test_right_angle():
    v1 = [1.0,0.0]
    v2 = [0.0,1.0]
    assert math.isclose(angle(v1,v2),math.pi/2)

def test_angle():
    v1 = [1.0,1.0]
    v2 = [0.0,2.0]
    assert math.isclose(angle(v1,v2),math.pi/4)
