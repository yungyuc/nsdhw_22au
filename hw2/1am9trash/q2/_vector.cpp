#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>
#include <assert.h>
#include <stdexcept>
using namespace std;

float cal_angle(vector<float> v1, vector<float> v2) {
    if (v1.size() != 2 || v2.size() != 2)
        throw invalid_argument("not 2-dim vector");
    if (v1[0] == 0 && v1[1] == 0)
        throw invalid_argument("0-length vector");
    if (v2[0] == 0 && v2[1] == 0)
        throw invalid_argument("0-length vector");

    float len1 = sqrt(v1[0] * v1[0] + v1[1] * v1[1]);
    float len2 = sqrt(v2[0] * v2[0] + v2[1] * v2[1]);
    float dot = v1[0] * v2[0] + v1[1] * v2[1];
    return acos(dot / (len1 * len2));
}

PYBIND11_MODULE(_vector, m) {
    m.doc() = "cal_angle()"; 
    m.def("cal_angle", &cal_angle, "caculator for 2 vectors' angle");
}
