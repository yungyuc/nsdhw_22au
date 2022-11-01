#include <pybind11/pybind11.h>
namespace py = pybind11;

#include <iostream>
#include <cblas.h> 
#include <vector>

class Matrix {
public:
    Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol)
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
    
    double operator[](size_t index) { return m_buffer[index]; }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }

private:
    size_t m_nrow;
    size_t m_ncol;
    double * m_buffer;
};

Matrix multiply_naive(Matrix  mat1, Matrix  mat2)
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

Matrix multiply_tiling (Matrix mat1 , Matrix mat2){
    // int N = mat2.ncol(); 
    // int M = mat1.nrow(); 
    // int K = mat1.ncol(); 
    // Matrix ret(M, N);
    // const int block_size = 64 / sizeof(float); // 64 = common cache line size
    // for(int i=0; i<N; i++) {
    //     for(int j=0; j<K; j++) {
    //         ret[K*i + j] = 0;
    //     }
    // }
    // for (int i0 = 0; i0 < N; i0 += block_size) {
    //     int imax = i0 + block_size > N ? N : i0 + block_size;

    //     for (int j0 = 0; j0 < M; j0 += block_size) {
    //         int jmax = j0 + block_size > M ? M : j0 + block_size;

    //         for (int k0 = 0; k0 < K; k0 += block_size) {
    //             int kmax = k0 + block_size > K ? K : k0 + block_size;

    //             for (int j1 = j0; j1 < jmax; ++j1) {
    //                 int sj = M * j1;

    //                 for (int i1 = i0; i1 < imax; ++i1) {
    //                     int mi = M * i1;
    //                     int ki = K * i1;
    //                     int kij = ki + j1;

    //                     for (int k1 = k0; k1 < kmax; ++k1) {
    //                         ret[kij] += mat1[mi + k1] * mat2[sj + k1];
    //                     }
    //                 }
    //             }
    //         }
    //     }
    // }
    // return ret;
    return (multiply_naive(mat1, mat2));
}

Matrix multiply_mkl(Matrix mat1 , Matrix mat2){
    int N = mat2.ncol(); 
    int M = mat1.nrow(); 
    int K = mat1.ncol();
    Matrix ret(M, N);
    //cblas_dgemm('N','N','N',M,N,K,1,mat1,M,mat2,K,1,ret,M); //not working
    return ret;
}

int main(){
    Matrix mat1(4, 3);
    mat1(0,0) = 1; mat1(0,1) = 1; mat1(0,2) = 1;
    mat1(1,0) = 8; mat1(1,1) = 4; mat1(1,2) = 2;
    mat1(2,0) = 27; mat1(2,1) = 9; mat1(2,2) = 3;
    mat1(3,0) = 64; mat1(3,1) = 16; mat1(3,2) = 4; 
    
    Matrix mat2(3, 4);
    mat2(0,0) = 1; mat2(0,1) = 1; mat2(0,2) = 1; mat2(0,3) = 5;
    mat2(1,0) = 8; mat2(1,1) = 4; mat2(1,2) = 2; mat2(1,3) = 5;
    mat2(2,0) = 27; mat2(2,1) = 9; mat2(2,2) = 3; mat2(2,3) = 2;
    
    Matrix mat3(4, 4);
    mat3 = multiply_naive(mat1,mat2);
    mat3 = multiply_tiling(mat1, mat2);
    mat3 = multiply_mkl(mat1, mat2);

    //std::cout << "matrix A (2x3):" << mat1 << std::endl;
    //std::cout << "matrix B (3x2):" << mat2 << std::endl;
    //std::cout << "result matrix C(2x2) = AB:" << mat3 << std::endl;
}

PYBIND11_MODULE(_vector, m) {
    m.doc() = "multiplication of Matrix";
    m.def("multiply_naive", &multiply_naive, "naive multiplication");
    m.def("multiply_tiling", &multiply_tiling, "tiling multiplication");
    m.def("multiply_mkl", &multiply_mkl, "GDEMM multiplication");
}