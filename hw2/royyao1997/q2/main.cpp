#include <iostream>
#include <stdexcept>
#include <math.h>
#include <pybind11/pybind11.h>

double get_radious(double x1, double y1, double x2, double y2){
	double n = std::sqrt(x1*x1 + y1*y1) * std::sqrt(x2*x2 + y2*y2);
    if(n == 0)
        throw std::runtime_error("Math error: Attempted to divide by Zero\n");
    return std::acos((x1*x2 + y1*y2)/n);
}

PYBIND11_MODULE(_vector,m){
    m.doc() = "test for pybind11";
    m.def("get_rad",&get_radious,"return angle(radious) between two vector in the 2-dimensional Cartesian coordinate system");
}