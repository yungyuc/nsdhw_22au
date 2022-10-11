#include <cstdint>
#include <stdexcept>
#include <vector>
#include <iostream>

class Line
{
private:
	vector<float> _x, _y;

public:
	Line() = default;
	Line(Line const & ) = default;
	Line(Line &&) = default;
	Line & operator =(Line const & ) = default;
	Line & operator =(Line &&) = default;
	
    Line(size_t size) {
        _x.resize(size);
        _y.resize(size);
    }
	
	
	~Line() = default;
	
	size_t size() const {return _x.size();}
	float const& x(size_t it) const {return _x[it];}
	float& x(size_t it) {return _x[it];}
	float const& y(size_t it) const {return _y[it];}
	float& y(size_t it) const {return _y[it];}
};