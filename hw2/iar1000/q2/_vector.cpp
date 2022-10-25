#include <cmath>
#include <iostream>
#include <pybind11/pybind11.h>

namespace py = pybind11;

// calculate angle of two vectors a and b given formula θ = cos-1 [ (a · b) / (|a| |b|) ].
float calcAngle(const float x1, const float y1, const float x2, const float y2){
	float dot = x1*x2 + y1*y2;
	float lenOne = sqrt(x1*x1 + y1*y1);
	float lenTwo = sqrt(x2*x2 + y2*y2);

	// no zero-length vectors
	if(lenOne == 0 || lenTwo == 0){ return -1; };

	// range cos
	float c = dot / (lenOne * lenTwo);
	if (c > 1) { c = 1; }
	else if (c < -1) { c = -1; };

	return acos(c);
}

PYBIND11_MODULE(_vector, m) {
    m.doc() = "calculates the angle between two vectors";
    m.def("calc_angle", &calcAngle, "A function that calculates the angle between two vectors");
}
