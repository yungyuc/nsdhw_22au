#include "Line.hpp"

Line::Line(size_t size) : mPoints(size)
{

}

const float& Line::x(size_t it) const
{
    return mPoints[it].x;
}

float& Line::x(size_t it)
{
    return mPoints[it].x;
}

const float& Line::y(size_t it) const
{
    return mPoints[it].y;
}

float& Line::y(size_t it)
{
    return mPoints[it].y;
}
