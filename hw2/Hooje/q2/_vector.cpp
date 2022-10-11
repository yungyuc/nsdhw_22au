#include <pybind11/pybind11.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <pybind11/stl.h>
namespace py = pybind11;
using namespace std;

float get_angle(vector<float>v1, vector<float>v2)
{
    float v_dot = abs(v1[0]*v2[0] + v1[1]*v2[1]);
    float len_product = sqrt(v1[0] * v1[0] + v1[1] * v1[1]) * sqrt(v2[0] * v2[0] + v2[1] * v2[1]);

    return acos(v_dot/len_product);
}

PYBIND11_MODULE(_vector, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring

    m.def("get_angle", &get_angle, "A function which can cal angles between two angles");
  

}