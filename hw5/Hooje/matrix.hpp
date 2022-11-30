//#include <pybind11/pybind11.h>
//#include <pybind11/stl.h>
#include <vector>
#include <mkl.h>
// based on https://yyc.solvcon.net/en/latest/nsd/04matrix/matrix.html
using namespace std;

class Matrix
{

public:
    size_t m_nrow;
    size_t m_ncol;
    double *m_buffer = nullptr;
    //constructor
    Matrix(size_t nrow, size_t ncol) : m_nrow(nrow), m_ncol(ncol)
    {
        reset_buffer(nrow, ncol);
    }
    //function 
    void reset_buffer(size_t nrow, size_t ncol)
    {
        if(m_buffer) //need to reset
        {
            delete[] m_buffer;
        }
        m_buffer = new double[nrow*ncol];
    }
    size_t nrow() const 
    {
        return m_nrow;
    }
    size_t ncol() const 
    {
        return m_ncol;
    }
    // operator
    double operator()(size_t row, size_t col) const
    {
        size_t idx = row * m_ncol + col;
        return m_buffer[idx];
    }
    double &operator()(size_t row, size_t col) 
    {
        size_t idx = row * m_ncol + col;
        return m_buffer[idx];
    }
    bool operator==(const Matrix &other) const
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
};

Matrix multiply_naive(const Matrix &mat1, const Matrix &mat2)
{
    Matrix ret(mat1.nrow(), mat2.ncol());
    for (size_t i = 0; i < mat1.nrow(); i++)
    {
        for (size_t k = 0; k < mat2.ncol(); k++)
        {
            double v = 0;
            for (size_t j = 0; j < mat1.ncol(); j++)
            {
                v += mat1(i,j) * mat2(j,k);
            }
            ret(i,k) = v;
        }
    }
    return ret;
}

// matrix tiled
Matrix multiply_tile(const Matrix &mat1, const Matrix &mat2,  size_t len_size)
{
    Matrix ret(mat1.nrow(), mat2.ncol());
    for (size_t i_start = 0; i_start < mat1.nrow(); i_start += len_size)
    {
        for (size_t k_start = 0; k_start < mat2.ncol(); k_start += len_size)
        {
            for (size_t j_start = 0; j_start < mat1.ncol(); j_start += len_size)
            {
                size_t i_end = min(i_start + len_size, mat1.nrow());
                size_t k_end = min(k_start + len_size, mat2.ncol());
                size_t j_end = min(j_start + len_size, mat1.ncol());

                for (size_t i = i_start; i < i_end; i++)
                {
                    for (size_t k = k_start; k < k_end; k++)
                    {
                        for (size_t j = j_start; j < j_end; j++)
                        {
                            ret(i, k) += mat1(i, j) * mat2(j, k);
                        }
                    }
                }
            }
        }    
    }
    return ret;
}


Matrix multiply_mkl(const Matrix &mat1, const Matrix &mat2)
{
    Matrix ret(mat1.nrow(), mat2.ncol());
    size_t m = mat1.nrow();
    size_t n = mat2.ncol();
    size_t k = mat1.ncol();
    double alpha = 1.0;
    double beta = 0.0;
    double* A = mat1.m_buffer;
    double* B = mat2.m_buffer;
    double* C = ret.m_buffer;
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, 
                m, n, k, alpha, A, k, B, n, beta, C, n);
    return ret;
}

