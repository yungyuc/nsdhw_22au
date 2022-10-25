#include <cstddef>
#include <vector>
using namespace std;
class Line{
private:
    // Member data.
    vector<pair<float, float>> my_points;
    size_t point_size;
public:
    Line();                 //
    Line(Line const & ref);    // Line const & a => a is read-only
    Line(Line       && ref);    //
    Line & operator=(Line const & ref); //
    Line & operator=(Line       && ref); //
    Line(size_t size); //
    ~Line(); //
    size_t size() const; // all member in this function is read-only
    float const & x(size_t it) const; // return value
    float & x(size_t it);             // set value
    float const & y(size_t it) const; //
    float & y(size_t it);             //


}; /* end class Line */
