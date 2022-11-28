#include <mkl.h>
#include <pybind11/numpy.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>

#include <iostream>
#include <limits>
#include <string>
#include <sstream>
#include <cstring>
#include <vector>

struct ByteCounterImpl
{
    std::size_t allocated = 0;
    std::size_t deallocated = 0;
    std::size_t refcount = 0;
}; /* end struct ByteCounterImpl */

class ByteCounter
{
public:
    ByteCounter()
      : m_impl(new ByteCounterImpl)
    {}

    void increase(std::size_t amount)
    {
        m_impl->allocated += amount;
    }

    void decrease(std::size_t amount)
    {
        m_impl->deallocated += amount;
    }

    std::size_t bytes() const { return m_impl->allocated - m_impl->deallocated; }
    std::size_t allocated() const { return m_impl->allocated; }
    std::size_t deallocated() const { return m_impl->deallocated; }

private:
    ByteCounterImpl * m_impl;
}; /* end class ByteCounter */

template <class T>
struct MyAllocator
{

    using value_type = T;

    // Just use the default constructor of ByteCounter for the data member
    // "counter".
    MyAllocator() = default;

    T * allocate(std::size_t n)
    {
        if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
        {
            throw std::bad_alloc();
        }
        const std::size_t bytes = n*sizeof(T);
        T * p = static_cast<T *>(std::malloc(bytes));
        if (p)
        {
            counter.increase(bytes);
            return p;
        }
        else
        {
            throw std::bad_alloc();
        }
    }

    void deallocate(T* p, std::size_t n) noexcept
    {
        std::free(p);

        const std::size_t bytes = n*sizeof(T);
        counter.decrease(bytes);
    }

    ByteCounter counter;

}; /* end struct MyAllocator */

MyAllocator<double> alloc;

// template <class T, class U>
// bool operator==(const MyAllocator<T> & a, const MyAllocator<U> & b)
// {
//     return a.counter == b.counter;
// }

// template <class T, class U>
// bool operator!=(const MyAllocator<T> & a, const MyAllocator<U> & b)
// {
//     return !(a == b);
// }

std::size_t bytes() { return alloc.counter.bytes(); }
std::size_t allocated() { return alloc.counter.allocated(); }
std::size_t deallocated() { return alloc.counter.deallocated(); }

class Matrix {
public:
    size_t nrow;
    size_t ncol;

    Matrix(size_t n_row, size_t n_col)
    {
        nrow = n_row;
        ncol = n_col;
        size_t nelement = nrow * ncol;
        m_buffer.resize(nelement);
    }

    // No bound check.
    double   operator() (size_t row, size_t col) const
    {
        return m_buffer[row* ncol + col];
    }
    double & operator() (size_t row, size_t col)
    {
        return m_buffer[row* ncol + col];
    }

	void load_from_python(pybind11::array_t<double> input) {
		pybind11::buffer_info buf = input.request();
		memcpy(get_data(), buf.ptr, nrow * ncol * sizeof(double));
	}
	void load_buffer(double* input) {
		memcpy(get_data(), input, nrow * ncol * sizeof(double));
	}

    friend bool operator == (const Matrix& A, const Matrix& B){
        if ((A.nrow!=B.nrow) || (A.ncol!=B.ncol)) return false;
        for (size_t i=0;i<A.nrow*A.ncol;i++){
            if (A.m_buffer[i] != B.m_buffer[i]) return false;
        }
        return true;
    }

    double* get_data() const {
        return (double*)&m_buffer[0];
    }
    
	std::string tostring() const {
		std::stringstream ss;
		ss << "[";
		for (size_t row = 0; row < nrow; row++) {
			if (row > 0) {
				ss << " ";
			}

			ss << "[";
			for (size_t col = 0; col < ncol; col++) {
				ss << (*this)(row, col) << " ";
			}
			ss << "]";
			if (row < nrow - 1) {
				ss << std::endl;
			}
		}
		ss << "]";
        ss << std::endl;
		return ss.str();
	}

private:
    std::vector<double, MyAllocator<double>> m_buffer = std::vector<double, MyAllocator<double>>(alloc);

};

Matrix multiply_naive(Matrix& matrix_a, Matrix& matrix_b){
    if (matrix_a.ncol != matrix_b.nrow){
        throw pybind11::value_error("The shape of the two given matrices are not matched.");
    }
    
    Matrix result(matrix_a.nrow, matrix_b.ncol);
    for (size_t row = 0; row < matrix_a.nrow; row++){
        for (size_t col = 0; col < matrix_b.ncol; col++){
            for (size_t i = 0; i < matrix_a.ncol; i++){
                result(row,col) += matrix_a(row,i) * matrix_b(i,col);
            }
        }
    }
    return result;
}

Matrix multiply_tile(Matrix& matrix_a, Matrix& matrix_b, size_t tsize){
    if (matrix_a.ncol != matrix_b.nrow){
        throw pybind11::value_error("The shape of the two given matrices are not matched.");
    }

    Matrix result(matrix_a.nrow, matrix_b.ncol);
    for (size_t tile_row_start=0; tile_row_start<matrix_a.nrow; tile_row_start+=tsize){
        // size_t tile_row_end = (tile_row_start+tsize>matrix_a.nrow)?matrix_a.nrow:tile_row_start+tsize;
        size_t tile_row_end = tile_row_start+tsize;
        tile_row_end = (tile_row_end>matrix_a.nrow)?matrix_a.nrow:tile_row_end;

        for (size_t tile_col_start=0; tile_col_start<matrix_b.ncol; tile_col_start+=tsize){
            // size_t tile_col_end = (tile_col_start+tsize>matrix_b.ncol)?matrix_b.ncol:tile_col_start+tsize;
            size_t tile_col_end = tile_col_start+tsize;
            tile_col_end = (tile_col_end>matrix_b.ncol)?matrix_b.ncol:tile_col_end;

            for (size_t t=0; t<matrix_a.ncol; t++){
                for (size_t i=tile_row_start; i<tile_row_end;i++){
                    for (size_t j=tile_col_start; j<tile_col_end;j++){
                        result(i,j) += matrix_a(i,t)*matrix_b(t,j);
                    }
                }
            }
        }
    }
    return result;
}

Matrix multiply_mkl(Matrix& matrix_a, Matrix& matrix_b) {
	if (matrix_a.ncol != matrix_b.nrow) {
		throw pybind11::value_error("The shape of the two given matrices are not matched.");
	}

	double* C = new double[matrix_a.nrow * matrix_b.ncol];

	int m = matrix_a.nrow;
	int k = matrix_a.ncol;
	int n = matrix_b.ncol;
	double alpha = 1.0, beta = 0.0;

	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
				m, n, k, alpha, matrix_a.get_data(), k, matrix_b.get_data(), n, beta, C, n);

	Matrix res(matrix_a.nrow, matrix_b.ncol);
    res.load_buffer(C);

	delete[] C;

	return res;
}

PYBIND11_MODULE(_matrix, m){
    m.doc() = "_matrix";

    pybind11::class_<Matrix>(m,"Matrix")
        .def(pybind11::init<size_t, size_t>())
        .def("__getitem__", [](Matrix& mat, std::pair<size_t,size_t> index){
            return mat(index.first, index.second);
        })
        .def("__setitem__", [](Matrix& mat, std::pair<size_t,size_t> index, double value){
            mat(index.first,index.second) = value;
        })
        .def("__str__", [](Matrix& mat){
            return mat.tostring();
        })
        .def("load", &Matrix::load_from_python)
        .def(pybind11::self == pybind11::self)
        .def_readonly("nrow",&Matrix::nrow)
        .def_readonly("ncol",&Matrix::ncol);
    
	m.def("multiply_naive", &multiply_naive);
	m.def("multiply_tile", &multiply_tile);
	m.def("multiply_mkl", &multiply_mkl);
	m.def("bytes", &bytes);
	m.def("allocated", &allocated);
	m.def("deallocated", &deallocated);
}

// int main(){
//     Matrix A = Matrix(2,3);
//     A(0,0)=1.0;
//     A(0,1)=2.0;
//     A(0,2)=3.0;
//     A(1,0)=4.0;
//     A(1,1)=5.0;
//     A(1,2)=6.0;

//     Matrix B = Matrix(3,2);
//     B(0,0)=7.0;
//     B(0,1)=8.0;
//     B(1,0)=9.0;
//     B(1,1)=10.0;
//     B(2,0)=11.0;
//     B(2,1)=12.0;

//     Matrix C = multiply_naive(A,B);
//     Matrix D = multiply_tile(A,B,1);
//     Matrix E = multiply_mkl(A,B);
//     std::cout << C.tostring();
//     std::cout << D.tostring();
//     std::cout << E.tostring();
//     std::cout << "bytes: " << bytes() << std::endl;
//     std::cout << "allocated: " << allocated() << std::endl;
//     std::cout << "deallocated: " << deallocated() << std::endl;
//     return 0;
// }