#include <pybind11/pybind11.h>
#include <cmath>

namespace py = pybind11;

struct Vector2
{
    Vector2(double a, double b): x(a), y(b) {}
    double x;
    double y;
};

double get_angle(const Vector2 &a, const Vector2 &b)
{
    double aDotB = a.x * b.x + a.y * b.y;
    double aDotA = a.x * a.x + a.y * a.y;
    double bDotB = b.x * b.x + b.y * b.y;
    double angle = std::acos(aDotB / std::sqrt(aDotA * bDotB));
    return angle;
}

PYBIND11_MODULE(_vector, m)
{
    py::class_<Vector2>(m, "Vector2")
        .def(py::init<double, double>())
        .def_readwrite("x", &Vector2::x)
        .def_readwrite("y", &Vector2::y);
    m.def("get_angle", &get_angle,
          "A function that calculates the angle (in radians) "
          "between two vectors");
}
