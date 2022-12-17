#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <cstddef>
#include <mkl.h>
#include <iostream>

namespace py = pybind11;

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
    if(other.row()!=row()||other.col()!=col())
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

  // numpy view
  py::array_t<T> ndArray(){
   return py::array_t<T>(
          py::buffer_info(
           _data, // dataptr
           sizeof(T), //itemsize
           py::format_descriptor<T>::format(),
           2, // ndim
           std::vector<size_t> {_row, _col }, // shape
           std::vector<size_t> {_col * sizeof(T), sizeof(T)} // strides
           )
      );
  }
  size_t row()const{return _row;}
  size_t col()const{return _col;}
  size_t _row, _col;
  T* _data;
};


Matrix multiply_naive(const Matrix &m1, const Matrix &m2)
{
    Matrix ans(m1.row(), m2.col());
    for (size_t i = 0; i < m1.row(); ++i){
        for (size_t j = 0; j < m2.col(); ++j){
            T dot = 0;
            for (size_t k = 0; k < m1.col(); ++k){
                dot += m1(i, k) * m2(k, j);
            }
            ans(i, j) = dot;
        }
    }
    return ans;
}


Matrix multiply_mkl(const Matrix &m1, const Matrix &m2)
{
    Matrix ans(m1.row(), m2.col());

    cblas_dgemm(
        CblasRowMajor,
        CblasNoTrans,
        CblasNoTrans,
        m1.row(),
        m2.col(),
        m1.col(),
        1.0 ,
        m1._data,
        m1.col(),
        m2._data ,
        m2.col() ,
        0.0,
        ans._data,
        ans.col()
    );
    return ans;
}




Matrix multiply_tile(const Matrix &m1, const Matrix &m2, size_t stride)
{
    Matrix ans(m1.row(), m2.col());
    for(size_t i = 0; i < m1.row(); i+=stride){
      for(size_t j = 0; j < m2.col(); j+=stride){
        for(size_t k = 0; k < m1.col(); k+=stride){
          for(size_t ii = i; ii < std::min(i+stride,m1.row()); ii++){
            for(size_t kk = k; kk < std::min(k+stride,m1.col()); kk++){
              for(size_t jj = j; jj < std::min(j+stride,m2.col()); jj++){ // get better spatial locality
                  ans(ii,jj) += m1(ii,kk) * m2(kk,jj);
              }
            }
          }
        }
      }
    }
    return ans;
}




PYBIND11_MODULE(_matrix, m)
{
    m.doc() = "maxtrix utility";

    pybind11::class_<Matrix>(m, "Matrix")
        .def(pybind11::init<size_t, size_t>())
        .def("__setitem__", [](Matrix&m, std::pair<size_t,size_t>key, T v){
            m.setter(key,v);
            })
        .def("__getitem__", [](Matrix&m, std::pair<size_t,size_t>key){
            return m.getter(key);
            })
        .def("__eq__", &Matrix::operator==)
        .def_property("array", &Matrix::ndArray, nullptr)
        .def_property("nrow", &Matrix::row, nullptr)
        .def_property("ncol", &Matrix::col, nullptr);

    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_mkl", &multiply_mkl);
    m.def("multiply_tile", &multiply_tile);
}
