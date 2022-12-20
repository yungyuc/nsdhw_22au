#include <cstddef>
#include <pybind11/numpy.h>

class Matrix {
public:
    Matrix();
    Matrix(int nrow, int ncol);
    Matrix(Matrix const &other);
    ~Matrix();
    inline double operator()(int nrow, int ncol) const;
    inline double &operator()(int nrow, int ncol);
    bool operator==(Matrix const &other) const;
    bool operator!=(Matrix const &other) const;
    double const *get_buffer() const;
    double *get_buffer();
    inline int const &get_nrow() const;
    inline int const &get_ncol() const;
    void display_matrix() const;
    pybind11::array_t<double> array();

private:
    int m_nrow = 0;
    int m_ncol = 0;
    double *m = nullptr;
    void reset_buffer(int nrow, int ncol);
    void check_inbound(int nrow, int ncol) const;
};

void check_multipliable(Matrix const &l_m, Matrix const &r_m);
Matrix multiply_naive(Matrix const &l_m, Matrix const &r_m);
void multiply_block(Matrix &m, Matrix const &l_m, Matrix const &r_m, int row, int col, int index, int tile_size);
Matrix multiply_tile(Matrix const &l_m, Matrix const &r_m, int tile_size);
Matrix multiply_mkl(Matrix const &l_m, Matrix const &r_m);
