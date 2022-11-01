#include <cstdlib>
#include <vector>
#include <pybind11/pybind11.h>
#include <mkl.h>

namespace py = pybind11;

class Matrix
{
public:
    Matrix(size_t m, size_t n) : nrow(m), ncol(n), m_buffer(m * n) {}

    double operator()(size_t row, size_t col) const
    {
        return m_buffer[row*ncol + col];
    }
    double& operator()(size_t row, size_t col)
    {
        return m_buffer[row*ncol + col];
    }
    double operator()(size_t index) const
    {
        return m_buffer[index];
    }
    double& operator()(size_t index)
    {
        return m_buffer[index];
    }

public:
    const size_t nrow;
    const size_t ncol;
    std::vector<double> m_buffer;
};

bool operator==(const Matrix &mat1, const Matrix &mat2)
{
    if (mat1.ncol != mat2.ncol || mat1.nrow != mat2.nrow) {
        return false;
    }
    for (size_t row = 0; row < mat1.nrow; row++) {
        for (size_t col = 0; col < mat1.ncol; col++) {
            if (mat1(row, col) != mat2(row, col)) {
                return false;
            }
        }
    }
    return true;
}

void validate_multiplication(const Matrix &mat1, const Matrix &mat2)
{
    if (mat1.ncol != mat2.nrow)
    {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }
}


Matrix multiply_naive(const Matrix &mat1, const Matrix &mat2)
{
    validate_multiplication(mat1, mat2);
    Matrix ret(mat1.nrow, mat2.ncol);
    for (size_t i = 0; i < ret.nrow; i++) {
        for (size_t k = 0; k < ret.ncol; k++) {
            for (size_t j = 0; j < mat1.ncol; j++) {
                ret(i, k) += mat1(i, j) * mat2(j, k);
            }
        }
    }
    return ret;
}

Matrix multiply_mkl(const Matrix &mat1, const Matrix &mat2)
{
    validate_multiplication(mat1, mat2);
    Matrix ret(mat1.nrow, mat2.ncol);
    cblas_dgemm(
        CblasRowMajor /* const CBLAS_LAYOUT Layout */
      , CblasNoTrans /* const CBLAS_TRANSPOSE transa */
      , CblasNoTrans /* const CBLAS_TRANSPOSE transb */
      , mat1.nrow /* const MKL_INT m */
      , mat2.ncol /* const MKL_INT n */
      , mat1.ncol /* const MKL_INT k */
      , 1.0 /* const double alpha */
      , mat1.m_buffer.data() /* const double *a */
      , mat1.ncol /* const MKL_INT lda */
      , mat2.m_buffer.data() /* const double *b */
      , mat2.ncol /* const MKL_INT ldb */
      , 0.0 /* const double beta */
      , ret.m_buffer.data() /* double * c */
      , ret.ncol /* const MKL_INT ldc */
    );
    return ret;
}

Matrix multiply_tile(const Matrix &mat1, const Matrix &mat2, size_t T)
{
    validate_multiplication(mat1, mat2);
    size_t M = mat1.nrow, N = mat2.ncol, K = mat1.ncol;
    Matrix ret(M, N);
    for (size_t m = 0; m < M; m += T) {
        for (size_t n = 0; n < N; n += T) {
            for (size_t k = 0; k < K; k += T) {
                const size_t minMt = std::min(m + T, M);
                const size_t minNt = std::min(n + T, N);
                const size_t minKt = std::min(k + T, K);
                for (size_t mt = m; mt < minMt; mt++) {
                    for (size_t nt = n; nt < minNt; nt++) {
                        for (size_t kt = k; kt < minKt; kt++) {
                            ret(mt * M + nt) += mat1(mt * M + kt)
                                                * mat2(kt * K + nt);
                        }
                    }
                }
            }
        }
    }
    return ret;
}

PYBIND11_MODULE(_matrix, m)
{
    py::class_<Matrix>(m, "Matrix")
        .def(py::init<size_t, size_t>())
        .def("__eq__", &operator==)
        .def("__getitem__",
            [](Matrix &matrix, std::pair<size_t, size_t> args) {
                return matrix(args.first, args.second);
            }
        )
        .def("__setitem__",
            [](Matrix &matrix, std::pair<size_t, size_t> args, double value) {
                matrix(args.first, args.second) = value;
            }
        )
        .def_readonly("nrow", &Matrix::nrow)
        .def_readonly("ncol", &Matrix::ncol);
    m.def("multiply_naive", &multiply_naive)
        .def("multiply_mkl", &multiply_mkl)
        .def("multiply_tile", &multiply_tile);
}
