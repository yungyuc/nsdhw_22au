# include <iostream>
# include <vector>
using namespace std;

class Line
{
public:

    Line(); // =default
    Line(Line const & ); // copy constructor
    Line(Line       &&); // move constructor 
    Line & operator=(Line const & ); // copy operator 
    Line & operator=(Line       &&); // move operator 
    Line(size_t size);
    
    ~Line();

    size_t size() const;
    float const & x(size_t it) const;
    float & x(size_t it);
    float const & y(size_t it) const;
    float & y(size_t it);
private:
    // Member data.
    vector<float> p_x; 
    vector<float> p_y;
    size_t p_s;

}; /* end class Line */