#include "Line.h"


Line::Line(){};

Line::Line(const size_t size){
  const float inf = std::numeric_limits<float>::infinity();
  for(size_t i = 0; i<size; i++){
    xs.push_back(inf);
    ys.push_back(inf);
  };
};
      
size_t Line::size() const{
  return xs.size();
};

float Line::x(size_t it) const {
  return xs.at(it);
};

float & Line::x(size_t it){
  return xs.at(it);
};

float Line::y(size_t it) const {
  return ys.at(it);
};

float & Line::y(size_t it){
  return ys.at(it);
};
