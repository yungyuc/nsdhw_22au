// https://yyc.solvcon.net/en/latest/nsd/04matrix/example.html#nsd-matrix-example-ma03-matrix-matrix

#include <iostream>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <numeric>

#include <mkl.h>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

#include "alloc.hpp"

namespace py = pybind11;
using std::vector;

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
        for (int i = 0; i < nrow * ncol; i++)
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
    vector<double, MyAllocator<double>>m_buffer;

};

size_t bytes() { return alloc.counter.bytes(); }
size_t allocated() { return alloc.counter.allocated(); }
size_t deallocated() { return alloc.counter.deallocated(); }

Matrix multiply_naive(Matrix const & mat1, Matrix const & mat2)
{
    if (mat1.ncol() != mat2.nrow())
    {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }
    Matrix ret(mat1.nrow(), mat2.ncol());
    for (size_t i=0; i<ret.nrow(); ++i)
    {
        for (size_t j=0; j<ret.ncol(); ++j)
        {
            double v = 0;
            for (size_t k=0; k<mat1.ncol(); ++k)
            {
                v += mat1(i,k) * mat2(k,j);
            }
            ret(i,j) = v;
        }
    }
    return ret;
}

Matrix multiply_tile(Matrix const & mat1, Matrix const & mat2, size_t tile_size) {
    if (mat1.ncol() != mat2.nrow())
    {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }

    Matrix ret(mat1.ncol(), mat2.nrow());

    // unify matrix to blocks by tiles
    for (size_t start_i=0; start_i<mat1.nrow(); start_i+=tile_size)
    {
        for (size_t start_j=0; start_j<mat2.ncol(); start_j+=tile_size)
        {   
            // // block by block matrix-matrix multiplication
            for (size_t start_k=0; start_k<mat1.ncol(); start_k+=tile_size)
            {
                for (size_t i=start_i; i<std::min(start_i+tile_size, mat1.nrow()); ++i)
                {
                    for (size_t k=start_k; k<std::min(start_k+tile_size, mat1.ncol()); ++k)
                    {
                        for (size_t j=start_j; j<std::min(start_j+tile_size, mat2.ncol()); ++j)
                        {
                            ret(i,j) += mat1(i,k) * mat2(k,j);
                        }
                    }
                }
            }
                
        }
    }
    return ret;
}

/*
 * Use MKL for the matrix matrix multiplication.
 */
Matrix multiply_mkl(Matrix const & mat1, Matrix const & mat2)
{

    Matrix ret(mat1.nrow(), mat2.ncol());

    cblas_dgemm(
        CblasRowMajor /* const CBLAS_LAYOUT Layout */
      , CblasNoTrans /* const CBLAS_TRANSPOSE transa */
      , CblasNoTrans /* const CBLAS_TRANSPOSE transb */
      , mat1.nrow() /* const MKL_INT m */
      , mat2.ncol() /* const MKL_INT n */
      , mat1.ncol() /* const MKL_INT k */
      , 1.0 /* const double alpha */
      , mat1.m_buffer.data() /* const double *a */
      , mat1.ncol() /* const MKL_INT lda */
      , mat2.m_buffer.data() /* const double *b */
      , mat2.ncol() /* const MKL_INT ldb */
      , 0.0 /* const double beta */
      , ret.m_buffer.data() /* double * c */
      , ret.ncol() /* const MKL_INT ldc */
    );

    return ret;
}

PYBIND11_MODULE(_matrix, m) {

    m.doc() = "Matrix";

    py::class_<Matrix>(m, "Matrix")
        .def(py::init<size_t, size_t>())
        .def(py::init<size_t, size_t, const std::vector<double> &>())
        .def("__getitem__", [](const Matrix &self, const std::vector<size_t> idx) {
            return self(idx[0], idx[1]);
        })
        .def("__setitem__", [](Matrix &self, const std::vector<size_t> idx, const double value) {
            self(idx[0], idx[1]) = value;
        })
        .def("__eq__", &Matrix::operator==)
        .def("__ne__", &Matrix::operator!=)
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol)
        .def_property_readonly("size", &Matrix::size);
    
    m.def("multiply_naive", &multiply_naive, "multiply 2 matrix naively.");
    m.def("multiply_tile", &multiply_tile, "multiply 2 matrix by tiling the matrices.");
    m.def("multiply_mkl", &multiply_mkl, "multiply 2 matrix by using mkl library.");
    m.def("bytes", &bytes);
    m.def("allocated", &allocated);
    m.def("deallocated", &deallocated);
}

