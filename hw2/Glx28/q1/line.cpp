#include "line.hpp"
#include <cstddef>
using namespace std;

Line::Line() {
  point_size = 0;
  mPoints.resize(0);
}

Line::Line(Line const &ref) {
  point_size = ref.point_size;
  mPoints = ref.mPoints;
}

Line::Line(Line &&ref) {
  point_size = ref.point_size;
  mPoints = ref.mPoints;
}

Line &Line::operator=(Line const &ref) {
  point_size = ref.point_size;
  mPoints = ref.mPoints;
  return (*this);
}

Line &Line::operator=(Line &&ref) {
  point_size = ref.point_size;
  mPoints = ref.mPoints;
  return (*this);
}

Line::Line(size_t size) {
  point_size = size;
  mPoints.resize(size);
  for (size_t i = 0; i < size; ++i) {
    mPoints[i].first = 0;
    mPoints[i].second = 0;
  }
}

Line::~Line() {
  mPoints.clear();
  point_size = 0;
}

size_t Line::size() const { return point_size; }

float const &Line::x(size_t it) const { return mPoints[it].first; }

float &Line::x(size_t it) { return mPoints[it].first; }

float const &Line::y(size_t it) const { return mPoints[it].second; }

float &Line::y(size_t it) { return mPoints[it].second; }
