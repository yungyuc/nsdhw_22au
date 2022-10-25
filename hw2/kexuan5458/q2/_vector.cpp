#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <stdexcept>
#include <cmath>
#include <vector>
using namespace std;
namespace py = pybind11;

float calculateAngle (vector<float> arr1, vector<float> arr2) {
  float magnitude1 = sqrt(arr1[0] * arr1[0] + arr1[1] * arr1[1]);
  float magnitude2 = sqrt(arr2[0] * arr2[0] + arr2[1] * arr2[1]);
  if ((magnitude1 == 0) || (magnitude2 == 0))
    throw domain_error("vector length is 0");

  float dotVectors = (arr1[0] * arr2[0]) + (arr1[1] * arr2[1]);
  float len = sqrt(arr1[0]*arr1[0] + arr1[1]*arr1[1]) * sqrt(arr2[0]*arr2[0] + arr2[1]*arr2[1]);
  //float angle = acos(dotVectors/ (magnitude1 * magnitude2));
  double angle = acos(dotVectors/len);
  return angle;
}


PYBIND11_MODULE(_vector, m) {
  m.doc() = "Use pybind11 to wrap C++ to Python";      // module doc string
  m.def("calculateAngle",                   // function name
  &calculateAngle,                          // function pointer
  "A function which calculates angle");     // function doc string
}
