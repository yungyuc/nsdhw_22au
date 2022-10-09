#include <pybind11/pybind11.h>
#include <math.h>

namespace py = pybind11;

class Vec{
private:
    float X1, Y1, X2, Y2;
public:
    Vec(float x1, float y1, float x2, float y2){
        X1 = x1;
        Y1 = y1;
        X2 = x2;
        Y2 = y2;
    }
    float angle_in_radian(){
        float dot = X1*X2 + Y1*Y2;
        float det = X1*Y2 - Y1*X2;
        float angle = atan2(det, dot);
        return angle;
    }
};


PYBIND11_MODULE(_vector, handle) {
    handle.doc() = "This is doc.";
    py::class_<Vec>(
        handle, "pyVec"
    )
    .def(py::init<float,float,float,float>())
    .def("angle", &Vec::angle_in_radian)
    ;
}