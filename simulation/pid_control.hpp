#pragma once

struct pid_control {
  float operator()(float signal, float set_point, float time_step) {
    pid_derivative = (signal - set_point - old_error) / time_step;
    old_error = signal - set_point;
    pid_integral += old_error * time_step;
    return kp * old_error + ki * pid_integral + kd * pid_derivative;
  }

  float kp = -32.0f;
  float ki = -1.0f;
  float kd = -10.0f;
  float pid_integral = 0;
  float pid_derivative = 0;
  float old_error = 0;
};