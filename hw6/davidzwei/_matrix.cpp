#include <vector>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/numpy.h>
#include "mkl.h"

namespace py = pybind11;

class Matrix
{

public:
    Matrix() : m_nrow(0), m_ncol(0){};

    Matrix(size_t nrow, size_t ncol) : m_nrow(nrow), m_ncol(ncol)
    {
        reset_buffer(nrow, ncol);
    }

    // copy assignment
    Matrix(Matrix const &other)
        : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
    {

        reset_buffer(other.m_nrow, other.m_ncol);
        for (size_t i = 0; i < m_nrow; ++i)
        {
            for (size_t j = 0; j < m_ncol; ++j)
            {
                (*this)(i, j) = other(i, j);
            }
        }
    }

    // move constructer
    Matrix(Matrix &&other)
        : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
    {
        reset_buffer(0, 0);
        std::swap(m_nrow, other.m_nrow);
        std::swap(m_ncol, other.m_ncol);
        std::swap(m_buffer, other.m_buffer);
    }

    // move assignment operator
    Matrix &operator=(Matrix &&other)
    {
        if (this == &other)
        {
            return *this;
        }
        reset_buffer(0, 0);
        std::swap(m_nrow, other.m_nrow);
        std::swap(m_ncol, other.m_ncol);
        std::swap(m_buffer, other.m_buffer);
        return *this;
    }

    double operator()(size_t row, size_t col) const
    {
        return m_buffer[index(row, col)];
    }

    double &operator()(size_t row, size_t col)
    {
        return m_buffer[index(row, col)];
    }

    // operator overloading ==
    bool operator==(const Matrix &mat2) const
    {
        if ((m_nrow != mat2.nrow()) || (m_ncol != mat2.ncol()))
        {
            return false;
        }

        for (size_t i = 0; i < mat2.nrow(); ++i)
        {
            for (size_t j = 0; j < mat2.ncol(); ++j)
            {
                if ((*this)(i, j) != mat2(i, j))
                {
                    return false;
                }
            }
        }
        return true;
    }

    // pybind array
    py::array_t<double> array()
    {
        return py::array_t<double>(
            {nrow(), ncol()},
            {sizeof(double) * ncol(), sizeof(double)}, 
            m_buffer,                                 
            py::cast(this)                             
        );
    }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }
    size_t size() const { return m_nrow * m_ncol; }
    double buffer(size_t i) const { return m_buffer[i]; }

    size_t index(size_t row, size_t col) const
    {
        return row * m_ncol + col;
    }

    void reset_buffer(size_t nrow, size_t ncol)
    {
        if (m_buffer)
        {
            delete[] m_buffer;
        }
        const size_t nelement = nrow * ncol;
        if (nelement)
        {
            m_buffer = new double[nelement];
        }
        else
        {
            m_buffer = nullptr;
        }
        m_nrow = nrow;
        m_ncol = ncol;
    }

    size_t m_nrow = 0;
    size_t m_ncol = 0;
    double *m_buffer = nullptr;
};

// naive
Matrix multiply_naive(const Matrix &mat1, const Matrix &mat2)
{
    Matrix res(mat1.nrow(), mat2.ncol());
    for (size_t i = 0; i < mat1.nrow(); ++i)
    {
        for (size_t k = 0; k < mat2.ncol(); ++k)
        {
            double result = 0;
            for (size_t j = 0; j < mat1.ncol(); ++j)
            {
                result += mat1(i, j) * mat2(j, k);
            }

            res(i, k) = result;
        }
    }
    return res;
}

// tiling
Matrix multiply_tile(Matrix const &mat1, Matrix const &mat2, size_t tile_size)
{
    if (mat1.ncol() != mat2.nrow())
    {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }

    Matrix ret(mat1.nrow(), mat2.ncol());

    for (size_t i = 0; i < mat1.nrow(); i += tile_size)
    {
        for (size_t j = 0; j < mat2.ncol(); j += tile_size)
        {
            for (size_t k = 0; k < mat1.ncol(); k += tile_size)
            {
                for (size_t x = i; x < std::min(i + tile_size, mat1.nrow()); x++)
                {
                    for (size_t y = j; y < std::min(j + tile_size, mat2.ncol()); y++)
                    {
                        for (size_t z = k; z < std::min(tile_size + k, mat1.ncol()); z++)
                        {
                            ret(x, y) += mat1(x, z) * mat2(z, y);
                        }
                    }
                }
            }
        }
    }

    return ret;
}

// multiply_mkl
Matrix multiply_mkl(Matrix const &mat1, Matrix const &mat2)
{
    if (mat1.ncol() != mat2.nrow())
    {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }

    Matrix ret(mat1.nrow(), mat2.ncol());

    cblas_dgemm(
        CblasRowMajor,
        CblasNoTrans,
        CblasNoTrans,
        mat1.nrow(),
        mat2.ncol(),
        mat1.ncol(),
        1.0,
        mat1.m_buffer,
        mat1.ncol(),
        mat2.m_buffer,
        mat2.ncol(),
        0.0,
        ret.m_buffer,
        ret.ncol());

    return ret;
}

PYBIND11_MODULE(_matrix, m)
{
    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_mkl", &multiply_mkl);
    m.def("multiply_tile", &multiply_tile);
    pybind11::class_<Matrix>(m, "Matrix")
        .def(pybind11::init<size_t, size_t>())
        .def("__setitem__", [](Matrix &self, std::pair<size_t, size_t> i, double val)
             { self(i.first, i.second) = val; })
        .def("__getitem__", [](Matrix &self, std::pair<size_t, size_t> i)
             { return self(i.first, i.second); })
        .def(pybind11::self == pybind11::self)
        .def_property("array", &Matrix::array, nullptr)
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol);
}
