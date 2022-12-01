// https://yyc.solvcon.net/en/latest/nsd/04matrix/example.html#nsd-matrix-example-ma03-matrix-matrix

#include <vector>
#include <stdexcept>

#include <mkl.h>


namespace py = pybind11;

class Matrix {

public:
    Matrix() : m_nrow(0), m_ncol(0) { };
    Matrix(size_t nrow, size_t ncol)
        : m_nrow(nrow), m_ncol(ncol)
    {
        reset_buffer(nrow, ncol);
    }

    Matrix(size_t nrow, size_t ncol, std::vector<double> const & vec)
      : m_nrow(nrow), m_ncol(ncol)
    {
        reset_buffer(nrow, ncol);
        (*this) = vec;
    }

    Matrix & operator=(std::vector<double> const & vec)
    {
        if (size() != vec.size())
        {
            throw std::out_of_range("number of elements mismatch");
        }

        size_t k = 0;
        for (size_t i=0; i<m_nrow; ++i)
        {
            for (size_t j=0; j<m_ncol; ++j)
            {
                (*this)(i,j) = vec[k];
                ++k;
            }
        }

        return *this;
    }

    Matrix(Matrix const & other)
      : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
    {
        reset_buffer(other.m_nrow, other.m_ncol);
        for (size_t i=0; i<m_nrow; ++i)
        {
            for (size_t j=0; j<m_ncol; ++j)
            {
                (*this)(i,j) = other(i,j);
            }
        }
    }

    Matrix & operator=(Matrix const & other)
    {
        if (this == &other) { return *this; }
        if (m_nrow != other.m_nrow || m_ncol != other.m_ncol)
        {
            reset_buffer(other.m_nrow, other.m_ncol);
        }
        for (size_t i=0; i<m_nrow; ++i)
        {
            for (size_t j=0; j<m_ncol; ++j)
            {
                (*this)(i,j) = other(i,j);
            }
        }
        return *this;
    }

    Matrix(Matrix && other)
      : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
    {
        reset_buffer(0, 0);
        std::swap(m_nrow, other.m_nrow);
        std::swap(m_ncol, other.m_ncol);
        std::swap(m_buffer, other.m_buffer);
    }

    Matrix & operator=(Matrix && other)
    {
        if (this == &other) { return *this; }
        reset_buffer(0, 0);
        std::swap(m_nrow, other.m_nrow);
        std::swap(m_ncol, other.m_ncol);
        std::swap(m_buffer, other.m_buffer);
        return *this;
    }

    ~Matrix()
    {
        reset_buffer(0, 0);
    }

    bool operator== (const Matrix & other) const
    {
        if ((nrow() != other.nrow()) || (ncol() != other.ncol())) { return false; }
        for (size_t i = 0; i < nrow(); ++i)
        {
            for (size_t j = 0; j < ncol(); ++j)
            {
                if ((*this)(i, j) != other(i, j)) { return false; }
            }
        }
        return true;
    }

    bool operator!= (const Matrix & other) const
    {
        return !(*this == other);
    }

    double   operator() (size_t row, size_t col) const { return m_buffer[index(row, col)]; }
    double & operator() (size_t row, size_t col)       { return m_buffer[index(row, col)]; }

    double   operator[] (size_t idx) const { return m_buffer[idx]; }
    double & operator[] (size_t idx)       { return m_buffer[idx]; }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }

    size_t size() const { return m_nrow * m_ncol; }

public:

    size_t index(size_t row, size_t col) const
    {
        return row * m_ncol + col;
    }

    void reset_buffer(size_t nrow, size_t ncol)
    {
        if (m_buffer) { delete[] m_buffer; }
        size_t nelement = nrow * ncol; // const?
        if (nelement)
        { 
            m_buffer = new double[nelement];
            memset(m_buffer, 0, sizeof(double)*nelement);
        }
        else { m_buffer = nullptr; }
        m_nrow = nrow;
        m_ncol = ncol;
    }

    size_t m_nrow = 0;
    size_t m_ncol = 0;
    double * m_buffer = nullptr;

};

Matrix multiply_naive(Matrix const & mat1, Matrix const & mat2)
{
    if (mat1.ncol() != mat2.nrow())
    {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }
    Matrix ret(mat1.nrow(), mat2.ncol());
    for (size_t i=0; i<ret.nrow(); ++i)
    {
        for (size_t j=0; j<ret.ncol(); ++j)
        {
            double v = 0;
            for (size_t k=0; k<mat1.ncol(); ++k)
            {
                v += mat1(i,k) * mat2(k,j);
            }
            ret(i,j) = v;
        }
    }
    return ret;
}

Matrix multiply_tile(Matrix const & mat1, Matrix const & mat2, size_t tile_size) {
    if (mat1.ncol() != mat2.nrow())
    {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }

    Matrix ret(mat1.ncol(), mat2.nrow());

    // unify matrix to blocks by tiles
    for (size_t start_i=0; start_i<mat1.nrow(); start_i+=tile_size)
    {
        for (size_t start_j=0; start_j<mat2.ncol(); start_j+=tile_size)
        {   
            // // block by block matrix-matrix multiplication
            for (size_t start_k=0; start_k<mat1.ncol(); start_k+=tile_size)
            {
                for (size_t i=start_i; i<std::min(start_i+tile_size, mat1.nrow()); ++i)
                {
                    for (size_t k=start_k; k<std::min(start_k+tile_size, mat1.ncol()); ++k)
                    {
                        for (size_t j=start_j; j<std::min(start_j+tile_size, mat2.ncol()); ++j)
                        {
                            ret(i,j) += mat1(i,k) * mat2(k,j);
                        }
                    }
                }
            }
                
        }
    }
    return ret;
}

/*
 * Use MKL for the matrix matrix multiplication.
 */
Matrix multiply_mkl(Matrix const & mat1, Matrix const & mat2)
{

    Matrix ret(mat1.nrow(), mat2.ncol());

    cblas_dgemm(
        CblasRowMajor /* const CBLAS_LAYOUT Layout */
      , CblasNoTrans /* const CBLAS_TRANSPOSE transa */
      , CblasNoTrans /* const CBLAS_TRANSPOSE transb */
      , mat1.nrow() /* const MKL_INT m */
      , mat2.ncol() /* const MKL_INT n */
      , mat1.ncol() /* const MKL_INT k */
      , 1.0 /* const double alpha */
      , mat1.m_buffer /* const double *a */
      , mat1.ncol() /* const MKL_INT lda */
      , mat2.m_buffer /* const double *b */
      , mat2.ncol() /* const MKL_INT ldb */
      , 0.0 /* const double beta */
      , ret.m_buffer /* double * c */
      , ret.ncol() /* const MKL_INT ldc */
    );

    return ret;
}

