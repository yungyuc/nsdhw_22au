#include <iostream>
#include <iomanip>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <mkl.h>
#include <vector>

#include "alloc.hpp"

namespace py = pybind11;


static MyAllocator<double> alloc;

class Matrix {

public:

    Matrix(size_t nrow, size_t ncol)
        : m_nrow(nrow), m_ncol(ncol), m_buffer(alloc)
    {
        reset_buffer(nrow, ncol);
    }

    Matrix(size_t nrow, size_t ncol, std::vector<double> const & vec)
      : m_nrow(nrow), m_ncol(ncol), m_buffer(alloc)
    {
        reset_buffer(nrow, ncol);
        for (size_t i = 0; i < nrow * ncol; i++)
            m_buffer[i] = vec[i];
    }

    Matrix & operator=(std::vector<double> const & vec)
    {
        if (size() != vec.size())
        {
            throw std::out_of_range("number of elements mismatch");
        }

        size_t k = 0;
        for (size_t i=0; i<m_nrow; ++i)
        {
            for (size_t j=0; j<m_ncol; ++j)
            {
                (*this)(i,j) = vec[k];
                ++k;
            }
        }

        return *this;
    }

    bool operator== (const Matrix & other) const
    {
        if ((nrow() != other.nrow()) || (ncol() != other.ncol())) { return false; }
        for (size_t i = 0; i < nrow(); ++i)
        {
            for (size_t j = 0; j < ncol(); ++j)
            {
                if ((*this)(i, j) != other(i, j)) { return false; }
            }
        }
        return true;
    }

    bool operator!= (const Matrix & other) const
    {
        return !(*this == other);
    }

    double   operator() (size_t row, size_t col) const { return m_buffer[index(row, col)]; }
    double & operator() (size_t row, size_t col)       { return m_buffer[index(row, col)]; }

    double   operator[] (size_t idx) const { return m_buffer[idx]; }
    double & operator[] (size_t idx)       { return m_buffer[idx]; }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }

    size_t size() const { return m_nrow * m_ncol; }

public:

    size_t index(size_t row, size_t col) const
    {
        return row * m_ncol + col;
    }

    void reset_buffer(size_t nrow, size_t ncol)
    {
        m_buffer.resize(nrow * ncol, 0);
        m_nrow = nrow;
        m_ncol = ncol;
    }

    size_t m_nrow = 0;
    size_t m_ncol = 0;
    std::vector<double, MyAllocator<double>>m_buffer;

};

size_t bytes() { return alloc.counter.bytes(); }
size_t allocated() { return alloc.counter.allocated(); }
size_t deallocated() { return alloc.counter.deallocated(); }

const Matrix multiply_naive(Matrix & m1, Matrix &m2) {

    Matrix resultM = Matrix(m1.nrow(), m2.ncol());

    for (size_t i = 0; i < m1.nrow(); ++i) {
        for (size_t k = 0; k < m2.ncol(); ++k) {
            double v = 0;
            for (size_t j = 0; j < m1.ncol(); ++j){
                v += m1(i, j) * m2(j, k);
            }
            resultM(i, k) = v;
        }
    }

    return resultM;
};


Matrix multiply_mkl(Matrix const &m1, Matrix const &m2){
  // mkl_set_num_threads(1);

    Matrix resultM(m1.nrow(), m2.ncol());
    cblas_dgemm(
        CblasRowMajor,
        CblasNoTrans,
        CblasNoTrans,
        m1.nrow(),
        m2.ncol(),
        m1.ncol(),
        1.0,
        m1.m_buffer.data(),
        m1.ncol(),
        m2.m_buffer.data(),
        m2.ncol(),
        0.0,
        resultM.m_buffer.data(),
        resultM.ncol()
    );

    return resultM;
}


const Matrix multiply_tile(Matrix& m1, Matrix& m2, size_t tile_size) {

    const size_t row = m1.nrow();
    const size_t col = m2.ncol();

    Matrix resultM = Matrix(m1.nrow(), m2.ncol());

    for (size_t i = 0; i < m1.nrow(); ++i) {
        for (size_t k = 0; k < m2.ncol(); ++k) {
            resultM(i, k) = 0;
        }
    }

    // size of cache line (64) devided by size of double(8)
    // bigger numbers of incr give slightly better performance, i guess that
    // is due to effective prefetching
    size_t incr = tile_size;
    // outer loops looping over tiles
    for (size_t i = 0; i < row; i += incr) {
        size_t x_row_min = std::min(i + incr, row);
        for (size_t j = 0; j < col; j += incr) {
            size_t j_col_min = std::min(j + incr, col);
            for (size_t k = 0; k < col; k += incr){
                size_t k_row_min = std::min(k + incr, row);

                // inner loops performing matrix multiplications on tiles
                for (size_t x = i; x < x_row_min; x++) {
                    for (size_t y = j; y < j_col_min; y++) {
                        for (size_t z = k; z < k_row_min; z++) {

                           // resultM[x * col + y] += m1[x * col + z] * m2[z * col + y];
                           resultM(x, y) += m1(x, z) * m2(z, y);
                        }
                    }
                }
            }
        }
    }
    return resultM;
}

PYBIND11_MODULE(_matrix, m) {
    m.doc() = "matrix muliplication";
    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_mkl", &multiply_mkl);
    m.def("multiply_tile", &multiply_tile);
    m.def("bytes", &bytes);
    m.def("allocated", &allocated);
    m.def("deallocated", &deallocated);

   py::class_<Matrix>(m, "Matrix")
        .def(py::init<size_t, size_t>())
        .def(py::init<size_t, size_t, const std::vector<double> &>())
        .def("__setitem__", [](Matrix &self, const std::vector<size_t> index, const double value) {self(index[0], index[1]) = value;})
        .def("__getitem__", [](const Matrix &self, const std::vector<size_t> index) {return self(index[0], index[1]);})
	.def("__eq__", &Matrix::operator==)
        .def("__ne__", &Matrix::operator!=)
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol)
        .def_property_readonly("size", &Matrix::size);
}
