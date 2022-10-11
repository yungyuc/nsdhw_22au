#include "line.h"


Line::Line(size_t size){
    this->m_x.resize(size,0);
    this->m_y.resize(size,0);
}
Line::Line(Line const & other_line){
    this->m_x=other_line.m_x;
    this->m_y=other_line.m_y;
}
int64_t & Line::x(size_t size){
    return this->m_x[size];
}
int64_t & Line::y(size_t size){
    return this->m_y[size];
}
size_t Line::size(){
    return this->m_x.size();
}
Line::~Line(){
    this->m_x.clear();
    this->m_y.clear();
}