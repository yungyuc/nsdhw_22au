#include "_vector.hpp"
#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(_vector, m) {
    py::class_<Vector>(m, "Vector")
        .def(py::init<double, double>());
    
    m.def("get_angle", &get_angle);
}