#pragma once

#include <vector>
#include <atomic>
#include <cstddef>

struct ByteCounterImpl {
    std::atomic_size_t allocated{0};
    std::atomic_size_t deallocated{0};
    std::atomic_size_t refcount{0};
};

class ByteCounter {
public:
    ByteCounter():m_impl(new ByteCounterImpl) {
        incref();
    }

    ByteCounter(ByteCounter const &other):m_impl(other.m_impl) {
        incref();
    }

    ByteCounter &operator=(ByteCounter const &other) {
        if (&other != this) {
            decref();
            m_impl = other.m_impl;
            incref();
        }
        return *this;
    }

    ByteCounter(ByteCounter &&other):m_impl(other.m_impl) {
        incref();
    }

    ByteCounter &operator=(ByteCounter &&other) {
        if (&other != this) {
            decref();
            m_impl = other.m_impl;
            incref();
        }
        return *this;
    }

    ~ByteCounter() { decref(); }

    void swap(ByteCounter &other) {
        std::swap(m_impl, other.m_impl);
    }

    void increase(std::size_t amount) {
        m_impl->allocated += amount;
    }

    void decrease(std::size_t amount) {
        m_impl->deallocated += amount;
    }

    std::size_t bytes() const { return m_impl->allocated - m_impl->deallocated; }
    std::size_t allocated() const { return m_impl->allocated; }
    std::size_t deallocated() const { return m_impl->deallocated; }
    std::size_t refcount() const { return m_impl->refcount; }

private:
    void incref() { ++m_impl->refcount; }

    void decref() {
        if (nullptr == m_impl) {
            // Do nothing.
        } else if (1 == m_impl->refcount) {
            delete m_impl;
            m_impl = nullptr;
        } else {
            --m_impl->refcount;
        }
    }

    ByteCounterImpl *m_impl;
};

template <class T>
struct CustomAllocator {
    using value_type = T;

    CustomAllocator() = default;

    template <class U> constexpr
    CustomAllocator(const CustomAllocator<U> &other) noexcept {
        counter = other.counter;
    }

    T *allocate(std::size_t n) {
        if (n > std::numeric_limits<std::size_t>::max() / sizeof(T)) {
            throw std::bad_alloc();
        }
        const std::size_t bytes = n * sizeof(T);
        T *p = static_cast<T *>(std::malloc(bytes));
        if (p) {
            counter.increase(bytes);
            return p;
        } else {
            throw std::bad_alloc();
        }
    }

    void deallocate(T *p, std::size_t n) noexcept {
        std::free(p);

        const std::size_t bytes = n * sizeof(T);
        counter.decrease(bytes);
    }

    ByteCounter counter;
};

static CustomAllocator<double> allocator;

std::size_t bytes(){
    return allocator.counter.bytes();
}

std::size_t allocated(){
    return allocator.counter.allocated();
}

std::size_t deallocated(){
    return allocator.counter.deallocated();
}

class Matrix {
public:
    Matrix(int nrow, int ncol);
    inline double operator()(int nrow, int ncol) const;
    inline double &operator()(int nrow, int ncol);
    bool operator==(Matrix const &other) const;
    bool operator!=(Matrix const &other) const;
    double const *get_buffer() const;
    double *get_buffer();
    inline int const &get_nrow() const;
    inline int const &get_ncol() const;
    void display_matrix() const;

private:
    int m_nrow = 0;
    int m_ncol = 0;
    std::vector<double, CustomAllocator<double>> m_vec;
    void reset_buffer(int nrow, int ncol);
    void check_inbound(int nrow, int ncol) const;
};

void check_multipliable(Matrix const &l_m, Matrix const &r_m);
Matrix multiply_naive(Matrix const &l_m, Matrix const &r_m);
void multiply_block(Matrix &m, Matrix const &l_m, Matrix const &r_m, int row, int col, int index, int tile_size);
Matrix multiply_tile(Matrix const &l_m, Matrix const &r_m, int tile_size);
Matrix multiply_mkl(Matrix const &l_m, Matrix const &r_m);