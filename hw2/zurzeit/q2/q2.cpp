#include <pybind11/pybind11.h>

namespace py = pybind11;

int func1(int a, int b){
    if(a >= b)
        return a;
    return b;
}



PYBIND11_MODULE(_vector, handle){
    handle.doc() = "This is doc.";
    handle.def("angle_between", &func1);
}