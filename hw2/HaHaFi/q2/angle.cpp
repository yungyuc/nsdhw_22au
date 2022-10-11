#include <cstdint>
#include <vector>
#include <cmath>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <stdexcept>

namespace py = pybind11;
// v1 : x,y  v2 : x,y
double angle(std::vector<double> v1, std::vector<double> v2)
{
    if (v1.size() < 2 || v2.size() < 2)
    {
        throw std::invalid_argument("each vector should have at least two value");
    }
    if ((v1[0] == 0 && v1[1] == 0) || (v2[0] == 0 && v2[1] == 0))
    {
        throw std::invalid_argument("received negative value");
    }

    double r1 = std::atan2(v2[1], v2[0]);
    double r2 = std::atan2(v1[1], v1[0]);

    return r2 > r1 ? r2 - r1 : r1 - r2;
}

PYBIND11_MODULE(_vector, m)
{
    m.doc() = "pratice angle"; // optional module docstring

    m.def("angle", &angle, "A function that calculate the angle with two vector which size is two (x, y)");
}