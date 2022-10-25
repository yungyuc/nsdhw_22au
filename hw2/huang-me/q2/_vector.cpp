#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <cmath>
namespace py = pybind11;

float GetAngle(const std::vector<float> v1, const std::vector<float> v2)
{
    float inner_product = v1[0] * v2[0] + v1[1] * v2[1];
    float v1_length = v1[0] * v1[0] + v1[1] * v1[1];
    float v2_length = v2[0] * v2[0] + v2[1] * v2[1];

    if (v1_length == 0 || v2_length == 0)
        return -1;
    float cos=inner_product / (sqrt(v1_length)* sqrt(v2_length) );
    if(cos>1){
        cos=1;
    }
    else if(cos<-1){
        cos=-1;
    }
    return acos(cos) ;
}


PYBIND11_MODULE(_vector, m)
{
    m.def("GetAngle", &GetAngle, "A function get angle");
}
