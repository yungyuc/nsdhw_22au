#include<pybind11/pybind11.h>
#include<iostream>
#include<math.h>

using namespace std;

float angle(const pair<float, float> v1, const pair<float, float> v2){
    
    float l1 = sqrt(v1.first * v1.first + v1.second * v1.second);
    float l2 = sqrt(v2.first * v2.first + v2.second * v2.second);
    float dot = v1.first * v2.first + v1.second * v2.second;

    if(l1 == 0.0 || l2 == 0.0)
        throw runtime_error("there is zero vector");
    if((v1.first == v2.first) && (v1.second == v2.second))
        return 0.0f;
    float ang = acos(dot/(l1*l2));

    return ang;
}

PYBIND11_MODULE(_vector, m){
    m.doc() = "angle calculate";
    m.def("angle", &angle, "calculate angle of two vector");
}

