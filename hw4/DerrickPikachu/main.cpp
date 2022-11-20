#include <iostream>
#include <vector>

#include "matrix.h"

int main()
{
    // matrix::Matrix m1(5, 5);
    // std::cout << "Matrix.test(): ";
    // m1.test();

    std::vector<double, matrix::MatrixAllocatorCounter<double>> test;
    for (int i = 0; i < 512; i++) {
        test.push_back((double)i);
    }
    std::cout << matrix::MatrixAllocatorCounter<double>::allocate_bytes << " " << matrix::MatrixAllocatorCounter<double>::deallocate_bytes << std::endl;

}