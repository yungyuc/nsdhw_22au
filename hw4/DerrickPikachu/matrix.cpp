#include <iostream>
#include <mkl.h>

#include "matrix.h"

namespace matrix
{

int bytes() { return MatrixAllocatorCounter<double>::allocate_bytes - MatrixAllocatorCounter<double>::deallocate_bytes; }
int allocated() { return MatrixAllocatorCounter<double>::allocate_bytes; }
int deallocated() { return MatrixAllocatorCounter<double>::deallocate_bytes; }

Matrix multiply_naive(const Matrix & m1, const Matrix & m2)
{
    if (m1.getNcol() != m2.getNrow()) { 
        std::cerr << "Invalid multiplication" << std::endl;
        exit(1);
    }

    Matrix result(m1.getNrow(), m2.getNcol());
    for (int i = 0; i < m1.getNrow(); i++) {
        for (int j = 0; j < m2.getNcol(); j++) {
            double value = 0.0;
            for (int k = 0; k < m1.getNcol(); k++) {
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
    Matrix result(m1.getNrow(), m2.getNcol());

    cblas_dgemm(
        CblasRowMajor,
        CblasNoTrans,
        CblasNoTrans,
        m1.getNrow(),
        m2.getNcol(),
        m1.getNcol(),
        1.0,
        m1.getBuffer(),
        m1.getNcol(),
        m2.getBuffer(),
        m2.getNcol(),
        0.0,
        result.getBuffer(),
        result.getNcol()
    );
    return result;
}

Matrix multiply_tile(const Matrix & m1, const Matrix & m2, int tsize)
{
    Matrix res(m1.getNrow(), m2.getNcol());
    for (int i = 0; i < m1.getNrow(); i += tsize) {
        for (int j = 0; j < m2.getNcol(); j += tsize) {
            for (int k = 0; k < m1.getNcol(); k += tsize) {
                multiply_block(res, m1, m2, i, j, k, tsize);
            }
        }
    }
    return res;
}

void multiply_block(Matrix & res, const Matrix & m1, const Matrix & m2, int row, int col, int multiply_iter, int tsize)
{
    for (int i = row; i < std::min(m1.getNrow(), row + tsize); i++) {
        for (int k = multiply_iter; k < std::min(m1.getNcol(), multiply_iter + tsize); k++) {
            for (int j = col; j < std::min(m2.getNcol(), col + tsize); j++) {
                res(i, j) += m1(i, k) * m2(k, j);
            }
        }
    }
}

Matrix::Matrix() : elements(MatrixAllocatorCounter<double>())
{
    nrow = ncol = 0;
}

Matrix::Matrix(int rsize, int csize) : elements(MatrixAllocatorCounter<double>())
{
    nrow = rsize;
    ncol = csize;
    elements.resize(nrow * ncol, 0);
}

bool Matrix::operator== (const Matrix & other) const
{
    if (nrow == other.getNrow() && ncol == other.getNcol()) {
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

}