#include <pybind11/pybind11.h>
#include <cmath>

class Coord{
    public:
        Coord(float x, float y) : _x(x), _y(y){
        }
        float dot(Coord &v){
            return _x * v._x + _y * v._y;
        }
        float norm(Coord &v){
            return std::sqrt(dot(v));
        }
    private:
        float _x, _y;
};
float angle(Coord u, Coord v){
    return std::acos(std::min((float)1.0, std::max((float)-1.0,
            u.dot(v) / (u.norm(u) * v.norm(v)))));
}

PYBIND11_MODULE(_vector, m) {
    pybind11::class_<Coord>(m, "Coord").def(pybind11::init<float, float>()).def("norm", &Coord::norm).def("dot", &Coord::dot);
    m.def("angle", &angle);

}

