#include <cmath>
#include <pybind11/pybind11.h>

namespace py = pybind11;

class Vector
{
public:
    Vector(double x, double y): x{x}, y{y} {}
    double x;
    double y;
};

double angle2vec(const Vector &v1, const Vector &v2)
{
    double dotProd = v1.x * v2.x + v1.y * v2.y;
    double v1Mag = (v1.x * v1.x + v1.y * v1.y);
    double v2Mag = (v2.x * v2.x + v2.y * v2.y);
    return std::acos(dotProd / std::sqrt(v1Mag * v2Mag));
}

PYBIND11_MODULE(_vector, m)
{
    m.doc() = "pybind11 example plugin"; 
    py::class_<Vector>(m, "Vector")
        .def(py::init<double, double>())
        .def_readwrite("x", &Vector::x)
        .def_readwrite("y", &Vector::y);
    m.def("angle2vec", &angle2vec, "calculates the angle (in radians) between two vectors");
}