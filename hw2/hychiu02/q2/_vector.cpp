#include <iostream>
#include <cmath>

#include <pybind11/pybind11.h>

#define PI 3.14159265358979323846

float calculate_angle(std::pair<float, float> v0, std::pair<float, float> v1)
{
    /* 
        a ∙ b = |a| * |b| * cos(α)
        cos(α) = a ∙ b / (|a| * |b|)
        α = arccos[(a ∙ b) / (|a| * |b|)]
    */
    float inner_prod, len_a, len_b, val;
    
    inner_prod = (v0.first * v1.first) + (v0.second * v1.second);
    len_a = std::hypot(v0.first, v0.second);
    len_b = std::hypot(v1.first, v1.second);

    if(len_a <= 0.0 || len_b <=0.0)
        return std::nanf("");

    val = inner_prod/(len_a * len_b);

    if(val <= -1.0)
        return PI;
    else if(val >= 1.0)
        return 0.0;
    else
        return acos(val);
}

PYBIND11_MODULE(_vector, m) {
    m.doc() = "Calculate angle(radians) between two vectors";

    m.def("calculate_angle", &calculate_angle, "A function calculate angle(radians) between two vectors");
}

