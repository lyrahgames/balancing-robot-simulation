#pragma once

struct simulation {
  simulation() = default;

  void advance(float time_step) {
    constexpr float pi = 3.14159;
    wheel_a -=
        mass /
        (wheel_mass + mass * std::sin(mass_angle) * std::sin(mass_angle)) *
        (rod_length * mass_angle_v * mass_angle_v * std::sin(mass_angle) -
         mass * g * std::sin(mass_angle) * std::cos(mass_angle));
    mass_angle_v += time_step / rod_length *
                    (g * std::sin(mass_angle) + wheel_a * std::cos(mass_angle));
    wheel_v += time_step * wheel_a;

    mass_angle += time_step * mass_angle_v;
    wheel_x += time_step * wheel_v;
    const float boundary_angle = pi - std::acos(wheel_radius / rod_length);
    const float delta_angle = std::abs(mass_angle) - boundary_angle;
    if (delta_angle >= 0) {
      // const float left_time = delta_angle / mass_angle_v;
      // mass_angle -= left_time * mass_angle_v;
      // wheel_x -= left_time * wheel_v;

      wheel_v -=
          (ground_restitution)*rod_length * mass_angle_v * std::cos(mass_angle);
      mass_angle_v = -ground_restitution * mass_angle_v;

      // mass_angle += left_time * mass_angle_v;
      // wheel_x += left_time * wheel_v;
      mass_angle += time_step * mass_angle_v;
      wheel_x += time_step * wheel_v;
    }

    // mass_angle =
    //     mass_angle - (2 * pi) * std::floor((mass_angle + pi) / (2 * pi));
    mass_x = wheel_x - rod_length * std::sin(mass_angle);
    mass_y = wheel_y + rod_length * std::cos(mass_angle);
    wheel_a = 0;
  }

  void control(float desired_angle) {
    float omega = std::sqrt(g / rod_length);
    // float scale = 100 * time_step;
    float scale = 0.1;
    wheel_a =
        -g / (1 - std::cosh(omega * scale)) *
        (desired_angle - mass_angle * std::cosh(omega * scale) -
         mass_angle_v * std::sqrt(rod_length / g) * std::sinh(omega * scale));
  }

  float ground_y = 0;
  float ground_restitution = 1.0;

  float wheel_x = 0;
  float wheel_v = 0;
  float wheel_a = 0;
  float wheel_radius = 0.4f;
  float wheel_y = wheel_radius + ground_y;
  float wheel_mass = 1;

  float rod_length = 2.5;
  float mass = 1;
  float mass_radius = 0.1f;
  float mass_angle = 0.1;
  float mass_angle_v = 0;
  float mass_x = 0;
  float mass_y = 0;

  // float time_step = 0.001f;
  // auto time = std::chrono::high_resolution_clock::now();
  float g = 9.81f;
};