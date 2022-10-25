#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <iostream>

double degree_of_vector(std::vector<double> v1, std::vector<double> v2) {
    if(v1.size() != 2 || v2.size() != 2) throw std::invalid_argument( "vectors length have to be 2\n" );
    double dot = v1[0]*v2[0] + v1[1]*v2[1];
    double det = v1[0]*v2[1] - v1[1]*v2[0];
    return abs(atan2(det, dot));
}

PYBIND11_MODULE(_vector, m){
    m.doc() = "vector angle calculate";
    m.def("degree_of_vector", &degree_of_vector, "calculate degree");
}