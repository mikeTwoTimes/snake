#pragma once

#include <cstdint>
#include <tuple>

struct Point {
  int8_t x, y;

  Point(const int8_t x, const int8_t y);

  bool operator==(const Point &other) const;
  bool operator!=(const Point &other) const;
  bool operator<(const Point &other) const;
};
