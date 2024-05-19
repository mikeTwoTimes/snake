#include "Snake.h"

const uint8_t Snake::s_grow_amount = 4;

void Snake::fill_points() {
  for (int i = 0; i < this->m_y_bound; i++) {
    for (int j = 0; j < this->m_x_bound; j += 2) {
      this->m_open.insert(Point(j, i));
    }
  }

  this->m_open.erase(this->m_body.front());
}

void Snake::spawn_apple() {
  if (this->m_open.empty()) {
    this->m_apple = Point(-1, -1);
    return;
  }
  
  const uint16_t idx = Random::get().Int(0, this->m_open.size() - 1);
  std::set<Point>::const_iterator it(this->m_open.begin());

  for (uint16_t i = 0; i < idx; i++, it++);

  this->m_apple = *it;
}

Snake::Snake(const uint8_t x_bound, const uint8_t y_bound)
  : m_apple(0, 0), m_x_bound(x_bound), m_y_bound(y_bound), m_on_self(false) {
  this->m_body.push_front(Point(x_bound / 2, y_bound / 2));
  this->fill_points();
  this->spawn_apple();
}

void Snake::slither(const Point diff) {
  const Point next(this->m_body.front().first + diff.first,
		   this->m_body.front().second + diff.second);
  const Point tail = this->m_body.back();
  
  this->m_body.push_front(next);
  this->m_body.pop_back();

  if ((next == tail && tail == this->m_body.back()) ||
      (!this->m_open.count(next) && next != tail)) {
    this->m_on_self = true;
  }

  if (tail != this->m_body.back()) {
    this->m_open.insert(tail);
  }

  this->m_open.erase(next);
}

void Snake::grow() {
  const Point tail = this->m_body.back();

  for (uint8_t i = 0; i < s_grow_amount; i++) {
    this->m_body.push_back(tail);
  }
}

const std::list<Point>& Snake::body() const {
  return this->m_body;
}

Point Snake::apple() const {
  return this->m_apple;
}

bool Snake::in_bounds() const {
  const Point head = this->m_body.front();

  return head.first < this->m_x_bound && head.first >= 0 &&
    head.second < this->m_y_bound && head.second >= 0;
}

bool Snake::alive() const {
  return !this->m_on_self && this->in_bounds();
}

bool Snake::on_apple() const {
  return this->m_body.front() == this->m_apple;
}
