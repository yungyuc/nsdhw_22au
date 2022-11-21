#include <iostream>
#include <limits>
#include <vector>
#include <cassert>


template<typename T>
class Matrix {
private:
    size_t m_rows, m_cols;
    std::vector<T, MyAllocator<double>> m_matrix = vector<T, MyAllocator<double>>(my_allocator);

public:
    Matrix(): m_rows(0), m_cols(0) {}

    Matrix(size_t rows, size_t cols): m_rows(rows), m_cols(cols){
        this->m_matrix.resize(this->m_rows * this->m_cols, 0);
    }

    Matrix(Matrix const& counter): m_rows(counter.m_rows), m_cols(counter.m_cols), m_matrix(counter.m_matrix){}

    Matrix(Matrix&& counter): m_rows(counter.m_rows), m_cols(counter.m_cols), m_matrix(move(counter.m_matrix)){}

    ~Matrix(){
        this->m_matrix.clear();
        this->m_matrix.shrink_to_fit();
    }

    Matrix& operator=(Matrix const& counter){
        if (this != &counter) {
            this->m_rows = counter.m_rows;
            this->m_cols = counter.m_cols;
            this->m_matrix = counter.m_matrix;
        }

        return *this;
    }

    Matrix& operator=(Matrix&& counter){
        if (this != &counter) {
            this->m_rows = counter.m_rows;
            this->m_cols = counter.m_cols;
            this->m_matrix = move(counter.m_matrix);
        }

        return *this;
    }

    T const& operator()(size_t i, size_t j) const{
        return this->m_matrix[i * this->m_cols +j];
    }

    T& operator()(size_t i, size_t j){
        return this->m_matrix[i * this->m_cols + j];
    }

    const T* data() const{
        return &(this->m_matrix[0]);
    }

    T* data(){
        return &(this->m_matrix[0]);
    }

    constexpr bool operator==(const Matrix& counter) const{
        return (this->m_matrix == counter.m_matrix);
    }

    constexpr size_t rows() const{
        return this->m_rows;
    }

    constexpr size_t cols() const{
        return this->m_cols;
    }

    friend ostream& operator<<(ostream& os, const Matrix<T>& matrix){
        for (size_t i = 0; i < matrix.rows(); i++) {
            for (size_t j = 0; j < matrix.cols(); j++) {
                os << matrix(i, j) << (j == matrix.cols() - 1 ? "" : " ");
            }

            if (i < matrix.rows() - 1) os << '\n';
        }

        return os;
    }
};