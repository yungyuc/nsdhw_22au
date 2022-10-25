#include "line.hpp"

Line::Line(size_t n) {
	elements.resize(n);
	return;
}

Line::Line(Line const & l) {
	elements = l.elements;
	return;
}

Line & Line::operator=(Line const &it) {
	elements = it.elements;
	return *this;
}

size_t Line::size() const {
	return elements.size();
}

float const & Line::x(size_t id) const {
	return elements[id].x;
}

float & Line::x(size_t id) {
	return elements[id].x;
}

float const & Line::y(size_t id) const {
	return elements[id].y;
}

float & Line::y(size_t id) {
	return elements[id].y;
}
