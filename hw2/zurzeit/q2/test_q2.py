import _vector
import math

def test_zero_angle():
	vec1 = _vector.pyVec(1,0,2,0)
	assert round(vec1.angle(),6) == round(math.atan2(0,1), 6)
	return
def test_right_angle():
	vec2 = _vector.pyVec(1,0,0,1)
	print(f'c++:{vec2.angle()}')
	print(f'python:{math.atan2(1,0)}')
	assert round(vec2.angle(),6) == round(math.atan2(1,0),6)
	return