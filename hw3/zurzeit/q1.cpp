#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <mkl.h>
#include <math.h>
#include <iostream>
#include <vector>
namespace py = pybind11;

class Matrix {

public:
    double * m_buffer = nullptr;

    Matrix(size_t nrow, size_t ncol)// constructor
      : m_nrow(nrow), m_ncol(ncol)
    {
        reset_buffer(nrow, ncol);
    }

    // // constructor with vector
    Matrix
    (
        size_t nrow, size_t ncol, std::vector<double> const & vec
    )
      : m_nrow(nrow), m_ncol(ncol)
    {
        reset_buffer(nrow, ncol);
        (*this) = vec;
    }

    // // copy assignment operator with vec
    Matrix & operator=(std::vector<double> const & vec)
    {
        if (size() != vec.size())
        {
            throw std::out_of_range("number of elements mismatch");
        }

        size_t k = 0;
        for (size_t i=0; i<m_nrow; ++i)
        {
            for (size_t j=0; j<m_ncol; ++j)
            {
                (*this)(i,j) = vec[k];
                ++k;
            }
        }

        return *this;
    }
    void setter(std::pair <size_t, size_t> key, double val){
        m_buffer[index(key.first,key.second)] = val;
    }
    double getter(std::pair <size_t, size_t> key){
        return m_buffer[index(key.first,key.second)];
    }
    Matrix & operator+=(Matrix const & mat1)
    {
        if ((*this).ncol() != mat1.ncol())
        {
            throw std::out_of_range(
                "the number of column "
                "differs from that of second matrix column");
        }
        else if ((*this).nrow() != mat1.nrow())
        {
            throw std::out_of_range(
                "the number of first matrix row "
                "differs from that of second matrix row");
        }

        for (size_t i=0; i<(*this).nrow(); ++i){
            for (size_t j=0; j<(*this).ncol(); ++j){
                (*this)(i,j) += mat1(i,j);
            }
        }

        return (*this);
    }
    bool operator==(const Matrix & other)const{
        if(other.nrow()!=nrow() || other.ncol() != ncol())
            return false;
        for(size_t i = 0; i < nrow(); i ++){
            for(size_t j = 0; j < ncol(); j ++){
                if((*this)(i, j) != other(i, j))
                    return false;
            }
        }
        return true;
    }

    // // to locate the value
    double   operator() (size_t row, size_t col) const
    {
        return m_buffer[index(row, col)];
    }
    double & operator() (size_t row, size_t col)
    {
        return m_buffer[index(row, col)];
    }

    size_t nrow() const { return m_nrow; }//return the number of row of the matrix
    size_t ncol() const { return m_ncol; }//return the number of col of the matrix

    size_t size() const { return m_nrow * m_ncol; }//return the number of element of the matrix

    void get_mat(){
        std::cout << "row:" << m_nrow << " col:" << m_ncol << std::endl;
        size_t k = 0;
        for(size_t i = 0; i < m_nrow; i ++){
            for(size_t j = 0; j < m_ncol; j ++){
                std::cout << m_buffer[k] << " ";
                k ++;
            }
            std::cout << "\n";
        }
    }
private:

    size_t index(size_t row, size_t col) const
    {
        return row * m_ncol + col;
    }

    void reset_buffer(size_t nrow, size_t ncol)
    {
        if (m_buffer) { delete[] m_buffer; }
        const size_t nelement = nrow * ncol;
        if (nelement) { 
            m_buffer = new double[nelement];
            for(size_t i = 0; i < nrow*ncol; i++)
                m_buffer[i] =  0 ;    
            }
        else          { m_buffer = nullptr; }
        m_nrow = nrow;
        m_ncol = ncol;
    }

    size_t m_nrow = 0;
    size_t m_ncol = 0;
};

Matrix operator*(Matrix const & mat1, Matrix const & mat2)
{
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
/*
 * Naive matrix matrix multiplication.
 */
Matrix multiply_naive(Matrix & mat1, Matrix & mat2){
    return mat1*mat2;
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
           m, n, k, alpha, mat1.m_buffer, k, mat2.m_buffer, n, beta, ans_mat.m_buffer, n);
    return ans_mat;
}



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
    m.def("multiply_naive", &multiply_naive, "A function that multiply_naive");
    m.def("multiply_tile", &multiply_tile, "A function that multiply_tile");
    m.def("multiply_mkl", &multiply_mkl, "A function that multiply_mkl");
}