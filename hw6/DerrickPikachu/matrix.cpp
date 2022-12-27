#include <iostream>
#include <mkl.h>

#include "matrix.h"


int bytes() { return MatrixAllocatorCounter<double>::allocate_bytes - MatrixAllocatorCounter<double>::deallocate_bytes; }
int allocated() { return MatrixAllocatorCounter<double>::allocate_bytes; }
int deallocated() { return MatrixAllocatorCounter<double>::deallocate_bytes; }

Matrix multiply_naive(const Matrix & m1, const Matrix & m2)
{
    if (m1.ncol() != m2.nrow()) { 
        std::cerr << "Invalid multiplication" << std::endl;
        exit(1);
    }

    Matrix result(m1.nrow(), m2.ncol());
    for (int i = 0; i < m1.nrow(); i++) {
        for (int j = 0; j < m2.ncol(); j++) {
            double value = 0.0;
            for (int k = 0; k < m1.ncol(); k++) {
                value += m1(i, k) * m2(k, j);
            }
            result(i, j) = value;
        }
    }

    return result;
}

Matrix multiply_mkl(const Matrix & m1, const Matrix & m2)
{
    mkl_set_num_threads(1);
    Matrix result(m1.nrow(), m2.ncol());

    cblas_dgemm(
        CblasRowMajor,
        CblasNoTrans,
        CblasNoTrans,
        m1.nrow(),
        m2.ncol(),
        m1.ncol(),
        1.0,
        m1.getBuffer(),
        m1.ncol(),
        m2.getBuffer(),
        m2.ncol(),
        0.0,
        result.getBuffer(),
        result.ncol()
    );
    return result;
}

Matrix multiply_tile(const Matrix & m1, const Matrix & m2, int tsize)
{
    Matrix res(m1.nrow(), m2.ncol());
    for (int i = 0; i < m1.nrow(); i += tsize) {
        for (int j = 0; j < m2.ncol(); j += tsize) {
            for (int k = 0; k < m1.ncol(); k += tsize) {
                multiply_block(res, m1, m2, i, j, k, tsize);
            }
        }
    }
    return res;
}

void multiply_block(Matrix & res, const Matrix & m1, const Matrix & m2, int row, int col, int multiply_iter, int tsize)
{
    for (int i = row; i < std::min(m1.nrow(), row + tsize); i++) {
        for (int k = multiply_iter; k < std::min(m1.ncol(), multiply_iter + tsize); k++) {
            for (int j = col; j < std::min(m2.ncol(), col + tsize); j++) {
                res(i, j) += m1(i, k) * m2(k, j);
            }
        }
    }
}

Matrix::Matrix() : elements(MatrixAllocatorCounter<double>())
{
    nrow_ = ncol_ = 0;
}

Matrix::Matrix(int rsize, int csize) : elements(MatrixAllocatorCounter<double>())
{
    nrow_ = rsize;
    ncol_ = csize;
    elements.resize(nrow_ * ncol_, 0);
}

bool Matrix::operator== (const Matrix & other) const
{
    if (nrow_ == other.nrow() && ncol_ == other.ncol()) {
        for (size_t i = 0; i < elements.size(); i++) {
            if (elements[i] != other.elements[i]) return false;
        }
        return true;
    }
    return false;
}

bool Matrix::operator!= (const Matrix & other) const 
{
    return !((*this) == other);
}
