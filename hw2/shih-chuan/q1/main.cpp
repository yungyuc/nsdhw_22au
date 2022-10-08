#include <cstdlib>
#include <vector>
#include <iostream>
#include <cassert>

class Line
{
public:
    Line(size_t size): mPoints(size, {0, 0}) {}

    size_t size() const
    {
        return mPoints.size();
    }

    float const & x(size_t it) const
    {
        assert(mPoints.size() > it && mPoints[it].size() > 0);
        return mPoints[it][0];
    }

    float & x(size_t it)
    {
        assert(mPoints.size() > it && mPoints[it].size() > 0);
        return mPoints[it][0];
    }

    float const & y(size_t it) const
    {
        assert(mPoints.size() > it && mPoints[it].size() > 1);
        return mPoints[it][1];
    }

    float & y(size_t it)
    {
        assert(mPoints.size() > it && mPoints[it].size() > 1);
        return mPoints[it][1];
    }
private:
    std::vector<std::vector<float>> mPoints;
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