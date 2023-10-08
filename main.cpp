#include "fake_arduino.hpp"
#include "mystd.hpp"
#include "motor.hpp"

using jkb21::Motor;

constexpr Motor left_motor
{
  .pwm_pin = 11,
  .in1_pin = 4,
  .in2_pin = 5
};

constexpr Motor right_motor
{
  .pwm_pin = 10,
  .in1_pin = 2,
  .in2_pin = 3
};

void setup() {
  left_motor.enable();
  right_motor.enable();
}

void loop() {
  // put your main code here, to run repeatedly:
  left_motor.update_speed(255);
  right_motor.update_speed(255);
}