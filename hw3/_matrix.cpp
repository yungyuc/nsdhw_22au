include<iostream>
include<vector>

using namespace std

class Matrix {

public:

    Matrix(size_t nrow, size_t ncol)
        : m_nrow(nrow), m_ncol(ncol)
    {
        reset_buffer(nrow , ncol)
    }

    
    Matrix(Matrix const& other) : m_nrow(nrow), m_ncol(ncol)
    {
        reset_buffer(other.m_nrow, other.m_nol);
        for (size_t i = 0; i < m_row; i += 1)
        {
            for (size_t j = 0; j < m_ncol; j += 1)
            {
                (*this)(i, j) = other(i, j);
            }
        }
    }
    void reset_buffer(size_t nrow, size_t ncol)
    {
        if (m_buffer)
        {
            delete[] m_buffer;
        }
        m_buffer = new double[nrow * ncol];
    }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }

    //operator
    double operator()(size_t row, size_t col)const
    {
        size_t index = row * m_ncol +col
        return m_buffer[index]
    }

    double& operator()(size_t row, size_t col)
    {
        size_t index = row * m_ncol + col
        return m_buffer[index]
    }

    bool operator==(const Matrix& other) const
    {
        //bool flag = true
        for (size_t i = 0; i < m_nrow; ++i)
        {
            for (size_t j = 0; j < m_ncol; ++j)
            {
                size_t idx = i * m_ncol + j;
                if (m_buffer[idx] != other.m_buffer[idx])
                    return false;
            }
        }

        return true;
    }


private:

    size_t m_nrow;
    size_t m_ncol;
    double* m_buffer;

};

Matrix multiply_naive(const Matrix& mat1, const Matrix& mat2)
{
    Matrix mat(mat1.nrow(), mat2.ncol());
    for (size_t i = 0; i < mat1.nrow(); i++)
    {
        for (size_t k = 0; k < mat2.ncol(); k++)
        {
            double v = 0;
            for (size_t j = 0; j < mat1.ncol(); j++)
            {
                v += mat1(i, j) * mat2(j, k);
            }
            mat(i, k) = v;
        }
    }
    return mat;
}


Matrix multiply_tile(const Matrix& mat1, const Matrix& mat2, size_t len_size)
{
    Matrix mat(mat1.nrow(), mat2.ncol());
    for (size_t i_start = 0; i_start < mat1.nrow(); i_start += len_size)
    {
        for (size_t k_start = 0; k_start < mat2.ncol(); k_start += len_size)
        {
            for (size_t j_start = 0; j_start < mat1.ncol(); j_start += len_size)
            {
                size_t i_end = min(i_start + len_size, mat1.nrow());
                size_t k_end = min(k_start + len_size, mat2.ncol());
                size_t j_end = min(j_start + len_size, mat1.ncol());

                for (size_t i = i_start; i < i_end; i++)
                {
                    for (size_t k = k_start; k < k_end; k++)
                    {
                        for (size_t j = j_start; j < j_end; j++)
                        {
                            mat(i, k) += mat1(i, j) * mat2(j, k);
                        }
                    }
                }
            }
        }
    }
    return mat;
}

Matrix multiply_mkl(const Matrix& mat1, const Matrix& mat2)
{
    Matrix mat(mat1.nrow(), mat2.ncol());
    size_t m = mat1.nrow();
    size_t n = mat2.ncol();
    size_t k = mat1.ncol();
    double alpha = 1.0;
    double beta = 0.0;
    double* A = mat1.m_buffer;
    double* B = mat2.m_buffer;
    double* C = ret.m_buffer;
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
        m, n, k, alpha, A, k, B, n, beta, C, n);
    return mat;
}
PYBIND11_MODULE(_matrix, m) {

    m.doc() = "Matrix";

    py::class_<Matrix>(m, "Matrix")
        .def(py::init<size_t, size_t>())
        .def(py::init<size_t, size_t, const std::vector<double>&>())
        .def("__getitem__", [](const Matrix& self, const std::vector<size_t> idx) {
        return self(idx[0], idx[1]);
            })
        .def("__setitem__", [](Matrix& self, const std::vector<size_t> idx, const double value) {
                self(idx[0], idx[1]) = value;
            })
                .def("__eq__", &Matrix::operator==)
                .def("__ne__", &Matrix::operator!=)
                .def_property_readonly("nrow", &Matrix::nrow)
                .def_property_readonly("ncol", &Matrix::ncol)
                .def_property_readonly("size", &Matrix::size);

            m.def("multiply_naive", &multiply_naive, "multiply 2 matrix naively.");
            m.def("multiply_tile", &multiply_tile, "multiply 2 matrix by tiling the matrices.");
            m.def("multiply_mkl", &multiply_mkl, "multiply 2 matrix by using mkl library.");
}