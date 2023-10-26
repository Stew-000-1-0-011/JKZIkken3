// 
// TA7291PにPWMをかけて無理やり動かす
#include "motor.hpp"
#include "photo.hpp"
#include "pid.hpp"

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  using namespace mystd::integer_types;
  using jkb21::motor::Motor;
  using jkb21::motor::Mode;
  using jkb21::photo::Photo;
  using jkb21::photo::PhotoLine;
  using jkb21::pid::Pid;

  constexpr u8 max_speed = 64;
  constexpr i8 straight_threshold = 20;

  auto left_motor = Motor::make(10, 11);
  auto right_motor = Motor::make(3,9);
  
  auto photo_line = PhotoLine<3>::make({Photo::make(A0, -124), Photo::make(A1, -41), Photo::make(A2, 124)});
  auto pid = Pid<i8>::make(1, 0, 0);

  left_motor.mode.value = Mode::CounterClockwise;
  right_motor.mode.value = Mode::Clockwise;

  while(true)
  {
    const auto v = photo_line.photos[1].read();
    Serial.println(v);
    if(v > 110)
    {
      break;
    }
  }

  auto t = millis();
  while(true)
  {
    const auto line_pos = photo_line.fusion();
    const auto pid_result = pid.calc(line_pos, 0);
    pid.update(line_pos);

    Serial.println("show");
    photo_line.show();
    Serial.println("raw_show");
    photo_line.raw_show();
    Serial.println();

    if(pid_result > straight_threshold)
    {
      left_motor.pwm = max_speed;
      // right_motor.pwm = max_speed - pid_result;
      right_motor.pwm = 0;
    }
    else if(pid_result < -straight_threshold)
    {
      left_motor.pwm = 0;
      right_motor.pwm = max_speed;
    }
    else
    {
      left_motor.pwm = max_speed;
      right_motor.pwm = max_speed;
    }

    left_motor.update();
    right_motor.update();

    // auto next_time = milis();

  }

  left_motor.pwm = 0;
  left_motor.update();
  right_motor.pwm = 0;
  right_motor.update();
}