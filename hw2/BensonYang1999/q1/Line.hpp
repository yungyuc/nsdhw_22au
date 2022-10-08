#include <cstdint>
#include <algorithm>
#include <utility>
#include <stdexcept>
#include <vector>

class Line
{
public:
    // Basic constructors.
    Line() = default; // default constructor.
    Line(Line const & ); // copy constructor.
    Line(Line       &&); // move constructor.
    Line & operator=(Line const & ); // copy assignment operator.
    Line & operator=(Line       &&); // move assignment operator.
    // Custom constructor.
    Line(size_t size) : m_size(size), m_data(size) {}
    // Desctructor.
    ~Line() = default;
    // Accessors.
    size_t size() const { return m_size; }
    float const & x(size_t it) const { check_range(it); return m_data[it].first; }
    float & x(size_t it)       { check_range(it); return m_data[it].first; }
    float const & y(size_t it) const { check_range(it); return m_data[it].second; }
    float & y(size_t it)       { check_range(it); return m_data[it].second; }
private:
    // Private helper.
    void check_range(size_t it) const
    { if (it >= m_size) { throw std::out_of_range("Line index out of range"); } }
    // Member data.
    size_t m_size = 0; // number of points.
    std::vector<std::pair<float, float>> m_data; // data structure for saving points
}; /* end class Line */
