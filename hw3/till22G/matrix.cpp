#include <iostream>
#include <iomanip>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <mkl.h>

namespace py = pybind11;

class Matrix
{
    friend Matrix multiply_naive(Matrix const &mat1, Matrix const &mat2);
    friend Matrix multiply_tile(Matrix const &mat1, Matrix const &mat2, size_t const tsize);
    friend Matrix multiply_mkl(Matrix const &mat1, Matrix const &mat2);
    friend bool operator==(Matrix const &mat1, Matrix const &mat2);

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

const Matrix multiply_naive(Matrix & m1, Matrix &m2) {

    Matrix resultM = Matrix(m1.nrow(), m2.ncol());

    for (size_t i = 0; i < m1.nrow(); ++i) {
        for (size_t k = 0; k < m2.ncol(); ++k) {
            double v = 0;
            for (size_t j = 0; j < m1.ncol(); ++j){
                v += m1(i, j) * m2(j, k);
            }
            resultM(i, k) = v;
        }
    }

    return resultM;
};


Matrix multiply_mkl(Matrix const &m1, Matrix const &m2){
    mkl_set_num_threads(1);

    Matrix resultM(m1.nrow(), m2.ncol());
    cblas_dgemm(
        CblasRowMajor,
        CblasNoTrans,
        CblasNoTrans,
        m1.nrow(),
        m2.ncol(),
        m1.ncol(),
        1.0,
        m1.m_buffer,
        m1.ncol(),
        m2.m_buffer,
        m2.ncol(),
        0.0,
        resultM.m_buffer,
        resultM.ncol()
    );

    return resultM;
}


const Matrix multiply_tile(Matrix& m1, Matrix& m2) {

    const size_t row = m1.nrow();
    const size_t col = m2.ncol();

    Matrix resultM = Matrix(m1.nrow(), m2.ncol());

    for (size_t i = 0; i < m1.nrow(); ++i) {
        for (size_t k = 0; k < m2.ncol(); ++k) {
            resultM(i, k) = 0;
        }
    }
   
    // size of cache line (64) devided by size of double(8)
    // bigger numbers of incr give slightly better performance, i guess that
    // is due to effective prefetching
    size_t incr = 8;
    // outer loops looping over tiles
    for (size_t i = 0; i < row; i += incr) {
        size_t x_row_min = std::min(i + incr, row);
        for (size_t j = 0; j < col; j += incr) {
            size_t j_col_min = std::min(j + incr, col);
            for (size_t k = 0; k < col; k += incr){
                size_t k_row_min = std::min(k + incr, row);

                // inner loops performing matrix multiplications on tiles
                for (size_t x = i; x < x_row_min; x++) {
                    for (size_t y = j; y < j_col_min; y++) {
                        for (size_t z = k; z < k_row_min; z++) {

                           // resultM[x * col + y] += m1[x * col + z] * m2[z * col + y];
                           resultM(x, y) += m1(x, z) * m2(z, y);
                        }
                    }
                }
            }
        }
    }
    return resultM;
}


PYBIND11_MODULE(_matrix, m) {
    m.doc() = "matrix muliplication";
    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_mkl", &multiply_mkl);
    m.def("multiply_tile", &multiply_tile);

    py::class_<Matrix>(m, "Matrix")
        .def(py::init<size_t, size_t>())
        .def("__setitem__", [](Matrix &self, std::pair<size_t, size_t> index, double val) { self(index.first, index.second) = val; })
        .def("__getitem__", [](Matrix &self, std::pair<size_t, size_t> index) { return self(index.first, index.second); })
        .def("__eq__", &operator==)
        .def_property("nrow", &Matrix::nrow, nullptr)
        .def_property("ncol", &Matrix::ncol, nullptr);
}
