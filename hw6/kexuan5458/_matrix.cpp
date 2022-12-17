
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/numpy.h>

#include <stdexcept>
#include <vector>
#include <atomic>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <numeric>
#include <mkl.h>

using namespace std;

class Matrix {

public:
    // initialize
    size_t m_nrow = 0;
    size_t m_ncol = 0;
    double* m_buffer = nullptr;

    Matrix(size_t nrow, size_t ncol): m_nrow(nrow), m_ncol(ncol)
    {
        size_t nelement = nrow * ncol;
        m_buffer = new double[nelement];
    }
    Matrix(size_t nrow, size_t ncol, const vector<double> &v): m_nrow(nrow), m_ncol(ncol)
    {
        reset_buffer(nrow, ncol);
        copy(v.begin(), v.end(), m_buffer);
    }
    ~Matrix()
    {
        delete[] m_buffer;
    }

    pybind11::array_t<double> array() {
        return pybind11::array_t<double>({nrow(), ncol()}, {sizeof(double) * ncol(), sizeof(double)}, m_buffer, pybind11::cast(this));
    }

    void reset_buffer(size_t nrow, size_t ncol)
    {
        if(m_buffer)    // if exist
            delete[] m_buffer;
        m_buffer = new double[nrow * ncol];
    }

     // copy constructor
    Matrix(const Matrix &other): m_nrow(other.m_nrow), m_ncol(other.m_ncol)
    {
        reset_buffer(other.m_nrow, other.m_ncol);
        copy(other.m_buffer, other.m_buffer + (other.m_nrow * other.m_ncol), m_buffer);
    }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }

    double  operator() (size_t row, size_t col) const
    {
        return m_buffer[row*m_ncol + col];
    }

    double &operator() (size_t row, size_t col)
    {
        return m_buffer[row*m_ncol + col];
    }

    bool operator==(const Matrix &other) const
    {
        for (size_t i = 0; i < m_nrow; i++)
        {
            for (size_t j = 0; j < m_ncol; j++)
            {
                size_t index = i * m_ncol + j;
                if (m_buffer[index] != other.m_buffer[index])
                    return false;
            }
        }
        return true;
    }  

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
        for (size_t j = 0; j < mat2.ncol(); j += tile_size)
        {
            for (size_t k = 0; k < mat1.ncol(); k += tile_size)
            {

                
                for(size_t l = i; l < min((i + tile_size), mat1.nrow()); l++)
                {
                    for(size_t m = j; m < min((j + tile_size), mat2.ncol()); m++)
                    {
                        for(size_t n = k; n < min((k + tile_size), mat1.ncol()); n++)
                        {
                            ret(l, m) += mat1(l, n) * mat2(n, m);
                        }
                    }
                }

            }
        }
    }
    return ret;
}

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
      , mat1.m_buffer /* const double *a */
      , k   /* const MKL_INT lda */
      , mat2.m_buffer /* const double *b */
      , n   /* const MKL_INT ldb */
      , 0.0 /* const double beta */
      , ret.m_buffer /* double * c */
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

    pybind11::class_<Matrix>(m, "Matrix")       // 
        .def(pybind11::init<size_t, size_t>())  //
        .def(pybind11::init<size_t, size_t, vector<double> const & >())
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
        .def_property_readonly("ncol", &Matrix::ncol)
        .def_property("array",  &Matrix::array, nullptr);
}