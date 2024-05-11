#include "Random.h"

Random::Random() { std::srand(std::time(0)); }

Random& Random::get() {
  static Random instance;

  return instance;
}

uint16_t Random::Int(const uint16_t min, const uint16_t max) {
  return (std::rand() % (max - min + 1)) + min;
}
