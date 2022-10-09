#include "line.hpp"
#include <stdlib.h>

Line::Line(){}

Line::Line(Line const & origLine)
{
    m_coord = origLine.m_coord;
}

Line::Line(Line && origLine)
{
    m_coord = std::move(origLine.m_coord);
}

Line & Line::operator=(Line const & origLine)
{
    this->m_coord = origLine.m_coord;
    return *this;
}

Line & Line::operator=(Line && origLine)
{
    this->m_coord = std::move(origLine.m_coord);
    return *this;
}

Line::Line(size_t size)
{
    m_coord.resize(size);
}

Line::~Line()
{
    m_coord.clear();
}

size_t Line::size() const
{
    return m_coord.size();
}

float const & Line::x(size_t it) const
{
    return m_coord.at(it).first;
}

float & Line::x(size_t it)
{
    return m_coord.at(it).first;
}

float const & Line::y(size_t it) const
{
    return m_coord.at(it).second;
}

float & Line::y(size_t it)
{
    return m_coord.at(it).second;
}