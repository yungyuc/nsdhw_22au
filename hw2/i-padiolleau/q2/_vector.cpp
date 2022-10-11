//#include ..//home/i-padiolleau/.local/lib/python3.8/site-packages/pybind11/include/pybind11/pybind11.h>

#include <pybind11/pybind11.h>
#include <iostream>
#include <cstdint>
#include <stdexcept>
#include <vector>

namespace py = pybind11;

float comp_angle(std::vector<float> v1 , std::vector<float> v2){
    float angle = 0;
    return angle ;
}

int main(){
    std::cout <<"Calcul en cours ..\n";
    printf("angle : %f\n",comp_angle({-3.4,4.5},{-6.7,8.9}));
}

PYBIND11_MODULE(q2_file, m) {
    m.doc() = "pybind11";
    m.def("comp_angle", &comp_angle, "Fonction computing the angle between two vectors");
}