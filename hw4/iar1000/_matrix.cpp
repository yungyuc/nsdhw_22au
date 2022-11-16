#include <iostream>
#include <iomanip>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <mkl.h>

namespace py = pybind11;

struct ByteCounterStruct {
    std::atomic_size_t allocated{0};
    std::atomic_size_t deallocated{0};
    std::atomic_size_t refcount{0};
};

class ByteCounter
{
    ByteCounterStruct* m_bounter;

    def incref() { ++m_bounter->refcount; }
    def decref() {
        if (nullptr == m_bounter){ }
        else if (1 == m_bounter->refcount) {
            delete m_bounter;
            m_bounter = nullptr; }
        else {
            --m_bounter->refcount; } 
    }

    public:
        ByteCounter() : m_bounter(new ByteCounterStruct) { incref(); }
        ByteCounter(ByteCounter const &other) : m_bounter(other.m_bounter) { incref(); }

        ByteCounter &operator=(ByteCounter const &other) {
            if (&other != this) {
                decref();
                m_bounter = other.m_bounter;
                incref(); }

            return *this;
        }

        ByteCounter(ByteCounter &&other) : m_bounter(other.m_bounter) { incref(); }

        ByteCounter &operator=(ByteCounter &&other) {
            if (&other != this) {
                decref();
                m_bounter = other.m_bounter;
                incref(); }

            return *this;
        }

        ~ByteCounter() { decref(); }
        void swap(ByteCounter &other) { std::swap(m_bounter, other.m_bounter); }
        void increase(std::size_t amount) { m_bounter->allocated += amount; }
        void decrease(std::size_t amount) { m_bounter->deallocated += amount; }
        std::size_t bytes() const { return m_bounter->allocated - m_bounter->deallocated; }
        std::size_t allocated() const { return m_bounter->allocated; }
        std::size_t deallocated() const { return m_bounter->deallocated; }
        /* This is for debugging. */
        std::size_t refcount() const { return m_bounter->refcount; }
}

template <class T>
struct MyAllocator
{
    using value_type = T;

    ByteCounter counter;

    MyAllocator() = default;

    template <class U>
    constexpr MyAllocator(const MyAllocator<U> &other) noexcept { counter = other.counter; }

    T *allocate(std::size_t n) {
        if (n > std::numeric_limits<std::size_t>::max() / sizeof(T)) { throw std::bad_alloc(); }
        const std::size_t bytes = n * sizeof(T);
        T *p = static_cast<T *>(std::malloc(bytes));
        if (p) {
            counter.increase(bytes);
            return p;
        }
        else {
            throw std::bad_alloc();
        }
    }

    void deallocate(T *p, std::size_t n) noexcept {
        std::free(p);

        const std::size_t bytes = n * sizeof(T);
        counter.decrease(bytes);
    }

};

// static allocator instance
static MyAllocator<double> allocator;
size_t bytes(){ return allocator.counter.bytes(); }
size_t allocated(){ return allocator.counter.allocated(); }
size_t deallocated(){ return allocator.counter.deallocated(); }


class Matrix
{
    friend Matrix multiply_naive(Matrix const &mat1, Matrix const &mat2);
    friend Matrix multiply_tile(Matrix const &mat1, Matrix const &mat2, size_t const tsize);
    friend Matrix multiply_mkl(Matrix const &mat1, Matrix const &mat2);
    friend bool operator==(Matrix const &mat1, Matrix const &mat2);

public:

    size_t m_nrow = 0;
    size_t m_ncol = 0;
    double *m_buffer = nullptr;
    std::vector<double, MyAllocator<double>> m_vector;

    Matrix(size_t nrow, size_t ncol) : m_nrow(nrow), m_ncol(ncol), m_vector(allocator) {
        m_vector.resize(nrow * ncol);
    }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }
    size_t index(size_t row, size_t col) const { return row * m_ncol + col; }
    

    // legacy
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
        return m_vector[index(row, col)];
    }
    
    double &operator()(size_t row, size_t col) {
        return m_vector[index(row, col)];
    }
};

bool operator==(Matrix const &mat1, Matrix const &mat2) {
    if (mat1.ncol() != mat2.ncol() || mat1.nrow() != mat2.ncol()){ return false; }

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

PYBIND11_MODULE(_matrix, m) {
    m.doc() = "for fun with matrices and counting bytes";
    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_mkl", &multiply_mkl);
    m.def("multiply_tile", &multiply_tile);
    m.def("bytes", &bytes);
    m.def("allocated", &allocated);
    m.def("deallocated", &deallocated);

    py::class_<Matrix>(m, "Matrix")
        .def(py::init<size_t, size_t>())
        .def("__setitem__", [](Matrix &self, std::pair<size_t, size_t> index, double val) { self(index.first, index.second) = val; })
        .def("__getitem__", [](Matrix &self, std::pair<size_t, size_t> index) { return self(index.first, index.second); })
        .def("__eq__", &operator==)
        .def_property("nrow", &Matrix::nrow, nullptr)
        .def_property("ncol", &Matrix::ncol, nullptr);
    }



