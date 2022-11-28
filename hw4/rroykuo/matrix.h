# include <vector>
# include <iostream>
# include "allocator.hpp"


static MyAllocator<double> alloc;



class Matrix {
  public:
    Matrix(size_t input_row_n, size_t input_col_n)
      : row_n(input_row_n), col_n(input_col_n), mat(alloc){
        this->mat.resize(input_col_n * input_row_n);
      }

    // Matrix(Matrix const &input_mat){
    //   this->row_n = input_mat.nrow();
    //   this->col_n = input_mat.ncol();
    //   this->mat = input_mat.mat;
    // }

    // Matrix(Matrix &&input_mat){
    //   this->row_n = std::move(input_mat.nrow());
    //   this->col_n = std::move(input_mat.ncol());
    //   this->mat = std::move(input_mat.mat);
    // }

    Matrix(Matrix const &input_mat)
      : row_n(input_mat.row_n), col_n(input_mat.col_n), mat(alloc){

        // input_mat.mat.resize(input_mat.col_n * input_mat.row_n);
        for (size_t i = 0; i < row_n; ++i)
        {
            for (size_t j = 0; j < col_n; ++j)
            {
                (*this)(i, j) = input_mat(i, j);
            }
        }
    }
    Matrix(Matrix &&input_mat)
        : row_n(input_mat.row_n), col_n(input_mat.col_n), mat(alloc){
        this->row_n = 0;
        this->col_n = 0;
        this->mat.resize(0);
        std::swap(this->row_n, input_mat.row_n);
        std::swap(this->col_n, input_mat.col_n);
        std::swap(this->mat, input_mat.mat);
    }


    // Matrix()
    // :row_n(0), col_n(0){}

    // ~Matrix(){
    //   mat.erase(mat.begin(), mat.end());
    //   mat.shrink_to_fit();
    // }

    double & operator() (size_t input_row, size_t input_col) {
      return mat[this->col_n * input_row + input_col];
    }

    double const&  operator() (size_t input_row, size_t input_col) const {
      return mat[this->col_n * input_row + input_col];
    }

    size_t nrow() const {return row_n;}
    size_t ncol() const {return col_n;}

    // Matrix & operator= (Matrix const &input_mat){
    //   if (this == &input_mat) {
    //         return *this;
    //     }
    //   this->row_n = input_mat.nrow();
    //   this->col_n = input_mat.ncol();
    //   this->mat = input_mat.mat;
    //   return *this;
    // }

    Matrix & operator= (Matrix &&input_mat){
      if (this == &input_mat) {
          return *this;
      }
      this->row_n = 0;
      this->col_n = 0;
      this->mat.resize(0);
      std::swap(this->row_n, input_mat.row_n);
      std::swap(this->col_n, input_mat.col_n);
      std::swap(this->mat, input_mat.mat);

      return *this;
    }

    bool operator== (const Matrix &mat2) const{
       if (this->row_n!= mat2.nrow() || this->col_n != mat2.ncol()){
             return false;
         }

         for (size_t i = 0; i < mat2.nrow(); ++i)
         {
             for (size_t j = 0; j < mat2.ncol(); ++j)
             {
                 if ((*this)(i, j) != mat2(i, j)){
                     return false;
                 }
             }
         }
        return true;
    }

    // const double* data() const{
    //   return &(this->mat[0]);
    // }
    // double* data(){
    //   return &(this->mat[0]);
    // }
    // friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix)
    // {
    //     for (size_t i = 0; i < matrix.nrow(); i++) {
    //         for (size_t j = 0; j < matrix.ncol(); j++) {
    //             os << matrix(i, j) << (j == matrix.ncol() - 1 ? "" : " ");
    //         }

    //         if (i < matrix.nrow() - 1) os << '\n';
    //     }
    //     return os;
    // }

  
    size_t row_n = 0;
    size_t col_n = 0;
    std::vector <double, MyAllocator<double>> mat;

};

size_t bytes(){
  return alloc.counter.bytes();
}

size_t allocated(){
  return alloc.counter.allocated();
}

size_t deallocated(){
  return alloc.counter.deallocated();
}

