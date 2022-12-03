#pragma once

#include <iostream>
#include <vector>
#include "mkl.h"

using namespace std;

template<typename T>
class Matrix {
private:
    size_t m_rows, m_cols;
    std::vector<T> m_matrix;

public:
    Matrix()
        : m_rows(0), m_cols(0)
    {
    }

    Matrix(size_t rows, size_t cols)
        : m_rows(rows), m_cols(cols)
    {
        this->m_matrix.resize(this->m_rows * this->m_cols, 0);
    }

    Matrix(Matrix const& rhs)
        : m_rows(rhs.m_rows), m_cols(rhs.m_cols), m_matrix(rhs.m_matrix)
    {
    }

    Matrix(Matrix&& rhs)
        : m_rows(rhs.m_rows), m_cols(rhs.m_cols), m_matrix(std::move(rhs.m_matrix))
    {
    }

    ~Matrix()
    {
        this->m_matrix.clear();
        this->m_matrix.shrink_to_fit();
    }

    Matrix& operator=(Matrix const& rhs)
    {
        if (this != &rhs) {
            this->m_rows = rhs.m_rows;
            this->m_cols = rhs.m_cols;
            this->m_matrix = rhs.m_matrix;
        }

        return *this;
    }

    Matrix& operator=(Matrix&& rhs)
    {
        if (this != &rhs) {
            this->m_rows = rhs.m_rows;
            this->m_cols = rhs.m_cols;
            this->m_matrix = std::move(rhs.m_matrix);
        }

        return *this;
    }

    T const& operator()(size_t i, size_t j) const
    {
        return this->m_matrix[i * this->m_cols +j];
    }

    T& operator()(size_t i, size_t j)
    {
        return this->m_matrix[i * this->m_cols + j];
    }

    const T* data() const
    {
        return &(this->m_matrix[0]);
    }

    T* data()
    {
        return &(this->m_matrix[0]);
    }

    constexpr bool operator==(const Matrix& rhs) const
    {
        return (this->m_matrix == rhs.m_matrix);
    }

    constexpr size_t rows() const
    {
        return this->m_rows;
    }

    constexpr size_t cols() const
    {
        return this->m_cols;
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix<T>& matrix)
    {
        for (size_t i = 0; i < matrix.rows(); i++) {
            for (size_t j = 0; j < matrix.cols(); j++) {
                os << matrix(i, j) << (j == matrix.cols() - 1 ? "" : " ");
            }

            if (i < matrix.rows() - 1) os << '\n';
        }

        return os;
    }
};

template<typename T>
Matrix<T> multiply_naive(const Matrix<T>& m1, const Matrix<T>& m2)
{
    assert(m1.cols() == m2.rows());

    Matrix<T> m3(m1.rows(), m2.cols());

    size_t rows = m1.rows();
    size_t cols = m2.cols();
    size_t inners = m1.cols();

    for (size_t row = 0; row < rows; row++) {
        for (size_t col = 0; col < cols; col++) {
            for (size_t inner = 0; inner < inners; inner++) {
                m3(row, col) += m1(row, inner) * m2(inner, col);
            }
        }
    }

    return m3;
}

template<typename T>
Matrix<T> multiply_tile(const Matrix<T>& m1, const Matrix<T>& m2, size_t size)
{
    assert(m1.cols() == m2.rows());

    Matrix<T> m3(m1.rows(), m2.cols());

    for (size_t row = 0; row < m1.rows(); row += size) {
        for (size_t col = 0; col < m2.cols(); col += size) {
            for (size_t inner = 0; inner < m1.cols(); inner += size) {
                // tile
                for (size_t k = inner; k < min(m1.cols(), inner + size); k++) {
                    for (size_t i = row; i < min(m1.rows(), row + size); i++) {
                        for (size_t j = col; j < min(m2.cols(), col + size); j++) {
                            m3(i, j) += m1(i, k) * m2(k, j);
                        }
                    }
                }
            }
        }
    }

    return m3;
}

template<typename T>
Matrix<T> multiply_mkl(Matrix<T>& m1, Matrix<T>& m2)
{
    assert(m1.cols() == m2.rows());

    Matrix<T> m3(m1.rows(), m2.cols());

    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m1.rows(), m2.cols(), m1.cols(), 1, m1.data(), m1.cols(), m2.data(), m2.cols(), 0, m3.data(), m3.cols());

    return m3;
}