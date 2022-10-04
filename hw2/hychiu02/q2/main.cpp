// #include <pybind11/pybind11.h>
// #include <pybind11/stl.h>

#include <cstdint>
#include <algorithm>
#include <utility>
#include <cmath>
#include <iostream>

template <class Derived>
class VectorBase
{
public:
    constexpr static const double PI = 3.14159265358979323846;
    VectorBase(std::pair<float,float> v0, std::pair<float,float> v1) : m_v0(v0), m_v1(v1) {}
    std::pair<float,float> const & v0() const { return m_v0; }
    std::pair<float,float>       & v0()       { return m_v0; }
    std::pair<float,float> const & v1() const { return m_v1; }
    std::pair<float,float>       & v1()       { return m_v1; }
    std::pair<float,float> dist() const
    {
        // Prevent the derived class from working if it doesn't define dist(),
        // just like what a pure virtual function does.
        static_assert(&VectorBase<Derived>::dist != &Derived::dist,
                      "derived class must define dist()");
        return derived().dist();
    }
private:
    Derived const & derived() const { return *static_cast<Derived const *>(this); }
    std::pair<float,float> m_v0;
    std::pair<float,float> m_v1;
}; /* end class PointBase */


class CalculateAngle : public VectorBase<CalculateAngle>
{
public:
    using base_type = VectorBase<CalculateAngle>;
    using base_type::base_type;
    float dist() const
    {
        /* a ∙ b = |a| * |b| * cos(α)
           cos(α) = a ∙ b / (|a| * |b|)
           α = arccos[(a ∙ b) / (|a| * |b|)]
         */
        float inner_prod, len_a, len_b, angle;
        
        inner_prod = (v0().first * v1().first) + (v0().second * v1().second);
        len_a = std::hypot(v0().first, v0().second);
        len_b = std::hypot(v1().first, v1().second);

        angle = acos(inner_prod/(len_a * len_b));

        return angle;
    }
}; /* end class CalculateAngle */

int main(int, char **)
{
    CalculateAngle ca({1,0}, {-1,0});

    std::cout << "sizeof(CalculateAngle = " << sizeof(CalculateAngle) << std::endl;

    std::cout << "Angle: radians = " << ca.dist() << std::endl;
    std::cout << "Angle: degrees = " << ca.dist()*(180/ca.PI) << std::endl;

    return 0;
}

/*
namespace py = pybind11;

PYBIND11_MODULE(calculate_angle, m)
{
    
    py::class_<VectorBase>(m, "VectorBase")
        .def(py::init<std::pair<float,float>, std::pair<float,float>>())
        .def_property("v0", &VectorBase::v0, VectorBase::&v0)
        .def_property("v1", &VectorBase::v1, VectorBase::&v1)
        .def("dist", &VectorBase::dist)
        .def_read("PI", &VectorBase::PI)

    py::class_<CalculateAngle, VectorBase<CalculateAngle>>(m, "CalculateAngle")
        .def(py::init<std::pair<float,float>, std::pair<float,float>>())
        .def("dist", &CalculateAngle::dist);
}
*/