#include <snake/utility.hpp>

int main() {
  uint16_t score = 0, high = 1;
  bool replay = true;

  Screen::get().print_best(high);
  Screen::get().log_start();
  pause();
  Screen::get().reset(high);

  while (replay) {
    score = play();

    if (score > high) {
      high = score;
      Screen::get().print_best(score);
      Screen::get().log_high_score(score);
    } else {
      Screen::get().log_game_over(score);
    }

    replay = play_again();
    Screen::get().reset(high);
  }

  return 0;
}
