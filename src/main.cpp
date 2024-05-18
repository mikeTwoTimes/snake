#include <map>
#include <bitset>
#include <cctype>
#include "Screen.h"
#include "File_Util.h"

uint16_t read_file(const char* file);
void write_file(const char* file, const uint16_t score);

bool play_again();
void wait_for_space(Screen& display, const Snake& snake);
uint16_t start(Screen& display, const Snake& snake, const uint8_t turns,
	       const std::map<int, Point>& directions);
uint16_t scan(Screen& display, const Snake& snake, const uint16_t prev);

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
    display.clean();
  }

  write_file(file, high);
  
  return 0;
}

uint16_t read_file(const char* file) {
  std::string temp = "";

  File_In(file).stream() >> temp;
  
  return (temp.empty()) ? 1
    : static_cast<uint16_t>(std::bitset<16>(temp).to_ulong());
}

void write_file(const char* file, const uint16_t score) {
  File_Out(file).stream() << std::bitset<16>(score).to_string() << "\n";
}

bool play_again() {
  uint16_t input = 0;

  do {
    input = std::toupper(getch());
  } while (input != ' ' && input != 'Q');

  return input == ' ';
}

void wait_for_space(Screen& display, const Snake& snake) {
  const uint8_t wait = 250;

  timeout(wait);

  while (getch() == ' ');
  while (getch() != ' ');

  timeout(1);
  display.restore(snake.body(), snake.apple());
}

uint16_t start(Screen& display, const Snake& snake, const uint8_t turns,
	       const std::map<int, Point>& directions) {
  uint16_t input = 0;
  
  display.start(snake.body().back(), snake.apple());

  if (!turns) {
    display.log_start();
    wait_for_space(display, snake);
  }

  do {
    input = getch();
  } while (!directions.count(input));

  return input;
}

uint16_t scan(Screen& display, const Snake& snake, const uint16_t prev) {
  uint16_t input = getch();

  switch (input) {
  case KEY_UP:
    return (prev != KEY_DOWN) ? input : prev;
  case KEY_DOWN:
    return (prev != KEY_UP) ? input : prev;
  case KEY_LEFT:
    return (prev != KEY_RIGHT) ? input : prev;
  case KEY_RIGHT:
    return (prev != KEY_LEFT) ? input : prev;
  case ' ':
    display.log_pause();
    wait_for_space(display, snake);
  }

  return prev;
}

uint16_t play(Screen& display, const uint8_t turns) {
  const std::map<int, Point> directions = {{KEY_UP, Point(0, -1)},
					   {KEY_DOWN, Point(0, 1)},
					   {KEY_LEFT, Point(-2, 0)},
					   {KEY_RIGHT, Point(2, 0)}};
  Snake snake(display.game_cols(), display.game_rows());
  Point old(0, 0);
  uint16_t input = start(display, snake, turns, directions);

  do {
    old = snake.body().back();
    snake.slither(directions.at(input));

    if (snake.on_apple()) {
      snake.grow();
      snake.spawn_apple();
      display.print_apple(snake.apple());
      display.print_length(snake.body().size());
    }

    display.refresh_snake(snake.body().front(), snake.body().back(), old);
    input = scan(display, snake, input);
  } while (snake.alive());

  display.highlight_head(snake.body().front());

  return snake.body().size();
}