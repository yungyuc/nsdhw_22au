#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
// https://dboyliao.medium.com/pybind11-%E5%85%A5%E9%96%80%E7%8E%A9%E6%A8%82%E6%89%8B%E8%A8%98-9da22f6193f2
#include <stdexcept>
#include <vector>
#include <atomic>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <numeric>
#include <mkl.h>


#include "CustomAllocator.hpp"
using namespace std;

static MyAllocator<double> Myalloc;

size_t bytes() {
	return Myalloc.counter.bytes();
}

size_t allocated() {
	return Myalloc.counter.allocated(); 
}

size_t deallocated() {
	return Myalloc.counter.deallocated();
}

class Matrix {

public:
    // initialize
    size_t m_nrow = 0;
    size_t m_ncol = 0;
    double* m_buffer = nullptr;

    Matrix(size_t nrow, size_t ncol): m_nrow(nrow), m_ncol(ncol), m_buffer(Myalloc)
    {
        size_t nelement = nrow * ncol;
        //m_buffer = new double[nelement];
        m_buffer.resize(nrow * ncol);
    }

    // ~Matrix()
    // {
    //     delete[] m_buffer;
    // }

    void reset_buffer(size_t nrow, size_t ncol)
    {
        if(m_buffer)    // if exist
            delete[] m_buffer;
        m_buffer = new double[nrow * ncol];
    }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }

    // set data & get data
    double  operator() (size_t row, size_t col) const
    {
        return m_buffer[row*m_ncol + col];
    }

    double &operator() (size_t row, size_t col)
    {
        return m_buffer[row*m_ncol + col];
    }

    // compare
    bool operator==(const Matrix &other) const
    {
        return equal(m_buffer.begin(), m_buffer.end(), other.m_buffer.begin());
    }

    vector<double, MyAllocator<double>> m_buffer; 

};

// Matrix multiply_naive
Matrix multiply_naive(Matrix const & mat1, Matrix const & mat2) {
    if (mat1.ncol() != mat2.nrow()) // mat1_row*mat1_column * mat2_row*mat2_column
    {
        throw out_of_range("2 matrix dimensions are mismatch");
    }

    Matrix ret(mat1.nrow(), mat2.ncol());

    for (size_t i = 0; i < ret.nrow(); i++)
    {
        for (size_t k = 0; k < ret.ncol(); k++)
        {
            double v = 0;
            for (size_t j = 0; j < mat1.ncol(); j++) {
                v += mat1(i,j) * mat2(j,k);
            }
            ret(i,k) = v;
        }
    }
    return ret;
}

// Matrix multiply_tile
Matrix multiply_tile(const Matrix &mat1, const Matrix &mat2,  size_t tile_size) {
    if (mat1.ncol() != mat2.nrow()) // mat1_row*mat1_column * mat2_row*mat2_column
    {
        throw out_of_range("2 matrix dimensions are mismatch");
    }

    Matrix ret(mat1.nrow(), mat2.ncol());

    for (size_t i = 0; i < mat1.nrow(); i += tile_size)
    {
        size_t i_max = min(i + tile_size, mat1.nrow());
        for (size_t j = 0; j < mat2.ncol(); j += tile_size)
        {
            size_t j_max = min(j + tile_size, mat2.ncol());
            for (size_t k = 0; k < mat1.ncol(); k += tile_size)
            {
                size_t k_max = min(k + tile_size, mat1.ncol());
                for(size_t j2 = j; j2 < j_max; j2++)
                {
                    for(size_t i2 = i; i2 < i_max; i2++)
                    {
                        for(size_t k2 = k; k2 < k_max; k2++)
                        {
                            ret(i2, j2) += mat1(i2, k2) * mat2(k2, j2);
                        }
                    }
                }
            }
        }
    }
    return ret;
}

// Matrix multiply_mkl
Matrix multiply_mkl(Matrix const & mat1, Matrix const & mat2)
{

    if (mat1.ncol() != mat2.nrow()) // mat1_row*mat1_column * mat2_row*mat2_column
    {
        throw out_of_range("2 matrix dimensions are mismatch");
    }
    Matrix ret(mat1.nrow(), mat2.ncol());
    size_t m = mat1.nrow();
    size_t n = mat2.ncol();
    size_t k = mat1.ncol();
    cblas_dgemm(
        CblasRowMajor /* const CBLAS_LAYOUT Layout */
      , CblasNoTrans /* const CBLAS_TRANSPOSE transa */
      , CblasNoTrans /* const CBLAS_TRANSPOSE transb */
      , m   /* const MKL_INT m */
      , n   /* const MKL_INT n */
      , k   /* const MKL_INT k */
      , 1.0 /* const double alpha */
      , mat1.m_buffer.data() /* const double *a */
      , k   /* const MKL_INT lda */
      , mat2.m_buffer.data() /* const double *b */
      , n   /* const MKL_INT ldb */
      , 0.0 /* const double beta */
      , ret.m_buffer.data() /* double * c */
      , n   /* const MKL_INT ldc */
    );
    return ret;
}

// first argument of PYBIND11_MODULE must be as the same as '.so' file you would compile later
PYBIND11_MODULE(_matrix, m)
{
    m.doc() = "maxtrix multiply function";      // module doc string
    m.def("multiply_naive", &multiply_naive);   //
    m.def("multiply_mkl", &multiply_mkl);       //
    m.def("multiply_tile", &multiply_tile);     //

    m.def("bytes", &bytes);
    m.def("allocated", &allocated);
    m.def("deallocated", &deallocated);
    pybind11::class_<Matrix>(m, "Matrix")       // 
        .def(pybind11::init<size_t, size_t>())  //
        .def("__setitem__", [](Matrix &mat, pair<size_t, size_t> row_col, double val) 
        {
            mat(row_col.first, row_col.second) = val;
        })
        .def("__getitem__", [](Matrix &mat, pair<size_t, size_t> row_col)
        {
            return mat(row_col.first, row_col.second);
        })
        .def("__eq__", &Matrix::operator==)
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol);
        // .def_property("nrow", &Matrix::nrow, nullptr)
        // .def_property("ncol", &Matrix::ncol, nullptr);
}