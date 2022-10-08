#include "Line.h"
using namespace std;
#include <cstddef>
Line::Line(){
    point_size = 0;
    my_points.resize(0);
}

Line::Line(Line const & ref){
    point_size = ref.point_size;
    my_points = ref.my_points;
}

Line::Line(Line && ref){
    point_size = ref.point_size;
    my_points = ref.my_points;
}

Line & Line::operator=(Line const & ref){
    point_size = ref.point_size;
    my_points = ref.my_points;
    return (*this);
}

Line & Line::operator=(Line && ref){
    point_size = ref.point_size;
    my_points = ref.my_points;
    return (*this);
}

Line::Line(size_t size){
    point_size = size;
    my_points.resize(size);
    for(size_t i=0; i < size; ++i){
        my_points[i].first = 0;
        my_points[i].second = 0;
    }
}

Line::~Line(){
    my_points.clear();
    point_size = 0;
}

size_t Line::size() const{
    return point_size;
}

float const & Line::x(size_t it) const{
    return my_points[it].first;
}

float & Line::x(size_t it){
    return my_points[it].first;
}

float const & Line::y(size_t it) const{
    return my_points[it].second;
}

float & Line::y(size_t it){
    return my_points[it].second;
}




