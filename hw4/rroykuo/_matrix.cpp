# include "matrix.h"
# include <pybind11/pybind11.h>
# include <pybind11/stl.h>
# include <pybind11/operators.h>
# include <mkl.h>
# include <iomanip>
# include <stdexcept>


using namespace std;


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

PYBIND11_MODULE(_matrix, m){
    m.doc() = "matrix module";
    pybind11::class_<Matrix>(m, "Matrix")
        .def(pybind11::init<size_t, size_t>())
        .def("__getitem__", [](Matrix& self, pair<size_t, size_t> index) {
            return self(index.first, index.second);
        })
        .def("__setitem__", [](Matrix& self, pair<size_t, size_t> index, double value) {
            self(index.first, index.second) = value;
        })
        .def(pybind11::self == pybind11::self)
        .def_property_readonly("nrow", &Matrix::nrow )
        .def_property_readonly("ncol", &Matrix::ncol );
    
    m.def("multiply_naive", &multiply_naive, "naive multiplication");
    m.def("multiply_tile", &multiply_tile, "tile multiplication");
    m.def("multiply_mkl", &multiply_mkl, "mkl multiplication");
    m.def("bytes", &bytes);
    m.def("allocated", &allocated);
    m.def("deallocated", &deallocated);
}
