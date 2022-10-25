#include "line.hpp"
#include <cstddef>
#include <vector>

Line::Line() = default;

Line::Line(Line const&) = default;
Line::Line(Line&&) = default;

Line& Line::operator=(Line const&) = default;
Line& Line::operator=(Line&&) = default;

Line::Line(size_t size)
    : m_size(size)
    , m_xs(size)
    , m_ys(size) {};

Line::~Line() = default;

size_t Line::size() const {
    return m_size;
};

float const& Line::x(size_t it) const {
    return m_xs[it];
};
float& Line::x(size_t it) {
    return m_xs[it];
};

float const& Line::y(size_t it) const {
    return m_ys[it];
};
float& Line::y(size_t it) {
    return m_ys[it];
};