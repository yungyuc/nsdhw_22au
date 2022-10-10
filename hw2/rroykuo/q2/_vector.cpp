# include <iostream>
# include <cmath>
# include <math.h>
# include <vector>
# include <pybind11/pybind11.h>
# include <pybind11/stl.h>


using namespace std;
namespace py = pybind11;

float angle (vector<float> array_1, vector<float> array_2){

  float tmp_1 = array_1[0] * array_2[0] + array_1[1] * array_2[1];
  float tmp_2 = sqrt(pow(array_1[0], 2) + pow(array_1[1], 2)) * sqrt(pow(array_2[0], 2) + pow(array_2[1], 2));
  if (tmp_2 == 0){
    throw "Vector is zero length!";
    exit (0);
  }
  return acos(tmp_1/tmp_2);
}

PYBIND11_MODULE(_vector, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring
    m.def("angle", &angle, "A function which calculate the angle of two vector");

}