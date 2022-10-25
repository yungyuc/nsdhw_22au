#!/usr/bin/env python3
import pytest
from _vector import calAngle
import numpy as np
import math

def test_zero_len():
    with pytest.raises(ValueError):
        calAngle([1,1], [0,0])
    with pytest.raises(ValueError):
        calAngle([0,0], [1,1])
    with pytest.raises(ValueError):
        calAngle([0,0], [0,0])

def test_angle_90():
    for i in range (10):
        val1 = np.random.randint(1,10)
        vec1 = np.array([val1, val1])
        mul = np.random.randint(1,10)
        vec2 = vec1 * np.array([-mul, mul])
        assert calAngle(vec1, vec2) == pytest.approx(math.pi / 2)

def test_angle_0():
    for i in range (10):
        vec1 = np.random.randint(1, 10, size = 2)
        mul = np.random.randint(1, 10)
        vec2 = vec1 * mul
        assert calAngle(vec1, vec2) == pytest.approx(0)

def test_angle_180():
    for i in range (10):
        vec1 = np.random.randint(1, 10, size = 2)
        mul = np.random.randint(-9, 0)
        vec2 = vec1 * mul
        assert calAngle(vec1, vec2) == pytest.approx(math.pi)