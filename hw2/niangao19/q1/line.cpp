#include <iostream>
#include <vector>
class Line
{
public:
    Line();
    Line(Line const & line) : m_x(line.m_x),m_y(line.m_y){}
    Line(Line && line) {
        m_x = std::move( line.m_x );
        m_y = std::move( line.m_y );
    }
    
    Line & operator=(Line const & );
    Line & operator=(Line       &&);
    Line(size_t size) {
        m_x.resize(size, 0);
        m_y.resize(size,0);
    };
    ~Line()  {
        m_x.clear();
        m_y.clear();
    };
    size_t size() const {return m_x.size(); }
    //float const & x(size_t it) const{return m_x[it]};
    float & x(size_t it){return m_x[it];}
    //float const & y(size_t it) const{ return m_y[it] };
    float & y(size_t it){return m_y[it];}
private:
    // Member data.
    std::vector<float> m_x;
    std::vector<float> m_y;
}; /* end class Line */

int main(int, char **)
{
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
