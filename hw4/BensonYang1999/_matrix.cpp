#ifndef NPYBIND
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#endif

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <numeric>
#include <mkl.h>

#include "CustomAllocator.hpp"

static MyAllocator<double> myAllocator;

size_t bytes() { return myAllocator.counter.bytes(); }
size_t allocated() { return myAllocator.counter.allocated(); }
size_t deallocated() { return myAllocator.counter.deallocated(); }

class Matrix
{

public:
    Matrix(size_t nrow, size_t ncol)
        : m_nrow(nrow), m_ncol(ncol), m_buffer(myAllocator)
    {
        set_buffer(nrow, ncol);
    }

    // get and set data
    double operator()(size_t row, size_t col) const
    {
        return m_buffer[row * m_ncol + col];
    }
    double &operator()(size_t row, size_t col)
    {
        return m_buffer[row * m_ncol + col];
    }

    // compare equalization
    bool operator==(const Matrix &other) const
    {
        return std::equal(m_buffer.begin(), m_buffer.end(), other.m_buffer.begin());
    }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }

    void set_buffer(size_t nrow, size_t ncol)
    {
        m_buffer.resize(nrow * ncol);
    }

    size_t m_nrow;
    size_t m_ncol;
    std::vector<double, MyAllocator<double>> m_buffer;
};

/*
 * Naive matrix matrix multiplication.
 */
Matrix multiply_naive(const Matrix &m1, const Matrix &m2)
{
    Matrix m3(m1.nrow(), m2.ncol());
    size_t m1r = m1.nrow(), m1c = m1.ncol(), m2c = m2.ncol();

    for (size_t i = 0; i < m1r; i++)
    {
        for (size_t j = 0; j < m2c; j++)
        {
            double temp = 0.0;
            for (size_t k = 0; k < m1c; k++)
                temp += m1(i, k) * m2(k, j);
            m3(i, j) = temp;
        }
    }

    return m3;
}

/*
 * Tiled matrix matrix multiplication.
 */
Matrix multiply_tile(const Matrix &m1, const Matrix &m2, size_t cache_size)
{
    Matrix m3(m1.nrow(), m2.ncol());
    size_t m1r = m1.nrow(), m1c = m1.ncol(), m2c = m2.ncol();

    for (size_t i0 = 0; i0 < m1r; i0 += cache_size)
    {
        size_t imax = std::min(i0 + cache_size, m1r);

        for (size_t j0 = 0; j0 < m2c; j0 += cache_size)
        {
            size_t jmax = std::min(j0 + cache_size, m2c);

            for (size_t k0 = 0; k0 < m1c; k0 += cache_size)
            {
                size_t kmax = std::min(k0 + cache_size, m1c);

                for (size_t j1 = j0; j1 < jmax; j1++)
                {
                    for (size_t i1 = i0; i1 < imax; i1++)
                    {
                        for (size_t k1 = k0; k1 < kmax; k1++)
                        {
                            m3(i1, j1) += m1(i1, k1) * m2(k1, j1);
                        }
                    }
                }
            }
        }
    }

    return m3;
}

/*
 * MKL matrix matrix multiplication.
 */
Matrix multiply_mkl(const Matrix &m1, const Matrix &m2)
{
    Matrix m3(m1.nrow(), m2.ncol());
    cblas_dgemm(CblasRowMajor,
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
                m3.m_buffer.data(),
                m3.ncol());
    return m3;
}

#ifndef NPYBIND

PYBIND11_MODULE(_matrix, m)
{
    m.doc() = "maxtrix multiplication";

    pybind11::class_<Matrix>(m, "Matrix")
        .def(pybind11::init<size_t, size_t>())
        .def("__getitem__", [](const Matrix &mat, std::pair<size_t, size_t> id)
             { return mat(id.first, id.second); })
        .def("__setitem__", [](Matrix &mat, std::pair<size_t, size_t> id, double val)
             { mat(id.first, id.second) = val; })
        .def("__eq__", &Matrix::operator==)
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol);

    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_tile", &multiply_tile);
    m.def("multiply_mkl", &multiply_mkl);

    m.def("bytes", &bytes);
    m.def("allocated", &allocated);
    m.def("deallocated", &deallocated);
}

#endif
