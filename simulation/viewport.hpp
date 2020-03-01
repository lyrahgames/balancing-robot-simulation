#pragma once

struct viewport {
  using vector = std::array<float, 2>;

  viewport(int width, int height, float f, vector o = {})
      : screen{width, height},
        origin{o},
        fov{f * width / height, f},
        min{o[0] - 0.5f * fov[0], o[1] - 0.5f * fov[1]},
        max{o[0] + 0.5f * fov[0], o[1] + 0.5f * fov[1]},
        pixel_scale{height / f},
        point_scale{f / height} {}

  vector pixels(vector point) const noexcept {
    return {(point[0] - min[0]) * pixel_scale,
            (max[1] - point[1]) * pixel_scale};
  }
  vector points(vector pixel) const noexcept {
    return {pixel[0] * point_scale + min[0], max[1] - pixel[1] * point_scale};
  }

  vector screen;
  vector origin;
  vector fov;
  vector min;
  vector max;
  float pixel_scale;
  float point_scale;
};