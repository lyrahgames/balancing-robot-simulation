#include "application.hpp"

application::application() {
  sf::ContextSettings settings;
  settings.antialiasingLevel = 8;
  window = new sf::RenderWindow(sf::VideoMode(view.screen[0], view.screen[1]),
                                "Balancing Robot Simulation",
                                sf::Style::Default, settings);
  // window->setVerticalSyncEnabled(true);
};

void application::execute() {
  auto time = std::chrono::high_resolution_clock::now();

  while (window->isOpen()) {
    handle_events();

    const auto new_time = std::chrono::high_resolution_clock::now();
    float time_step = std::chrono::duration<float>(new_time - time).count();
    time = new_time;

    if (pid_controlling) {
      sys.wheel_a = pid(sys.mass_angle, desired_angle, time_step);
      // sys.control(desired_angle);
    }
    sys.advance(time_step);

    render();
  }
}

void application::handle_events() {
  sf::Event event;
  while (window->pollEvent(event)) {
    switch (event.type) {
      case sf::Event::Closed:
        window->close();
        break;

      case sf::Event::KeyPressed:
        switch (event.key.code) {
          case sf::Keyboard::Escape:
            window->close();
            break;

          case sf::Keyboard::C:
            sys.wheel_x = view.origin[0];
            break;

          case sf::Keyboard::P:
            pid_controlling = !pid_controlling;
        }
        break;
    }
  }

  if (pid_controlling) {
    desired_angle = 0.0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
      desired_angle = 0.2;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
      desired_angle = -0.2;
  } else {
    sys.wheel_a = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
      sys.wheel_a = -5;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
      sys.wheel_a = 5;
  }
}

void application::render() {
  constexpr float line_thickness = 1;

  sf::RectangleShape ground{{view.screen[0], line_thickness}};
  ground.setPosition(0, view.pixels({0, sys.ground_y})[1]);
  ground.setFillColor(sf::Color::Black);

  sf::CircleShape wheel(sys.wheel_radius * view.pixel_scale);
  wheel.setOrigin(sys.wheel_radius * view.pixel_scale,
                  sys.wheel_radius * view.pixel_scale);
  const auto wheel_pos = view.pixels({sys.wheel_x, sys.wheel_y});
  // wheel.setPosition({(wheel_x - wheel_radius - view_min_x) * width / fov_x,
  //                    (view_max_y - wheel_y - wheel_radius) * height /
  //                    fov_y});
  wheel.setPosition({wheel_pos[0], wheel_pos[1]});
  wheel.setFillColor(sf::Color::White);
  wheel.setOutlineThickness(line_thickness);
  wheel.setOutlineColor(sf::Color::Black);

  sf::RectangleShape rod({line_thickness, sys.rod_length * view.pixel_scale});
  // rod.setPosition({(wheel_x - view_min_x) * width / fov_x,
  //                  (view_max_y - wheel_y) * height / fov_y});
  rod.setPosition(wheel_pos[0], wheel_pos[1]);
  rod.rotate(180.0f - sys.mass_angle / 3.14159f * 180.0f);
  rod.setFillColor(sf::Color::Black);

  sf::CircleShape masspoint{sys.mass_radius * view.pixel_scale};
  masspoint.setOrigin(sys.mass_radius * view.pixel_scale,
                      sys.mass_radius * view.pixel_scale);
  const auto masspoint_pixel = view.pixels({sys.mass_x, sys.mass_y});
  masspoint.setPosition({masspoint_pixel[0], masspoint_pixel[1]});
  masspoint.setFillColor(sf::Color::Black);

  window->clear(sf::Color::White);
  window->draw(wheel);
  window->draw(ground);
  window->draw(rod);
  window->draw(masspoint);
  window->display();
}
