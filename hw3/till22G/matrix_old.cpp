#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <functional>
#include <chrono>
#include <math.h>
//#include <mkl.h>

#include <pybind11/stl.h>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

namespace py = pybind11;

class Matrix {

public:

    Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol)
    {
        size_t nelement = nrow * ncol;
        m_buffer = new double[nelement];
    }

    ~Matrix()
    {
        delete[] m_buffer;
    }

    // No bound check.
    double   operator() (size_t row, size_t col) const
    {
        return m_buffer[row*m_ncol + col];
    }
    double & operator() (size_t row, size_t col)
    {
        return m_buffer[row*m_ncol + col];
    }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }

private:

    size_t m_nrow;
    size_t m_ncol;
    double * m_buffer;

};

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

const Matrix multiply_tiling(Matrix const& m1, Matrix const& m2) {

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
            for (size_t k = 0; k < row; k += incr){
                size_t k_row_min = std::min(k + incr, row);

                // inner loops performing matrix multiplications on tiles
                for (size_t x = i; x < x_row_min; x++) {
                    for (size_t y = j; y < j_col_min; y++) {
                        for (size_t z = k; z < k_row_min; z++) {

                            resultM[x * col + y] += m1[x * col + z] * m2[z * col + y];

                        }
                    }
                }
            }
        }
    }
    return resultM;
}

/*
Matrix multiply_mkl(Matrix const& m1, Matrix const& m2)
{
    mkl_set_num_threads(1);
    Matrix resultM = Matrix(m1.nrow(), m2.ncol());

    cblas_dgemm(
        CblasRowMajor
        , CblasNoTrans
        , CblasNoTrans
        , m1.nrow()
        , m2.ncol()
        , m1.ncol()
        , 1.0
        , m1.m_buffer
        , m1.ncol()
        , m2.m_buffer
        , m2.ncol()
        , 0.0
        , resultM.m_buffer
        , resultM.ncol()
    );

   return resultM;
}
*/

PYBIND11_MODULE(_matrix, handle){
	handle.doc() =  "";
	handle.def("multiply_tiling", &multiply_tiling);
	handle.def("multiply_naive", &multiply_naive);
	py::class_<Matrix>(handle, "Matrix")
        	.def(py::init<size_t, size_t>())
		.def(py::init<size_t, size_t, std::vector<double> const&>())
		.def("__getitem__", [](Matrix &self, std::pair<size_t, size_t> index, double val) {self(index.first, index.second) = val;})
		.def("__setitem__", [](Matrix &self, std::pair<size_t, size_t> index) {return self(index.first, index.second);});

};

