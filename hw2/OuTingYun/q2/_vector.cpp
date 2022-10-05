#include <iostream>
#include <stdexcept>
#include <math.h>
#include <pybind11/pybind11.h>


double angle(std::pair<double,double> v1,std::pair<double,double> v2){
    double len_v1 = sqrt(v1.first*v1.first+v1.second*v1.second);
    double len_v2 = sqrt(v2.first*v2.first+v2.second*v2.second);
    double cos = (v1.first*v2.first + v1.second*v2.second) / (len_v1 * len_v2);
    if (len_v1==0||len_v2==0){
        return -1;
    }
    return acos(cos);
}

PYBIND11_MODULE(_vector,m){
    m.doc() = "calculate angle";
    m.def("angle",&angle,"calculate angle");

}