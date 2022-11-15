#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>
#include <mkl.h>
#include <atomic>

// based on https://yyc.solvcon.net/en/latest/nsd/04matrix/matrix.html
using namespace std;
struct ByteCounterImpl
{
    atomic_size_t allocated = 0;
    atomic_size_t deallocated = 0;
    atomic_size_t refcount = 0;

}; /* end struct ByteCounterImpl */

/**
 * One instance of this counter is shared among a set of allocators.
 *
 * The counter keeps track of the bytes allocated and deallocated, and report
 * those two numbers in addition to bytes that remain allocated.
 */
class ByteCounter
{

public:

    ByteCounter()
      : m_impl(new ByteCounterImpl)
    { incref(); }

    ByteCounter(ByteCounter const & other)
      : m_impl(other.m_impl)
    { incref(); }

    ByteCounter & operator=(ByteCounter const & other)
    {
        if (&other != this)
        {
            decref();
            m_impl = other.m_impl;
            incref();
        }

        return *this;
    }

    ByteCounter(ByteCounter && other)
      : m_impl(other.m_impl)
    { incref(); }

    ByteCounter & operator=(ByteCounter && other)
    {
        if (&other != this)
        {
            decref();
            m_impl = other.m_impl;
            incref();
        }

        return *this;
    }

    ~ByteCounter() { decref(); }

    void swap(ByteCounter & other)
    {
        std::swap(m_impl, other.m_impl);
    }

    void increase(std::size_t amount)
    {
        m_impl->allocated += amount;
    }

    void decrease(std::size_t amount)
    {
        m_impl->deallocated += amount;
    }

    std::size_t bytes() const { return m_impl->allocated - m_impl->deallocated; }
    std::size_t allocated() const { return m_impl->allocated; }
    std::size_t deallocated() const { return m_impl->deallocated; }
    /* This is for debugging. */
    std::size_t refcount() const { return m_impl->refcount; }

private:

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

    ByteCounterImpl * m_impl;

}; /* end class ByteCounter */

template <class T>
struct MyAllocator
{

    using value_type = T;

    // Just use the default constructor of ByteCounter for the data member
    // "counter".
    MyAllocator() = default;

    template <class U> constexpr
    MyAllocator(const MyAllocator<U> & other) noexcept
    {
        counter = other.counter;
    }

    T * allocate(std::size_t n)
    {
        if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
        {
            throw std::bad_alloc();
        }
        const std::size_t bytes = n*sizeof(T);
        T * p = static_cast<T *>(std::malloc(bytes));
        if (p)
        {
            counter.increase(bytes);
            return p;
        }
        else
        {
            throw std::bad_alloc();
        }
    }

    void deallocate(T* p, std::size_t n) noexcept
    {
        std::free(p);

        const std::size_t bytes = n*sizeof(T);
        counter.decrease(bytes);
    }

    ByteCounter counter;

}; /* end struct MyAllocator */

template <class T, class U>
bool operator==(const MyAllocator<T> & a, const MyAllocator<U> & b)
{
    return a.counter == b.counter;
}

template <class T, class U>
bool operator!=(const MyAllocator<T> & a, const MyAllocator<U> & b)
{
    return !(a == b);
}


static MyAllocator<double> alloc;

size_t bytes() {
	return alloc.counter.bytes();
}

size_t allocated() {
	return alloc.counter.allocated(); 
}

size_t deallocated() {
	return alloc.counter.deallocated();
}

class Matrix
{

public:
    vector<double,MyAllocator<double>> m_buffer;
    size_t m_nrow;
    size_t m_ncol;
    //double *m_buffer = nullptr;
    //constructor
    Matrix(size_t nrow, size_t ncol) : m_buffer(alloc), m_nrow(nrow), m_ncol(ncol)
    {
        reset_buffer(nrow, ncol);
    }
    //function 
    void reset_buffer(size_t nrow, size_t ncol)
    {
        if(m_buffer.size()) //need to reset
        {
            //delete[] m_buffer;
            m_buffer.clear();
        }
        m_buffer.resize(nrow * ncol);

        //m_buffer = new double[nrow*ncol];
    }
    double* get_data()
    {
        return m_buffer.data();
    }
    size_t nrow() const 
    {
        return m_nrow;
    }
    size_t ncol() const 
    {
        return m_ncol;
    }
    // operator
    double operator()(size_t row, size_t col) const
    {
        size_t idx = row * m_ncol + col;
        return m_buffer[idx];
    }
    double &operator()(size_t row, size_t col) 
    {
        size_t idx = row * m_ncol + col;
        return m_buffer[idx];
    }
    bool operator==(const Matrix &other) const
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
};

Matrix multiply_naive(const Matrix &mat1, const Matrix &mat2)
{
    Matrix ret(mat1.nrow(), mat2.ncol());
    for (size_t i = 0; i < mat1.nrow(); i++)
    {
        for (size_t k = 0; k < mat2.ncol(); k++)
        {
            double v = 0;
            for (size_t j = 0; j < mat1.ncol(); j++)
            {
                v += mat1(i,j) * mat2(j,k);
            }
            ret(i,k) = v;
        }
    }
    return ret;
}

// matrix tiled
Matrix multiply_tile(const Matrix &mat1, const Matrix &mat2,  size_t len_size)
{
    Matrix ret(mat1.nrow(), mat2.ncol());
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
                            ret(i, k) += mat1(i, j) * mat2(j, k);
                        }
                    }
                }
            }
        }    
    }
    return ret;
}


Matrix multiply_mkl( Matrix &mat1, Matrix &mat2)
{
    Matrix ret(mat1.nrow(), mat2.ncol());
    size_t m = mat1.nrow();
    size_t n = mat2.ncol();
    size_t k = mat1.ncol();
    double alpha = 1.0;
    double beta = 0.0;

    
    /*
    double A_v = *mat1.m_buffer.data();
    double B_v = *mat2.m_buffer.data();
    double C_v = *ret.m_buffer.data();
    
    double* A = new double[sizeof(*mat1.m_buffer.data())];
    *A = *mat1.m_buffer.data();
    double* B = new double[sizeof(*mat2.m_buffer.data())];
    *B = *mat2.m_buffer.data();
    double* C = new double[sizeof(*ret.m_buffer.data())];
    *C = *ret.m_buffer.data();

    */
    
    double* A = mat1.get_data();//m_buffer.data();
    double* B = mat2.get_data();//m_buffer.data();
    double* C = ret.get_data();//.m_buffer.data();
    
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, 
                m, n, k, alpha, A, k, B, n, beta, C, n);
    return ret;
}

PYBIND11_MODULE(_matrix, m)
{
    m.doc() = "maxtrix multiply function";
    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_mkl", &multiply_mkl);
    m.def("multiply_tile", &multiply_tile);    
    m.def("bytes", &bytes);
    m.def("allocated", &allocated);
    m.def("deallocated", &deallocated);
    pybind11::class_<Matrix>(m, "Matrix")        
        .def(pybind11::init<size_t, size_t>())
        .def("__setitem__", [](Matrix &mat1, pair<size_t, size_t> row_col, double val) 
        {
            mat1(row_col.first, row_col.second) = val;
        })
        .def("__getitem__", [](Matrix &mat, pair<size_t, size_t> row_col)
        {
            return mat(row_col.first, row_col.second);
        })
        .def("__eq__", &Matrix::operator==)
        .def_property("nrow", &Matrix::nrow, nullptr)
        .def_property("ncol", &Matrix::ncol, nullptr);
}