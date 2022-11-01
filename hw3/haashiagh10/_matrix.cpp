#include<iostream>
#include <mkl.h>
#include<vector>
#include <pybind11/stl.h>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
using namespace std;

class Matrix {

public:

    Matrix(size_t nrow, size_t ncol)
        : m_nrow(nrow), m_ncol(ncol)
    {
        reset_buffer(nrow, ncol);
    }


    Matrix(Matrix const& other) 
        : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
    {
        reset_buffer(other.m_nrow, other.m_ncol);
        for (size_t i = 0; i < m_nrow; i += 1)
        {
            for (size_t j = 0; j < m_ncol; j += 1)
            {
                (*this)(i, j) = other(i, j);
            }
        }
    }
    void reset_buffer(size_t nrow, size_t ncol)
    {
        if (m_buffer)
        {
            delete[] m_buffer;
        }
        m_buffer = new double[nrow * ncol];
    }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }

    //operator
    double operator()(size_t row, size_t col)const
    {
        size_t index = row * m_ncol + col;
        return m_buffer[index];
    }

    double& operator()(size_t row, size_t col)
    {
        size_t index = row * m_ncol + col;
        return m_buffer[index];
    }

    bool operator==(const Matrix& other) const
    {
        //bool flag = true
        for (size_t i = 0; i < m_nrow; ++i)
        {
            for (size_t j = 0; j < m_ncol; ++j)
            {
                size_t idx = i * m_ncol + j;
                if (m_buffer[idx] != other.m_buffer[idx])
                    return false;
            }
        }

        return true;
    }

    size_t m_nrow;
    size_t m_ncol;
    double* m_buffer;

};

Matrix multiply_naive(Matrix const& mat1, Matrix const& mat2) {
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
                v += mat1(i, j) * mat2(j, k);
            }
            ret(i, k) = v;
        }
    }

    return ret;
}

// Matrix multiply_tile
Matrix multiply_tile(const Matrix& mat1, const Matrix& mat2, size_t tile_size) {
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


                for (size_t l = i; l < min((i + tile_size), mat1.nrow()); l++)
                {
                    for (size_t m = j; m < min((j + tile_size), mat2.ncol()); m++)
                    {
                        for (size_t n = k; n < min((k + tile_size), mat1.ncol()); n++)
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

Matrix multiply_mkl(Matrix const& mat1, Matrix const& mat2)
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
        CblasRowMajor 
        , CblasNoTrans 
        , CblasNoTrans 
        , m   
        , n   
        , k   
        , 1.0 
        , mat1.m_buffer 
        , k   
        , mat2.m_buffer 
        , n   
        , 0.0 
        , ret.m_buffer 
        , n   
    );

    return ret;
}


PYBIND11_MODULE(_matrix, m)
{
    m.doc() = "maxtrix multiply function"; // ??????????????????????????????????????????
    m.def("multiply_naive", &multiply_naive);   //
    m.def("multiply_mkl", &multiply_mkl);       //
    m.def("multiply_tile", &multiply_tile);     //
    pybind11::class_<Matrix>(m, "Matrix")       // 
        .def(pybind11::init<size_t, size_t>())  //
        .def("__setitem__", [](Matrix& mat, pair<size_t, size_t> row_col, double val)
            {
                mat(row_col.first, row_col.second) = val;
            })
        .def("__getitem__", [](Matrix& mat, pair<size_t, size_t> row_col)
            {
                return mat(row_col.first, row_col.second);
            })
                .def("__eq__", &Matrix::operator==)
                // .def_property_readonly("nrow", &Matrix::nrow)
                // .def_property_readonly("ncol", &Matrix::ncol);
                .def_property("nrow", &Matrix::nrow, nullptr)
                .def_property("ncol", &Matrix::ncol, nullptr);
}