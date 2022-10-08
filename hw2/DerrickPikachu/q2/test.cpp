#include <iostream>
#include <vector>

#include "angle.h"

#define PI 3.14159265

int main(int argc, char** argv)
{
    std::vector<float> vec1(2);
    std::vector<float> vec2(2);
    vec1[0] = 1; vec1[1] = 0;
    vec2[0] = 1; vec2[1] = 1;
    float radian = 180 / PI;
    std::cout << dimension2::angle_between(vec1, vec2) * radian << std::endl;
    // std::cout << std::acos(0) * 180 / PI << std::endl;
    return 0;
}