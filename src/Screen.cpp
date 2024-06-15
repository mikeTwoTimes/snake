#include "Screen.h"

Screen Screen::s_instance;

Point Screen::start_ncurses() const {
  initscr();
  noecho();
  cbreak();
  curs_set(0);
  keypad(stdscr, true);
  timeout(1);

  return Point((COLS / 2) - (this->m_game_cols / 2),
	       (LINES / 2) - ((this->m_game_rows + 2) / 2));
}

void Screen::start_paint() const {
  const RGB board = {0, 0, 0};
  const RGB apple = {918, 200, 137};
  const RGB congrats = {1000, 1000, 329};
  const RGB snake = {435, 859, 255};
  const RGB edge = {114, 192, 290};
  const RGB text = {1000, 1000, 1000};
  
  start_color();
  init_color(Paint::black, board.r, board.g, board.b);
  init_color(Paint::red, apple.r, apple.g, apple.b);
  init_color(Paint::yellow, congrats.r, congrats.g, congrats.b);
  init_color(Paint::green, snake.r, snake.g, snake.b);
  init_color(Paint::blue, edge.r, edge.g, edge.b);
  init_color(Paint::white, text.r, text.g, text.b);
  init_pair(Paint::black, Paint::white, Paint::black);
  init_pair(Paint::red, Paint::red, Paint::red);
  init_pair(Paint::yellow, Paint::yellow, Paint::black);
  init_pair(Paint::green, Paint::green, Paint::green);
  init_pair(Paint::blue, Paint::white, Paint::blue);
  init_pair(Paint::white, Paint::white, Paint::white);
}

void Screen::print_score_tags() const {
  const uint8_t row = this->m_game_rows + this->m_game_start.second + 1;
  const uint8_t best_col = this->m_game_cols + this->m_game_start.first - 9;

  mvprintw(row, this->m_game_start.first, "Length: ");
  mvprintw(row, best_col, "Best: ");
}

Screen::Screen()
  : m_game_cols(64), m_game_rows(31), m_print_tick(80),
    m_game_start(start_ncurses()), m_game_screen(newwin(m_game_rows, m_game_cols,
							m_game_start.second,
							m_game_start.first)) {
  this->start_paint();
  bkgd(COLOR_PAIR(Paint::blue));
  this->print_score_tags();
  this->print_length(1);
  wbkgd(this->m_game_screen, COLOR_PAIR(Paint::black));
  wrefresh(this->m_game_screen);
}

Screen::~Screen() {
  delwin(this->m_game_screen);
  endwin();
}

Screen& Screen::get() {
  return s_instance;
}

uint8_t Screen::game_cols() const {
  return this->m_game_cols;
}

uint8_t Screen::game_rows() const {
  return this->m_game_rows;
}

void Screen::start(const Point head, const Point apple) {
  wattron(this->m_game_screen, COLOR_PAIR(Paint::green));
  mvwprintw(this->m_game_screen, head.second, head.first, "  ");
  wattroff(this->m_game_screen, COLOR_PAIR(Paint::green));
  this->print_apple(apple);
}

void Screen::refresh_snake(const Point head, const Point new_tail,
			   const Point old_tail, const Point apple) {
  if (old_tail != new_tail && old_tail != apple) {
    mvwprintw(this->m_game_screen, old_tail.second, old_tail.first, "  ");
  }

  wattron(this->m_game_screen, COLOR_PAIR(Paint::green));
  mvwprintw(this->m_game_screen, head.second, head.first, "  ");
  wattroff(this->m_game_screen, COLOR_PAIR(Paint::green));
  wrefresh(this->m_game_screen);
  napms(this->m_print_tick);
}

void Screen::print_apple(const Point apple) {
  wattron(this->m_game_screen, COLOR_PAIR(Paint::red));
  mvwprintw(this->m_game_screen, apple.second, apple.first, "  ");
  wattroff(this->m_game_screen, COLOR_PAIR(Paint::red));
  wrefresh(this->m_game_screen);
}

void Screen::restore(const std::list<Point>& body, const Point apple) {
  wclear(this->m_game_screen);
  wattron(this->m_game_screen, COLOR_PAIR(Paint::green));

  for (const Point& pos : body) {
    mvwprintw(this->m_game_screen, pos.second, pos.first, "  ");
  }

  wattroff(this->m_game_screen, COLOR_PAIR(Paint::green));
  this->print_apple(apple);
}

void Screen::create_logger() {
  const std::string line(42, ' ');
  const uint8_t rows = 11;
  const uint8_t start_x = 12;
  const uint8_t start_y = 10;

  for (uint8_t i = 0; i < rows; i++) {
    mvwprintw(this->m_game_screen, start_y + i, start_x, line.c_str());
  }
}

void Screen::print_middle(const uint8_t y, const char* str) {
  const uint8_t x = ((this->m_game_cols / 2) + 1) - (strlen(str) / 2);
  
  mvwprintw(this->m_game_screen, y, x, str);
}

void Screen::log_start() {
  const uint8_t row = 12;

  this->create_logger();
  this->print_middle(row, "Use the arrow keys on your keyboard");
  this->print_middle(row + 1, "to control the snake");
  this->print_middle(row + 4, "Press [space] to start.");
  wrefresh(this->m_game_screen);
}

void Screen::log_pause() {
  const uint8_t row = 14;

  this->create_logger();
  this->print_middle(row, "[Paused]");
  this->print_middle(row + 1, "Press [space] to unpause.");
  wrefresh(this->m_game_screen);
}

void Screen::log_game_over(const uint16_t length) {
  const std::string score = "Length: " + std::to_string(length);
  const uint8_t row = 12;

  this->create_logger();
  this->print_middle(row, "GAME OVER");
  this->print_middle(row + 2, score.c_str());
  this->print_middle(row + 5, "Press [space] to play again,");
  this->print_middle(row + 6, "press [Q] to quit.");
  wrefresh(this->m_game_screen);
}

void Screen::log_high_score(const uint16_t length) {
  const std::string score = "Length: " + std::to_string(length);
  const uint8_t row = 11;

  this->create_logger();
  wattron(this->m_game_screen, COLOR_PAIR(Paint::yellow));
  this->print_middle(row, "CONGRATULATIONS!");
  wattroff(this->m_game_screen, COLOR_PAIR(Paint::yellow));
  this->print_middle(row + 1, "You got a new HIGH SCORE!");
  this->print_middle(row + 3, score.c_str());
  this->print_middle(row + 6, "Press [space] to play again,");
  this->print_middle(row + 7, "press [Q] to quit.");
  wrefresh(this->m_game_screen);
}

void Screen::highlight_head(const Point head) {
  if (head.first < this->m_game_cols && head.first >= 0 &&
      head.second < this->m_game_rows && head.second >= 0) {
    wattron(this->m_game_screen, COLOR_PAIR(Paint::white));
    mvwprintw(this->m_game_screen, head.second, head.first, "  ");
    wattroff(this->m_game_screen, COLOR_PAIR(Paint::white));
    wrefresh(this->m_game_screen);
    return;
  }

  const Point edge(head.first + this->m_game_start.first,
		   head.second + this->m_game_start.second);

  attron(COLOR_PAIR(Paint::white));
  mvprintw(edge.second, edge.first, "  ");
  attroff(COLOR_PAIR(Paint::white));
  refresh();
}

void Screen::reset(const uint16_t high) {
  wclear(this->m_game_screen);
  clear();
  this->print_score_tags();
  this->print_length(1);
  this->print_best(high);
}

void Screen::print_length(const uint16_t length) const {
  const int row = this->m_game_rows + this->m_game_start.second + 1;
  const int col = this->m_game_start.first + 8;

  mvprintw(row, col, "%d", length);
  refresh();
}

void Screen::print_best(const uint16_t best) const {
  const int row = this->m_game_rows + this->m_game_start.second + 1;
  const int col = this->m_game_cols + this->m_game_start.first - 3;

  mvprintw(row, col, "%d", best);
  refresh();
}
