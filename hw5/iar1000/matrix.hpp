#include <iostream>
#include <iomanip>
#include <mkl.h>

namespace py = pybind11;

class Matrix
{
public:

    Matrix(size_t nrow, size_t ncol) : m_nrow(nrow), m_ncol(ncol) {
        reset_buffer(nrow, ncol);
    }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }
    size_t index(size_t row, size_t col) const { return row * m_ncol + col; }
    

    void reset_buffer(size_t nrow, size_t ncol) {
        if (m_buffer) {
            delete[] m_buffer;
        }
        const size_t nelement = nrow * ncol;
        if (nelement) {
            m_buffer = new double[nelement];
        }
        else {
            m_buffer = nullptr;
        }
        m_nrow = nrow;
        m_ncol = ncol;
    }

    double operator()(size_t row, size_t col) const {
        return m_buffer[index(row, col)];
    }
    
    double &operator()(size_t row, size_t col) {
        return m_buffer[index(row, col)];
    }

    double getitem(std::pair<size_t, size_t> id){
        return (*this)(id.first, id.second);
    }
    
    void setitem(std::pair<size_t, size_t> id, double value){
        (*this)(id.first, id.second) = value;
    }

    size_t m_nrow = 0;
    size_t m_ncol = 0;
    double *m_buffer = nullptr;
};

bool operator==(Matrix const &mat1, Matrix const &mat2) {
    for (size_t i = 0; i < mat1.nrow(); ++i) {
        for (size_t j = 0; j < mat1.ncol(); ++j){
            if (mat1(i, j) != mat2(i, j)){
                return false;
            }
        }
    }

    return true;
}


Matrix multiply_naive(const Matrix &mat1, const Matrix &mat2) {
    Matrix result(mat1.nrow(), mat2.ncol());

    for (size_t i = 0; i < mat1.nrow(); ++i){
        for (size_t k = 0; k < mat2.ncol(); ++k){
            double worker = 0;
            for (size_t j = 0; j < mat1.ncol(); ++j){
                worker += mat1(i, j) * mat2(j, k);
            }
            result(i, k) = worker;
        }
    }

    return result;
}

Matrix multiply_mkl(Matrix const &mat1, Matrix const &mat2){
    mkl_set_num_threads(1);

    Matrix result(mat1.nrow(), mat2.ncol());
    cblas_dgemm(
        CblasRowMajor,
        CblasNoTrans,
        CblasNoTrans,
        mat1.nrow(),
        mat2.ncol(),
        mat1.ncol(),
        1.0,
        mat1.m_buffer,
        mat1.ncol(),
        mat2.m_buffer,
        mat2.ncol(),
        0.0,
        result.m_buffer,
        result.ncol()
    );

    return result;
}

// reference: https://stackoverflow.com/questions/15829223/loop-tiling-blocking-for-large-dense-matrix-multiplication
Matrix multiply_tile(const Matrix &mat1, const Matrix &mat2, size_t ts){
    Matrix result(mat1.nrow(), mat2.ncol());
    for (size_t i0 = 0; i0 < mat1.nrow(); i0 += ts){
        for (size_t j0 = 0; j0 < mat2.ncol(); j0 += ts){
            for (size_t k0 = 0; k0 < mat2.ncol(); k0 += ts){
                for (size_t i = i0; i < std::min(i0 + ts, mat1.nrow()); i++){
                    for (size_t j = j0; j < std::min(j0 + ts, mat2.ncol()); j++){
                        for (size_t k = k0; k < std::min(ts + k0, mat1.nrow()); k++){
                            result(i, j) += mat1(i, k) * mat2(k, j);
                        }
                    }
                }
            }
        }
    }

    return result;
}

