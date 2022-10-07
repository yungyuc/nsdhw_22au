#!python3
import pytest
from  _vector import angle
from math import pi


class TestClass:
    def test_zero_len(self):
        with pytest.raises(ValueError):
            angle([0,0],[0,0])
        with pytest.raises(ValueError):
            angle([0,0],[1,2.3])
        with pytest.raises(ValueError):
            angle([2.3,1],[0,0])
    def test_zero_angle(self):
        # vector with same direction
        assert angle([1,0],[9,0]) == 0.0
        assert angle([1,4],[0.75,3]) == 0.0
        assert angle([0,1],[0,0.25]) == 0.0
    def test_ninety_angle(self):
        # 90 degree
        assert angle([1,0],[0,-2])==pi/2
        assert angle([1,0],[0,-8])==pi/2
    def test_other_angle(self):
        # 45 degree
        assert angle([1,0],[1,1])==pi/4
        # 180 degree
        assert angle([1,0],[-0.78,0])==pi



if __name__ =="__main__":
    retcode = pytest.main()
    

# import unittest
# class TestAngleFun(unittest.TestCase):
    
#     def test_zero_len(self):
#         with self.assertRaises(ValueError):
#             angle([0,0],[0,0])
#         with self.assertRaises(ValueError):
#             angle([0,0],[1,2.3])
#         with self.assertRaises(ValueError):
#             angle([2.3,1],[0,0])
#     def test_zero_angle(self):
#         # vector with same direction
#         self.assertEqual(angle([1,0],[9,0]) ,0.0)
#         self.assertEqual(angle([1,4],[0.75,3]) ,0.0)
#         self.assertEqual(angle([0,1],[0,0.25]) ,0.0)
#     def test_ninety_angle(self):
#         # 90 degree
#         self.assertEqual(angle([1,0],[0,-2]),pi/2)
#         self.assertEqual(angle([1,0],[0,-8]),pi/2)
#     def test_other_angle(self):
#         # 45 degree
#         self.assertEqual(angle([1,0],[1,1]),pi/4)
#         # 180 degree
#         self.assertEqual(angle([1,0],[-0.78,0]),pi)
