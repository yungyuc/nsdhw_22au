#include "Line.hpp"
/* Define the copy constructor */
Line::Line(Line const & other)
{
    m_data = other.m_data;
    m_size = other.m_size;
}

/* Define the move constructor */
Line::Line(Line && other)
{
    std::swap(other.m_size, m_size);
    std::swap(other.m_data, m_data);
}

/* Define the copy assignment operator */
Line & Line::operator=(Line const & other)
{
    if (this == &other) { return *this; } // don't move to self.
    // This part is the same as what's in the copy constructor.
    m_data = other.m_data;
    m_size = other.m_size;
    return *this;
}

/* Define the move assignment operator */
Line & Line::operator=(Line && other)
{
    if (this == &other) { return *this; } // don't move to self.
    std::swap(other.m_size, m_size);
    std::swap(other.m_data, m_data);
    return *this;
}
