#include <iostream>
#include <vector>

namespace matrix
{

class Matrix {
public:
    Matrix();
    Matrix(int rsize, int csize);

    const int & getNrow() const { return nrow; }
    const int & getNcol() const { return ncol; }
    double operator() (size_t row, size_t col) const { return elements[row * ncol + col]; }
    double & operator() (size_t row, size_t col) { return elements[row * ncol + col]; }
    bool operator== (const Matrix & other) const;
    bool operator!= (const Matrix & other) const;
    const double * getBuffer() const { return elements.data(); }
    double * getBuffer() { return elements.data(); }

private:
    int nrow;
    int ncol;
    std::vector<double> elements;
};

Matrix multiply_naive(const Matrix & m1, const Matrix & m2);
Matrix multiply_mkl(const Matrix & m1, const Matrix & m2);
Matrix multiply_tile(const Matrix & m1, const Matrix & m2, int tsize);
void multiply_block(Matrix & res, const Matrix & m1, const Matrix & m2, int row, int col, int multiply_iter, int tsize);

}