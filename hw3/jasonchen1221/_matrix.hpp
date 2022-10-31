
#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <cstdlib>
#include <vector>


class Matrix
{
public:
    //constructor
    Matrix() : m_nrow(0), m_ncol(0) { };
    Matrix(size_t nrow, size_t ncol);    
    //Matrix(size_t nrow, size_t ncol, std::vector<double> const & vec);
    //copy constructor
    Matrix(Matrix const & other);
    Matrix & operator=(Matrix const & other);
    //move constructor
    //Matrix(Matrix && other);
    //Matrix & operator=(Matrix && other);
    //destructor
    ~Matrix() { reset_buffer(0,0); }
    
    //operator overloading
    Matrix & operator=(std::vector<double> const & vec);

    double & operator()(size_t row, size_t col)       { return m_buffer[index(row,col)]; }
    double   operator()(size_t row, size_t col) const { return m_buffer[index(row,col)]; }
    
    double & operator[](size_t idx)                   { return m_buffer[idx]; }
    double   operator[](size_t idx) const             { return m_buffer[idx]; }

    //bool operator==(Matrix & mat) const;
    //bool operator== (Matrix &mat) const;
    
    //funcitons
    
    double *val_addr() {return &(this->m_buffer[0]);}
    double getvalue(std::pair<size_t, size_t> index) 
        {return (*this)(index.first, index.second);}
	void setvalue(std::pair<size_t, size_t> index, const double value) 
        {(*this)(index.first, index.second) = value;}
    
    //
    size_t size()                        const { return m_nrow * m_nrow; }
    size_t index(size_t row, size_t col) const { return row*m_ncol + col; }
    size_t nrow()                        const { return m_nrow; }
    size_t ncol()                        const { return m_ncol; }

    //members
    size_t m_nrow = 0;
    size_t m_ncol = 0;
    double *m_buffer = nullptr;
    void reset_buffer(size_t nrow, size_t ncol);
};

template<size_t N>
struct Block
{
    static constexpr const size_t NDIM = N;

    double   operator[] (size_t idx) const { return m_buffer[idx]; }
    double & operator[] (size_t idx)       { return m_buffer[idx]; }

    Block<N> & operator= (double v)
    {
        for (size_t i=0; i<N*N; ++i) { m_buffer[i] = v; }
        return *this;
    }

    Block<N> & operator+= (Block<N> const & other)
    {
        for (size_t i=0; i<N*N; ++i) { m_buffer[i] += other.m_buffer[i]; }
        return *this;
    }

    void save(Matrix & mat, size_t it, size_t jt);

    double m_buffer[N * N];
};

template<size_t N>
struct Tiler
{
    static constexpr const size_t NDIM = N;

    void load(
        Matrix const & mat1, size_t it1, size_t jt1
      , Matrix const & mat2, size_t it2, size_t jt2
    );

    void multiply();

    Block<N> m_mat1;    // row-major
    Block<N> m_mat2;    // column-major
    Block<N> m_ret;     // row-major
};


void multibility(Matrix const & mat1, Matrix const & mat2);
Matrix multiply_naive(Matrix const & mat1, Matrix & mat2);
Matrix multiply_tile(Matrix const & mat1, Matrix const & mat2, size_t blocksize);
Matrix multiply_mkl(Matrix & mat1, Matrix & mat2);



#endif  //MATRIX_HPP