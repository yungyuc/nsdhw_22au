#pragma once

#include <cstdlib>
#include <vector>

class Line
{
public:
    Line(size_t size = 0);
    ~Line() = default;
    Line(const Line &) = default;
    Line(Line &&) = default;
    Line& operator=(const Line &) = default;
    Line& operator=(Line &&) = default;

    const float& x(size_t it) const;
    float& x(size_t it);
    const float& y(size_t it) const;
    float& y(size_t it);

    inline size_t size() const
    {
        return mPoints.size();
    };

private:
    struct Point
    {
        float x = 0.0f;
        float y = 0.0f;
    };

private:
    std::vector<Point> mPoints;
};
