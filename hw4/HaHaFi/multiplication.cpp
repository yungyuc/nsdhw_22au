#include <iostream>
#include <stdexcept>
#include <stddef.h>
#include "matrix.h"
#include <mkl/mkl.h>
#include <vector>
#include <utility>
#include <tuple>
#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>

Allocator<double> alloc;
namespace py = pybind11;

void Check_mutiply_valid(Matrix &mat1, Matrix &mat2)
{
    if (mat1.n_col() != mat2.n_row())
    {
        throw std::invalid_argument("The matrix shape is invalid to mutiply");
    }
}

Matrix multiply_naive(Matrix &mat1, Matrix &mat2)
{
    Check_mutiply_valid(mat1, mat2);
    const size_t K = mat1.n_col(), M = mat1.n_row(), N = mat2.n_col();
    Matrix ret_mat(M, N);
    for (size_t i = 0; i < M; i++)
    {
        const size_t ret_leader = i * N;
        for (size_t j = 0; j < N; j++)
        {
            double count = 0;
            const size_t m1_leader = i * K;
            for (size_t k = 0; k < K; k++)
            {
                count += mat1.m_buffer[m1_leader + k] * mat2.m_buffer[k * N + j];
            }
            ret_mat.m_buffer[ret_leader + j] = count;
        }
    }
    return ret_mat;
}

Matrix multiply_tile(Matrix &mat1, Matrix &mat2, size_t tile_size)
{
    Check_mutiply_valid(mat1, mat2);
    const size_t K = mat1.n_col(), M = mat1.n_row(), N = mat2.n_col();
    Matrix ret_mat(M, N);

    for (size_t m1_start_index = 0; m1_start_index < M; m1_start_index += tile_size)
    {
        const size_t m1_end_index = std::min(m1_start_index + tile_size, M);
        for (size_t common_start_index = 0; common_start_index < K; common_start_index += tile_size)
        {
            const size_t common_end_index = std::min(common_start_index + tile_size, K);
            for (size_t m2_start_index = 0; m2_start_index < N; m2_start_index += tile_size)
            {
                const size_t m2_end_index = std::min(m2_start_index + tile_size, N);
                /*
                start calculate multiply on  tile_size*tile_size (mat1 block * mat2 block)
                mat1 block = (m1_start_index ~ m1_end_index) * (common_start_index ~ common_end_index)
                mat2 block = (common_start_index ~ common_end_index) * (m2_start_index ~ m2_end_index)
                */
                for (size_t i = m1_start_index; i < m1_end_index; ++i)
                {
                    const size_t leader_i = i * K;
                    const size_t leader_ret = i * N;
                    for (size_t _k = common_start_index; _k < common_end_index; ++_k)
                    {
                        const size_t mat_1_index = leader_i + _k;
                        const size_t mat_2_leader = _k * N;
                        for (size_t j = m2_start_index; j < m2_end_index; ++j)
                        {
                            ret_mat.m_buffer[leader_ret + j] += mat1.m_buffer[mat_1_index] * mat2.m_buffer[mat_2_leader + j];
                        }
                    }
                }
            }
        }
    }
    return ret_mat;
}

Matrix multiply_mkl(Matrix &mat1, Matrix &mat2)
{
    Check_mutiply_valid(mat1, mat2);
    const int m = mat1.n_row(), n = mat2.n_col(), k = mat1.n_col();

    const double alpha = 1.0, beta = 0.0;
    Matrix ret_mat(m, n);

    // void cblas_dgemm(const CBLAS_LAYOUT Layout, const CBLAS_TRANSPOSE TransA,
    //              const CBLAS_TRANSPOSE TransB, const MKL_INT M, const MKL_INT N,
    //              const MKL_INT K, const double alpha, const double *A,
    //              const MKL_INT lda, const double *B, const MKL_INT ldb,
    //              const double beta, double *C, const MKL_INT ldc) NOTHROW;
    cblas_dgemm(
        CblasRowMajor,
        CblasNoTrans,
        CblasNoTrans,
        m,
        n,
        k,
        alpha,
        &mat1.m_buffer[0],
        k,
        &mat2.m_buffer[0],
        n,
        beta,
        &ret_mat.m_buffer[0],
        n);
    return ret_mat;
}
// hw4
size_t bytes() { return alloc.counter.bytes(); };
size_t allocated() { return alloc.counter.allocated(); };
size_t deallocated() { return alloc.counter.deallocated(); };
size_t refcount() { return alloc.counter.refcount(); };
PYBIND11_MODULE(_matrix, m)
{

    m.def("multiply_mkl", &multiply_mkl, "use cblas", py::arg("mat1"), py::arg("mat2"));
    m.def("multiply_naive", &multiply_naive, "navie ", py::arg("mat1"), py::arg("mat2"));
    m.def("multiply_tile", &multiply_tile, "tile version", py::arg("mat1"), py::arg("mat2"), py::arg("tile_size"));
    m.def("bytes", &bytes, "counter info of allocating memory");
    m.def("allocated", &allocated, "counter info of allocated memory");
    m.def("deallocated", &deallocated, "counter info of deallocated memory");
    m.def("refcount", &refcount, "counter info of refcount");
    py::class_<Matrix>(m, "Matrix")
        .def(py::init<size_t, size_t>())
        .def_readonly("nrow", &Matrix::m_row)
        .def_readonly("ncol", &Matrix::m_col)
        .def(
            "__getitem__", [](Matrix &c, std::tuple<size_t, size_t> a)
            { return c(std::get<0>(a), std::get<1>(a)); },
            py::is_operator())
        .def(
            "__setitem__", [](Matrix &c, std::tuple<size_t, size_t> a, double value)
            { c(std::get<0>(a), std::get<1>(a)) = value; },
            py::is_operator())
        .def(
            "__eq__", [](Matrix &c, Matrix &a)
            { return c == a; },
            py::is_operator())
        .def(
            "__ne__", [](Matrix &c, Matrix &a)
            { return !(c == a); },
            py::is_operator());
}