#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <vector>
#include <mkl.h>

namespace py = pybind11;

class Matrix
{
    friend Matrix multiply_naive(Matrix const &mat1, Matrix const &mat2);
    friend Matrix multiply_tile(Matrix const &mat1, Matrix const &mat2, size_t const tsize);
    friend Matrix multiply_mkl(Matrix const &mat1, Matrix const &mat2);
    friend bool operator==(Matrix const &mat1, Matrix const &mat2);

public:
    Matrix(size_t nrow, size_t ncol) : m_nrow(nrow), m_ncol(ncol)
    {
        reset_buffer(nrow, ncol);
    }
    size_t index(size_t row, size_t col) const
    {
        return row * m_ncol + col;
    }
    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }
    void reset_buffer(size_t nrow, size_t ncol)
    {
        if (m_buffer)
        {
            delete[] m_buffer;
        }
        const size_t nelement = nrow * ncol;
        if (nelement)
        {
            m_buffer = new double[nelement];
        }
        else
        {
            m_buffer = nullptr;
        }
        m_nrow = nrow;
        m_ncol = ncol;
    }
    double operator()(size_t row, size_t col) const
    {
        return m_buffer[index(row, col)];
    }
    double &operator()(size_t row, size_t col)
    {
        return m_buffer[index(row, col)];
    }

    size_t m_nrow = 0;
    size_t m_ncol = 0;
    double *m_buffer = nullptr;
};
bool operator==(Matrix const &mat1, Matrix const &mat2)
{
    for (size_t i = 0; i < mat1.nrow(); ++i)
    {
        for (size_t j = 0; j < mat1.ncol(); ++j)
        {
            if (mat1(i, j) != mat2(i, j))
                return false;
        }
    }
    return true;
}

Matrix multiply_naive(const Matrix &mat1, const Matrix &mat2)
{
    Matrix res(mat1.nrow(), mat2.ncol());
    for (size_t i = 0; i < mat1.nrow(); ++i)
    {
        for (size_t k = 0; k < mat2.ncol(); ++k)
        {
            double result = 0;
            for (size_t j = 0; j < mat1.ncol(); ++j)
            {
                result += mat1(i, j) * mat2(j, k);
            }

            res(i, k) = result;
        }
    }
    return res;
}

Matrix multiply_mkl(Matrix const &mat1, Matrix const &mat2)
{
    mkl_set_num_threads(1);

    Matrix ret(mat1.nrow(), mat2.ncol());

    cblas_dgemm(
        CblasRowMajor /* const CBLAS_LAYOUT Layout */
        ,
        CblasNoTrans /* const CBLAS_TRANSPOSE transa */
        ,
        CblasNoTrans /* const CBLAS_TRANSPOSE transb */
        ,
        mat1.nrow() /* const MKL_INT m */
        ,
        mat2.ncol() /* const MKL_INT n */
        ,
        mat1.ncol() /* const MKL_INT k */
        ,
        1.0 /* const double alpha */
        ,
        mat1.m_buffer /* const double *a */
        ,
        mat1.ncol() /* const MKL_INT lda */
        ,
        mat2.m_buffer /* const double *b */
        ,
        mat2.ncol() /* const MKL_INT ldb */
        ,
        0.0 /* const double beta */
        ,
        ret.m_buffer /* double * c */
        ,
        ret.ncol() /* const MKL_INT ldc */
    );
    return ret;
}

Matrix multiply_tile(const Matrix &mat1, const Matrix &mat2, size_t tsize)
{
    Matrix ret(mat1.nrow(), mat2.ncol());
    for (size_t o_i = 0; o_i < mat1.nrow(); o_i += tsize)
    {
        for (size_t o_j = 0; o_j < mat2.ncol(); o_j += tsize)
        {
            for (size_t o_k = 0; o_k < mat2.ncol(); o_k += tsize) //ret(o_i,o_j)=mat1(o_i,o_k)*mat2(o_k,o_j)
            {
                for (size_t i = o_i; i < std::min(o_i + tsize, mat1.nrow()); i++) // tile
                {
                    for (size_t j = o_j; j < std::min(o_j + tsize, mat2.ncol()); j++)
                    {
                        for (size_t k = o_k; k < std::min(tsize + o_k, mat1.nrow()); k++)
                        {
                            ret(i, j) += mat1(i, k) * mat2(k, j);
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
    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_mkl", &multiply_mkl);
    m.def("multiply_tile", &multiply_tile);
    py::class_<Matrix>(m, "Matrix")
        .def(py::init<size_t, size_t>())
        .def("__setitem__", [](Matrix &self, std::pair<size_t, size_t> i, double val) {
            self(i.first, i.second) = val;
        })
        .def("__getitem__", [](Matrix &self, std::pair<size_t, size_t> i) {
            return self(i.first, i.second);
        })
        .def("__eq__", &operator==)
        .def_property("nrow", &Matrix::nrow, nullptr)
        .def_property("ncol", &Matrix::ncol, nullptr);
}