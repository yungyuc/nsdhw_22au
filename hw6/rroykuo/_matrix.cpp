#pragma once

# include <vector>
# include <iostream>
# include <mkl.h>
# include <iomanip>
# include <stdexcept>
# include <pybind11/pybind11.h>
# include <pybind11/stl.h>
# include <pybind11/operators.h>
# include <pybind11/numpy.h>

class Matrix {
  public:
    Matrix(size_t input_row_n, size_t input_col_n)
      : row_n(input_row_n), col_n(input_col_n){
        this->mat = new double[input_col_n * input_row_n];
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
    :row_n(0), col_n(0){
    }

    // ~Matrix(){
    //   free(mat);
    //   delete(mat);
    // }

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

    bool operator== (const Matrix &input_mat) const{
      for (size_t i=0; i<this->nrow(); i++){
        for (size_t j=0; j<this->ncol(); j++){
          if(mat[this->col_n * i + j] != input_mat(i, j)){
            return false;
          }
        }
      }
      return true;
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

  pybind11::array_t<double> array(){
    return pybind11::array_t<double>(
      {nrow(), ncol()},
      {sizeof(double) * ncol(), sizeof(double) },
      mat,
      pybind11::cast(this)
    );
  }



  private:
    size_t row_n;
    size_t col_n;
    double* mat = nullptr;

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

PYBIND11_MODULE(_matrix, m)
{
    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_mkl", &multiply_mkl);
    m.def("multiply_tile", &multiply_tile);
    pybind11::class_<Matrix>(m, "Matrix")
        .def(pybind11::init<size_t, size_t>())
        .def("__setitem__", [](Matrix &self, std::pair<size_t, size_t> i, double val)
             { self(i.first, i.second) = val; })
        .def("__getitem__", [](Matrix &self, std::pair<size_t, size_t> i)
             { return self(i.first, i.second); })
        .def(pybind11::self == pybind11::self)
        .def_property("array", &Matrix::array, nullptr)
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol);
}