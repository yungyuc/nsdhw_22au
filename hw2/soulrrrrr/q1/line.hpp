#pragma once
#include <cstddef>
#include <vector>

class Line {
public:
    // Basic constructors
    Line(); // default constructor
    Line(Line const & ); // copy constructor
    Line(Line       &&); // move constructor
    Line & operator=(Line const & ); // copy assignment operator
    Line & operator=(Line       &&); // move assignment operator
    Line(size_t size);
    ~Line(); // destructor
    size_t size() const;
    float const & x(size_t it) const; // getter
    float & x(size_t it); // setter
    float const & y(size_t it) const; // getter
    float & y(size_t it); //setter
private:
    size_t m_size;
    std::vector<float> m_x;
    std::vector<float> m_y;
    void check_range(size_t it) const;
};