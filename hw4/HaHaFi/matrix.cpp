#include "matrix.h"
#include <stddef.h>
#include <algorithm>
#include <iostream>

// constructor
Matrix::Matrix(size_t n_row, size_t n_col) : m_row(n_row), m_col(n_col), m_buffer(alloc)
{
    reset_buffer(n_row, n_col);
}

// // destructor
Matrix::~Matrix()
{
    reset_buffer(0, 0);
}

// // copy constructor
Matrix::Matrix(Matrix const &other) : m_buffer(alloc)
{
    // std::cout << "copy construct\n";
    this->reset_buffer(other.n_row(), other.n_col());
    for (size_t i = 0; i < other.n_row(); i++)
    {
        for (size_t j = 0; j < other.n_col(); j++)
        {
            const size_t index = this->index(i, j);
            this->m_buffer[index] = other.m_buffer[index];
        }
    }
    // this->alloc = other.alloc;
}

// copy assigment operator
Matrix &Matrix::operator=(Matrix const &other)
{
    // std::cout << "copy assigment operator \n";

    if (this == &other)
    {
        return *this;
    }
    if (other.n_col() != this->n_col() || other.n_row() != this->n_row())
    {
        this->reset_buffer(other.n_row(), other.n_col());
    }
    for (size_t i = 0; i < other.n_row(); i++)
    {
        for (size_t j = 0; j < other.n_col(); j++)
        {
            const size_t index = this->index(i, j);
            this->m_buffer[index] = other.m_buffer[index];
        }
    }
    // this->alloc = other.alloc;
    return *this;
}

// move constructor
Matrix::Matrix(Matrix &&other) : m_buffer(alloc)
{
    // std::cout << "move construct\n";
    // reset_buffer(0, 0);
    // std::cout << alloc.counter.refcount() << std::endl;
    std::swap(this->m_col, other.m_col);
    std::swap(this->m_row, other.m_row);
    std::swap(this->m_buffer, other.m_buffer);
    // std::cout << alloc.counter.refcount() << std::endl;
    // std::swap(this->alloc, other.alloc);
}

// move assigment constructor
Matrix &Matrix::operator=(Matrix &&other)
{
    if (this == &other)
    {
        return *this;
    }
    reset_buffer(0, 0);
    std::swap(this->m_row, other.m_row);
    std::swap(this->m_col, other.m_col);
    std::swap(this->m_buffer, other.m_buffer);
    // std::swap(this->alloc, other.alloc);
    return *this;
}

size_t Matrix::n_col() const
{
    return this->m_col;
}

size_t Matrix::n_row() const
{
    return this->m_row;
}

size_t Matrix::size()
{
    return this->m_row * this->m_col;
}

void Matrix::reset_buffer(size_t n_row, size_t n_col)
{

    const size_t n_element = n_col * n_row;
    // this->m_buffer.clear();
    this->m_buffer.resize(n_element);
    // need to init
    for (size_t i = 0; i < n_element; i++)
    {
        this->m_buffer[i] = 0;
    }

    this->m_col = n_col;
    this->m_row = n_row;
}

size_t Matrix::index(size_t n_row, size_t n_col) const
{
    return this->m_col * n_row + n_col;
}

double Matrix::operator()(size_t n_row, size_t n_col) const
{
    return this->m_buffer[index(n_row, n_col)];
}
double &Matrix::operator()(size_t n_row, size_t n_col)
{
    return this->m_buffer[index(n_row, n_col)];
}
bool Matrix::operator==(Matrix const &other) const
{
    if (this->n_row() != other.n_row() || this->n_col() != other.n_col())
    {
        return false;
    }
    for (size_t i = 0; i < other.n_row(); i++)
    {
        for (size_t j = 0; j < other.n_col(); j++)
        {
            if (other(i, j) != (*this)(i, j))
            {
                return false;
            }
        }
    }

    return true;
}