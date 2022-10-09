#include <cmath>
#include "_vector.hpp"

Vector::Vector(double dx, double dy) : _dx(dx), _dy(dy) {};

double get_angle(Vector const& v1, Vector const& v2) {
    return acos((v1._dx * v2._dx + v1._dy * v2._dy) / (sqrt(v1._dx * v1._dx + v1._dy * v1._dy) * sqrt(v2._dx * v2._dx + v2._dy * v2._dy)));
}