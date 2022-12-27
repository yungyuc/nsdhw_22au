#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/numpy.h>
#include "matrix.hpp"

namespace py = pybind11;

PYBIND11_MODULE(_matrix, m){
    py::class_<Matrix>(m, "Matrix", py::buffer_protocol())
        .def(py::init<const size_t, const size_t>())
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol)
        .def("assign", &Matrix::operator=)
        .def("__setitem__", 
            [](Matrix &mat, std::pair<size_t, size_t> idx, double val){
                return mat(idx.first, idx.second) = val;
            })
        .def("__getitem__", 
            [](Matrix &mat, std::pair<size_t, size_t> idx){
                return mat(idx.first, idx.second);
            })
        .def("__eq__", 
            [](const Matrix &mat1, const Matrix &mat2){
                return mat1 == mat2;
            })
        .def_property("array",
                     [](Matrix &mat){
                        return py::array_t<double, py::array::c_style | py::array::forcecast>(
                            { mat.ncol(), mat.ncol() },
                            { sizeof(double) * mat.ncol(), sizeof(double) },
                            mat.buffer(),
                            py::cast(mat)
                        );
                     },
                     nullptr, 
                     py::return_value_policy::move
                     )
    ;

    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_tile",  &multiply_tile);
    m.def("multiply_mkl",   &multiply_mkl);
}