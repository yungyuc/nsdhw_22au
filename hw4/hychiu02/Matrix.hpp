#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <iostream>
#include <vector>

#include "CustomAllocator.hpp"

CustomAllocator<double> alloc;

size_t bytes()
{
    return alloc.counter.allocated() - alloc.counter.deallocated();
}

size_t allocated()
{
    return alloc.counter.allocated();
}

size_t deallocated()
{
    return alloc.counter.deallocated();
}

class Matrix
{

public:
    Matrix()
        : m_nrow(0), m_ncol(0)
    {
    }

    /* constructor */
    Matrix(size_t nrow, size_t ncol)
        : m_nrow(nrow), m_ncol(ncol)
    {
        m_buffer.resize(m_nrow * m_ncol);        
    }
    /* destructor */
    ~Matrix()
    {
        m_buffer.clear();
    }
    /* copy constructor */
    Matrix(const Matrix &other)
        : m_nrow(other.nrow()), m_ncol(other.ncol()), m_buffer(other.m_buffer)
    {
    }
    /* copy */
    Matrix &operator=(Matrix const & other)
    {
        if (this == &other) { return *this; }
        
        m_nrow = other.nrow();
        m_ncol = other.ncol();
        m_buffer = other.m_buffer; 

        return *this;

    }
    /* move constructor */
    Matrix(Matrix && other)
        : m_nrow(other.nrow()), m_ncol(other.ncol()), m_buffer(std::move(other.m_buffer))
    {
    }
    /* move */
    Matrix &operator=(Matrix && other)
    {
        if (this == &other) { return *this; }
        
        m_nrow = other.nrow();
        m_ncol = other.ncol();
        m_buffer = other.m_buffer; 

        return *this;
    }

    /* get and set data */
    double operator()(size_t row, size_t col) const
    {
        return m_buffer[row * m_ncol + col];
    }
    double & operator()(size_t row, size_t col)
    {
        return m_buffer[row * m_ncol + col];
    }

    /* determine equivalent  */
    bool operator==(const Matrix &other) const
    {
        return m_buffer == other.m_buffer;
    }
    /* get information */
    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }

    double buffer(size_t i) const { return m_buffer[i]; }

    double * data() { return &m_buffer[0]; }

private:
    size_t index(size_t row, size_t col) const { return row * m_ncol + col; }

    size_t m_nrow;
    size_t m_ncol;
    vector<double, CustomAllocator<double>> m_buffer{alloc};
};

#endif