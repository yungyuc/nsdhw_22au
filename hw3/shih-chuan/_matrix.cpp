#include <mkl.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;

class Matrix 
{
public:
    Matrix(size_t nrow, size_t ncol) : m_nrow{ nrow }, m_ncol{ ncol }
    {
        m_buffer = new double[nrow * ncol];
    }
    size_t const &nrow() const { return m_nrow; } // getter
    size_t const &ncol() const { return m_ncol; } // getter
    double *const &buffer() const { return m_buffer; } // getter
    double operator() (size_t row, size_t col) const { return m_buffer[row * m_ncol + col]; }
    double &operator() (size_t row, size_t col) { return m_buffer[row * m_ncol + col]; }
    bool operator== (const Matrix &m2) const {
        if (m_nrow != m2.nrow() || m_ncol != m2.ncol())
            return false;
        for (size_t i = 0; i < m_nrow * m_ncol; i++) {
            if (m_buffer[i] != m2.buffer()[i])
                return false;
        }
        return true;
    };
    bool operator!= (const Matrix &m2) const { return !(m2 == (*this)); }
private:
    size_t m_nrow;
    size_t m_ncol;
    double * m_buffer;
};

void validate_multiplication(Matrix const & mat1, Matrix const & mat2) {
    if (mat1.ncol() != mat2.nrow()) {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }
}

Matrix multiply_naive(Matrix &mat1, Matrix &mat2) {
    validate_multiplication(mat1, mat2);
    Matrix ret(mat1.nrow(), mat2.ncol());
    for (size_t i=0; i<mat1.nrow(); ++i) {
        for (size_t k=0; k<mat2.ncol(); ++k) {
            double v = 0;
            for (size_t j=0; j<mat1.ncol(); ++j) {
                v += mat1(i,j) * mat2(j,k);
            }
            ret(i,k) = v;
        }
    }
    return ret;
}

Matrix multiply_mkl(const Matrix &m1, const Matrix &m2) {
    validate_multiplication(m1, m2);
    Matrix rslt(m1.nrow(), m2.ncol());
    cblas_dgemm(
        CblasRowMajor,
        CblasNoTrans,
        CblasNoTrans,
        m1.nrow(),
        m2.ncol(),
        m1.ncol(),
        1.0,
        m1.buffer(),
        m1.ncol(),
        m2.buffer(),
        m2.ncol(),
        0.0,
        rslt.buffer(),
        rslt.ncol()
    );
    return rslt;
};

Matrix multiply_tile(Matrix const &A, Matrix const &B, size_t T) {
    validate_multiplication(A, B);
    size_t M = A.nrow();
    size_t N = B.ncol();
    size_t K = A.ncol();
    Matrix C(A.nrow(), B.ncol());
    for (size_t m = 0; m < M; m += T) {
        for (size_t n = 0; n < N; n += T) {
            for (size_t k = 0; k < K; k += T) {
                const size_t minMt = std::min(m + T, M);
                const size_t minNt = std::min(n + T, N);
                const size_t minKt = std::min(k + T, K);
                for (size_t mt = m; mt < minMt; mt++) {
                    for (size_t nt = n; nt < minNt; nt++) {
                        for (size_t kt = k; kt < minKt; kt++) {
                            C(mt, nt) += A(mt, kt) * B(kt, nt);
                        }
                    }
                }
            }
        }
    }
    return C;
}

PYBIND11_MODULE(_matrix, m)
{
    m.doc() = "pybind11 example plugin"; 
    py::class_<Matrix>(m, "Matrix")
        .def(py::init<size_t, size_t>())
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol)
        .def("__getitem__", [](const Matrix & mat, std::pair<size_t, size_t> pos) {
            return mat(pos.first, pos.second);
        })
        .def("__setitem__", [](Matrix & mat, std::pair<size_t, size_t> pos, float value) {
            mat(pos.first, pos.second) = value;
        })
        .def("__eq__", [](const Matrix & m1, const Matrix & m2) {
            return m1 == m2;
        })
        .def("__ne__", [](const Matrix & m1, const Matrix & m2) {
            return m1 != m2;
        });
    m.def("multiply_naive", &multiply_naive)
        .def("multiply_mkl", &multiply_mkl)
        .def("multiply_tile", &multiply_tile);
}