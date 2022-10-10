#include <pybind11/pybind11.h>
#include <vector>
#include <bits/stdc++.h>
#include <pybind11/stl.h>
#include <stdexcept>
using namespace std;
double angle(vector<double> v1, vector<double>v2){
  double len1 = sqrt(v1[0]*v1[0] + v1[1]*v1[1]);
  double len2 = sqrt(v2[0]*v2[0] + v2[1]*v2[1]);
  if(v1[0]==0&&v1[1]==0)
    throw std::invalid_argument( "received zero-length vector" );
  if(v2[0]==0&&v2[1]==0)
    throw std::invalid_argument( "received zero-length vector" );
    
  return acos((v1[0]*v2[0] + v1[1] * v2[1]) / (len1*len2));
}


PYBIND11_MODULE(_vector, m) {
  m.doc() = "an angle caculator for two vectors"; 
  m.def("angle", &angle, "an angle caculator for two vectors");
}
