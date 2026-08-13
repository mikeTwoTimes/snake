#pragma once

#include <cctype>
#include <map>
#include <snake/screen.hpp>
#include <snake/snake.hpp>

uint16_t read_key(const Snake &snake, const uint16_t prev);
bool play_again();
void pause();
uint16_t start();
uint16_t play();
