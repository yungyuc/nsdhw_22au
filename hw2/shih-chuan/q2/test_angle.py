from _vector import Vector, angle2vec
import math

class TestClass:
    # Test for zero-length 2-vector
    def test1(self):
        v1 = Vector(0, 0)
        v2 = Vector(0, 0)
        assert math.isnan(angle2vec(v1, v2))
    #Test for right angle (90-deg)
    def test2(self):
        v1 = Vector(0, 1)
        v2 = Vector(1, 0)
        assert abs(angle2vec(v1, v2) - math.radians(90)) <= 1e-6
    # Test for zero angle
    def test3(self):
        v1 = Vector(0, 1)
        v2 = Vector(0, 1)
        assert abs(angle2vec(v1, v2) - math.radians(0))<= 1e-6
    # Test for one other angle
    def test4(self):
        v1 = Vector(4, 0)
        v2 = Vector(4, 3)
        assert abs(angle2vec(v1, v2) - 0.6435011087932843) <= 1e-6