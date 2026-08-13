#include <snake/utility.h>

const std::map<uint16_t, Point> directions = {{KEY_UP, Point(0, -1)},
                                              {KEY_DOWN, Point(0, 1)},
                                              {KEY_LEFT, Point(-2, 0)},
                                              {KEY_RIGHT, Point(2, 0)}};

bool play_again() {
  uint16_t key = 0;

  do {
    key = std::toupper(getch());
  } while (key != ' ' && key != 'Q');

  return key == ' ';
}

void pause() {
  const uint8_t wait = 250;

  timeout(wait);

  while (getch() == ' ')
    ;
  while (getch() != ' ')
    ;

  timeout(1);
}

uint16_t start() {
  uint16_t key = 0;

  do {
    key = getch();
  } while (!directions.count(key));

  return key;
}

uint16_t read_key(const Snake &snake, const uint16_t prev) {
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
    pause();
    Screen::get().restore(snake.body(), snake.apple());
  }

  return prev;
}

uint16_t play() {
  Snake snake(Screen::get().game_cols(), Screen::get().game_rows());
  Point tail(0, 0);
  uint16_t key = 0;

  Screen::get().start(snake.body().back(), snake.apple());
  key = start();

  do {
    tail = snake.body().back();
    snake.slither(directions.at(key));

    if (snake.on_apple()) {
      snake.grow();
      snake.spawn_apple();
      Screen::get().print_apple(snake.apple());
      Screen::get().print_length(snake.body().size());
    }

    Screen::get().refresh_snake(snake.body().front(), snake.body().back(), tail,
                                snake.apple());
    key = read_key(snake, key);
  } while (snake.alive());

  Screen::get().highlight_head(snake.body().front());

  return snake.body().size();
}
