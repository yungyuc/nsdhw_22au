#ifndef LINE_HPP
#define LINE_HPP
#include <cstddef>
#include <vector>

class Line
{
public:
    Line();
    Line(Line const & );                // copy constructor
    Line(Line       &&);                // move constructor
    Line & operator=(Line const & );    // copy assignment operator
    Line & operator=(Line       &&);    // move assignment operator
    Line(size_t size);                  // custom constructor
    ~Line();

    size_t size() const;

    float const & x(size_t it) const;   // Primer p275-277
    float & x(size_t it);
    float const & y(size_t it) const;
    float & y(size_t it);

private:
    std::vector<std::pair<float,float>> m_coord;
};  /* End of class Line */

#endif  //LINE_HPP