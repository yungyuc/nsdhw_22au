#pragma once

# include <vector>
# include <iostream>
# include <mkl.h>
# include <iomanip>
# include <stdexcept>

class Matrix {
  public:
    Matrix(size_t input_row_n, size_t input_col_n)
      : row_n(input_row_n), col_n(input_col_n){
        this->mat.resize(input_col_n * input_row_n, 0);
      }

    Matrix(Matrix const &input_mat){
      this->row_n = input_mat.nrow();
      this->col_n = input_mat.ncol();
      this->mat = input_mat.mat;
    }

    Matrix(Matrix &&input_mat){
      this->row_n = std::move(input_mat.nrow());
      this->col_n = std::move(input_mat.ncol());
      this->mat = std::move(input_mat.mat);
    }

    Matrix()
    :row_n(0), col_n(0){}

    ~Matrix(){
      mat.erase(mat.begin(), mat.end());
      mat.shrink_to_fit();
    }

    double & operator() (size_t input_row, size_t input_col) {
      return mat[this->col_n * input_row + input_col];
    }

    double const&  operator() (size_t input_row, size_t input_col) const {
      return mat[this->col_n * input_row + input_col];
    }

    size_t nrow() const {return row_n;}
    size_t ncol() const {return col_n;}

    Matrix & operator= (Matrix const &input_mat){
      if (this != &input_mat) {
            this->row_n = input_mat.nrow();
            this->col_n = input_mat.ncol();
            this->mat = input_mat.mat;
        }
      return *this;
    }

    Matrix & operator= (Matrix &&input_mat){
      if (this != &input_mat) {
            this->row_n = input_mat.nrow();
            this->col_n = input_mat.ncol();
            this->mat = std::move(input_mat.mat);
      }

        return *this;
    }

    bool operator== (const Matrix &mat2) const{
      return (this->mat == mat2.mat);
    }

    const double* data() const{
      return &(this->mat[0]);
    }
    double* data(){
      return &(this->mat[0]);
    }
    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix)
    {
        for (size_t i = 0; i < matrix.nrow(); i++) {
            for (size_t j = 0; j < matrix.ncol(); j++) {
                os << matrix(i, j) << (j == matrix.ncol() - 1 ? "" : " ");
            }

            if (i < matrix.nrow() - 1) os << '\n';
        }

        return os;
    }



  private:
    size_t row_n;
    size_t col_n;
    std::vector <double> mat;

};

Matrix multiply_naive(Matrix const &mat1, Matrix const &mat2){
    if (mat1.ncol() != mat2.ncol()){
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }

    Matrix ret(mat1.nrow(), mat2.ncol());
    for (size_t i=0; i<mat1.nrow(); ++i){
        for (size_t k=0; k<mat2.ncol(); ++k){
            double v = 0;
            for (size_t j=0; j<mat1.ncol(); ++j){
                v += mat1(i,j) * mat2(j,k);
            }
            ret(i,k) = v;
        }
    }

    return ret;

}
Matrix multiply_tile(Matrix const &mat1, Matrix const &mat2, size_t tile_size){
    if (mat1.ncol() != mat2.nrow()){
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }
    Matrix ret(mat1.nrow(), mat2.ncol());
    for (size_t i = 0; i < mat1.nrow(); i += tile_size){

        for (size_t j = 0; j < mat2.ncol(); j += tile_size){

            for (size_t k = 0; k < mat1.ncol(); k += tile_size){

                for (size_t x = i; x < std::min(i + tile_size, mat1.nrow()); x++){

                    for (size_t y = j; y < std::min(j + tile_size, mat2.ncol()); y++){
                        
                        for (size_t z = k; z < std::min(tile_size + k, mat1.ncol()); z++){
                            ret(x, y) += mat1(x, z) * mat2(z, y);
                        }
                    }
                }
            }
        }
    }


    return ret;
}
Matrix multiply_mkl(Matrix const &mat1, Matrix const &mat2){
    if (mat1.ncol() != mat2.ncol()){
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }

    Matrix ret(mat1.nrow(), mat2.ncol());

    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, mat1.nrow(), mat2.ncol(), mat1.ncol(), 1, mat1.data(), mat1.ncol(), mat2.data(), mat2.ncol(), 0, ret.data(), ret.ncol());

    return ret;

    
}