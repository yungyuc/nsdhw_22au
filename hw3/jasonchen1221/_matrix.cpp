#include <stdexcept>
#include <pybind11/pybind11.h> 
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <mkl.h>

#include "_matrix.hpp"

//constructor
Matrix::Matrix(size_t nrow, size_t ncol)    
    : m_nrow(nrow), m_ncol(ncol)
{
    reset_buffer(nrow, ncol);
}
/*
Matrix::Matrix(size_t nrow, size_t ncol, std::vector<double> const & vec)
    : m_nrow(nrow), m_ncol(ncol)
{
    reset_buffer(nrow, ncol);
    (*this) = vec;  //need overloading for "="
}
*/
//copy constructor
Matrix::Matrix(Matrix const & other)
    : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
{
    reset_buffer(other.m_nrow, other.m_ncol);
    for(size_t i = 0; i < m_nrow; ++i)
    {
        for(size_t j = 0; j < m_ncol; ++j)
        {
            (*this)(i,j) = other(i,j);
        }
    }
}

Matrix & Matrix::operator=(Matrix const & other)
{
    if(this == &other) { return *this; }
    
    //if((m_nrow != other.m_nrow) || (m_ncol != other.m_ncol))
    //{
    reset_buffer(other.m_nrow, other.m_ncol);
    //}
    for(size_t i = 0; i < m_nrow; ++i)
    {
        for(size_t j = 0; j < m_ncol; ++j)
        {
            (*this)(i,j) = other(i,j);
        }
    }
    return *this;
}

//move constructor
/*
Matrix::Matrix(Matrix && other)
    : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
{
    reset_buffer(0,0);
    std::swap(m_nrow, other.m_nrow);
    std::swap(m_ncol, other.m_ncol);
    std::swap(m_buffer, other.m_buffer);
}

Matrix & Matrix::operator=(Matrix && other)
{
    if(this == &other) { return *this; }
    reset_buffer(0,0);
    std::swap(m_nrow, other.m_nrow);
    std::swap(m_ncol, other.m_ncol);
    std::swap(m_buffer, other.m_buffer);
    return *this;
}
*/
// operator overloading
Matrix & Matrix::operator=(std::vector<double> const & vec)
{
    if( size() != vec.size() )
    {
        throw std::out_of_range("Number of elements mismatch.");
    }

    size_t k = 0;
    for(size_t i = 0; i < m_nrow; ++i)
    {
        for(size_t j = 0; j < m_ncol; ++j)
        {
            (*this)(i,j) = vec[k];  // need operator overlading of "()"
            k++;
        }
    }

    return *this;
}
/*
bool Matrix::operator==(Matrix & mat) const
{
    if(this->nrow() != mat.nrow() || this->ncol() != mat.ncol()) return false;

    for(size_t i = 0; i < this->nrow(); ++i)
    {
        for(size_t j = 0; j < this->ncol(); ++j)
        {
            if((*this)(i,j) != mat(i,j)) return false;
        }
    }

    return true;
}
*/

//funcitons
void Matrix::reset_buffer(size_t nrow, size_t ncol)
{
    if(m_buffer) { delete[] m_buffer; }
    
    const size_t nelement = nrow * ncol;
    if(nelement) { m_buffer = new double[nelement]; }
    else         { m_buffer = nullptr; }

    m_nrow = nrow;
    m_ncol = ncol;

    for(size_t i = 0; i < nelement; ++i)
    {
        m_buffer[i] = 0;
    }

}

bool operator== (Matrix const &mat1, Matrix const &mat2)
{
	if(mat1.ncol() != mat2.ncol() || mat1.nrow() != mat2.nrow()) return false;

	for(size_t i = 0; i < mat1.nrow(); i++){
		for(size_t j = 0; j < mat1.ncol(); j++){
			if(mat1(i, j) != mat2(i, j)) return false;
		}
	}

	return true;
}

bool operator!= (Matrix const &mat1, Matrix const &mat2)
{
    return !(mat1 == mat2);
}

//Matrix multiplication
void check_multibility(Matrix const & mat1, Matrix const & mat2)
{
    if( mat1.ncol() != mat2.nrow() )
    {
        throw std::out_of_range("Matrix1 can't multiply with Matrix2 due two dimension mismatch.");
    }
}

Matrix multiply_naive(Matrix const & mat1, Matrix & mat2)
{
    check_multibility(mat1, mat2);

    Matrix res(mat1.nrow(), mat2.ncol());

    for(size_t i = 0; i < mat1.nrow(); ++i)
    {
        for(size_t j = 0; j < mat2.ncol(); ++j)
        {
            double sum = 0;
            for(size_t k = 0; k < mat1.ncol(); ++k)
            {
                sum += mat1(i,k) * mat2(k,j);
            }
            res(i,j) = sum;
        }    
    }

    return res;
}

Matrix multiply_tile(Matrix const & mat1, Matrix const & mat2, size_t blocksize)
{
    check_multibility(mat1, mat2);

    Matrix res(mat1.nrow(), mat2.ncol());

    for(size_t blocki = 0 ; blocki < mat1.nrow() ; blocki += blocksize)
    {
        size_t i_bound = std::min( blocki + blocksize, mat1.nrow() );
        for(size_t blockj = 0 ; blockj < mat2.ncol() ; blockj += blocksize)
        {
            size_t j_bound = std::min( blockj + blocksize, mat2.ncol() );
            for(size_t blockk = 0 ; blockk < mat1.ncol() ; blockk += blocksize)
            {
                size_t k_bound = std::min( blockk + blocksize, mat1.ncol() );
                for(size_t k = blockk ; k < k_bound ; k++)
                {
                    for(size_t i = blocki ; i < i_bound ; i++)
                    {
                        for(size_t j = blockj ; j < j_bound ; j++)
                        {
                            res(i,j) += mat1(i,k) * mat2(k,j);
                        }
                    }
                }
            }
        }
    }

    return res;
}

Matrix multiply_mkl(Matrix & mat1, Matrix & mat2)
{
    check_multibility(mat1, mat2);

    Matrix res(mat1.nrow(), mat2.ncol());
    /*
    const size_t m = mat1.nrow();
    const size_t n = mat2.ncol();
    const size_t k = mat1.ncol();

    const double alpha = 1.0; 
    const double beta  = 0.0;

    double* A = mat1.val_addr();
    double* B = mat2.val_addr();
    double* C = res.val_addr();

    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m, n, k, alpha, A, k, B, n, beta, C, n);
    */
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, mat1.nrow(), mat2.ncol(), mat1.ncol(), 1, mat1.val_addr(), mat1.ncol(), mat2.val_addr(), mat2.ncol(), 0, res.val_addr(), res.ncol());
    return res;
}



//pybind
PYBIND11_MODULE(_matrix, m)
{
    m.doc() = "Matrix Multiplication funciton unit test.";

    pybind11::class_<Matrix>(m, "Matrix")
        .def(pybind11::init<>())
        .def(pybind11::init<size_t, size_t>())
        //.def(pybind11::init<size_t &, size_t &, std::vector<double> const&>())
        .def(pybind11::self == pybind11::self)  //operator overloading 
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol)
        .def("__getitem__", &Matrix::getvalue)
        .def("__setitem__", &Matrix::setvalue);

    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_tile", &multiply_tile);
    m.def("multiply_mkl", &multiply_mkl);
}

