#include <stddef.h>
#include <vector>
#include "Allocator.h"

extern Allocator<double> alloc;

class Matrix
{

private:
    void reset_buffer(size_t n_row, size_t n_col);
    size_t index(size_t, size_t) const;

public:
    // should be private but need to access mlk library
    std::vector<double, Allocator<double>> m_buffer;
    size_t m_row = 0;
    size_t m_col = 0;

    // constructor
    Matrix(size_t n_row, size_t n_col);
    // copy constructor
    Matrix(Matrix const &);
    // copy assigment operator
    Matrix &operator=(Matrix const &);
    // move assigment constructor
    Matrix &operator=(Matrix &&);
    // move constructor
    Matrix(Matrix &&);
    // destructor
    ~Matrix();
    double operator()(size_t n_row, size_t n_col) const;
    double &operator()(size_t n_row, size_t n_col);
    bool operator==(Matrix const &) const;
    // get row,col,size
    size_t n_row() const;
    size_t n_col() const;
    size_t size();
};
