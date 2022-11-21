#include <vector>
#include <iostream>
#include <algorithm>
#include "MyAllocator.h"

using std::cout;
using std::endl;
using std::size_t;

bool operator==(const ByteCounter & a, const ByteCounter & b)
{
    return a == b;
}

MyAllocator<double> allocator;

class Matrix {
  
public:
    
    Matrix() = default;
    Matrix(size_t nrow, size_t ncol)// constructor
      : m_nrow(nrow), m_ncol(ncol)
    {
// cout << "constructor\n";
        reset_buffer(nrow, ncol);
    }

    // // constructor with vector
    Matrix
    (
        size_t nrow, size_t ncol, std::vector<double> const & vec
    )
      : m_nrow(nrow), m_ncol(ncol)
    {
// cout << "constructor with vector\n";
        if (size() != vec.size())
        {
            throw std::out_of_range("number of elements mismatch");
        }
        for( size_t i = 0; i < vec.size(); i ++){
            m_buffer.push_back(vec.at(i));
        }
    }
    // copy constructor
    Matrix(const Matrix & other){
// cout << "copy constructor\n";
        m_buffer = other.m_buffer;
        m_nrow = other.m_nrow;
        m_ncol = other.m_ncol;
    }
    // copy assign operator
    Matrix & operator=(const Matrix & other){
// cout << "copy assign operator\n";
        m_buffer = other.m_buffer;
        m_nrow = other.m_nrow;
        m_ncol = other.m_ncol;
        return (*this);
    }
    // move constructor
    Matrix( Matrix && other){
// cout << "move constructor\n";

        m_buffer.swap(other.m_buffer);
        m_nrow = other.m_nrow;
        m_ncol = other.m_ncol;
    }
    // move assign operator
    Matrix & operator=(Matrix && other){
// cout << "move assign operator\n";
        if(this != &other){
            // cout << allocator << endl;
            m_buffer.swap(other.m_buffer);
            // cout << allocator << endl;
            m_buffer = other.m_buffer;
            // m_buffer = std::move(other.m_buffer);
            // std::move(other.m_buffer.begin(), other.m_buffer.end(), this->m_buffer.begin());
            m_nrow = other.m_nrow;
            m_ncol = other.m_ncol;
        }
        return (*this);
    }
    ~Matrix(){
        m_buffer.clear();
        m_buffer.shrink_to_fit();
        m_nrow = 0;
        m_ncol = 0;
    }

    Matrix & operator+=(Matrix const & mat1)
    {
        if ((*this).m_ncol != mat1.m_ncol)
        {
            throw std::out_of_range(
                "the number of column "
                "differs from that of second matrix column");
        }
        else if ((*this).m_nrow != mat1.m_nrow)
        {
            throw std::out_of_range(
                "the number of first matrix row "
                "differs from that of second matrix row");
        }

        std::transform(m_buffer.begin(), m_buffer.end(), mat1.m_buffer.begin(), 
        m_buffer.begin(), std::plus<double>());

        return (*this);
    }
    void setter(std::pair <size_t, size_t> key, double val){
        m_buffer[index(key.first,key.second)] = val;
    }
    double getter(std::pair <size_t, size_t> key){
        return m_buffer.at(index(key.first,key.second));
    }
    bool operator==(const Matrix & other)const{
        if(other.nrow()!=nrow() || other.ncol() != ncol())
            return false;
        return std::equal((*this).m_buffer.begin(), (*this).m_buffer.end(), other.m_buffer.begin());
    }
    double * get_buffer_pointer(){
        return &m_buffer[0];
    }
    // // // to locate the value
    double   operator() (size_t row, size_t col) const
    {
        return m_buffer.at(index(row, col));
    }
    double & operator() (size_t row, size_t col)
    {
        return m_buffer.at(index(row, col));
    }

    size_t nrow() const { return m_nrow; }//return the number of row of the matrix
    size_t ncol() const { return m_ncol; }//return the number of col of the matrix

    size_t size() const { return m_nrow * m_ncol; }//return the number of element of the matrix

    void get_mat(){
        std::cout << "row:" << m_nrow << " col:" << m_ncol << std::endl;
        size_t k = 0;
        for(size_t i = 0; i < m_nrow; i ++){
            for(size_t j = 0; j < m_ncol; j ++){
                std::cout << m_buffer.at(k) << " ";
                k ++;
            }
            std::cout << "\n";
        }
    }


private:

    size_t index(size_t row, size_t col) const
    {
        return row * m_ncol + col;
    }

    void reset_buffer(size_t nrow, size_t ncol)
    {
        const size_t nelement = nrow * ncol;
        m_buffer.resize(nelement);
        m_nrow = nrow;
        m_ncol = ncol;
    }
    
    std::vector<double, MyAllocator<double>> m_buffer{allocator};
    
    size_t m_nrow = 0;
    size_t m_ncol = 0;
};