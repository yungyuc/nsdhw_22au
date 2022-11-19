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
Matrix multiply_naive(const Matrix &mat1, const Matrix &mat2)
{
    if (mat1.ncol() != mat2.nrow())
    {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }
    Matrix res(mat1.nrow(), mat2.ncol());
    for(size_t i = 0; i < mat1.nrow(); ++i)
    {
        for (size_t j = 0; j < mat2.ncol(); ++j)
        {
            double temp = 0;
            for (size_t k = 0; k < mat1.ncol(); ++k)
            {
                temp += mat1(i, k) * mat2(k, j);
            }
            res(i,j) = temp;
        }

    }
    return res;
}
Matrix multiply_tile(const Matrix &mat1, const Matrix &mat2, size_t t_size)
{
    if (mat1.ncol() != mat2.nrow())
    {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }
    Matrix res(mat1.nrow(), mat2.ncol());
    for (int ii = 0; ii < mat1.nrow(); ii+=t_size)
    {
        for (int jj = 0; jj < mat2.ncol(); jj+=t_size)
        {
            for(int kk = 0; kk < mat1.ncol(); kk+=t_size)
            {
                for (int i = ii; i < std::min(ii+t_size, mat1.nrow());  i++)
                {
                    for (int j = jj; j < std::min(jj+t_size, mat2.ncol()) ; j++)
                    {
                        for (int k = kk; k < std::min(kk+t_size, mat1.ncol()) ; k++)
                        {
                            res(i,j) += mat1(i,k) * mat2(k,j);
                        }
                    }

                }

            }
        }
    }
    return res;
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
