# include <vector>
# include <iostream>

class Matrix {

  public:

    Matrix(size_t input_row_n, size_t input_col_n)
      : row_n(input_row_n), col_n(input_col_n){
        this->matrix_n = input_row_n * input_col_n;
        this->mat.resize(this->matrix_n, 0);
      }
    
    ~Matrix(){
      mat.erase(mat.begin(), mat.end());
      mat.shrink_to_fit();
    }

  private:
    size_t row_n;
    size_t col_n;
    size_t matrix_n;
    std::vector <double> mat;

};
