#include <cmath>
#include <iostream>
#include <math.h>
#include <pybind11/pybind11.h>

float calcAngle(const float x1, const float y1, const float x2,
                const float y2) {
  float dotProduct = x1 * x2 + y1 * y2;
  float len_of_1 = sqrt(x1 * x1 + y1 * y1);
  float len_of_2 = sqrt(x2 * x2 + y2 * y2);
  if (len_of_1 == 0 || len_of_2 == 0) {
    return -1;
  };

  float cos = dotProduct/(len_of_1 * len_of_2);
  if (cos > 1){
    cos = 1;
  } else if (cos < -1){
    cos = -1;
  }
}
