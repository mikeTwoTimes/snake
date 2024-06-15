#pragma once

#include <ncurses.h>
#include <string>
#include <cstring>
#include "Snake.h"

struct RGB {
  uint16_t r, g, b;
};

enum Paint : uint8_t {
  black = 9, red, yellow, green, blue, white
};

class Screen {
public:
  void start(const Point head, const Point apple);
  void refresh_snake(const Point head, const Point new_tail,
		     const Point old_tail, const Point apple);
  void print_apple(const Point apple);
  void restore(const std::list<Point>& body, const Point apple);
  void log_start();
  void log_pause();
  void log_game_over(const uint16_t score);
  void log_high_score(const uint16_t score);
  void highlight_head(const Point head);
  void reset(const uint16_t high);
  ~Screen();

  static Screen& get();
  uint8_t game_cols() const;
  uint8_t game_rows() const;

  void print_length(const uint16_t length) const;
  void print_best(const uint16_t best) const;
  
private:
  static Screen s_instance;
  const uint8_t m_game_cols, m_game_rows, m_print_tick;
  const Point m_game_start;
  WINDOW* m_game_screen;

  Screen();
  void create_logger();
  void print_middle(const uint8_t y, const char* str);

  Point start_ncurses() const;
  void start_paint() const;
  void print_score_tags() const;

  Screen(const Screen&) = delete;
  Screen& operator=(const Screen&) = delete;
};
