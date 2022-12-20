#include <mkl.h>
#include <stdexcept>
#include <vector>

struct Matrix {
public:
    Matrix(std::size_t nrow, std::size_t ncol)
        : m_nrow(nrow)
        , m_ncol(ncol) {
        reset_buffer(nrow, ncol);
    }

    Matrix(std::size_t nrow, std::size_t ncol, std::vector<double> const& vec)
        : m_nrow(nrow)
        , m_ncol(ncol) {
        reset_buffer(nrow, ncol);
        std::copy(vec.begin(), vec.end(), m_buffer);
    }

    Matrix(Matrix const& other)
        : m_nrow(other.m_nrow)
        , m_ncol(other.m_ncol) {
        reset_buffer(other.m_nrow, other.m_ncol);
        std::copy(other.m_buffer, other.m_buffer + m_nrow * m_ncol, m_buffer);
    }

    Matrix& operator=(Matrix const& other) {
        if (this == &other) {
            return *this;
        }
        if (m_nrow != other.m_nrow || m_ncol != other.m_ncol) {
            reset_buffer(other.m_nrow, other.m_ncol);
        }
        std::copy(other.m_buffer, other.m_buffer + m_nrow * m_ncol, m_buffer);
        return *this;
    }

    bool operator==(Matrix const& other) const {
        if (m_nrow != other.m_nrow || m_ncol != other.m_ncol) {
            return false;
        }
        return std::equal(m_buffer, m_buffer + (m_nrow * m_ncol), other.m_buffer);
    }

    ~Matrix() {
        reset_buffer(0, 0);
    }

    double operator()(std::size_t row, std::size_t col) const { return m_buffer[index(row, col)]; }
    double& operator()(std::size_t row, std::size_t col) { return m_buffer[index(row, col)]; }

    std::size_t nrow() const { return m_nrow; }
    std::size_t ncol() const { return m_ncol; }

    std::size_t size() const { return m_nrow * m_ncol; }
    double* buffer() const { return m_buffer; }

private:
    std::size_t index(std::size_t row, std::size_t col) const {
        return row * m_ncol + col;
    }

    void reset_buffer(std::size_t nrow, std::size_t ncol) {
        if (m_buffer) {
            delete[] m_buffer;
        }
        const std::size_t nelement = nrow * ncol;
        if (nelement) {
            m_buffer = new double[nelement]();
        } else {
            m_buffer = nullptr;
        }
        m_nrow = nrow;
        m_ncol = ncol;
    }

    std::size_t m_nrow = 0;
    std::size_t m_ncol = 0;
    double* m_buffer = nullptr;
};

Matrix multiply_naive(Matrix const& a, Matrix const& b) {
    if (a.ncol() != b.nrow()) {
        throw std::out_of_range("matrix dimension mismatch");
    }

    Matrix ret(a.nrow(), b.ncol());

    for (std::size_t i = 0; i < ret.nrow(); ++i) {
        for (std::size_t j = 0; j < ret.ncol(); ++j) {
            for (std::size_t k = 0; k < a.ncol(); ++k) {
                ret(i, j) += a(i, k) * b(k, j);
            }
        }
    }

    return ret;
}

Matrix multiply_mkl(Matrix const& a, Matrix const& b) {
    if (a.ncol() != b.nrow()) {
        throw std::out_of_range("matrix dimension mismatch");
    }

    Matrix ret(a.nrow(), b.ncol());

    cblas_dgemm(
        CblasRowMajor,
        CblasNoTrans,
        CblasNoTrans,
        a.nrow(),
        b.ncol(),
        a.ncol(),
        1.0,
        a.buffer(),
        a.ncol(),
        b.buffer(),
        b.ncol(),
        0.0,
        ret.buffer(),
        ret.ncol());

    return ret;
}

Matrix multiply_tile(Matrix const& a, Matrix const& b, std::size_t tiling_size) {
    if (a.ncol() != b.nrow()) {
        throw std::out_of_range("matrix dimension mismatch");
    }

    Matrix ret(a.nrow(), b.ncol());

    for (std::size_t i = 0; i < ret.nrow(); i += tiling_size) {
        for (std::size_t j = 0; j < ret.ncol(); j += tiling_size) {
            for (std::size_t k = 0; k < a.ncol(); k += tiling_size) {
                for (std::size_t ii = i; ii < std::min(i + tiling_size, ret.nrow()); ++ii) {
                    for (std::size_t jj = j; jj < std::min(j + tiling_size, ret.ncol()); ++jj) {
                        for (std::size_t kk = k; kk < std::min(k + tiling_size, a.ncol()); ++kk) {
                            ret(ii, jj) += a(ii, kk) * b(kk, jj);
                        }
                    }
                }
            }
        }
    }

    return ret;
}