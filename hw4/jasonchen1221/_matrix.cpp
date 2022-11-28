#include <stdexcept>
#include <pybind11/pybind11.h> 
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include "_matrix.hpp"


ByteCounter& ByteCounter::operator=(ByteCounter const & other)
{
    if (&other != this)
	{
		decref();
		m_impl = other.m_impl;
		incref();
	}
    return *this;
}

ByteCounter& ByteCounter::operator=(ByteCounter const && other)
{
    if (&other != this)
	{
		decref();
		m_impl = other.m_impl;
		incref();
	}
    return *this;
}


template <class T>
T* CustomAllocator<T>::allocate(std::size_t n)
{
    if(n > std::numeric_limits<std::size_t>::max() / sizeof(T) )
    {
        throw std::bad_alloc();
    }
    
    const std::size_t bytes = n * sizeof(T);
    T* p = static_cast<T*>(std::malloc(bytes));
    if(p)
    {
        counter.increase(bytes);
        return p;
    }
    else
    {
        throw std::bad_alloc();
    }
}

template <class T>
void CustomAllocator<T>::deallocate(T* p, std::size_t n) noexcept
{
    std::free(p);
    
    const std::size_t bytes = n * sizeof(T);
    counter.decrease(bytes);
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
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, 
                mat1.nrow(), mat2.ncol(), mat1.ncol(), 1.0, 
                mat1.data(), mat1.ncol(), mat2.data(), mat2.ncol(), 
                0.0, res.data(), res.ncol());
    return res;
}

//pybind
PYBIND11_MODULE(_matrix, m)
{
    m.doc() = "Matrix Multiplication funciton unit test.";

    pybind11::class_<Matrix>(m, "Matrix")
        //.def(pybind11::init<>())
        .def(pybind11::init<size_t, size_t>())
        .def(pybind11::self == pybind11::self)  //operator overloading 
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol)
        .def("__getitem__", &Matrix::getvalue)
        .def("__setitem__", &Matrix::setvalue);

    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_tile", &multiply_tile);
    m.def("multiply_mkl", &multiply_mkl);
    m.def("bytes", &bytes);
    m.def("allocated", &allocated);
    m.def("deallocated", &deallocated);
}