#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>
#include <stdexcept>
#include <cmath>


float get_angle(std::vector<float> v1, std::vector<float>v2){
    // vector is zero
    if((v1[0] == 0.0 && v1[1] == 0.0) || (v2[0] == 0.0 && v2[1] == 0.0))
        throw std::domain_error("input vector length is zero!");
    
    float dot = v1[0]*v2[0] + v1[1]*v2[1];
    float len = sqrt(v1[0]*v1[0] + v1[1]*v1[1]) * sqrt(v2[0]*v2[0] + v2[1]*v2[1]);
    float angle = acos(dot/len);

    return angle;
}

PYBIND11_MODULE(_vector, m) {
    m.doc() = "get angle of two vectors";                          // module doc string
    m.def("get_angle",                                             // function name
            &get_angle,                                                // function pointer
            "A function which calculates the angle of two vectors" //function doc string
        );
}
