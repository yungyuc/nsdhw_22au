import pytest
from _vector import calculate_angle
import numpy as np
from random import random
import math 

def test_zero_length():
    angle = calculate_angle([0., 0.], [0., 0.])

    assert np.isnan(angle)

def test_zero_angle():
    angle = calculate_angle([1., 1.], [1., 1.])
    
    assert angle == pytest.approx(0.)

def test_right_angle():
    angle = calculate_angle([1., 0.], [0., 1.])
    
    assert angle == pytest.approx(math.pi/2)

def test_zero_angle():
    v0 = [0.7027418321943354, 0.9206890990594353]
    v1 = [0.8805969890497817, 0.0048339040196602]

    angle = calculate_angle(v0, v1)

    assert angle == pytest.approx(0.913362)

