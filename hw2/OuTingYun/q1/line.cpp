#include "line.h"
Line::Line(){}
Line::Line(Line const & line ){
    this->x_data = line.x_data;
    this->y_data = line.y_data;
}

Line::Line(Line && line){
    this->x_data = std::move(line.x_data);
    this->y_data = std::move(line.y_data);
}
Line::Line(size_t size){
    this->x_data.resize(size);
    this->y_data.resize(size) ;  
}
Line & Line::operator=(Line const &other ){
    if(this!=&other){
        this->x_data = other.x_data;
        this->y_data = other.y_data;
    }
    return *this;
}
Line & Line::operator=(Line &&other){
    if(this!=&other){
        this->x_data = other.x_data;
        this->y_data = other.y_data;
    }
    return *this;
}

Line::~Line(){
    this->x_data.clear();
    this->y_data.clear();
}
size_t Line::size() const{
    return this->x_data.size();
}
float const & Line::x(size_t it) const{
    return this->x_data[it];
}
float & Line::x(size_t it){
    return this->x_data[it];
}
float const & Line::y(size_t it) const{
    return this->y_data[it];
}
float & Line::y(size_t it){
    return this->y_data[it];
}