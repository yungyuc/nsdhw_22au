#include "mkl.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <functional>
#include <utility>

class Matrix {
private:
    size_t m_nrow, m_ncol;
    std::vector<double> m_buffer;

public:
    Matrix()
        : m_nrow(0), m_ncol(0){}

    Matrix(size_t nrow, size_t ncol)
        : m_nrow(nrow), m_ncol(ncol)
    {
        this->m_buffer.resize(this->m_nrow * this->m_ncol, 0);
    }

    Matrix(Matrix const& other)
        : m_nrow(other.m_nrow), m_ncol(other.m_ncol), m_buffer(other.m_buffer){}

    Matrix(Matrix&& other)
        : m_nrow(other.m_nrow), m_ncol(other.m_ncol), m_buffer(std::move(other.m_buffer)){}

    ~Matrix()
    {
        this->m_buffer.clear();
        this->m_buffer.shrink_to_fit();
    }

    Matrix& operator=(Matrix const& other)
    {
        if (this != &other) {
            this->m_nrow = other.m_nrow;
            this->m_ncol = other.m_ncol;
            this->m_buffer = other.m_buffer;
        }

        return *this;
    }

    Matrix& operator=(Matrix&& other)
    {
        if (this != &other) {
            this->m_nrow = other.m_nrow;
            this->m_ncol = other.m_ncol;
            this->m_buffer = std::move(other.m_buffer);
        }

        return *this;
    }

    double const& operator()(size_t i, size_t j) const
    {
        return this->m_buffer[i * this->m_ncol + j];
    }

    double& operator()(size_t i, size_t j)
    {
        return this->m_buffer[i * this->m_ncol + j];
    }

    const double* data() const
    {
        return &(this->m_buffer[0]);
    }

    double* data()
    {
        return &(this->m_buffer[0]);
    }

    bool operator==(const Matrix& other) const
    {
        return (this->m_buffer == other.m_buffer);
    }

    size_t nrow() const
    {
        return this->m_nrow;
    }

    size_t ncol() const
    {
        return this->m_ncol;
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix)
    {
        for (size_t i = 0; i < matrix.nrow(); i++) {
            for (size_t j = 0; j < matrix.ncol(); j++) {
                os << matrix(i, j) << (j == matrix.ncol() - 1 ? "" : " ");
            }

            if (i < matrix.nrow() - 1) os << '\n';
        }

        return os;
    }
};

Matrix multiply_naive(const Matrix& mat1, const Matrix& mat2)
{
    assert(mat1.ncol() == mat2.nrow());

    Matrix mat3(mat1.nrow(), mat2.ncol());

    size_t nrow = mat1.nrow();
    size_t ncol = mat2.ncol();
    size_t nin = mat1.ncol();

    for (size_t row = 0; row < nrow; row++) {
        for (size_t col = 0; col < ncol; col++) {
            for (size_t inner = 0; inner < nin; inner++) {
                mat3(row, col) += mat1(row, inner) * mat2(inner, col);
            }
        }
    }

    return mat3;
}


Matrix multiply_tile(const Matrix& mat1, const Matrix& mat2, size_t size)
{
    assert(mat1.ncol() == mat2.nrow());

    Matrix mat3(mat1.nrow(), mat2.ncol());

    for (size_t row = 0; row < mat1.nrow(); row += size) {
        for (size_t col = 0; col < mat2.ncol(); col += size) {
            for (size_t inner = 0; inner < mat1.ncol(); inner += size) {
                // tile
                for (size_t k = inner; k < std::min(mat1.ncol(), inner + size); k++) {
                    for (size_t i = row; i < std::min(mat1.nrow(), row + size); i++) {
                        for (size_t j = col; j < std::min(mat2.ncol(), col + size); j++) {
                            mat3(i, j) += mat1(i, k) * mat2(k, j);
                        }
                    }
                }
            }
        }
    }

    return mat3;
}


Matrix multiply_mkl(Matrix& mat1, Matrix& mat2)
{
    assert(mat1.ncol() == mat2.nrow());

    Matrix mat3(mat1.nrow(), mat2.ncol());

    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, mat1.nrow(), mat2.ncol(), mat1.ncol(), 1, mat1.data(), mat1.ncol(), mat2.data(), mat2.ncol(), 0, mat3.data(), mat3.ncol());

    return mat3;
}