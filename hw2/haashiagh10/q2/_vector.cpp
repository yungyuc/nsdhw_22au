# include <iostream>
# include <cmath>
# include <math.h>
# include <vector>
# include <pybind11/pybind11.h>
# include <pybind11/stl.h>


using namespace std;
namespace py = pybind11;

float angle (std::vector<float> v1, std::vector<float> v2)
{
	float length1 = v1[0]+v1[1];
	float length2 = v2[0]+v2[1];
	float dot_cal = v1[0]*v2[0]+v1[1]*v2[1];
	float angle = dot_cal/(length1*length2);
	return acos(angle);
}

PYBIND11_MODULE(_vector, m) {
    m.doc() = "pybind11 example "; // optional module docstring
    m.def("pyb11_cal_angle", &angle, "A function which calculate the angle of two vector");

}