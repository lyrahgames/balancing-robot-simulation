#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <chrono>
#include <cmath>

#include "pid_control.hpp"
#include "simulation.hpp"
#include "viewport.hpp"

class application {
 public:
  application();
  ~application() = default;
  void execute();

 private:
  void handle_events();
  void render();

 private:
  sf::RenderWindow *window;
  viewport view{800, 450, 10, {0, 1}};
  simulation sys{};
  pid_control pid{};
  bool pid_controlling = true;
  float desired_angle = 0;
};