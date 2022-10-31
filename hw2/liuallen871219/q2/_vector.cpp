//syszux.cpp
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <cmath>

double angle(std::vector<double> v1, std::vector<double> v2){
    if(v1[0] == 0 && v1[1] == 0)
        throw std::invalid_argument( "zero-dim vector" );
    if(v2[0] == 0 && v2[1] == 0)
        throw std::invalid_argument( "zero-dim vector" );
    double dot = v1[0] * v2[0] + v1[1] * v2[1];
    double v1_len = sqrt(v1[0] * v1[0] + v1[1] * v1[1]);
    double v2_len = sqrt(v2[0] * v2[0] + v2[1] * v2[1]);
    double result = dot / (v1_len * v2_len);

    return acos(result);
}
PYBIND11_MODULE(_vector, m) {
    m.doc() = "angle betweem two 2-dims vectors"; // optional module docstring
    m.def("angle", &angle, "A function which adds two numbers");
}