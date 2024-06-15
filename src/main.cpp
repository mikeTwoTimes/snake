#include <map>
#include <bitset>
#include <cctype>
#include "Screen.h"
#include "File_Util.h"

uint16_t read_file(const char* file);
uint16_t read_key(const Snake& snake, const uint16_t prev);

bool play_again();
void pause(const Snake& snake);
uint16_t start(const Snake& snake, const uint8_t turns,
	       const std::map<uint16_t, Point>& directions);
uint16_t play(Screen& display, const uint8_t turns);

int main() {
  const char* file = ".snake.dat";
  Screen& display = Screen::get();
  uint16_t high = read_file(file);
  uint16_t score = 1;
  uint8_t turns = 0;
  bool replay = true;

  display.print_best(high);

  while (replay) {
    score = play(display, turns);

    if (score > high) {
      high = score;
      display.print_best(score);
      display.log_high_score(score);
    } else {
      display.log_game_over(score);
    }

    replay = play_again();
    turns++;
    display.reset(high);
  }

  File_Out(file).stream() << std::bitset<16>(high).to_string() << "\n";
  
  return 0;
}

uint16_t read_file(const char* file) {
  std::string temp = "";

  File_In(file).stream() >> temp;
  
  return (temp.empty()) ? 1
    : static_cast<uint16_t>(std::bitset<16>(temp).to_ulong());
}

bool play_again() {
  uint16_t key = 0;

  do {
    key = std::toupper(getch());
  } while (key != ' ' && key != 'Q');

  return key == ' ';
}

void pause(const Snake& snake) {
  const uint8_t wait = 250;

  timeout(wait);

  while (getch() == ' ');
  while (getch() != ' ');

  timeout(1);
  Screen::get().restore(snake.body(), snake.apple());
}

uint16_t start(const Snake& snake, const uint8_t turns,
	       const std::map<uint16_t, Point>& directions) {
  uint16_t key = 0;
  
  Screen::get().start(snake.body().back(), snake.apple());

  if (!turns) {
    Screen::get().log_start();
    pause(snake);
  }

  do {
    key = getch();
  } while (!directions.count(key));

  return key;
}

uint16_t read_key(const Snake& snake, const uint16_t prev) {
  uint16_t key = getch();

  switch (key) {
  case KEY_UP:
    return (prev != KEY_DOWN) ? key : prev;
  case KEY_DOWN:
    return (prev != KEY_UP) ? key : prev;
  case KEY_LEFT:
    return (prev != KEY_RIGHT) ? key : prev;
  case KEY_RIGHT:
    return (prev != KEY_LEFT) ? key : prev;
  case ' ':
    Screen::get().log_pause();
    pause(snake);
  }

  return prev;
}

uint16_t play(Screen& display, const uint8_t turns) {
  const std::map<uint16_t, Point> directions = {{KEY_UP, Point(0, -1)},
						{KEY_DOWN, Point(0, 1)},
						{KEY_LEFT, Point(-2, 0)},
						{KEY_RIGHT, Point(2, 0)}};
  Snake snake(display.game_cols(), display.game_rows());
  Point tail(0, 0);
  uint16_t key = start(snake, turns, directions);

  do {
    tail = snake.body().back();
    snake.slither(directions.at(key));

    if (snake.on_apple()) {
      snake.grow();
      snake.spawn_apple();
      display.print_apple(snake.apple());
      display.print_length(snake.body().size());
    }

    display.refresh_snake(snake.body().front(), snake.body().back(), tail,
			  snake.apple());
    key = read_key(snake, key);
  } while (snake.alive());

  display.highlight_head(snake.body().front());

  return snake.body().size();
}
