#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <cstdlib>
#include <vector>
#include <atomic>
#include <new>
#include <memory>
#include <limits>
#include <mkl.h>

struct ByteCounterImpl
{
    std::atomic_size_t allocated{0};
    std::atomic_size_t deallocated{0};
    std::atomic_size_t refcount{0};
};

class ByteCounter
{
public:
    ByteCounter(): m_impl(new ByteCounterImpl)                      { incref(); }
    ByteCounter(ByteCounter const & other): m_impl(other.m_impl)    { incref(); }
    ByteCounter& operator=(ByteCounter const & other);
    ByteCounter(ByteCounter && other): m_impl(other.m_impl)         { incref(); }
    ByteCounter& operator=(ByteCounter const && other);
    ~ByteCounter()                                                  { decref(); }

    void swap(ByteCounter & other)      { std::swap(m_impl, other.m_impl); }
    void increase(std::size_t amount)   { m_impl->allocated += amount; }
    void decrease(std::size_t amount)   { m_impl->deallocated += amount; }
    std::size_t allocated()   const     { return m_impl->allocated; }
    std::size_t deallocated() const     { return m_impl->deallocated; }
    std::size_t bytes()       const     { return (m_impl->allocated - m_impl->deallocated); }
    
    std::size_t refcount() const        { return m_impl->refcount; }

private:
    ByteCounterImpl* m_impl;
    void incref() { ++m_impl->refcount; }
    void decref()
    {
        if (nullptr == m_impl)
			{
				// Do nothing.
			}
			else if (1 == m_impl->refcount)
			{
				delete m_impl;
				m_impl = nullptr;
			}
			else
			{
				--m_impl->refcount;
			}
    }
};

template <class T>
struct CustomAllocator
{
    using value_type = T;
    CustomAllocator() = default;

    template <class U> constexpr
    CustomAllocator(const CustomAllocator<U> & other) noexcept { counter = other.counter; }
    
    T* allocate(std::size_t n);
    void deallocate(T* p, std::size_t n) noexcept;

    ByteCounter counter;
};

static CustomAllocator<double> allocator;
size_t bytes()          { return allocator.counter.bytes(); }
size_t allocated()      { return allocator.counter.allocated(); }
size_t deallocated()    { return allocator.counter.deallocated(); }

class Matrix
{
public:
    Matrix(size_t nRow, size_t nCol): m_array(allocator), m_nrow(nRow), m_ncol(nCol)
    { m_array.resize(nRow * nCol); }

    size_t nrow() const                                      { return m_nrow; }
    size_t ncol() const                                      { return m_ncol; }

    double  operator()(size_t row, size_t col) const         { return m_array[m_ncol * row + col]; }
    double& operator()(size_t row, size_t col)               { return m_array[m_ncol * row + col]; }

    double getvalue(std::pair<size_t, size_t> idx)           { return (*this)(idx.first, idx.second); }
    void setvalue(std::pair<size_t, size_t> idx, double val) { (*this)(idx.first, idx.second) = val;  } 

    double* data()                                           { return m_array.data(); }

private:
    std::vector<double, CustomAllocator<double>> m_array;
    size_t m_nrow;
    size_t m_ncol;
};

void multibility(Matrix const & mat1, Matrix const & mat2);
Matrix multiply_naive(Matrix const & mat1, Matrix & mat2);
Matrix multiply_tile(Matrix const & mat1, Matrix const & mat2, size_t blocksize);
Matrix multiply_mkl(Matrix & mat1, Matrix & mat2);

#endif  //MATRIX_HPP