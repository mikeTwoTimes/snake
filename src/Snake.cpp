#include <snake/snake.hpp>

const uint8_t Snake::s_grow_amount = 4;

void Snake::erase_opening(const Point a) {
  const std::size_t ind = m_open_map[a];
  const Point b = this->m_open_vector.back();

  this->m_open_vector[ind] = b;
  this->m_open_map[b] = ind;
  this->m_open_vector.pop_back();
  this->m_open_map.erase(a);
}

void Snake::fill_points() {
  for (int8_t i = 0; i < this->m_y_bound; i++) {
    for (int8_t j = 0; j < this->m_x_bound; j += 2) {
      Point p = {j, i};
      this->m_open_map[p] = this->m_open_vector.size();
      this->m_open_vector.push_back(p);
    }
  }

  this->erase_opening(this->m_body.front());
}

void Snake::spawn_apple() {
  if (this->m_open_map.empty()) {
    this->m_apple = Point(-1, -1);
    return;
  }

  std::uniform_int_distribution<uint16_t> d(0, this->m_open_vector.size() - 1);
  this->m_apple = this->m_open_vector[d(this->m_rng)];
}

Snake::Snake(const uint8_t x_bound, const uint8_t y_bound)
    : m_rng(std::random_device{}()), m_apple(0, 0), m_x_bound(x_bound),
      m_y_bound(y_bound), m_on_self(false) {
  this->m_body.push_front(Point(x_bound / 2, y_bound / 2));
  this->m_open_vector.reserve(this->m_y_bound * ((this->m_x_bound - 1) / 2));
  this->fill_points();
  this->spawn_apple();
}

void Snake::slither(const Point diff) {
  const Point next(this->m_body.front().x + diff.x,
                   this->m_body.front().y + diff.y);
  const Point tail = this->m_body.back();

  this->m_body.push_front(next);
  this->m_body.pop_back();

  if ((next == tail && tail == this->m_body.back()) ||
      (!this->m_open_map.count(next) && next != tail)) {
    this->m_on_self = true;
  }

  if (tail != this->m_body.back()) {
    this->m_open_map[tail] = this->m_open_vector.size();
    this->m_open_vector.push_back(tail);
  }

  this->erase_opening(next);
}

void Snake::grow() {
  const Point tail = this->m_body.back();

  for (uint8_t i = 0; i < s_grow_amount; i++) {
    this->m_body.push_back(tail);
  }
}

const std::list<Point> &Snake::body() const { return this->m_body; }

Point Snake::apple() const { return this->m_apple; }

bool Snake::in_bounds() const {
  const Point head = this->m_body.front();

  return head.x < this->m_x_bound && head.x >= 0 && head.y < this->m_y_bound &&
         head.y >= 0;
}

bool Snake::alive() const { return !this->m_on_self && this->in_bounds(); }

bool Snake::on_apple() const { return this->m_body.front() == this->m_apple; }
