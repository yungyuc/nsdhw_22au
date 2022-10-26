#include<vector>
#include<iostream>
#include<cmath>

#include <pybind11/stl.h>
#include<pybind11/pybind11.h>

namespace py = pybind11;

float const calculate_angle (std::vector<float> &x, std::vector<float> &y) {

	// ceck dimensions of vectors
	if ( x.size() != x.size() || x.size() != 2){
		std::cout << "Size does not match, returned 100" << std::endl;
		return 100;
   	}

	// calculate dot product
	float dot_prod = 0;
	for(size_t i = 0; i < x.size(); i++){
		dot_prod += x[i] * y[i]; 
	}

	// calculate magnitude
	float mag_x = 0;
	for(size_t i = 0; i < x.size(); i++){
		mag_x += pow(x[i], 2);
	}
	mag_x = sqrt(mag_x);

	float mag_y = 0;
	for(size_t i = 0; i < y.size(); i++){
		mag_y += pow(y[i], 2);
	}
	mag_y = sqrt(mag_y);

	// calculate radiance
	float tmp = dot_prod / (mag_x * mag_y);

	// c++ acos returns radiance by default
	return acos((tmp));
}


PYBIND11_MODULE(_vector, handle){
	handle.doc() =  "";
	handle.def("calculate_angle_cpp", &calculate_angle);
}
