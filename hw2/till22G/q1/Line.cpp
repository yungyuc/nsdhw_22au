#include<iostream>
#include<vector>

class Line
{
public:
    Line() {};
    Line(Line const& other){
        m_x = other.m_x;
        m_y = other.m_y;
    };



    Line(size_t size) {
        for (int i = 0; i < size; i++) {
            m_x.push_back(0);
            m_y.push_back(0);
        }
    };
    ~Line(){};
    size_t size() const { return m_x.size(); };
    float const& x(size_t it) const { return m_x[it]; };
    float& x(size_t it) { return m_x[it];};
    float const& y(size_t it) const { return m_y[it]; };
    float& y(size_t it) { return m_y[it]; };
private:
    std::vector<float> m_x;
    std::vector<float> m_y;
   
};


int main(int, char**)
{
    Line line(3);
    line.x(0) = 0; line.y(0) = 1;
    line.x(1) = 1; line.y(1) = 3;
    line.x(2) = 2; line.y(2) = 5;

    Line line2(line);
    line2.x(0) = 9;

    std::cout << "line: number of points = " << line.size() << std::endl;
    for (size_t it = 0; it < line.size(); ++it)
    {
        std::cout << "point " << it << ":"
            << " x = " << line.x(it)
            << " y = " << line.y(it) << std::endl;
    }

    std::cout << "line2: number of points = " << line.size() << std::endl;
    for (size_t it = 0; it < line.size(); ++it)
    {
        std::cout << "point " << it << ":"
            << " x = " << line2.x(it)
            << " y = " << line2.y(it) << std::endl;
    }

    return 0;
}
