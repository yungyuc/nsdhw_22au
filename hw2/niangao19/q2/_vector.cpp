#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <cmath>
#include<iostream>
#include <vector>
#include <stdexcept>

namespace py = pybind11;

float calculates_angle( std::vector<float>  v1,  std::vector<float>  v2)  {
    // calculate vector len
    float v1len = sqrt(pow(abs(v1[0]), 2) + pow(abs(v1[1]), 2));
    float v2len = sqrt(pow(abs(v2[0]), 2) + pow(abs(v2[1]), 2));
    
    // v1xv2 = v1len*v2len*cos(angle)
    float exlen = v1[0]*v2[0] + v1[1]*v2[1];
    float cosa = v1len * v2len / exlen;
    double angle = acos( cosa );
    return angle;
}


PYBIND11_MODULE(_vector, m) {
  m.doc() = "calculates_angle";      // module doc string
  m.def("calculates_angle",                              // function name
        &calculates_angle,                               // function pointer
        "A function which calculates angle between two vectors" //function doc string
       );
}
