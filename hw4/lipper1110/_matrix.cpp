#include <cstddef>
#include <mkl.h>
#include <stdexcept>
#include <vector>
#include <pybind11/pybind11.h>
#include <iomanip>
#include <iostream>
#include "allocator.hpp"

static MyAllocator<double> allocator;

class Matrix {
public:
    Matrix(size_t nrow, size_t ncol)
        : _nrow(nrow), _ncol(ncol), _buffer(allocator) {
        _reset_buffer(nrow, ncol);
    }

    double operator[](std::pair<size_t, size_t> idx) const {
        if (idx.first > _nrow || idx.second > _ncol) {
            throw std::out_of_range("Out");
        }
        return _buffer[_index(idx.first, idx.second)];
    }

    double operator()(size_t row, size_t col) const {
        if (row > _nrow || col > _ncol) {
            throw std::out_of_range("Out");
        }
        return _buffer[_index(row, col)];
    }

    double& operator()(size_t row, size_t col) {
        if (row > _nrow || col > _ncol) {
            throw std::out_of_range("Out");
        }
        return _buffer[_index(row, col)];
    }

    bool operator==(Matrix& other) const {
        if (other.nrow() != _nrow && other.ncol() != _ncol) {
            throw std::length_error("row != nrow or col != ncol");
        }
        for (size_t i = 0; i < _nrow; i++) {
            for (size_t j = 0; j < _ncol; j++) {
                if (_buffer[_index(i, j)] != other(i, j))
                    return false;
            }
        }
        return true;
    }

    size_t nrow() const {
        return _nrow;
    }

    size_t ncol() const {
        return _ncol;
    }

    size_t size() const {
        return _nrow * _ncol;
    }

    void _reset_buffer(size_t nrow, size_t ncol) {
        _buffer.resize(nrow * ncol);
        _nrow = nrow;
        _ncol = ncol;
    }

    size_t _index(size_t row, size_t col) const {
        return row * _ncol + col;
    }

    size_t _nrow = 0, _ncol = 0;
    std::vector<double, MyAllocator<double>> _buffer;
};

Matrix multiply_naive(Matrix const& mat1, Matrix const& mat2) {
    if (mat1.ncol() != mat2.nrow()) {
        throw std::out_of_range("mat1.col != mat2.row");
    }
    Matrix ret(mat1.nrow(), mat2.ncol());
    for (size_t i = 0; i < ret.nrow(); i++) {
        for (size_t k = 0; k < ret.ncol(); k++) {
            for (size_t j = 0; j < mat1.ncol(); j++) {
                ret(i, k) += mat1(i, j) * mat2(j, k);
            }
        }
    }
    return ret;
}

Matrix multiply_tile(Matrix& mat1, Matrix& mat2, size_t tsize) {
    if (mat1.ncol() != mat2.nrow()) {
        throw std::out_of_range("the number of first matrix column differs from that of second matrix row");
    }
    Matrix ret(mat1.nrow(), mat2.ncol());

    for (size_t row = 0; row < mat1.nrow(); row += tsize)
        for (size_t col = 0; col < mat2.ncol(); col += tsize)
            for (size_t inner = 0; inner < mat1.ncol(); inner += tsize)
                for (size_t i = row; i < std::min(mat1.nrow(), row + tsize); i++)
                    for (size_t j = col; j < std::min(mat2.ncol(), col + tsize); j++)
                        for (size_t k = inner; k < std::min(mat1.ncol(), inner + tsize); k++)
                            ret(i, j) += mat1(i, k) * mat2(k, j);
    return ret;
}

Matrix multiply_mkl(Matrix const& mat1, Matrix const& mat2) {
    if (mat1.ncol() != mat2.nrow()) {
        throw std::out_of_range("mat1.col != mat2.row");
    }
    Matrix ret(mat1.nrow(), mat2.ncol());
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                mat1.nrow(), mat2.ncol(), mat1.ncol(), 1.0, mat1._buffer.data(), mat1.ncol(),
                mat2._buffer.data(), mat2.ncol(), 0.0, ret._buffer.data(), ret.ncol());
    return ret;
}

size_t bytes() {
    return allocator.counter.bytes();
}

size_t allocated() {
    return allocator.counter.allocated();
}

size_t deallocated() {
    return allocator.counter.deallocated();
}

PYBIND11_MODULE(_matrix, m) {
    pybind11::class_<Matrix>(m, "Matrix").def(pybind11::init<size_t, size_t>()).def("__setitem__", [](Matrix& self, std::pair<size_t, size_t> idx, double num) { self(idx.first, idx.second) = num; }).def("__getitem__", &Matrix::operator[]).def("__eq__", &Matrix::operator==).def_property_readonly("nrow", &Matrix::nrow).def_property_readonly("ncol", &Matrix::ncol);
    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_tile", &multiply_tile);
    m.def("multiply_mkl", &multiply_mkl);
    m.def("bytes", &bytes);
    m.def("allocated", &allocated);
    m.def("deallocated", &deallocated);
}
