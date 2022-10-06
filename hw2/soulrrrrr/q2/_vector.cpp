#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <stdexcept>
#include <cmath>
namespace py = pybind11;

double cal_angle(const std::vector<double> &v1, const std::vector<double> &v2)
{
    if (v1.size() != 2 || v2.size() != 2)
        throw std::invalid_argument("Both vectors must have length 2");
    
    double v1_dot_v2 = v1[0] * v2[0] + v1[1] * v2[1];
    double v1_len = sqrt(v1[0] * v1[0] + v1[1] * v1[1]);
    double v2_len = sqrt(v2[0] * v2[0] + v2[1] * v2[1]);
    double angle = acos(v1_dot_v2 / (v1_len * v2_len));
    return angle;
}

PYBIND11_MODULE(_vector, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring

    m.def("cal_angle", &cal_angle, "Calculates the angle (in radians) between two vectors in the 2-dimensional Cartesian coordinate system");
}
