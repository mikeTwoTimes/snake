#pragma once

#include <list>
#include <random>
#include <snake/point.hpp>
#include <unordered_set>
#include <utility>

class Snake {
public:
  Snake(const uint8_t x_bound, const uint8_t y_bound);
  void slither(const Point diff);
  void grow();
  void spawn_apple();

  const std::list<Point> &body() const;
  Point apple() const;

  bool alive() const;
  bool on_apple() const;

private:
  static const uint8_t s_grow_amount;
  std::list<Point> m_body;
  std::unordered_set<Point, PointHash> m_open;
  std::mt19937 m_rng;
  Point m_apple;
  uint8_t m_x_bound, m_y_bound;
  bool m_on_self;

  void fill_points();

  bool in_bounds() const;

  Snake() = delete;
  Snake(const Snake &) = delete;
  Snake &operator=(const Snake &) = delete;
};
