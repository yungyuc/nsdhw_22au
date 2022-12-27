#include <mkl.h>
#include <vector>
#include <stdexcept>

class Matrix
{
public:
    Matrix() : m_nrow(0), m_ncol(0) { };
    Matrix(size_t nrow, size_t ncol): m_nrow(nrow), m_ncol(ncol) {
        reset_buffer(nrow, ncol);
    }

    Matrix(const Matrix& other) : m_nrow(other.m_nrow), m_ncol(other.m_ncol) {
        reset_buffer(other.m_nrow, other.m_ncol);
        for (size_t i = 0; i < m_nrow; ++i)
            for (size_t j = 0; j < m_ncol; ++j)
                (*this)(i, j) = other(i, j);
    }

    Matrix& operator=(const Matrix& other) {
        if (this == &other) { return *this; }
        if ((m_nrow != other.m_nrow) || (m_ncol != other.m_ncol))
            reset_buffer(other.m_nrow, other.m_ncol);
        
        for (size_t i = 0; i < m_nrow; i++)
            for (size_t j = 0; j < m_ncol; j++)
                (*this)(i, j) = other(i, j);
            
        return *this;
    }

    ~Matrix() {  delete[] m_buffer; }


    size_t nrow() const                               { return m_nrow; }
    size_t ncol() const                               { return m_ncol; }
    size_t index(size_t row, size_t col) const        { return row * m_ncol + col; }
    
    double & operator()(size_t row, size_t col)       { return m_buffer[index(row,col)]; }
    double   operator()(size_t row, size_t col) const { return m_buffer[index(row,col)]; }

    double   operator[] (size_t idx) const            { return m_buffer[idx]; }
    double & operator[] (size_t idx)                  { return m_buffer[idx]; }
    double *buffer()                                  { return m_buffer; }
    
    double *val_addr()                                { return &(this->m_buffer[0]); }

    bool operator==(Matrix const & mat) const {
        if(this->nrow() != mat.nrow() || this->ncol() != mat.ncol()) return false;
        for(size_t i = 0; i < this->nrow(); ++i){
            for(size_t j = 0; j < this->ncol(); ++j){
                if((*this)(i,j) != mat(i,j)) return false;
            }
        }
        return true;
    }

private:
    size_t m_nrow  = 0;
    size_t m_ncol  = 0;
    double* m_buffer = nullptr;
    
    void reset_buffer(size_t nrow, size_t ncol) {
        if(m_buffer != nullptr) { delete[] m_buffer; }
        const size_t num = nrow * ncol;
        if(num != 0) { m_buffer = new double[num](); }
        else         { m_buffer = nullptr; }
        m_nrow = nrow;
        m_ncol = ncol;
    }
};

void check_multibility(Matrix const & mat1, Matrix const & mat2){
    if( mat1.ncol() != mat2.nrow() ){
        throw std::out_of_range("Matrix1 can't multiply with Matrix2 due two dimension mismatch.");
    }
}

Matrix multiply_naive(const Matrix& mat1, const Matrix& mat2){
    check_multibility(mat1, mat2);

    Matrix res(mat1.nrow(), mat2.ncol());

    for(size_t i = 0; i < mat1.nrow(); ++i){
        for(size_t j = 0; j < mat2.ncol(); ++j){
            double sum = 0;
            for(size_t k = 0; k < mat1.ncol(); ++k){
                sum += mat1(i,k) * mat2(k,j);
            }
            res(i,j) = sum;
        }    
    }

    return res;
}

Matrix multiply_tile(const Matrix & mat1, const Matrix& mat2, size_t blocksize){
    check_multibility(mat1, mat2);

    Matrix res(mat1.nrow(), mat2.ncol());

    for(size_t blocki = 0 ; blocki < mat1.nrow() ; blocki += blocksize){
        size_t i_bound = std::min( blocki + blocksize, mat1.nrow() );
        for(size_t blockj = 0 ; blockj < mat2.ncol() ; blockj += blocksize){
            size_t j_bound = std::min( blockj + blocksize, mat2.ncol() );
            for(size_t blockk = 0 ; blockk < mat1.ncol() ; blockk += blocksize){
                size_t k_bound = std::min( blockk + blocksize, mat1.ncol() );
                for(size_t k = blockk ; k < k_bound ; k++){
                    for(size_t i = blocki ; i < i_bound ; i++){
                        for(size_t j = blockj ; j < j_bound ; j++){
                            res(i,j) += mat1(i,k) * mat2(k,j);
                        }
                    }
                }
            }
        }
    }

    return res;
}

Matrix multiply_mkl(Matrix & mat1, Matrix & mat2){
    check_multibility(mat1, mat2);

    Matrix res(mat1.nrow(), mat2.ncol());
    /*
    const size_t m = mat1.nrow();
    const size_t n = mat2.ncol();
    const size_t k = mat1.ncol();
    const double alpha = 1.0; 
    const double beta  = 0.0;
    double* A = mat1.val_addr();
    double* B = mat2.val_addr();
    double* C = res.val_addr();
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m, n, k, alpha, A, k, B, n, beta, C, n);
    */
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, mat1.nrow(), mat2.ncol(), mat1.ncol(), 1, mat1.val_addr(), mat1.ncol(), mat2.val_addr(), mat2.ncol(), 0, res.val_addr(), res.ncol());
    return res;
}