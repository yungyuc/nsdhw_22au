# include <vector>
# include <iostream>

class Matrix {
  public:
    Matrix(size_t input_row_n, size_t input_col_n)
      : row_n(input_row_n), col_n(input_col_n){
        this->mat.resize(input_col_n * input_row_n, 0);
      }

    Matrix(Matrix const &input_mat){
      this->row_n = input_mat.nrow();
      this->col_n = input_mat.ncol();
      this->mat = input_mat.mat;
    }

    Matrix(Matrix &&input_mat){
      this->row_n = std::move(input_mat.nrow());
      this->col_n = std::move(input_mat.ncol());
      this->mat = std::move(input_mat.mat);
    }

    Matrix()
    :row_n(0), col_n(0){}

    ~Matrix(){
      mat.erase(mat.begin(), mat.end());
      mat.shrink_to_fit();
    }

    double & operator() (size_t input_row, size_t input_col) {
      return mat[this->col_n * input_row + input_col];
    }

    double const&  operator() (size_t input_row, size_t input_col) const {
      return mat[this->col_n * input_row + input_col];
    }

    size_t nrow() const {return row_n;}
    size_t ncol() const {return col_n;}

    Matrix & operator= (Matrix const &input_mat){
      if (this != &input_mat) {
            this->row_n = input_mat.nrow();
            this->col_n = input_mat.ncol();
            this->mat = input_mat.mat;
        }
      return *this;
    }

    Matrix & operator= (Matrix &&input_mat){
      if (this != &input_mat) {
            this->row_n = input_mat.nrow();
            this->col_n = input_mat.ncol();
            this->mat = std::move(input_mat.mat);
      }

        return *this;
    }

    bool operator== (const Matrix &mat2) const{
      return (this->mat == mat2.mat);
    }

    const double* data() const{
      return &(this->mat[0]);
    }
    double* data(){
      return &(this->mat[0]);
    }
    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix)
    {
        for (size_t i = 0; i < matrix.nrow(); i++) {
            for (size_t j = 0; j < matrix.ncol(); j++) {
                os << matrix(i, j) << (j == matrix.ncol() - 1 ? "" : " ");
            }

            if (i < matrix.nrow() - 1) os << '\n';
        }

        return os;
    }



  private:
    size_t row_n;
    size_t col_n;
    std::vector <double> mat;

};
