#include <vector>
#include <iostream>
#include <mkl.h>
#include <math.h>
#include "matrix.h"

using std::cout;
using std::endl;
using std::size_t;

#define DEBUG 0
#if DEBUG == 0
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;
#endif


Matrix multiply_naive(Matrix & mat1, Matrix & mat2){
    if (mat1.ncol() != mat2.nrow())
    {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }

    Matrix ret(mat1.nrow(), mat2.ncol());

    for (size_t i=0; i<ret.nrow(); ++i)
    {
        for (size_t k=0; k<ret.ncol(); ++k)
        {
            double v = 0;
            for (size_t j=0; j<mat1.ncol(); ++j)
            {
                v += mat1(i,j) * mat2(j,k);
            }
            ret(i,k) = v;
        }
    }
    return ret;
}

Matrix multiply_tile(Matrix & mat1, Matrix & mat2, size_t tsize){
    Matrix ret(mat1.nrow(), mat2.ncol());
    size_t tile_limit_i, tile_limit_j, tile_limit_k;
    for (size_t i=0; i<ret.nrow(); i += tsize){
        for (size_t j=0; j<ret.ncol(); j += tsize){
            for (size_t k=0; k<mat1.ncol(); k += tsize){

                tile_limit_i = tsize;
                tile_limit_j = tsize;
                tile_limit_k = tsize;
                if(i + tsize >= ret.nrow())
                    tile_limit_i = ret.nrow()-i;
                if(j + tsize >= ret.ncol())
                    tile_limit_j = ret.ncol()-j;
                if(k + tsize >= mat1.ncol())
                    tile_limit_k = mat1.ncol()-k;
                
                for (size_t tile_i =i; tile_i < i+tile_limit_i; tile_i ++){
                    for (size_t tile_k =k; tile_k < k+tile_limit_k; tile_k ++){
                        for (size_t tile_j =j; tile_j < j+tile_limit_j; tile_j ++){
                            ret(tile_i, tile_j) += mat1(tile_i,tile_k) * mat2(tile_k,tile_j);
                        }
                    }
                }
            }
        }
    }
    
    return ret;
}

Matrix multiply_mkl(Matrix & mat1, Matrix & mat2){
    Matrix ans_mat(mat1.nrow(), mat2.ncol());
    int m = mat1.nrow();
    int n = mat1.ncol();
    int k = mat2.ncol();
    double alpha = 1;
    double beta = 0;

    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
           m, n, k, alpha, mat1.get_buffer_pointer(), k, mat2.get_buffer_pointer(), n, beta, ans_mat.get_buffer_pointer(), n);
    return ans_mat;
}

size_t bytes(){
    return allocator.counter.allocated() - allocator.counter.deallocated();
}
size_t allocated(){
    return allocator.counter.allocated();
}
size_t deallocated(){
    return allocator.counter.deallocated();
}

#if DEBUG == 0
PYBIND11_MODULE(_matrix, m) {
    m.doc() = "This is doc.";
    py::class_<Matrix>(m, "Matrix")
        .def(py::init([](size_t nrow, size_t ncol) { return new Matrix(nrow, ncol); }))
        .def(py::init([](size_t nrow, size_t ncol, std::vector<double> const & vec) { return new Matrix(nrow, ncol, vec); }))
        .def("get_mat", &Matrix::get_mat)
        .def("__setitem__", [](Matrix & mat,std::pair<size_t,size_t> key, double val) { mat.setter(key,val); })
        .def("__getitem__", [](Matrix & mat,std::pair<size_t,size_t> key) { return mat.getter(key); })
        .def_property("nrow", &Matrix::nrow, nullptr)
        .def_property("ncol", &Matrix::ncol, nullptr)
        .def("__eq__", &Matrix::operator==)
        ;
    m.def("bytes", &bytes, "A function that bytes");
    m.def("allocated", &allocated, "A function that allocated");
    m.def("deallocated", &deallocated, "A function that deallocated");
    m.def("multiply_naive", &multiply_naive, "A function that multiply_naive");
    m.def("multiply_tile", &multiply_tile, "A function that multiply_tile");
    m.def("multiply_mkl", &multiply_mkl, "A function that multiply_mkl");
}
#endif

#if DEBUG == 1
int main(int argc, char ** argv)
{
    size_t nrow = 100, ncol = 100;
    Matrix mat1(nrow,ncol), mat2(nrow, ncol);
    cout << "create:" << endl;
    cout << "bytes:" << bytes() << endl;
    cout << "allocated:" << allocated() << endl;
    cout << "deallocated:" << deallocated() << endl;
    Matrix mat3;
    cout << "create empty mat3:" << endl;
    cout << "bytes:" << bytes() << endl;
    cout << "allocated:" << allocated() << endl;
    cout << "deallocated:" << deallocated() << endl;
    mat3 = multiply_mkl(mat1, mat2);
    cout << "mkl:" << endl;
    cout << "bytes:" << bytes() << endl;
    cout << "allocated:" << allocated() << endl;
    cout << "deallocated:" << deallocated() << endl;
    Matrix mat4();
    cout << "create mat 4 by move constructor of mkl:" << endl;
    cout << "bytes:" << bytes() << endl;
    cout << "allocated:" << allocated() << endl;
    cout << "deallocated:" << deallocated() << endl;

    return 0;
}
#endif