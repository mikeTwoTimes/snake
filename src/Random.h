#pragma once

#include <cstdint>
#include <cstdlib>
#include <ctime>

class Random {
public:
  static Random& get();

  uint16_t Int(const uint16_t min, const uint16_t max);

private:
  Random();

  Random(const Random&) = delete;
  Random& operator=(const Random&) = delete;
};
