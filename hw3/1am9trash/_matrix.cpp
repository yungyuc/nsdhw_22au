#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <mkl.h>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <vector>
#include "_matrix.h"

namespace py = pybind11;

Matrix::Matrix() {
    m_nrow = 0;
    m_ncol = 0;
    m = nullptr;
}

Matrix::Matrix(int nrow, int ncol) {
    m_nrow = 0;
    m_ncol = 0;
    reset_buffer(nrow, ncol);
}

Matrix::Matrix(Matrix const &other) {
    reset_buffer(other.get_nrow(), other.get_ncol());
    for (int r = 0; r < other.get_nrow(); r++) {
        for (int c = 0; c < other.get_ncol(); c++) {
            (*this)(r, c) = other(r, c);
        }
    }
}

Matrix::~Matrix() {
    reset_buffer(0, 0);
}

inline double Matrix::operator()(int nrow, int ncol) const {
    // check_inbound(nrow, ncol);
    return *(m + nrow * m_ncol + ncol);
}

inline double &Matrix::operator()(int nrow, int ncol) {
    // check_inbound(nrow, ncol);
    return *(m + nrow * m_ncol + ncol);
}

bool Matrix::operator==(Matrix const &other) const {
    if (get_nrow() != other.get_nrow() || get_ncol() != other.get_ncol())
        return false;
    for (int r = 0; r < get_nrow(); r++) {
        for (int c = 0; c < get_ncol(); c++) {
            if ((*this)(r, c) != other(r, c))
                return false;
        }
    }
    return true;
}

bool Matrix::operator!=(Matrix const &other) const {
    return !((*this) == other);
}

double const *Matrix::get_buffer() const {
    return m;
}

double *Matrix::get_buffer() {
    return m;
}

inline int const &Matrix::get_nrow() const {
    return m_nrow;
}

inline int const &Matrix::get_ncol() const {
    return m_ncol;
}

void Matrix::display_matrix() const {
    std::cout << "[\n";
    for (int r = 0; r < get_nrow(); r++) {
        for (int c = 0; c < get_ncol(); c++) {
            std::cout << " " << std::setw(6) << (*this)(r, c);
        }
        std::cout << "\n";
    }
    std::cout << "]\n";
}

void Matrix::reset_buffer(int nrow, int ncol) {
    if (m != nullptr) {
        delete[] m;
        m = nullptr;
    }
    if (nrow > 0 && ncol > 0) {
        m_nrow = nrow;
        m_ncol = ncol;
        m = new double[nrow * ncol]();
        memset(m, 0, sizeof(double) * nrow * ncol);
    } else {
        m_nrow = 0;
        m_ncol = 0;
    }
}

void Matrix::check_inbound(int nrow, int ncol) const {
    if (nrow < 0 || nrow >= get_nrow() || ncol < 0 || ncol >= get_ncol())
        throw std::out_of_range("Not inbound");
}

void check_multipliable(Matrix const &l_m, Matrix const &r_m) {
    if (l_m.get_ncol() != r_m.get_nrow())
        throw std::out_of_range("Not multipliable");
}

Matrix multiply_naive(Matrix const &l_m, Matrix const &r_m) {
    check_multipliable(l_m, r_m);

    Matrix ret(l_m.get_nrow(), r_m.get_ncol());
    for (int i = 0; i < ret.get_nrow(); i++) {
        for (int j = 0; j < ret.get_ncol(); j++) {
            for (int k = 0; k < l_m.get_ncol(); k++) {
                ret(i, j) += l_m(i, k) * r_m(k, j);
            }
        }
    }
    return ret;
}

void multiply_block(Matrix &m, Matrix const &l_m, Matrix const &r_m, int row, int col, int index, int tile_size) {
    for (int i = row; i < std::min(l_m.get_nrow(), row + tile_size); i++) {
        for (int k = index; k < std::min(l_m.get_ncol(), index + tile_size); k++) {
            for (int j = col; j < std::min(r_m.get_ncol(), col + tile_size); j++) {
                m(i, j) += l_m(i, k) * r_m(k, j);
            }
        }
    }
}

Matrix multiply_tile(Matrix const &l_m, Matrix const &r_m, int tile_size) {
    check_multipliable(l_m, r_m);

    Matrix ret(l_m.get_nrow(), r_m.get_ncol());
    for (int i = 0; i < l_m.get_nrow(); i += tile_size) {
        for (int j = 0; j < r_m.get_ncol(); j += tile_size) {
            for (int k = 0; k < l_m.get_ncol(); k += tile_size) {
                multiply_block(ret, l_m, r_m, i, j, k, tile_size);
            }
        }
    }
    return ret;
}

Matrix multiply_mkl(Matrix const &l_m, Matrix const &r_m) {
    // mkl_set_num_threads(1);

    Matrix ret(l_m.get_nrow(), r_m.get_ncol());
    cblas_dgemm(
        CblasRowMajor,
        CblasNoTrans,
        CblasNoTrans,
        l_m.get_nrow(),
        r_m.get_ncol(),
        l_m.get_ncol(),
        1.0,
        l_m.get_buffer(),
        l_m.get_ncol(),
        r_m.get_buffer(),
        r_m.get_ncol(),
        0.0,
        ret.get_buffer(),
        ret.get_ncol()
    );
    return ret;
}

PYBIND11_MODULE(_matrix, m) {
    m.doc() = "c++ matrix class with optimization";

    py::class_<Matrix>(m, "Matrix")
        .def(py::init<>())
        .def(py::init<int, int>())
        .def(py::init<Matrix const &>())
        .def_property_readonly("nrow", &Matrix::get_nrow)
        .def_property_readonly("ncol", &Matrix::get_ncol)
        .def("__eq__", &Matrix::operator==)
        .def("__ne__", &Matrix::operator!=)
        .def("__getitem__", [](Matrix const &m, const std::vector<size_t> index) {
            return m(index[0], index[1]);
        })
        .def("__setitem__", [](Matrix &m, const std::vector<size_t> index, const double value) {
            m(index[0], index[1]) = value;
        });

    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_tile", &multiply_tile);
    m.def("multiply_mkl", &multiply_mkl);
}
