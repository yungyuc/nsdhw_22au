#include <cstdint>
#include <stdexcept>
#include <vector>
#include <iostream>

class Line
{
private:
	size_t m_size;
	
	std::vector<float> x_size;
	std::vector<float> y_size;

public:
	Line() = default;
	Line(Line const & ) = default;
	Line(Line &&) = default;
	Line & operator =(Line const & ) = default;
	Line & operator =(Line &&) = default;
	
	Line(size_t size)
		:m_size(size)
		,x_size(size)
		,y_size(size) {};
	
	
	~Line() = default;
	
	size_t size() const {return m_size;}
	float& x(size_t it);
	float& y(size_t it);
	float const& x(size_t it) const {return x_size[it];}
	float& x(size_t it) const {return x_size[it];}
	float const& y(size_t it) const {return y_size[it];}
	float& y(size_t it) const {return y_size[it];}
}