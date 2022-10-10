#include <cstdint>
#include <algorithm>
#include <utility>
#include <iostream>
#include <vector>

class Line
{
public:
    // Basic constructors.
    Line() = default; // default constructor.
    Line(Line const & ) = default;
    Line(Line       &&) = default;
    Line & operator=(Line const & ) = default;
    Line & operator=(Line       &&) = default;
    // Custom constructor.
    Line(size_t size){
      _X.resize(size);
      _Y.resize(size);
    }
    // Desctructor.
    ~Line() = default;
    // Accessors.
    size_t size() const { return _X.size(); }
    const float & x(size_t it) const { check_range(it); return _X.at(it); }
    float & x(size_t it)       { check_range(it); return _X.at(it); }
    const float & y(size_t it) const { check_range(it); return _Y.at(it); }
    float & y(size_t it)       { check_range(it); return _Y.at(it); }
private:
    // Private helper.
    void check_range(size_t it) const
    { if (it >= _X.size()) { throw std::out_of_range("Line index out of range"); } }
    // Member data.

    std::vector<float>_X;
    std::vector<float>_Y;
}; /* end class Line */


int main(){

    Line line(3);
    line.x(0) = 0; line.y(0) = 1;
    line.x(1) = 1; line.y(1) = 3;
    line.x(2) = 2; line.y(2) = 5;

    Line line2(line);
    line2.x(0) = 9;

    std::cout << "line: number of points = " << line.size() << std::endl;
    for (size_t it=0; it<line.size(); ++it)
    {
        std::cout << "point " << it << ":"
                  << " x = " << line.x(it)
                  << " y = " << line.y(it) << std::endl;
    }

    std::cout << "line2: number of points = " << line.size() << std::endl;
    for (size_t it=0; it<line.size(); ++it)
    {
        std::cout << "point " << it << ":"
                  << " x = " << line2.x(it)
                  << " y = " << line2.y(it) << std::endl;
    }

  return 0;
}
