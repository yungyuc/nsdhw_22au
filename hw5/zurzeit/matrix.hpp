#ifndef MATRIX_H
#define MATRIX_H

#include <math.h>
#include <iostream>
#include <vector>
#include <mkl.h>
#include <cstring>
#include <algorithm>


class Matrix {

public:
    
    Matrix(); 
    Matrix(size_t nrow, size_t ncol);// constructor
    Matrix(size_t nrow, size_t ncol, std::vector<double> const & vec);// constructor with vector
    //copy assignment operator with vec
    Matrix & operator=(std::vector<double> const & vec);

    void setter(std::pair <size_t, size_t> key, double val);
    double getter(std::pair <size_t, size_t> key);
    // copy constructor
    Matrix(const Matrix & other);
    // copy assign operator
    Matrix & operator=(const Matrix & other);
    // move constructor
    Matrix( Matrix && other);
    // move assign operator
    Matrix & operator=(Matrix && other);
    ~Matrix();

    Matrix & operator+=(Matrix const & mat1);
    bool operator==(const Matrix & other)const;
    // // to locate the value
    double   operator() (size_t row, size_t col) const;
    double & operator() (size_t row, size_t col);
    Matrix row_maj_to_col_maj() const;
    size_t nrow() const;//return the number of row of the matrix
    size_t ncol() const;//return the number of col of the matrix

    size_t size() const;//return the number of element of the matrix

    void get_mat();
    double * get_buffer_pointer() const;
    double * m_buffer;
private:

    size_t index(size_t row, size_t col) const;

    void reset_buffer(size_t nrow, size_t ncol);
    
    size_t m_nrow;
    size_t m_ncol;
    
};

Matrix operator*(Matrix const & mat1, Matrix const & mat2);
Matrix multiply_naive(Matrix const & mat1, Matrix const & mat2);
Matrix multiply_tile(Matrix const & mat1, Matrix const & mat2, size_t tsize);
Matrix multiply_mkl(Matrix const & mat1, Matrix const & mat2);

#endif
