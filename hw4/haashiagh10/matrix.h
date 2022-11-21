#pragma once

#include <iostream>
#include <limits>
#include <vector>
#include <cassert>

using namespace std;

// ---------- Implement my ByteCounter ----------
struct ByteCounterImplement {
    size_t allocated = 0;
    size_t deallocated = 0;
};

class ByteCounter {
private:
    ByteCounterImplement* m_implement;

public:
    ByteCounter(): m_implement(new ByteCounterImplement) {}

    ByteCounter(const ByteCounter& counter): m_implement(counter.m_implement) {}

    ByteCounter(ByteCounter&& counter): m_implement(counter.m_implement) {}

    ~ByteCounter() = default;

    ByteCounter& operator=(const ByteCounter& counter) = delete;
    ByteCounter& operator=(ByteCounter&& counter) = delete;

    void increase(size_t n_size){
        this->m_implement->allocated += n_size;
    }

    void decrease(size_t n_size){
        this->m_implement->deallocated += n_size;
    }

    size_t bytes() const{
        return (this->m_implement->allocated - this->m_implement->deallocated);
    }

    size_t allocated() const{
        return this->m_implement->allocated;
    }

    size_t deallocated() const{
        return this->m_implement->deallocated;
    }

    constexpr bool operator==(const ByteCounter& counter) const{
        return (this->m_implement == counter.m_implement);
    }
};

// ---------- Implement Allocator ----------
template<class T>
class Allocator {
private:
    ByteCounter my_counter;

public:
    using value_type = T;

    Allocator() = default;

    template<class U>
    Allocator(const Allocator<U>& rhs) noexcept : my_counter(rhs.my_counter) {}

    ~Allocator() = default;

    T* allocate(size_t n){
        // check allocate size
        assert(n <= numeric_limits<size_t>::max() / sizeof(T));

        const size_t bytes = n * sizeof(T);
        T* ptr = static_cast<T*>(malloc(bytes));

        // check malloc status
        assert(ptr);

        this->my_counter.increase(bytes);
        cout << "Allocate increase: " << this->my_counter.bytes() << endl;

        return ptr;
    }

    void deallocate(T* ptr, size_t n){
        free(ptr);
        this->my_counter.decrease(n * sizeof(T));
        cout << "Deallocate decrease: " << this->my_counter.bytes() << endl;
    }

    constexpr size_t bytes() const{
        return this->my_counter.bytes();
    }

    constexpr size_t allocated() const{
        return this->my_counter.allocated();
    }

    constexpr size_t deallocated() const{
        return this->my_counter.deallocated();
    }

    constexpr bool operator==(const Allocator<T>& rhs) const{
        return (this->my_counter == rhs.my_counter);
    }

    friend ostream& operator<<(ostream& out, const Allocator<T>& rhs){
        out << "MyAllocator: bytes = " << rhs.bytes() << '\n'\
            << "               allocated = " << rhs.allocated() << '\n'\
            << "               deallocated = " << rhs.deallocated() << endl;

        return out;
    }
};

// Allocator init in global
Allocator<double> my_allocator;

size_t bytes(){
    return my_allocator.bytes();
}

size_t allocated(){
    return my_allocator.allocated();
}

size_t deallocated(){
    return my_allocator.deallocated();
}

template<typename T>
class Matrix {
private:
    size_t m_rows, m_cols;
    std::vector<T, Allocator<double>> m_matrix = vector<T, Allocator<double>>(my_allocator);

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