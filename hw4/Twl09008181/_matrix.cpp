
#include <pybind11/pybind11.h>
#include <cstddef>
#include <mkl.h>
#include <iostream>
#include <vector>


template <class T>
struct matrixAllocator
{
  using value_type = T;
  T* allocate(std::size_t n){
    T*p = static_cast<T*>(malloc(n*sizeof(T)));
    allocateNum += n;
    return p;
  }
  void deallocate(T* p, std::size_t n){ 
    std::free(p);
    deallocateNum+=n;
  }
  static std::size_t allocateNum;
  static std::size_t deallocateNum;
};

template <class T> std::size_t matrixAllocator<T>::allocateNum = 0;
template <class T> std::size_t matrixAllocator<T>::deallocateNum = 0;



using T = double;
class Matrix{
public:
  Matrix(size_t m, size_t n)
    :_row{m}, _col{n}{ 
      _vec = new vectorType(m*n,_alloc);
    }
  ~Matrix(){
    if(_vec){
       delete _vec;
    }
  }

  Matrix(const Matrix&other)
    :_row{other._row}, _col{other._col}
  {
    _vec = new vectorType(other._col*other._row,_alloc);
    std::copy(_vec->begin(),_vec->end(),other._vec->begin());
  }
    
  Matrix(Matrix&&other)
    :_row{other._row}, _col{other._col}
  {
    _vec = other._vec;
    other._vec = nullptr;
  }
  bool operator==(const Matrix&other)const{
    return *_vec == *other._vec;
  }
  T operator()(size_t i, size_t j)const{
    return (*_vec)[i*_row+j];
  }
  T& operator()(size_t i, size_t j){
    return (*_vec)[i*_row+j];
  }
  void setter(std::pair<size_t, size_t> key, T val){
    (*_vec)[key.first*_row+key.second] = val;
  }
  T getter(std::pair<size_t, size_t> key)const{
    return (*_vec)[key.first*_row+key.second];
  }
  size_t row()const{return _row;}
  size_t col()const{return _col;}
  size_t _row, _col;

  using vectorType = std::vector<T, matrixAllocator<T>>;
  matrixAllocator<T>_alloc;
  vectorType *_vec;
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
        &m1._vec->at(0),
        m1.col(),
        &m2._vec->at(0),
        m2.col() ,
        0.0,
        &ans._vec->at(0),
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
std::size_t allocatedBytes(){
  return matrixAllocator<T>::allocateNum * sizeof(T);
}
std::size_t deallocatedBytes(){
  return matrixAllocator<T>::deallocateNum * sizeof(T);
}
std::size_t bytes(){
  return allocatedBytes() - deallocatedBytes();
}
//
//int main(){
//
//
//
//  Matrix m1(2, 2);
//  m1(0,0) = 1;
//  m1(0,1) = 2;
//  m1(1,0) = 3;
//  m1(0,1) = 4;
//
//  Matrix m2 = multiply_naive(m1,m1);
//  Matrix m3 = multiply_tile(m1,m1, 1);
//  Matrix m4 = multiply_mkl(m1,m1);
//
//  std::cout<<(m1==m2)<<"\n";
//  std::cout<<(m2==m3)<<"\n";
//  std::cout<<(m2==m4)<<"\n";
//  std::cout<<matrixAllocator<T>::allocateNum<<"\n";
//  std::cout<<matrixAllocator<T>::deallocateNum<<"\n";
//
//
//  return 0;
//}




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
        .def_property("nrow", &Matrix::row, nullptr)
        .def_property("ncol", &Matrix::col, nullptr);

    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_mkl", &multiply_mkl);
    m.def("multiply_tile", &multiply_tile);
    m.def("allocated", &allocatedBytes);
    m.def("deallocated", &deallocatedBytes);
    m.def("bytes", &bytes);

}
