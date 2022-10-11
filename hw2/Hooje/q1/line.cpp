#include "line.hpp"


Line::Line(Line const & line ) = default ;
Line::Line(Line       && line) = default ;
Line & Line::operator=(Line const & line) 
{
    this->x_value = line.x_value;
    this->y_value = line.y_value;
    return *this;
}
Line & Line::operator=(Line       && line) 
{
    this->x_value = line.x_value;
    this->y_value = line.y_value;
    return *this;
}
Line::Line(size_t size)
{
    this->x_value.resize(size);
    this->y_value.resize(size);
}

size_t Line::size() const
{
    return this->x_value.size();
}


float const & Line::x(size_t it) const
{
    return this->x_value[it];
}

float & Line::x(size_t it)
{
    return this->x_value[it];
}

float const & Line::y(size_t it) const
{
    return this->y_value[it];
}

float & Line::y(size_t it)
{
    return this->y_value[it];
}

