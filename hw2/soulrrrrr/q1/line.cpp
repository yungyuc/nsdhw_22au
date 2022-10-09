#include "line.hpp"
#include <stdexcept>

Line::Line() {
    m_size = 0;
}

Line::Line(size_t size) {
    m_size = size;
    m_x.resize(size, 0);
    m_y.resize(size, 0);
}

Line::Line(Line const & line) = default;

Line::Line(Line && line) = default;

Line & Line::operator=(Line const & line) = default;

Line & Line::operator=(Line && line) = default;

size_t Line::size() const {
    return m_size;
}

Line::~Line() = default;

float const & Line::x(size_t it) const {
    check_range(it);
    return m_x[it];
} // getter

float & Line::x(size_t it) {
    check_range(it);
    return m_x[it];
} // setter

float const & Line::y(size_t it) const {
    check_range(it);
    return m_y[it];
} // getter

float & Line::y(size_t it) {
    check_range(it);
    return m_y[it];
} // setter

void Line::check_range(size_t it) const {
    if (it >= m_size) {
        throw std::out_of_range("Vector index out of range");
    }
}