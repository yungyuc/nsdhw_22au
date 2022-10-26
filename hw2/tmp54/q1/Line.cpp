#include "Line.h"

// default constructor
Line::Line() = default;

// copy constructor
Line::Line(Line const &rhs): points(rhs.points) {}

// move constructor
Line::Line(Line &&rhs): points(std::move(rhs.points)) {}

// copy assignment constructor
Line& Line::operator=(Line const &rhs) {
  points = rhs.points;
  return *this;
}

// move assignment constructor
Line& Line::operator=(Line &&rhs) {
  points = std::move(rhs.points);
  return *this;
}

// custom constructor
Line::Line(size_t size): points(std::vector<Point>(size)) {}

// destructor
Line::~Line() = default;

// accessors
size_t Line::size() const {
  return points.size();
}

float const& Line::x(size_t it) const {
  return points[it].x;
}

float& Line::x(size_t it) {
  return points[it].x;
}

float const& Line::y(size_t it) const {
  return points[it].y;
}

float& Line::y(size_t it) {
  return points[it].y;
}
