#include <iostream>
#include <vector>

template <typename T>
class Matrix
{
    private:
        size_t n_row, n_col;
        std::vector<T> matrix;
    public:
        Matrix()
        {
            this->n_row = 0;
            this->n_col = 0;
        }

        Matrix(size_t n_row, size_t n_col)
        {
            this->n_row = n_row;
            this->n_col = n_col;
            this->matrix.resize(n_row*n_col, 0);
        }

        Matrix(Matrix const& rv)
        {
            this->n_row = rv.n_row;
            this->n_col = rv.n_col;
            this->matrix = rv.matrix;
        }

        ~Matrix()
        {
            this->matrix.clear();
            this->matrix.shrink_to_fit();
        }

        Matrix& operator=(Matrix const& rv)
        {
            if(this != &rv)
            {
                this->n_col = rv.n_col;
                this->n_row = rv.n_row;
                this->matrix = rv.matrix;
            }

            return *this;
        }

        Matrix& operator=(Matrix && rv)
        {
            if(this != &rv)
            {
                this->n_col = rv.n_col;
                this->n_row = rv.n_row;
                this->matrix = rv.matrix;
            }

            return *this;
        }

        T const& operator()(size_t i, size_t j) const //getter
        {
            return this->matrix[i*this->n_col + j];
        }

        T & operator()(size_t i, size_t j) //setter
        {
            return this->matrix[i*this->n_col + j];
        }

        const T* data() const
        {
            return &(this->matrix[0]);
        }

        T* data()
        {
            return &(this->matrix[0]);
        }

        constexpr bool operator==(const Matrix& rv) const
        {
            return this->matrix == rv.matrix;
        }

        constexpr size_t rows() const
        {
            return this->n_row;
        }

        constexpr size_t cols() const
        {
            return this->n_col;
        }

        void output() const
        {   
            for(size_t row=0; row<this->n_row; ++row)
            {
                for(size_t col=0; col<this->n_col; ++col)
                    std::cout << this->matrix[row*this->n_row + col] << " ";
                std::cout << std::endl;
            }
        }
};