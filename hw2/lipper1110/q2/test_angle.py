from _vector import *
import pytest
import math
import numpy as np

def test_zero_length():
    u = Coord(0., 0.)
    v = Coord(0., 0.)
    assert angle(u,v) == pytest.approx(math.pi)

def test_zero_angle():
    u = Coord(1., 1.)
    v = Coord(1., 1.)
    assert angle(u,v) == pytest.approx(0.)

def test_right_angle():
    u = Coord(0., 1.)
    v = Coord(1., 0.)
    assert angle(u,v) == pytest.approx(math.pi / 2)

def test_other_angle():
    u_np = np.random.rand(2)
    v_np = np.random.rand(2)
    u = Coord(u_np[0], u_np[1])
    v = Coord(v_np[0], v_np[1])
    assert angle(u, v) == pytest.approx(np.arccos(np.dot(u_np / np.linalg.norm(u_np), v_np / np.linalg.norm(v_np))), rel=1e-4)
