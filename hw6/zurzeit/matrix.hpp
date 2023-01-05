#ifndef MATRIX_H
#define MATRIX_H

#include <math.h>
#include <iostream>
#include <vector>
#include <mkl.h>
#include <cstring>
#include <algorithm>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/numpy.h>
namespace py = pybind11;

class Matrix {

public:
    
    Matrix(); 
    Matrix(size_t nrow, size_t ncol);// constructor
    Matrix(size_t nrow, size_t ncol, std::vector<double> const & vec);// constructor with vector
    //copy assignment operator with vec
    Matrix & operator=(std::vector<double> const & vec);

    void setter(std::pair <size_t, size_t> key, double val);
    double getter(std::pair <size_t, size_t> key);
    // copy constructor
    Matrix(const Matrix & other);
    // copy assign operator
    Matrix & operator=(const Matrix & other);
    // move constructor
    Matrix( Matrix && other);
    // move assign operator
    Matrix & operator=(Matrix && other);
    ~Matrix();

    Matrix & operator+=(Matrix const & mat1);
    bool operator==(const Matrix & other)const;
    // // to locate the value
    double   operator() (size_t row, size_t col) const;
    double & operator() (size_t row, size_t col);
    Matrix row_maj_to_col_maj() const;
    size_t nrow() const;//return the number of row of the matrix
    size_t ncol() const;//return the number of col of the matrix

    size_t size() const;//return the number of element of the matrix

    void get_mat();
    double * get_buffer_pointer() const;
    double * m_buffer;
private:

    size_t index(size_t row, size_t col) const;

    void reset_buffer(size_t nrow, size_t ncol);
    
    size_t m_nrow;
    size_t m_ncol;
    
};

Matrix operator*(Matrix const & mat1, Matrix const & mat2);
Matrix multiply_naive(Matrix const & mat1, Matrix const & mat2);
Matrix multiply_tile(Matrix const & mat1, Matrix const & mat2, size_t tsize);
Matrix multiply_mkl(Matrix const & mat1, Matrix const & mat2);

PYBIND11_MODULE(_matrix, m) {
    m.doc() = "This is a class matrix.";
    py::class_<Matrix>(m, "Matrix")
        .def(py::init([](size_t nrow, size_t ncol) { return new Matrix(nrow, ncol); }))
        .def(py::init([](size_t nrow, size_t ncol, std::vector<double> const & vec) { return new Matrix(nrow, ncol, vec); }))
        .def("get_mat", &Matrix::get_mat)
        .def("__setitem__", [](Matrix & mat,std::pair<size_t,size_t> key, double val) { mat.setter(key,val); })
        .def("__getitem__", [](Matrix & mat,std::pair<size_t,size_t> key) { return mat.getter(key); })
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol)
        .def_property_readonly("array", [](Matrix &m) -> py::array_t<double>
        {
            return py::array_t<double>(
                { m.nrow(), m.ncol() },                              // Buffer dimensions
                { sizeof(double) * m.ncol(), sizeof(double) },       // Strides (in bytes) for each index
                m.get_buffer_pointer(),                              // Pointer to buffer
                py::cast(m)
            );
        })
	.def("__eq__", &Matrix::operator==)
        ;
    m.def("multiply_naive", &multiply_naive, "A function that multiply_naive");
    m.def("multiply_tile", &multiply_tile, "A function that multiply_tile");
    m.def("multiply_mkl", &multiply_mkl, "A function that multiply_mkl");
}

#endif
