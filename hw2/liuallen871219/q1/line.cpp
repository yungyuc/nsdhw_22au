#include<vector>
#include<iostream>
#include"line.hpp"
using namespace std;

Line::Line(){
    len = 0;
}
Line::Line(size_t size){
    len = size;
    x_p.resize(len, 0);
    y_p.resize(len, 0);
}
Line::Line(const Line& line){
    len = line.len;
    x_p = line.x_p;
    y_p = line.y_p;
}
Line::Line(Line &&line){
    x_p = move(line.x_p);
    y_p = move(line.y_p);
    len = move(line.len);
}
Line::~Line(){
    len = 0;
    x_p.clear();
    y_p.clear();
}
Line & Line::operator=(Line const & line){
    if(this == &line)
        return *this;
    x_p = line.x_p;
    y_p = line.y_p;
    len = line.len;
    return *this;
}
Line & Line::operator=(Line && line){
    x_p = move(line.x_p);
    y_p = move(line.y_p);
    len = move(line.len);
    return *this;
}
size_t Line::size() const{
    return len;
}
float const & Line::x(size_t it) const{
    return x_p.at(it);
}
float & Line::x(size_t it){
    return x_p.at(it);
}
float const & Line::y(size_t it) const{
    return y_p.at(it);
}
float & Line::y(size_t it){
    return y_p.at(it);
}