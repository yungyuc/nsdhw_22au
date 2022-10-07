#include "line.h"
#include <algorithm>

Line::Line()
{
    p_size = 0;
    p_x.resize(p_size);
    p_y.resize(p_size);
}

// copy constructor
Line::Line(Line const &other)
{
    p_x = other.p_x;
    p_y = other.p_y;
}

// move constructor
Line::Line(Line &&other)
{
    std::swap(*this, other);
}

// copy constructor assignment
Line &Line::operator=(Line const &other)
{
    p_x = other.p_x;
    p_y = other.p_y;
    return *this;
}

// move constructor assignment
Line &Line::operator=(Line &&other)
{
    *this = other;
    return *this;
}

Line::Line(size_t size)
{
    p_size = size;
    p_x.resize(p_size);
    p_y.resize(p_size);
}

Line::~Line()
{
    p_x.clear();
    p_y.clear();
}

size_t Line::size() const
{
    return p_size;
}

float const &Line::x(size_t it) const { return p_x.at(it); }
float &Line::x(size_t it) { return p_x.at(it); }
float const &Line::y(size_t it) const { return p_y.at(it); }
float &Line::y(size_t it) { return p_y.at(it); }
