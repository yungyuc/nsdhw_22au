#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>

class Line
{
    public:
        Line() = default;
        Line(size_t it): vec_x(std::vector<float> (it, 0)), vec_y(std::vector<float> (it, 0)) {}
        float & x(size_t it) { return vec_x[it]; }
        float & y(size_t it) { return vec_y[it]; }
        size_t size() {return vec_x.size();}
    private:
        std::vector<float> vec_x, vec_y;
};

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