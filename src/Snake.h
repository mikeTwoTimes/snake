#pragma once

#include <list>
#include <set>
#include <utility>
#include "Random.h"

typedef std::pair<int8_t, int8_t> Point;

class Snake {
public:
  Snake(const uint8_t x_bound, const uint8_t y_bound);
  void slither(const Point diff);
  void grow();
  void spawn_apple();
  
  const std::list<Point>& body() const;
  Point apple() const;

  bool alive() const;
  bool on_apple() const;
  
private:
  static const uint8_t s_grow_amount;
  std::list<Point> m_body;
  std::set<Point> m_open;
  Point m_apple;
  uint8_t m_x_bound, m_y_bound;
  bool m_on_self;

  void fill_points();

  bool in_bounds() const;

  Snake() = delete;
  Snake(const Snake&) = delete;
  Snake& operator=(const Snake&) = delete;
};
