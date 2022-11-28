#include "custom_allocator.hpp"
#include <mkl.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <stdexcept>
#include <vector>

CustomAllocator<double> allocator;
size_t bytes() {
    return allocator.counter.bytes();
}

size_t allocated() {
    return allocator.counter.allocated();
}

size_t deallocated() {
    return allocator.counter.deallocated();
}

class Matrix {
public:
    Matrix(std::size_t nrow, std::size_t ncol)
        : m_nrow(nrow)
        , m_ncol(ncol)
        , m_buffer(allocator) {
        m_buffer.resize(nrow * ncol);
    }

    bool operator==(Matrix const& other) const {
        if (m_nrow != other.m_nrow || m_ncol != other.m_ncol) {
            return false;
        }

        for (std::size_t i = 0; i < m_nrow * m_ncol; ++i) {
            if (m_buffer[i] != other.m_buffer[i]) {
                return false;
            }
        }

        return true;
    }

    bool operator!=(Matrix const& other) const {
        return !(*this == other);
    }

    double operator()(std::size_t row, std::size_t col) const { return m_buffer[index(row, col)]; }
    double& operator()(std::size_t row, std::size_t col) { return m_buffer[index(row, col)]; }

    std::size_t nrow() const { return m_nrow; }
    std::size_t ncol() const { return m_ncol; }

    std::vector<double, CustomAllocator<double>>& buffer() { return m_buffer; }

private:
    std::size_t index(std::size_t row, std::size_t col) const {
        return row * m_ncol + col;
    }

    std::size_t m_nrow = 0;
    std::size_t m_ncol = 0;
    std::vector<double, CustomAllocator<double>> m_buffer;
};

Matrix multiply_naive(Matrix const& a, Matrix const& b) {
    if (a.ncol() != b.nrow()) {
        throw std::out_of_range("matrix dimension mismatch");
    }

    Matrix ret(a.nrow(), b.ncol());

    for (std::size_t i = 0; i < ret.nrow(); ++i) {
        for (std::size_t j = 0; j < ret.ncol(); ++j) {
            for (std::size_t k = 0; k < a.ncol(); ++k) {
                ret(i, j) += a(i, k) * b(k, j);
            }
        }
    }

    return ret;
}

Matrix multiply_mkl(Matrix& a, Matrix& b) {
    if (a.ncol() != b.nrow()) {
        throw std::out_of_range("matrix dimension mismatch");
    }

    Matrix ret(a.nrow(), b.ncol());

    cblas_dgemm(
        CblasRowMajor,
        CblasNoTrans,
        CblasNoTrans,
        a.nrow(),
        b.ncol(),
        a.ncol(),
        1.0,
        a.buffer().data(),
        a.ncol(),
        b.buffer().data(),
        b.ncol(),
        0.0,
        ret.buffer().data(),
        ret.ncol());

    return ret;
}

Matrix multiply_tile(Matrix const& a, Matrix const& b, std::size_t tiling_size) {
    if (a.ncol() != b.nrow()) {
        throw std::out_of_range("matrix dimension mismatch");
    }

    Matrix ret(a.nrow(), b.ncol());

    for (std::size_t i = 0; i < ret.nrow(); i += tiling_size) {
        for (std::size_t j = 0; j < ret.ncol(); j += tiling_size) {
            for (std::size_t k = 0; k < a.ncol(); k += tiling_size) {
                for (std::size_t ii = i; ii < std::min(i + tiling_size, ret.nrow()); ++ii) {
                    for (std::size_t jj = j; jj < std::min(j + tiling_size, ret.ncol()); ++jj) {
                        for (std::size_t kk = k; kk < std::min(k + tiling_size, a.ncol()); ++kk) {
                            ret(ii, jj) += a(ii, kk) * b(kk, jj);
                        }
                    }
                }
            }
        }
    }

    return ret;
}

PYBIND11_MODULE(_matrix, m) {
    pybind11::class_<Matrix>(m, "Matrix")
        .def(pybind11::init<std::size_t, std::size_t>())
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol)
        .def("__setitem__", [](Matrix& m, std::pair<std::size_t, std::size_t> const& index, double value) {
            m(index.first, index.second) = value;
        })
        .def("__getitem__", [](Matrix const& m, std::pair<std::size_t, std::size_t> const& index) {
            return m(index.first, index.second);
        })
        .def(pybind11::self == pybind11::self);

    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_mkl", &multiply_mkl);
    m.def("multiply_tile", &multiply_tile);

    m.def("bytes", &bytes);
    m.def("allocated", &allocated);
    m.def("deallocated", &deallocated);
}