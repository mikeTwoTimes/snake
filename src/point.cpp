#include <snake/point.hpp>

Point::Point(const int8_t x, const int8_t y) : x(x), y(y) {}

bool Point::operator==(const Point &other) const {
  return this->x == other.x && this->y == other.y;
}

bool Point::operator!=(const Point &other) const { return !(*this == other); }

std::size_t PointHash::operator()(const Point p) const {
  std::size_t seed = std::hash<int8_t>{}(p.x);
  seed ^= std::hash<int8_t>{}(p.y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  return seed;
}
