#include <mkl.h>
#include <vector>
#include <stdexcept>

class Matrix
{
public:


    friend Matrix multiply_naive(Matrix const &mat1, Matrix const &mat2);
    friend Matrix multiply_tile(Matrix const &mat1, Matrix const &mat2, size_t const tsize);
    friend Matrix multiply_mkl(Matrix const &mat1, Matrix const &mat2); 

    Matrix() : m_nrow(0), m_ncol(0) { };
    Matrix(size_t nrow, size_t ncol): m_nrow(nrow), m_ncol(ncol) {
        reset_buffer(nrow, ncol);
    }

    size_t nrow() const                               { return m_nrow; }
    size_t ncol() const                               { return m_ncol; }
    size_t index(size_t row, size_t col) const        { return row * m_ncol + col; }

    double & operator()(size_t row, size_t col)       { return m_buffer[index(row,col)]; }
    double   operator()(size_t row, size_t col) const { return m_buffer[index(row,col)]; }

    double *val_addr()                                { return &(this->m_buffer[0]); }

    bool operator==(const Matrix & mat) const {
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

const Matrix multiply_naive(Matrix & m1, Matrix &m2) {

    Matrix resultM = Matrix(m1.nrow(), m2.ncol());

    for (size_t i = 0; i < m1.nrow(); ++i) {
        for (size_t k = 0; k < m2.ncol(); ++k) {
            double v = 0;
            for (size_t j = 0; j < m1.ncol(); ++j){
                v += m1(i, j) * m2(j, k);
            }
            resultM(i, k) = v;
        }
    }

    return resultM;
};


Matrix multiply_mkl(Matrix const &m1, Matrix const &m2){
  // mkl_set_num_threads(1);

    Matrix resultM(m1.nrow(), m2.ncol());
    cblas_dgemm(
        CblasRowMajor,
        CblasNoTrans,
        CblasNoTrans,
        m1.nrow(),
        m2.ncol(),
        m1.ncol(),
        1.0,
        m1.m_buffer,
        m1.ncol(),
        m2.m_buffer,
        m2.ncol(),
        0.0,
        resultM.m_buffer,
        resultM.ncol()
    );

    return resultM;
}


const Matrix multiply_tile(Matrix& m1, Matrix& m2, size_t tile_size) {

    const size_t row = m1.nrow();
    const size_t col = m2.ncol();

    Matrix resultM = Matrix(m1.nrow(), m2.ncol());

    for (size_t i = 0; i < m1.nrow(); ++i) {
        for (size_t k = 0; k < m2.ncol(); ++k) {
            resultM(i, k) = 0;
        }
    }
   
    // size of cache line (64) devided by size of double(8)
    // bigger numbers of incr give slightly better performance, i guess that
    // is due to effective prefetching
    size_t incr = tile_size;
    // outer loops looping over tiles
    for (size_t i = 0; i < row; i += incr) {
        size_t x_row_min = std::min(i + incr, row);
        for (size_t j = 0; j < col; j += incr) {
            size_t j_col_min = std::min(j + incr, col);
            for (size_t k = 0; k < col; k += incr){
                size_t k_row_min = std::min(k + incr, row);

                // inner loops performing matrix multiplications on tiles
                for (size_t x = i; x < x_row_min; x++) {
                    for (size_t y = j; y < j_col_min; y++) {
                        for (size_t z = k; z < k_row_min; z++) {

                           // resultM[x * col + y] += m1[x * col + z] * m2[z * col + y];
                           resultM(x, y) += m1(x, z) * m2(z, y);
                        }
                    }
                }
            }
        }
    }
    return resultM;
}
