#include <pybind11/pybind11.h>
#include <iostream>
#include <cmath>
 
float calAngle(std::pair<float,float> vec1, std::pair<float,float> vec2)
{
    float vec1Len = vec1.first * vec1.first + vec1.second * vec1.second;
    float vec2Len = vec2.first * vec2.first + vec2.second * vec2.second;
    if(vec1Len == 0 || vec2Len == 0)
    {
        throw std::invalid_argument
        ("The length of vector must greater than 0.");
        exit(0);
    }

    float innerProduct = vec1.first * vec2.first
                       + vec1.second * vec2.second;
    float lenProduct = vec1Len * vec2Len;

    return acos(innerProduct / sqrt(lenProduct));
}

PYBIND11_MODULE(_vector, m)
{
    m.doc() = "Use pybind11 to wrap the function that calculates the angle (in radians) between two vectors in the 2-dimensional Cartesian coordinate system.";
    m.def("calAngle", &calAngle, "Calculates the angle between two vectors"); 
}
