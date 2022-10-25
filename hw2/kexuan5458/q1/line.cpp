#include <cstddef>
#include <iostream>
#include <vector>
#include "line.hpp"

Line::Line() = default;
Line::Line(Line const&) = default;
Line::Line(Line&&) = default;

Line::~Line() = default;
/* Define the copy constructor */
/* Define the move constructor */
/* Define the copy assignment operator */
/* Define the move assignment operator */
Line& Line::operator=(Line const&) = default;
Line& Line::operator=(Line&&) = default;
// ????????????????

// ????????????????
Line::Line(size_t size):
	m_size(size),
	x_val(size),
	y_val(size) {};

float const& Line::x(size_t it) const
{
    return this->x_val[it];
}
float const& Line::y(size_t it) const
{
    return this->y_val[it];
}

float& Line::x(size_t it)
{
    return this->x_val[it];
}
float& Line::y(size_t it)
{
    return this->y_val[it];
}