#include <iostream>
#include <vector>

#include "matrix.h"

int main()
{
   Matrix test_mat(5, 5);
   std::cout << test_mat.nrow() << " " << test_mat.ncol() << std::endl; 
   return 0;
}