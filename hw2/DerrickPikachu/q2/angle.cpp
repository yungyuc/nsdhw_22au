#include <cmath>

#include "angle.h"

namespace dimension2
{

float dot(const std::vector<float>& vec1, const std::vector<float>& vec2)
{
    return vec1[0] * vec2[0] + vec1[1] * vec2[1];
}

float len_of(const std::vector<float>& vec)
{
    return std::sqrt(vec[0] * vec[0] + vec[1] * vec[1]);
}

float angle_between(const std::vector<float>& vec1, const std::vector<float>& vec2)
{
    float dot_product = dot(vec1, vec2);
    float angle = std::acos(dot_product / (len_of(vec1) * len_of(vec2)));
    return angle * 180 / PI;
}

}