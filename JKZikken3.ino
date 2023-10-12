// TA7291PにPWMをかけて無理やり動かす

#include "fake_arduino.hpp"
#include "motor.hpp"
#include "logger.hpp"

using jkb21::Motor;
using jkb21::logger::Logger;
namespace jkb21::main {
  auto logger = Logger<1000>::make();
  auto left_motor = Motor::make(5, 6);
  auto right_motor = Motor::make(10, 11);
  auto counter = 0;
  auto t = millis();
}

void setup() {
  using namespace jkb21::main;

  left_motor.enable();
  left_motor.mode.value = jkb21::impl::Mode::Clockwise;
  right_motor.enable();
  right_motor.mode.value = jkb21::impl::Mode::Clockwise;

  t = millis();
}

void loop() {
  using namespace jkb21::main;

  left_motor.pwm = 255;
  right_motor.pwm = 255;

  left_motor.update();
  right_motor.update();

  const auto end = millis();
  logger.log(counter, ": ", end - t, '\n');
  t = end;
}