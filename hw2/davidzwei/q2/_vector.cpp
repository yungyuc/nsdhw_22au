#include <iostream>
#include <vector>
#include <math.h>
#include <pybind11/stl.h>
#include <pybind11/pybind11.h>

double cal_angle(std::vector<float> v1, std::vector<float> v2)
{
    // zero length
    if ((!v1[0] && !v1[1]) || (!v2[0] && !v2[1]))
        throw std::invalid_argument("Invalid input");

    double len1 = sqrt(pow(v1[0], 2) + pow(v1[1], 2));
    double len2 = sqrt(pow(v2[0], 2) + pow(v2[1], 2));
    double dot = v1[0] * v2[0] + v1[1] * v2[1];
    double angle = dot / (len1 * len2);
    return acos(angle);
}

PYBIND11_MODULE(_vector, m)
{
    m.doc() = "pybind11 example";
    m.def("py_cal_angle", &cal_angle, "calculate angle");
}
