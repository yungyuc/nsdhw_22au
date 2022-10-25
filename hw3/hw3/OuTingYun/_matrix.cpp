#include <pybind11/pybind11.h>

class Matrix {

private:

    size_t m_nrow;
    size_t m_ncol;
    double * m_buffer;

public:

    Matrix(size_t nrow, size_t ncol: m_nrow(nrow), m_ncol(ncol)
    {
        size_t nelement = nrow * ncol;
        m_buffer = new double[nelement];
    }

    ~Matrix()
    {
        delete[] m_buffer;
    }

    // No bound check.
    double   operator() (size_t row, size_t col) const
    {
        return m_buffer[row*m_ncol + col];
    }
    double & operator() (size_t row, size_t col)
    {
        return m_buffer[row*m_ncol + col];
    }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }



};

PYBIND11_MODULE(_matrix, m) {
	m.doc() = "pybind11 plugin";
	pybind11::class_<Matrix>(m, "Matrix")
		.def(pybind11::init<size_t, size_t>())
		.def(pybind11::self == pybind11::self)	
		.def("__getitem__", &Matrix::getitem)
		.def("__setitem__", &Matrix::setitem)
		.def_property_readonly("nrow", &Matrix::nrow)
		.def_property_readonly("ncol", &Matrix::ncol);

	m.def("multiply_naive", &multiply_naive);
	m.def("multiply_tile", &multiply_tile);
	m.def("multiply_mkl", &multiply_mkl);
}