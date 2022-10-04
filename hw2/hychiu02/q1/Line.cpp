#include<iostream>
#include "Line.hpp"

Line::Line()
{

}

Line::Line(Line const & line_)
{
    coordinate = line_.coordinate;
}

Line::Line(Line      &&line_)
{
    coordinate = line_.coordinate;
}

Line & Line::operator=(Line const & line_)
{
    *this = line_;

    return *this;
}

Line & Line::operator=(Line      &&line_)
{
    coordinate, line_.coordinate;

    return *this;
}

Line::Line(size_t size)
{
    coordinate.resize(size);
}

size_t Line::size() const
{
    return coordinate.size();
}

Line::~Line()
{
    coordinate = std::vector<std::pair<float,float>>();
}

float const & Line::x(size_t it) const
{
    return coordinate[it].first;
}

float & Line::x(size_t it)
{
    return coordinate[it].first;
}

float const & Line::y(size_t it) const
{
    return coordinate[it].second;
}

float & Line::y(size_t it)
{
    return coordinate[it].second;
}

void Line:: check_range(size_t it) const
{
    if(it < this->size()){
        throw std::out_of_range("Line index out of range");
    }
}