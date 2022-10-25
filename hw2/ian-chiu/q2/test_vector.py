from _vector import *
import math

class TestClass:
    tolerance = 1e-6

    def test_one(self):
        """Test for zero-length 2-vector"""
        v1 = Vector2(0, 0)
        v2 = Vector2(0, 0)
        assert math.isnan(get_angle(v1, v2))

    def test_two(self):
        """Test for right angle (90-deg)"""
        v1 = Vector2(0, 1)
        v2 = Vector2(1, 0)
        angle = get_angle(v1, v2)
        assert math.isclose(angle, math.radians(90), rel_tol=self.tolerance), \
            "angle between (0, 1) and (1, 0) should be 0, " \
             f"but get_angle gives {angle}."

    def test_three(self):
        """Test for zero angle"""
        v1 = Vector2(1, 1)
        v2 = Vector2(1, 1)
        angle = get_angle(v1, v2)
        assert math.isclose(angle, math.radians(0), rel_tol=self.tolerance), \
            "angle between (1, 1) and (1, 1) should be 0, " \
             f"but get_angle gives {angle}."

    def test_four(self):
        """Test for one other angle"""
        v1 = Vector2(5, -2)
        v2 = Vector2(-4, 5)
        angle = get_angle(v1, v2)
        assert math.isclose(angle, 2.626043646130814, rel_tol=self.tolerance), \
            "angle between (1, 1) and (1, 1) should be 2.626043, " \
             f"but get_angle gives {angle}."
