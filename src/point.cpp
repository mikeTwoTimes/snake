#include <snake/point.hpp>

Point::Point(const int8_t x, const int8_t y) : x(x), y(y) {}

bool Point::operator==(const Point &other) const {
  return this->x == other.x && this->y == other.y;
}

bool Point::operator!=(const Point &other) const { return !(*this == other); }

bool Point::operator<(const Point &other) const {
  return std::tie(x, y) < std::tie(other.x, other.y);
}
