#include <exception>
#include <iostream>
#include <sstream>
#include <tuple>
#include <vector>

#include <mkl.h>
#include <pybind11/numpy.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

using T = double;

class Matrix {
private:
    size_t m_nrow = 0;
    size_t m_ncol = 0;
    T* m = nullptr;

public:
    Matrix(size_t _m_nrow, size_t _m_ncol)
    {
        reset_buffer(_m_nrow, _m_ncol);
    }

    Matrix(const Matrix& rhs)
    {
        reset_buffer(rhs.nrow(), rhs.ncol());
        for (size_t r = 0; r < rhs.nrow(); r++) {
            for (size_t c = 0; c < rhs.ncol(); c++) {
                (*this)(r, c) = rhs(r, c);
            }
        }
    }

    ~Matrix()
    {
        reset_buffer(0, 0);
    }

    size_t nrow() const
    {
        return m_nrow;
    }

    size_t ncol() const
    {
        return m_ncol;
    }

    std::vector<T> get_buffer() const
    {
        if (m == nullptr) {
            return std::vector<T>();
        }

        return std::vector<T>(m, m + nrow() * ncol());
    }

    T* get_buffer_address() const
    {
        if (m == nullptr) {
            return nullptr;
        }
        return &m[0];
    }

    T get(std::tuple<int, int> idx)
    {
        auto& [r, c] = idx;
        return (*this)(r, c);
    }

    void set(std::tuple<int, int> idx, T val)
    {
        auto& [r, c] = idx;
        (*this)(r, c) = val;
    }

    std::string repr() const
    {
        std::stringstream ss;
        ss << "<Matrix row=" << nrow() << " col=" << ncol() << ">";
        return ss.str();
    }

    std::string str() const
    {
        std::stringstream ss;
        for (size_t r = 0; r < nrow(); r++) {
            ss << "[";
            for (size_t c = 0; c < ncol(); c++) {
                ss << " " << (*this)(r, c);
            }
            ss << " ]"
               << "\n "[r == nrow() - 1];
        }
        return ss.str();
    }

    T& operator()(size_t r, size_t c) const
    {
        this->ensure_inbound(r, c);
        return m[r * ncol() + c];
    }

    bool operator==(const Matrix& rhs) const
    {
        if (nrow() != rhs.nrow() || ncol() != rhs.ncol()) {
            return false;
        }

        for (size_t r = 0; r < nrow(); r++) {
            for (size_t c = 0; c < ncol(); c++) {
                if ((*this)(r, c) != (rhs)(r, c)) {
                    return false;
                }
            }
        }

        return true;
    }

    bool operator!=(const Matrix& rhs) const
    {
        return !((*this) == rhs);
    }

private:
    void reset_buffer(size_t nrow, size_t ncol)
    {
        if (m != nullptr) {
            delete[] m;
        }

        m_nrow = nrow;
        m_ncol = ncol;

        size_t size = nrow * ncol;
        if (size) {
            m = new T[size]();
        }
    }

    inline void ensure_inbound(size_t r, size_t c) const
    {
        if (r < 0 || nrow() <= r || c < 0 || ncol() <= c) {
            throw std::out_of_range("Index out of range");
        }
    }
};

inline void ensure_multipliable(Matrix lhs, Matrix rhs)
{
    if (lhs.ncol() != rhs.nrow()) {
        throw std::domain_error("lhs.ncol != rhs.nrow");
    }
}

Matrix multiply_naive(Matrix lhs, Matrix rhs)
{
    ensure_multipliable(lhs, rhs);

    Matrix res(lhs.nrow(), rhs.ncol());
    for (size_t i = 0; i < lhs.nrow(); i++) {
        for (size_t j = 0; j < rhs.ncol(); j++) {
            for (size_t k = 0; k < lhs.ncol(); k++) {
                res(i, j) += lhs(i, k) * rhs(k, j);
            }
        }
    }

    return res;
}

Matrix multiply_mkl(Matrix lhs, Matrix rhs)
{
    ensure_multipliable(lhs, rhs);

    Matrix res(lhs.nrow(), rhs.ncol());
    cblas_dgemm(
        CBLAS_LAYOUT::CblasRowMajor,
        CBLAS_TRANSPOSE::CblasNoTrans,
        CBLAS_TRANSPOSE::CblasNoTrans,
        lhs.nrow(),
        rhs.ncol(),
        lhs.ncol(),
        1,
        lhs.get_buffer_address(),
        lhs.ncol(),
        rhs.get_buffer_address(),
        rhs.ncol(),
        0,
        res.get_buffer_address(),
        res.ncol());

    return res;
}

Matrix multiply_tile(Matrix lhs, Matrix rhs, size_t block_size)
{
    ensure_multipliable(lhs, rhs);

    Matrix res(lhs.nrow(), rhs.ncol());
    for (size_t ii = 0; ii < lhs.nrow(); ii += block_size) {
        for (size_t jj = 0; jj < rhs.ncol(); jj += block_size) {
            for (size_t kk = 0; kk < lhs.ncol(); kk += block_size) {
                size_t min_i = std::min(ii + block_size, lhs.nrow());
                for (size_t i = ii; i < min_i; i++) {
                    size_t min_j = std::min(jj + block_size, rhs.ncol());
                    for (size_t j = jj; j < min_j; j++) {
                        size_t min_k = std::min(kk + block_size, lhs.ncol());
                        for (size_t k = kk; k < min_k; k++) {
                            res(i, j) += lhs(i, k) * rhs(k, j);
                        }
                    }
                }
            }
        }
    }
    return res;
}

PYBIND11_MODULE(_matrix, m)
{
    m.doc() = "meow";

    py::class_<Matrix>(m, "Matrix")
        .def(py::init<const Matrix&>())
        .def(py::init<size_t, size_t>())
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol)
        .def_property_readonly("array", [](Matrix& m) -> py::array_t<T> {
            return py::array_t<T>(
                py::memoryview::from_buffer(
                    m.get_buffer_address(),
                    { m.nrow(), m.ncol() },
                    { sizeof(T) * m.ncol(), sizeof(T) }));
        })
        .def("get_buffer", &Matrix::get_buffer)
        .def("__getitem__", &Matrix::get)
        .def("__setitem__", &Matrix::set)
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def("__repr__", &Matrix::repr)
        .def("__str__", &Matrix::str);

    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_mkl", &multiply_mkl);
    m.def("multiply_tile", &multiply_tile);
}
