#include "matrix.hpp"
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <mkl/mkl.h>
#include <vector>
#include <utility>
#include <tuple>

// constructor
Matrix::Matrix(size_t n_row, size_t n_col) : m_row(n_row), m_col(n_col)
{
    reset_buffer(n_row, n_col);
}
// destructor
Matrix::~Matrix()
{
    reset_buffer(0, 0);
}

// copy constructor
Matrix::Matrix(Matrix const &other)
{
    this->reset_buffer(other.nrow(), other.ncol());
    for (size_t i = 0; i < other.nrow(); i++)
    {
        for (size_t j = 0; j < other.ncol(); j++)
        {
            const size_t index = this->index(i, j);
            this->m_buffer[index] = other.m_buffer[index];
        }
    }
}

// copy assigment operator
Matrix &Matrix::operator=(Matrix const &other)
{
    if (this == &other)
    {
        return *this;
    }
    if (other.ncol() != this->ncol() || other.nrow() != this->nrow())
    {
        this->reset_buffer(other.nrow(), other.ncol());
    }
    for (size_t i = 0; i < other.nrow(); i++)
    {
        for (size_t j = 0; j < other.ncol(); j++)
        {
            const size_t index = this->index(i, j);
            this->m_buffer[index] = other.m_buffer[index];
        }
    }
    return *this;
}
// move constructor
Matrix::Matrix(Matrix &&other)
{
    reset_buffer(0, 0);
    std::swap(this->m_col, other.m_col);
    std::swap(this->m_row, other.m_row);
    std::swap(this->m_buffer, other.m_buffer);
}
// move assigment constructor
Matrix &Matrix::operator=(Matrix &&other)
{
    if (this == &other)
    {
        return *this;
    }
    reset_buffer(0, 0);
    std::swap(this->m_row, other.m_row);
    std::swap(this->m_col, other.m_col);
    std::swap(this->m_buffer, other.m_buffer);

    return *this;
}

size_t Matrix::ncol() const
{
    return this->m_col;
}

size_t Matrix::nrow() const
{
    return this->m_row;
}

size_t Matrix::size()
{
    return this->m_row * this->m_col;
}

void Matrix::reset_buffer(size_t n_row, size_t n_col)
{
    if (this->m_buffer)
    {
        delete this->m_buffer;
    }
    const size_t n_element = n_col * n_row;
    if (n_element)
    {
        this->m_buffer = new double[n_element];
        // need to init
        for (size_t i = 0; i < n_element; i++)
        {
            this->m_buffer[i] = 0;
        }
    }
    else
    {
        this->m_buffer = nullptr;
    }
    this->m_col = n_col;
    this->m_row = n_row;
}

size_t Matrix::index(size_t n_row, size_t n_col) const
{
    return this->m_col * n_row + n_col;
}

double Matrix::operator()(size_t n_row, size_t n_col) const
{
    return this->m_buffer[index(n_row, n_col)];
}
double &Matrix::operator()(size_t n_row, size_t n_col)
{
    return this->m_buffer[index(n_row, n_col)];
}
bool Matrix::operator==(Matrix const &other) const
{
    if (this->nrow() != other.nrow() || this->ncol() != other.ncol())
    {
        return false;
    }
    for (size_t i = 0; i < other.nrow(); i++)
    {
        for (size_t j = 0; j < other.ncol(); j++)
        {
            if (other(i, j) != (*this)(i, j))
            {
                return false;
            }
        }
    }

    return true;
}

inline void Check_mutiply_valid(Matrix mat1, Matrix mat2)
{
    if (mat1.ncol() != mat2.nrow())
    {
        throw std::invalid_argument("The matrix shape is invalid to mutiply");
    }
}

Matrix multiply_naive(Matrix &mat1, Matrix &mat2)
{
    Check_mutiply_valid(mat1, mat2);
    const size_t K = mat1.ncol(), M = mat1.nrow(), N = mat2.ncol();
    Matrix ret_mat(M, N);
    for (size_t i = 0; i < M; i++)
    {
        const size_t ret_leader = i * N;
        for (size_t j = 0; j < N; j++)
        {
            double count = 0;
            const size_t m1_leader = i * K;
            for (size_t k = 0; k < K; k++)
            {
                count += mat1.m_buffer[m1_leader + k] * mat2.m_buffer[k * N + j];
            }
            ret_mat.m_buffer[ret_leader + j] = count;
        }
    }
    return ret_mat;
}

Matrix multiply_tile(Matrix &mat1, Matrix &mat2, size_t tile_size)
{
    Check_mutiply_valid(mat1, mat2);
    const size_t K = mat1.ncol(), M = mat1.nrow(), N = mat2.ncol();
    Matrix ret_mat(M, N);

    for (size_t m1_start_index = 0; m1_start_index < M; m1_start_index += tile_size)
    {
        const size_t m1_end_index = std::min(m1_start_index + tile_size, M);
        for (size_t common_start_index = 0; common_start_index < K; common_start_index += tile_size)
        {
            const size_t common_end_index = std::min(common_start_index + tile_size, K);
            for (size_t m2_start_index = 0; m2_start_index < N; m2_start_index += tile_size)
            {
                const size_t m2_end_index = std::min(m2_start_index + tile_size, N);
                /*
                start calculate multiply on  tile_size*tile_size (mat1 block * mat2 block)
                mat1 block = (m1_start_index ~ m1_end_index) * (common_start_index ~ common_end_index)
                mat2 block = (common_start_index ~ common_end_index) * (m2_start_index ~ m2_end_index)
                */
                for (size_t i = m1_start_index; i < m1_end_index; ++i)
                {
                    const size_t leader_i = i * K;
                    const size_t leader_ret = i * N;
                    for (size_t _k = common_start_index; _k < common_end_index; ++_k)
                    {
                        const size_t mat_1_index = leader_i + _k;
                        const size_t mat_2_leader = _k * N;
                        for (size_t j = m2_start_index; j < m2_end_index; ++j)
                        {
                            ret_mat.m_buffer[leader_ret + j] += mat1.m_buffer[mat_1_index] * mat2.m_buffer[mat_2_leader + j];
                        }
                    }
                }
            }
        }
    }
    return ret_mat;
}

Matrix multiply_mkl(Matrix &mat1, Matrix &mat2)
{
    Check_mutiply_valid(mat1, mat2);
    const int m = mat1.nrow(), n = mat2.ncol(), k = mat1.ncol();
    const double alpha = 1.0, beta = 0.0;
    Matrix ret_mat(m, n);

    // void cblas_dgemm(const CBLAS_LAYOUT Layout, const CBLAS_TRANSPOSE TransA,
    //              const CBLAS_TRANSPOSE TransB, const MKL_INT M, const MKL_INT N,
    //              const MKL_INT K, const double alpha, const double *A,
    //              const MKL_INT lda, const double *B, const MKL_INT ldb,
    //              const double beta, double *C, const MKL_INT ldc) NOTHROW;
    cblas_dgemm(
        CblasRowMajor,
        CblasNoTrans,
        CblasNoTrans,
        m,
        n,
        k,
        alpha,
        mat1.m_buffer,
        k,
        mat2.m_buffer,
        n,
        beta,
        ret_mat.m_buffer,
        n);
    return ret_mat;
}