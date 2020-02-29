#include <SFML/Graphics.hpp>
#include <chrono>
#include <cmath>

int main() {
  float origin_x = 0;
  float origin_y = 1;
  float fov = 5;
  int width = 800;
  int height = 450;

  float aspect_ratio = static_cast<float>(width) / height;
  float fov_x = fov * aspect_ratio;
  float fov_y = fov;
  float view_min_x = origin_x - 0.5f * fov_x;
  float view_max_y = origin_y + 0.5f * fov_y;

  float ground_y = -1;
  float ground_thick = 2;

  float wheel_x = 0;
  float wheel_v = 0;
  float wheel_a = 0;
  float wheel_radius = 0.5f;
  float wheel_thick = 2;
  float wheel_y = wheel_radius + ground_y;

  float rod_length = 3.0;
  float rod_thick = 2;
  float mass = 1;
  float mass_radius = 0.1f;
  float mass_angle = 0.1;
  float mass_angle_v = 0;

  // float time_step = 0.001f;
  auto time = std::chrono::high_resolution_clock::now();
  float g = 9.81f;

  float kp = -20.0f;
  float ki = -10.0f;
  float kd = -10.0f;
  float pid_integral = 0;
  float pid_derivative = 0;
  float old_angle = mass_angle;
  bool pid_control = true;

  sf::ContextSettings settings;
  settings.antialiasingLevel = 8;
  sf::RenderWindow window(sf::VideoMode(width, height), "SFML works!",
                          sf::Style::Default, settings);
  sf::Clock clock;

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();
      if (event.type == sf::Event::KeyPressed &&
          event.key.code == sf::Keyboard::Escape)
        window.close();
      if (event.type == sf::Event::KeyPressed &&
          event.key.code == sf::Keyboard::C)
        wheel_x = origin_x;
      if (event.type == sf::Event::KeyPressed &&
          event.key.code == sf::Keyboard::P)
        pid_control = !pid_control;
    }

    const auto new_time = std::chrono::high_resolution_clock::now();
    float time_step = std::chrono::duration<float>(new_time - time).count();
    time = new_time;

    if (pid_control) {
      float desired_angle = 0;
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        desired_angle = 0.2;
      else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        desired_angle = -0.2;

      pid_derivative = (mass_angle - desired_angle - old_angle) / time_step;
      old_angle = mass_angle - desired_angle;
      pid_integral += old_angle * time_step;
      wheel_a = kp * old_angle + ki * pid_integral + kd * pid_derivative;

      // float omega = std::sqrt(g / rod_length);
      // float scale = 100 * time_step;
      // wheel_a =
      //     -g / (1 - std::cosh(omega * scale)) *
      //     (desired_angle - mass_angle * std::cosh(omega * scale) -
      //      mass_angle_v * std::sqrt(rod_length / g) * std::sinh(omega *
      //      scale));
    } else {
      wheel_a = 0;
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        wheel_a = -5;
      else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        wheel_a = 5;
    }

    mass_angle_v += time_step / rod_length *
                    (g * std::sin(mass_angle) + wheel_a * std::cos(mass_angle));
    wheel_v += time_step * wheel_a;
    mass_angle += time_step * mass_angle_v;
    wheel_x += time_step * wheel_v;
    float mass_x = wheel_x - rod_length * std::sin(mass_angle);
    float mass_y = wheel_y + rod_length * std::cos(mass_angle);

    sf::RectangleShape ground{{static_cast<float>(width), ground_thick}};
    ground.setPosition(0, (view_max_y - ground_y) * height / fov_y);
    ground.setFillColor(sf::Color::Black);

    sf::CircleShape wheel(wheel_radius * height / fov_y);
    wheel.setPosition({(wheel_x - wheel_radius - view_min_x) * width / fov_x,
                       (view_max_y - wheel_y - wheel_radius) * height / fov_y});
    wheel.setFillColor(sf::Color::White);
    wheel.setOutlineThickness(wheel_thick);
    wheel.setOutlineColor(sf::Color::Black);

    sf::RectangleShape rod({rod_thick, rod_length * height / fov_y});
    rod.setPosition({(wheel_x - view_min_x) * width / fov_x,
                     (view_max_y - wheel_y) * height / fov_y});
    rod.rotate(180.0f - mass_angle / 3.14159f * 180.0f);
    rod.setFillColor(sf::Color::Black);

    sf::CircleShape masspoint{mass_radius * height / fov_y};
    masspoint.setPosition(
        {(mass_x - mass_radius - view_min_x) * width / fov_x,
         (view_max_y - mass_y - mass_radius) * height / fov_y});
    masspoint.setFillColor(sf::Color::Black);

    window.clear(sf::Color::White);
    window.draw(wheel);
    window.draw(ground);
    window.draw(rod);
    window.draw(masspoint);
    window.display();
  }
}