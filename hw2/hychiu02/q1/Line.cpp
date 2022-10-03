#include<iostream>
#include "Line.hpp"

Line::Line()
{

}

Line::Line(Line const & l)
{
    coordinate = l.coordinate;
}

Line::Line(Line      &&l)
{
    coordinate = l.coordinate;
}

Line & Line::operator=(Line const & line)
{
    *this = line;

    return *this;
}

Line & Line::operator=(Line      &&line)
{
    return line;
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