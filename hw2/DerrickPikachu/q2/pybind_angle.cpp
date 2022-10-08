#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>

PYBIND11_MAKE_OPAQUE(std::vector<float>);

#include "angle.h"

namespace py = pybind11;

PYBIND11_MODULE(angle, m) {
    m.doc() = "pybind c++ angle";

    m.def("angle_between", &dimension2::angle_between, "A function to calculate angle within two vector");

    py::bind_vector<std::vector<float>>(m, "VectorFloat");
}

