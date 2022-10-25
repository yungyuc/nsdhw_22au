#include "line.h"

Line::Line(Line const & other) { coord_ = other.coord_; }
Line::Line(Line && other) { coord_ = std::move(other.coord_); }
Line & Line::operator=(Line const & other) { coord_ = other.coord_; }
Line & Line::operator=(Line && other) { coord_ = std::move(other.coord_); }
Line::Line(size_t size) { coord_.resize(size, std::vector<float>(2)); }
