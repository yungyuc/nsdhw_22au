#include <pybind11/pybind11.h>
#include <cstddef>
#include <mkl.h>
#include <iostream>

using T = double;
class Matrix{
public:
  Matrix(size_t m, size_t n)
    :_row{m}, _col{n}, _data{new T[m*n]}{
      for(size_t i = 0; i < _row*_col;i++)
        _data[i] =  0 ;
    }
  Matrix(const Matrix& other)
    :_row{other._row}, _col{other._col}{
      _data = new T[_row*_col];
      for(size_t i = 0; i < _row*_col;i++)
        _data[i] = other._data[i];
    }
  ~Matrix(){
    delete []_data;
  }

  bool operator==(const Matrix&other)const{
    if(other.nrow()!=nrow()||other.ncol()!=ncol())
      return false;
    for(size_t i = 0; i < _row*_col; i++){
      if(_data[i]!=other._data[i])
        return false;
    }
    return true;
  }

  T operator()(size_t i, size_t j)const{
    return _data[i*_row+j];
  }
  T& operator()(size_t i, size_t j){
    return _data[i*_row+j];
  }
  void setter(std::pair<size_t, size_t> key, T val){
    _data[key.first*_row+key.second] = val;
  }
  T getter(std::pair<size_t, size_t> key)const{
    return _data[key.first*_row+key.second];
  }
  size_t nrow()const{return _row;}
  size_t ncol()const{return _col;}
  size_t _row, _col;
  T* _data;
};


Matrix multiply_naive(const Matrix &m1, const Matrix &m2)
{
    Matrix ans(m1.nrow(), m2.ncol());
    for (size_t i = 0; i < m1.nrow(); ++i){
        for (size_t j = 0; j < m2.ncol(); ++j){
            T dot = 0;
            for (size_t k = 0; k < m1.ncol(); ++k){
                dot += m1(i, k) * m2(k, j);
            }
            ans(i, j) = dot;
        }
    }
    return ans;
}


Matrix multiply_mkl(const Matrix &m1, const Matrix &m2)
{
    Matrix ans(m1.nrow(), m2.ncol());

    cblas_dgemm(
        CblasRowMajor,
        CblasNoTrans,
        CblasNoTrans,
        m1.nrow(),
        m2.ncol(),
        m1.ncol(),
        1.0 ,
        m1._data,
        m1.ncol(),
        m2._data ,
        m2.ncol() ,
        0.0,
        ans._data,
        ans.ncol()
    );
    return ans;
}




Matrix multiply_tile(const Matrix &m1, const Matrix &m2, size_t stride)
{
    Matrix ans(m1.nrow(), m2.ncol());
    for(size_t i = 0; i < m1.nrow(); i+=stride){
      for(size_t j = 0; j < m2.ncol(); j+=stride){
        for(size_t k = 0; k < m1.ncol(); k+=stride){
          for(size_t ii = i; ii < std::min(i+stride,m1.nrow()); ii++){
            for(size_t kk = k; kk < std::min(k+stride,m1.ncol()); kk++){
              for(size_t jj = j; jj < std::min(j+stride,m2.ncol()); jj++){ // get better spatial locality
                  ans(ii,jj) += m1(ii,kk) * m2(kk,jj);
              }
            }
          }
        }
      }
    }
    return ans;
}

