#pragma once

#include <cstdint>
#include <functional>

struct Point {
  int8_t x, y;

  Point(const int8_t x, const int8_t y);

  bool operator==(const Point &other) const;
  bool operator!=(const Point &other) const;
};

// Source - https://stackoverflow.com/a/16473277
// Posted by Michael Anderson, modified by community. See post 'Timeline' for
// change history Retrieved 2026-08-07, License - CC BY-SA 3.0
struct PointHash {
  std::size_t operator()(const Point p) const;
};
