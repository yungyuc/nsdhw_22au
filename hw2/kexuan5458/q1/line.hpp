#ifndef LINE_HPP
#define LINE_HPP
#include <cstdint>
#include <cstddef>
#include <iostream>
#include <vector>
class Line
{
public:
    // Basic constructors.
    Line(); // default constructor.
    Line(Line const & ); // copy constructor.
    Line(Line       &&); // move constructor.
    Line & operator=(Line const & ); // copy assignment operator.
    Line & operator=(Line       &&); // move assignment operator.
    
    // Custom constructor.
    // Line(size_t size) : m_size(size) {}
    Line(size_t size);
    // Desctructor.
    // ~Line() { if (m_coord.size() != 0) { delete[] m_coord; } }
    ~Line();
    // Accessors.
    size_t size() const { return m_size; }
    float const& x (size_t it) const;
    float& x(size_t it);
    float const& y(size_t it) const;
    float& y(size_t it);
private:
    // Private helper.
    void check_range(size_t it) const
    { if (it >= m_size) { throw std::out_of_range("Line index out of range"); } }
    // Member data.
    size_t m_size; // number of points.
    // float * m_coord = nullptr; // memory buffer for the points.
    std::vector<float> x_val, y_val; // memory buffer for the points.
}; /* end class Line */

#endif