#pragma once

#include <iostream>
#include <limits>
#include <vector>
#include <cassert>

using namespace std;

// ---------- Implement my ByteCounter ----------
struct ByteCounterImplement {
    size_t allocated = 0;
    size_t deallocated = 0;
};

class ByteCounter {
private:
    ByteCounterImplement* m_implement;

public:
    ByteCounter(): m_implement(new ByteCounterImplement) {}

    ByteCounter(const ByteCounter& counter): m_implement(counter.m_implement) {}

    ByteCounter(ByteCounter&& counter): m_implement(counter.m_implement) {}

    ~ByteCounter() = default;

    ByteCounter& operator=(const ByteCounter& counter) = delete;
    ByteCounter& operator=(ByteCounter&& counter) = delete;

    void increase(size_t n_size){
        this->m_implement->allocated += n_size;
    }

    void decrease(size_t n_size){
        this->m_implement->deallocated += n_size;
    }

    size_t bytes() const{
        return (this->m_implement->allocated - this->m_implement->deallocated);
    }

    size_t allocated() const{
        return this->m_implement->allocated;
    }

    size_t deallocated() const{
        return this->m_implement->deallocated;
    }

    constexpr bool operator==(const ByteCounter& counter) const{
        return (this->m_implement == counter.m_implement);
    }
};

// ---------- Implement Allocator ----------
template<class T>
class Allocator {
private:
    ByteCounter my_counter;

public:
    using value_type = T;

    Allocator() = default;

    template<class U>
    Allocator(const Allocator<U>& rhs) noexcept : my_counter(rhs.my_counter) {}

    ~Allocator() = default;

    T* allocate(size_t n){
        // check allocate size
        assert(n <= numeric_limits<size_t>::max() / sizeof(T));

        const size_t bytes = n * sizeof(T);
        T* ptr = static_cast<T*>(malloc(bytes));

        // check malloc status
        assert(ptr);

        this->my_counter.increase(bytes);
        cout << "Allocate increase: " << this->my_counter.bytes() << endl;

        return ptr;
    }

    void deallocate(T* ptr, size_t n){
        free(ptr);
        this->my_counter.decrease(n * sizeof(T));
        cout << "Deallocate decrease: " << this->my_counter.bytes() << endl;
    }

    constexpr size_t bytes() const{
        return this->my_counter.bytes();
    }

    constexpr size_t allocated() const{
        return this->my_counter.allocated();
    }

    constexpr size_t deallocated() const{
        return this->my_counter.deallocated();
    }

    constexpr bool operator==(const Allocator<T>& rhs) const{
        return (this->my_counter == rhs.my_counter);
    }

    friend ostream& operator<<(ostream& out, const Allocator<T>& rhs){
        out << "MyAllocator: bytes = " << rhs.bytes() << '\n'\
            << "               allocated = " << rhs.allocated() << '\n'\
            << "               deallocated = " << rhs.deallocated() << endl;

        return out;
    }
};

// Allocator init in global
Allocator<double> my_allocator;

size_t bytes(){
    return my_allocator.bytes();
}

size_t allocated(){
    return my_allocator.allocated();
}

size_t deallocated(){
    return my_allocator.deallocated();
}

template<typename T>
class Matrix {
private:
    size_t m_rows, m_cols;
    std::vector<T, Allocator<double>> m_matrix = vector<T, Allocator<double>>(my_allocator);

public:
    Matrix(): m_rows(0), m_cols(0) {}

    Matrix(size_t rows, size_t cols): m_rows(rows), m_cols(cols){
        this->m_matrix.resize(this->m_rows * this->m_cols, 0);
    }

    Matrix(Matrix const& counter): m_rows(counter.m_rows), m_cols(counter.m_cols), m_matrix(counter.m_matrix){}

    Matrix(Matrix&& counter): m_rows(counter.m_rows), m_cols(counter.m_cols), m_matrix(move(counter.m_matrix)){}

    ~Matrix(){
        this->m_matrix.clear();
        this->m_matrix.shrink_to_fit();
    }

    Matrix& operator=(Matrix const& counter){
        if (this != &counter) {
            this->m_rows = counter.m_rows;
            this->m_cols = counter.m_cols;
            this->m_matrix = counter.m_matrix;
        }

        return *this;
    }

    Matrix& operator=(Matrix&& counter){
        if (this != &counter) {
            this->m_rows = counter.m_rows;
            this->m_cols = counter.m_cols;
            this->m_matrix = move(counter.m_matrix);
        }

        return *this;
    }

    T const& operator()(size_t i, size_t j) const{
        return this->m_matrix[i * this->m_cols +j];
    }

    T& operator()(size_t i, size_t j){
        return this->m_matrix[i * this->m_cols + j];
    }

    const T* data() const{
        return &(this->m_matrix[0]);
    }

    T* data(){
        return &(this->m_matrix[0]);
    }

    constexpr bool operator==(const Matrix& counter) const{
        return (this->m_matrix == counter.m_matrix);
    }

    constexpr size_t rows() const{
        return this->m_rows;
    }

    constexpr size_t cols() const{
        return this->m_cols;
    }

    friend ostream& operator<<(ostream& os, const Matrix<T>& matrix){
        for (size_t i = 0; i < matrix.rows(); i++) {
            for (size_t j = 0; j < matrix.cols(); j++) {
                os << matrix(i, j) << (j == matrix.cols() - 1 ? "" : " ");
            }

            if (i < matrix.rows() - 1) os << '\n';
        }

        return os;
    }
};
 98  
hw4/LanceWang12/main.cpp
Viewed
@@ -0,0 +1,98 @@
#include <cassert>
#include <utility>
#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

#include "mkl.h"

#include "Matrix.h"

using namespace std;

template<typename T>
Matrix<T> multiply_naive(const Matrix<T>& m1, const Matrix<T>& m2)
{
    assert(m1.cols() == m2.rows());

    Matrix<T> m3(m1.rows(), m2.cols());

    size_t rows = m1.rows();
    size_t cols = m2.cols();
    size_t inners = m1.cols();

    for (size_t row = 0; row < rows; row++) {
        for (size_t col = 0; col < cols; col++) {
            for (size_t inner = 0; inner < inners; inner++) {
                m3(row, col) += m1(row, inner) * m2(inner, col);
            }
        }
    }

    return m3;
}

template<typename T>
Matrix<T> multiply_tile(const Matrix<T>& m1, const Matrix<T>& m2, size_t size)
{
    assert(m1.cols() == m2.rows());

    Matrix<T> m3(m1.rows(), m2.cols());

    for (size_t row = 0; row < m1.rows(); row += size) {
        for (size_t col = 0; col < m2.cols(); col += size) {
            for (size_t inner = 0; inner < m1.cols(); inner += size) {
                // tile
                for (size_t k = inner; k < min(m1.cols(), inner + size); k++) {
                    for (size_t i = row; i < min(m1.rows(), row + size); i++) {
                        for (size_t j = col; j < min(m2.cols(), col + size); j++) {
                            m3(i, j) += m1(i, k) * m2(k, j);
                        }
                    }
                }
            }
        }
    }

    return m3;
}

template<typename T>
Matrix<T> multiply_mkl(Matrix<T>& m1, Matrix<T>& m2)
{
    assert(m1.cols() == m2.rows());

    Matrix<T> m3(m1.rows(), m2.cols());

    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m1.rows(), m2.cols(), m1.cols(), 1, m1.data(), m1.cols(), m2.data(), m2.cols(), 0, m3.data(), m3.cols());

    return m3;
}

PYBIND11_MODULE(_matrix, m)
{
    pybind11::class_<Matrix<double>>(m, "Matrix")
        .def(pybind11::init<size_t, size_t>())
        .def("__getitem__", [](Matrix<double>& m, pair<size_t, size_t> index) {
            return m(index.first, index.second);
        })
        .def("__setitem__", [](Matrix<double>& m, pair<size_t, size_t> index, double value) {
            m(index.first, index.second) = value;
        })
        .def("__str__", [](Matrix<double>& m) {
            stringstream ss;
            ss << m;

            return ss.str();
        })
        .def(pybind11::self == pybind11::self)
        .def_property_readonly("nrow", &Matrix<double>::rows)
        .def_property_readonly("ncol", &Matrix<double>::cols);

    m.def("multiply_naive", multiply_naive<double>, "Matrix multiplication with naive method.");
    m.def("multiply_tile", multiply_tile<double>, "Matrix multiplication with tile method.");
    m.def("multiply_mkl", multiply_mkl<double>, "Matrix multiplication with mkl method.");
    m.def("bytes", bytes, "Allocator bytes.");
    m.def("allocated", allocated, "Allocator allocated.");
    m.def("deallocated", deallocated, "Allocator deallocated.");
}