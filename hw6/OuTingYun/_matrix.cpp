//gemfield.cpp
#include <iostream>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <numeric>

#include <mkl.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include <bits/stdc++.h>


using namespace std;
namespace py = pybind11;

class Matrix {  

public:
    vector<double> m_buffer;
    size_t n_row;
    size_t n_col;
    Matrix(size_t nrow, size_t ncol) : n_row(nrow), n_col(ncol){


        m_buffer.resize(nrow * ncol);
        for (size_t i=0;i<nrow;i++)
            for (size_t j=0;j<ncol;j++)
                m_buffer[i*nrow+j]=0;
    }



    size_t nrow() const { return n_row; }
    size_t ncol() const { return n_col; }

    double operator()(size_t row, size_t col)const{
        return m_buffer[row * n_col + col];

    }
    double &operator()(size_t row, size_t col){
			return m_buffer[row * n_col + col];
	}
    bool operator==(Matrix const &matrix) const{
        for (size_t i=0;i<n_row;i++)
            for(size_t j=0;j<n_col;j++)
                if (m_buffer[i*n_row+j]!=matrix(i,j))
                    return false;
        return true;
    }
    double* addr() { return m_buffer.data(); }

    double getitem(std::pair<size_t, size_t> index){
			return (*this)(index.first, index.second);
		}

    void setitem(std::pair<size_t, size_t> index, double value) {
        (*this)(index.first, index.second) = value;
    }

    pybind11::array_t<double> array() {
		return pybind11::array_t<double>(
			std::vector<size_t>{nrow(), ncol()},
			std::vector<size_t>{sizeof(double) * ncol(), sizeof(double)},
			m_buffer.data(),
			pybind11::cast(this));
    }

    
    

};

Matrix multiply_naive(Matrix& mat1, Matrix& mat2){
	Matrix res(mat1.nrow(), mat2.ncol());
	for(size_t k=0; k < mat2.ncol(); k++)
		for(size_t i=0; i < mat1.nrow(); i++)
			for(size_t j=0; j < mat1.ncol(); j++)
				res(i, k) += mat1(i,j) * mat2(j,k);

	return res;
}
Matrix multiply_tile(Matrix const & matrix1, Matrix const & matrix2, size_t tilesize)
{
    

    size_t row_max = matrix1.nrow();
    size_t col_max = matrix2.ncol();
    size_t inner_max = matrix1.ncol();

    if(row_max != col_max){
        throw out_of_range("mat1 col is different from mat2 row");
    }

    Matrix ret(row_max, col_max);
    
    // Run for every block.
    for (size_t row = 0; row < row_max; row += tilesize){
        size_t imax = min(row_max, row + tilesize);
    
        for (size_t col = 0; col < col_max; col += tilesize) {
            size_t jmax = min(col_max, col + tilesize);
            
            for (size_t inner = 0; inner < inner_max; inner += tilesize) {
                size_t kmax = min(inner_max, inner + tilesize);

                //Runing inside the block
                for (size_t i = row; i < imax; ++i){
                    size_t base1 = i * inner_max;

                    for (size_t j = col; j < jmax; ++j){
                        size_t index = i * col_max + j;

                        for (size_t k = inner; k < kmax; ++k) {   
                            ret.m_buffer[index] += matrix1.m_buffer[base1 + k] * matrix2(k, j);
                        }
                    }
                }
            }
        }
    }

    return ret;

}
Matrix multiply_mkl(Matrix& mat1, Matrix& mat2)
{
    if(mat1.ncol() != mat2.nrow()){
        throw out_of_range("mat1 col is different from mat2 row");
    }

    Matrix ret(mat1.nrow(), mat2.ncol());

    cblas_dgemm(
        CblasRowMajor, 
        CblasNoTrans, 
        CblasNoTrans, 
        mat1.nrow(), 
        mat2.ncol(), 
        mat1.ncol(), 
        1.0, 
        mat1.addr(), 
        mat1.ncol(), 
        mat2.addr(), 
        mat2.ncol(), 
        0.0, 
        ret.addr(), 
        mat2.ncol());

    return ret;
}

PYBIND11_MODULE(_matrix, m) {
	m.doc() = "pybind11 plugin";
	pybind11::class_<Matrix>(m, "Matrix")
		.def(pybind11::init<size_t, size_t>())
		// .def(pybind11::self == pybind11::self)	
        .def("__getitem__", &Matrix::getitem)
        .def("__setitem__", &Matrix::setitem)
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol)
		.def_property("array", &Matrix::array, nullptr);

	m.def("multiply_naive",&multiply_naive);
    m.def("multiply_tile",&multiply_tile);
    m.def("multiply_mkl",&multiply_mkl);
}

// PYBIND11_MODULE(_matrix, m) {
//     pybind11::class_<Matrix>(m, "Matrix")
//         .def(pybind11::init<size_t, size_t>())
//         .def("__getitem__", &Matrix::getitem)
// 		.def("__setitem__", &Matrix::setitem)
//         .def("__eq__", [](const Matrix &mat, const Matrix &other) { return mat == other; })
//         .def_property_readonly("nrow", &Matrix::nrow)
//         .def_property_readonly("ncol", &Matrix::ncol);
    
//     m.def("bytes",&bytes);
//     m.def("allocated", &allocated);
// 	m.def("deallocated", &deallocated);


// }
