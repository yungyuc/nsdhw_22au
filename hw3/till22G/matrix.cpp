#include <iostream>
#include <vector>
#include <stdexcept>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include "mkl.h"

namespace py = pybind11;
class Matrix {

public:

    Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol)
    {
        reset_buffer(nrow, ncol);
    }
    double   operator() (size_t row, size_t col) const
    {
        return m_buffer[index(row, col)];
    }
    double & operator() (size_t row, size_t col)
    {
        return m_buffer[index(row, col)];
    }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }
    size_t size() const { return m_nrow * m_ncol; }
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
    double * m_buffer = nullptr;
private:


};

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

const Matrix multiply_tile(Matrix& m1, Matrix& m2, int tile_size) {

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

Matrix multiply_mkl(Matrix const &mat1, Matrix const &mat2)
{
    // mkl_set_num_threads(1);

    Matrix ret(mat1.nrow(), mat2.ncol());

    cblas_dgemm(
        CblasRowMajor /* const CBLAS_LAYOUT Layout */
        ,
        CblasNoTrans /* const CBLAS_TRANSPOSE transa */
        ,
        CblasNoTrans /* const CBLAS_TRANSPOSE transb */
        ,
        mat1.nrow() /* const MKL_INT m */
        ,
        mat2.ncol() /* const MKL_INT n */
        ,
        mat1.ncol() /* const MKL_INT k */
        ,
        1.0 /* const double alpha */
        ,
        mat1.m_buffer /* const double *a */
        ,
        mat1.ncol() /* const MKL_INT lda */
        ,
        mat2.m_buffer /* const double *b */
        ,
        mat2.ncol() /* const MKL_INT ldb */
        ,
        0.0 /* const double beta */
        ,
        ret.m_buffer /* double * c */
        ,
        ret.ncol() /* const MKL_INT ldc */
    );
    return ret;
}

std::ostream & operator << (std::ostream & ostr, Matrix const & mat)
{
    for (size_t i=0; i<mat.nrow(); ++i)
    {
        for (size_t j=0; j<mat.ncol(); ++j)
        {
            ostr << mat(i, j) << ' ';
        }
        ostr << std::endl;
    }

    return ostr;
}
bool operator == (Matrix const &mat1, Matrix const &mat2)
{
     for (size_t i = 0; i < mat1.nrow(); ++i)
     {
         for (size_t j = 0; j < mat1.ncol(); ++j)
         {
             if (mat1(i, j) != mat2(i, j))
                 return false;
         }
     }
     return true;
}
PYBIND11_MODULE(_matrix, m)
{
     m.def("multiply_naive", &multiply_naive);
     m.def("multiply_mkl", &multiply_mkl);
     m.def("multiply_tile", &multiply_tile);
     py::class_<Matrix>(m, "Matrix")
        .def(py::init<size_t, size_t>())
        .def("__setitem__", [](Matrix &self, std::pair<size_t, size_t> i, double val) {
            self(i.first, i.second) = val;
        })
        .def("__getitem__", [](Matrix &self, std::pair<size_t, size_t> i) {
            return self(i.first, i.second);
        })
        .def("__eq__", &operator==)
        .def_property("nrow", &Matrix::nrow, nullptr)
        .def_property("ncol", &Matrix::ncol, nullptr);
}
