#include <pybind11/pybind11.h>
#include <sstream>

namespace py = pybind11;

class Vector {
public:
  double x;
  double y;

  Vector(double x, double y): x(x), y(y) {}

  static Vector create(double x, double y) {
    return Vector(x, y);
  }

  std::string to_string() const {
    std::stringstream ss;
    ss << "<Vector x=" << x << " y=" << y << ">";
    return ss.str();
  }

  double length() const {
    return hypot(x, y);
  }

  double dot(const Vector& rhs) const {
    return x * rhs.x + y * rhs.y;
  }

  double angle_between(const Vector& rhs) const {
    double lhs_length = this->length();
    double rhs_length = rhs.length();
    double epsilon = std::numeric_limits<double>::epsilon();

    if (lhs_length < epsilon || rhs_length < epsilon) {
      throw py::value_error("Zero vector found");
    }

    return fabs(acos(this->dot(rhs) / (lhs_length * rhs_length)));
  }

private:
  Vector() = delete;
};

PYBIND11_MODULE(_vector, m) {
  m.doc() = "Hello, world!";

  py::class_<Vector>(m, "Vector")
    .def(py::init(&Vector::create))
    .def_readwrite("x", &Vector::x)
    .def_readwrite("y", &Vector::y)
    .def("length", &Vector::length)
    .def("dot", &Vector::dot)
    .def("angle_between", &Vector::angle_between)
    .def("__str__", &Vector::to_string)
    .def("__repr__", &Vector::to_string);
}
