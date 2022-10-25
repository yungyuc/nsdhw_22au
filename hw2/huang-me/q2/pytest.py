import _vector
import math
rel_tol=1e-06

def test_right_angle():
	v1=[1,0]
	v2=[0,1]
	angle=_vector.GetAngle(v1,v2)
	print('angle=',angle)
	assert math.isclose((math.pi/2),angle,rel_tol=rel_tol)

def test_zero_length():
	v1=[0,0]
	v2=[1,1]
	angle=_vector.GetAngle(v1,v2)
	assert math.isclose(-1,angle,rel_tol=rel_tol)

def test_zero_angle():
	v1=[2,2]
	v2=[1,1]
	angle=_vector.GetAngle(v1,v2)
	assert math.isclose(0,angle,rel_tol=rel_tol)

def test_other_length():
	ans=math.acos((15+48)/float(5*13))
	v1=[3,4]
	v2=[5,12]
	angle=_vector.GetAngle(v1,v2)
	assert math.isclose(ans,angle,rel_tol=rel_tol)
