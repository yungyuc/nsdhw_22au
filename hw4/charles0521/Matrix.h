#include <iostream>
#include <vector>

struct ByteCounterImpl
{

    size_t allocated = 0;
    size_t deallocated = 0;

}; /* end struct ByteCounterImpl */

class ByteCounter
{

private:

    ByteCounterImpl * m_impl;

public:

    ByteCounter()
      : m_impl(new ByteCounterImpl)
    {

    }

    ByteCounter(ByteCounter const & other)
      : m_impl(other.m_impl)
    {

    }

    ByteCounter & operator=(ByteCounter const & other)
    {
        if (&other != this)
        {
            m_impl = other.m_impl;
        }

        return *this;
    }

    ByteCounter(ByteCounter && other)
      : m_impl(other.m_impl)
    {

    }

    ByteCounter & operator=(ByteCounter && other)
    {
        if (&other != this)
        {
            m_impl = other.m_impl;
        }

        return *this;
    }

    ~ByteCounter() 
    {

    }

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

    constexpr bool operator==(const ByteCounter& other) const
    {
        return (this->m_impl == other.m_impl);
    }

    std::size_t bytes() const { return m_impl->allocated - m_impl->deallocated; }
    std::size_t allocated() const { return m_impl->allocated; }
    std::size_t deallocated() const { return m_impl->deallocated; }

}; /* end class ByteCounter */



template <class T>
struct MyAllocator
{
private:
    ByteCounter counter;

public:
    using value_type = T;

    MyAllocator() = default;

    

    template <class U>
    MyAllocator(const MyAllocator<U> & other) noexcept
    {
        counter = other.counter;
    }

    ~MyAllocator() = default;

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


    constexpr size_t bytes() const{
        return this->counter.bytes();
    }

    constexpr size_t allocated() const{
        return this->counter.allocated();
    }

    constexpr size_t deallocated() const{
        return this->counter.deallocated();
    }

    constexpr bool operator==(const MyAllocator<T>& other) const{
        return (this->counter == other.counter);
    }


}; /* end struct MyAllocator */


MyAllocator<double> alloc;

size_t bytes()
{
    return alloc.bytes();
}
size_t allocated()
{
    return alloc.allocated();
}

size_t deallocated()
{
    return alloc.deallocated();
}


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



template <typename T>
class Matrix
{
    private:
        size_t n_row, n_col;
        std::vector<T, MyAllocator<double>> matrix = std::vector<T, MyAllocator<double>>(alloc);

    public:
        Matrix()
        {
            this->n_row = 0;
            this->n_col = 0;
        }

        Matrix(size_t n_row, size_t n_col)
        {
            this->n_row = n_row;
            this->n_col = n_col;
            this->matrix.resize(n_row*n_col, 0);
        }

        Matrix(Matrix const& other)
        {
            this->n_row = other.n_row;
            this->n_col = other.n_col;
            this->matrix = other.matrix;
        }

        Matrix(Matrix && other)
        {
            this->n_row = other.n_row;
            this->n_col = other.n_col;
            this->matrix = std::move(other.matrix);
        }

        ~Matrix()
        {
            this->matrix.clear();
            this->matrix.shrink_to_fit();
        }

        Matrix& operator=(Matrix const& other)
        {
            if(this != &other)
            {
                this->n_col = other.n_col;
                this->n_row = other.n_row;
                this->matrix = other.matrix;
            }

            return *this;
        }

        Matrix& operator=(Matrix && other)
        {
            if(this != &other)
            {
                this->n_col = other.n_col;
                this->n_row = other.n_row;
                this->matrix = std::move(other.matrix);
            }

            return *this;
        }

        T const& operator()(size_t i, size_t j) const //getter
        {
            return this->matrix[i*this->n_col + j];
        }

        T & operator()(size_t i, size_t j) //setter
        {
            return this->matrix[i*this->n_col + j];
        }

        const T* data() const
        {
            return &(this->matrix[0]);
        }

        T* data()
        {
            return &(this->matrix[0]);
        }

        constexpr bool operator==(const Matrix& other) const
        {
            return this->matrix == other.matrix;
        }

        constexpr size_t rows() const
        {
            return this->n_row;
        }

        constexpr size_t cols() const
        {
            return this->n_col;
        }

        void output() const
        {   
            for(size_t row=0; row<this->n_row; ++row)
            {
                for(size_t col=0; col<this->n_col; ++col)
                    std::cout << this->matrix[row*this->n_row + col] << " ";
                std::cout << std::endl;
            }
        }
};