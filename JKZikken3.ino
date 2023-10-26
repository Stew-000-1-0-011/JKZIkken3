// 
// TA7291PにPWMをかけて無理やり動かす
#include "motor.hpp"
#include "photo.hpp"
#include "pid.hpp"

enum class State : mystd::integer_types::u8
{
  Default,
  Lost
};

void setup()
{
  Serial.begin(9600);

  using namespace mystd::integer_types;
  using jkb21::motor::Motor;
  using jkb21::motor::Mode;
  using jkb21::motor::flip_mode;
  using jkb21::photo::Photo;
  using jkb21::photo::PhotoLine;
  using jkb21::pid::Pid;

  constexpr u8 max_speed = 70;
  constexpr i8 straight_threshold = 100;

  auto left_motor = Motor::make(10, 11);
  auto right_motor = Motor::make(3,9);
  
  // auto photo_line = PhotoLine<3>::make({Photo::make(A0, -124), Photo::make(A1, -41), Photo::make(A2, 124)});
  auto photo_line = PhotoLine<3>::make({Photo::make(A0, -124), Photo::make(A1, 0), Photo::make(A2, 124)});
  auto pid = Pid<i8>::make(1, 0, 0);

  constexpr auto left_direction = Mode::CounterClockwise;
  constexpr auto right_direction = Mode::Clockwise;

  left_motor.mode.value = left_direction;
  right_motor.mode.value = right_direction;

  State state = State::Default;

  while(true)
  {
    switch(state)
    {
      case State::Default:
      {
        const auto line_pos = photo_line.fusion();
        Serial.print("line_pos:");
        Serial.print(line_pos);
        Serial.print(",");

        const auto pid_result = pid.calc(line_pos, 0);
        pid.update(line_pos);

        Serial.print("pid_result:");
        Serial.print(pid_result);
        Serial.print(",");

        if(photo_line.is_lost())
        {
          state = State::Lost;
          left_motor.mode.value = flip_mode(left_direction);
          right_motor.mode.value = flip_mode(right_direction);

          left_motor.pwm = 64;
          right_motor.pwm = 64;
        }

        if(pid_result > straight_threshold)
        {
          left_motor.pwm = max_speed;
          right_motor.pwm = max_speed * (128 - mystd::math_algo::abs(pid_result)) / 128;
        }
        else if(pid_result < -straight_threshold)
        {
          left_motor.pwm = max_speed * (128 - mystd::math_algo::abs(pid_result)) / 128;
          right_motor.pwm = max_speed;
        }
        else
        {
          left_motor.pwm = max_speed;
          right_motor.pwm = max_speed;
        }
      
        Serial.println();
      }
      break;

      case State::Lost:
      {
        if(!photo_line.is_lost())
        {
          state = State::Default;
          left_motor.mode.value = left_direction;
          right_motor.mode.value = right_direction;
        }
      }
      break;
    }

    Serial.print(state == State::Default ? "default" : "lost");

    left_motor.update();
    right_motor.update();
  }

  left_motor.pwm = 0;
  left_motor.update();
  right_motor.pwm = 0;
  right_motor.update();
}

void loop()
{
  volatile u8 dummy = 0;
  dummy = dummy + 1;
}