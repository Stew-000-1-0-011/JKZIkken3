#include "mystd/integer_types.hpp"
#include "sanitized_arduino.hpp"

namespace mystd {
  template<class T>
  auto max(const T l, const T r) noexcept -> T
  {
    return l < r ? r : l;
  }

  template<class T>
  auto min(const T l, const T r) noexcept -> T
  {
    return l < r ? l : r;
  }
}

namespace jkb21
{
  struct Motor final
  {
    u8 in1_pin;
    u8 in2_pin;
    bool is_forward_cw;

    static auto make(const u8 in1_pin, const u8 in2_pin, const bool is_forward_cw) noexcept -> Motor
    {
      return Motor{in1_pin, in2_pin, is_forward_cw};
    }

    void init() noexcept
    {
      pinMode(in1_pin, OUTPUT);
      pinMode(in2_pin, OUTPUT);
    }

    void forward(u8 pwm) noexcept
    {
      if(is_forward_cw) {
        analogWrite(in1_pin, pwm);
        analogWrite(in2_pin, 0);
      } else {
        analogWrite(in1_pin, 0);
        analogWrite(in2_pin, pwm);
      }
    }

    void backward(u8 pwm) noexcept
    {
      if(is_forward_cw) {
        analogWrite(in1_pin, 0);
        analogWrite(in2_pin, pwm);
      } else {
        analogWrite(in1_pin, pwm);
        analogWrite(in2_pin, 0);
      }
    }

    void brake() noexcept
    {
      analogWrite(in1_pin, 255);
      analogWrite(in2_pin, 255);
    }

    void stop() noexcept
    {
      analogWrite(in1_pin, 0);
      analogWrite(in2_pin, 0);
    }
  };

  struct Photo final {

    static constexpr u16 photo_min = 894;
    static constexpr u16 photo_max = 1020;

    u8 pin;

    static auto make(const u8 pin) noexcept -> Photo
    {
      return Photo{pin};
    }
    
    void init() noexcept
    {
      pinMode(pin, INPUT);
    }

    auto read() noexcept -> u8
    {
      const u16 raw = analogRead(this->pin);
			const u16 cliped = mystd::max<u16>(photo_min, mystd::min<u16>(photo_max, raw));
			return 255 * (cliped - photo_min) / (photo_max - photo_min);
    }
  };
}

namespace Mains {
  using namespace mystd::integer_types;
  using mystd::integer_types::u8;
  using mystd::integer_types::u16;
  using mystd::integer_types::u32;

  using jkb21::Motor;
  using jkb21::Photo;

  const auto left_motor = Motor::make(10, 11, true);
  const auto right_motor = Motor::make(3, 9, false);
  Photo photos[5] = {
    Photo::make(A4)
    , Photo::make(A2)
    , Photo::make(A1)
    , Photo::make(A0)
    , Photo::make(A3)
  };

  constexpr u8 threthold = 50;
  constexpr u8 sample_count = 20;

  constexpr u8 max_speed = 80;
  constexpr u8 middle_speed = 70;
  constexpr u8 min_speed = 60;

  void main_1()
  {
    Serial.begin(2'000'000);
    left_motor.init();
    right_motor.init();
    for(auto& photo: photos) photo.init();
    delay(3000);

    while(true) {
      u16 sensor_values[5]{};
      for(u8 sample_time = 0; sample_time < sample_count; ++sample_time) for(u8 i = 0; i < 5; ++i) {
        sensor_values[i] += photos[i].read();
      }

      u8 max_sensor = 0;
      u8 index = 0;
      for(u8 i = 0; i < 5; ++i) {
        const u8 this_sensor = sensor_values[i] / sample_count;
        // Serial.print("s");
        // Serial.print(i);
        // Serial.print(":");
        // Serial.print(this_sensor);
        // Serial.print(",");
        if(max_sensor < this_sensor) {
          max_sensor = this_sensor;
          index = i;
        }
      }
      // Serial.print("\n");

      if(max_sensor < threthold) {
        break;
      } else {
        switch(index) {
          case 0:
          left_motor.forward(min_speed);
          right_motor.forward(max_speed);
          break;
          
          case 1:
          left_motor.forward(middle_speed);
          right_motor.forward(max_speed);
          break;

          case 2:
          left_motor.forward(max_speed);
          right_motor.forward(max_speed);
          break;

          case 3:
          left_motor.forward(max_speed);
          right_motor.forward(middle_speed);
          break;

          case 4:
          left_motor.forward(max_speed);
          right_motor.forward(min_speed);
          break;
        }
      }
    }

    left_motor.brake();
    right_motor.brake();
    delay(500);

    while(true) {
      u16 sensor_values[5]{};
      for(u8 sample_time = 0; sample_time < sample_count; ++sample_time) for(u8 i = 0; i < 5; ++i) {
        sensor_values[i] += photos[i].read();
      }

      u8 max_sensor = 0;
      u8 index = 0;
      for(u8 i = 0; i < 5; ++i) {
        if(const u8 this_sensor = sensor_values[i] / sample_count; max_sensor < this_sensor) {
          max_sensor = this_sensor;
          index = i;
        }
      }

      switch(index) {
        case 0:
        left_motor.backward(max_speed);
        right_motor.backward(min_speed);
        break;
        
        case 1:
        left_motor.backward(max_speed);
        right_motor.backward(middle_speed);
        break;

        case 2:
        left_motor.backward(max_speed);
        right_motor.backward(max_speed);
        break;

        case 3:
        left_motor.backward(middle_speed);
        right_motor.backward(max_speed);
        break;

        case 4:
        left_motor.backward(min_speed);
        right_motor.backward(max_speed);
        break;
      }
    }
  }

  void main_2() {
    using jkb21::Motor;
    using jkb21::Photo;

    Serial.begin(2'000'000);
    left_motor.init();
    right_motor.init();
    for(auto& photo: photos) photo.init();
    delay(3000);

    go_forward: while(true) {
      u16 sensor_values[5]{};
      for(u8 sample_time = 0; sample_time < sample_count; ++sample_time) for(u8 i = 0; i < 5; ++i) {
        sensor_values[i] += photos[i].read();
      }

      u8 max_sensor = 0;
      u8 index = 0;
      for(u8 i = 0; i < 5; ++i) {
        if(const u8 this_sensor = sensor_values[i] / sample_count; max_sensor < this_sensor) {
          max_sensor = this_sensor;
          index = i;
        }
      }

      if(max_sensor < threthold) {
        break;
      } else {
        switch(index) {
          case 0:
          left_motor.forward(min_speed);
          right_motor.forward(max_speed);
          break;
          
          case 1:
          left_motor.forward(middle_speed);
          right_motor.forward(max_speed);
          break;

          case 2:
          left_motor.forward(max_speed);
          right_motor.forward(max_speed);
          break;

          case 3:
          left_motor.forward(max_speed);
          right_motor.forward(middle_speed);
          break;

          case 4:
          left_motor.forward(max_speed);
          right_motor.forward(min_speed);
          break;
        }
      }
    }

    left_motor.brake();
    right_motor.brake();
    delay(500);

    u32 start_time = millis();
    while(millis() - start_time < 5000) {
      left_motor.backward(middle_speed);
      right_motor.forward(middle_speed);
    }

    goto go_forward;
  }

  void main_3() {
    using jkb21::Motor;
    using jkb21::Photo;

    Serial.begin(2'000'000);
    left_motor.init();
    right_motor.init();
    for(auto& photo: photos) photo.init();
    delay(3000);

    while(true) {
      u16 sensor_values[5]{};
      for(u8 sample_time = 0; sample_time < sample_count; ++sample_time) for(u8 i = 0; i < 5; ++i) {
        sensor_values[i] += photos[i].read();
      }

      u8 max_sensor = 0;
      u8 index = 0;
      for(u8 i = 0; i < 5; ++i) {
        if(const u8 this_sensor = sensor_values[i] / sample_count; max_sensor < this_sensor) {
          max_sensor = this_sensor;
          index = i;
        }
      }

      if(max_sensor < threthold) {
        break;
      } else {
        switch(index) {
          case 0:
          left_motor.forward(min_speed);
          right_motor.forward(max_speed);
          break;
          
          case 1:
          left_motor.forward(middle_speed);
          right_motor.forward(max_speed);
          break;

          case 2:
          left_motor.forward(max_speed);
          right_motor.forward(max_speed);
          break;

          case 3:
          left_motor.forward(max_speed);
          right_motor.forward(middle_speed);
          break;

          case 4:
          left_motor.forward(max_speed);
          right_motor.forward(min_speed);
          break;
        }
      }
    }
  }
}

// void main_1()
// {
//   Serial.begin(9600);

//   using namespace mystd::integer_types;

//   auto left_motor = Motor::make(10, 11, true);
//   auto right_motor = Motor::make(3, 9, false);

//   auto line_pos = 0;
//   bool is_backing = false;

//   while (true)
//   {
//     if (photo_line.is_lost())
//     {
//       left_motor.mode.value = Mode::Brake;
//       right_motor.mode.value = Mode::Brake;

//       delay(500);

//       left_motor.mode.value = flip_mode(left_direction);
//       right_motor.mode.value = flip_mode(right_direction);
//       is_backing = true;
//     }

//     line_pos = photo_line.fusion() * (is_backing ? 1 : -1);

//     if (mystd::math_algo::abs(line_pos) < straight_threshold)
//     {
//       left_motor.pwm = max_speed;
//       right_motor.pwm = max_speed;
//     }
//     else if (line_pos > 0)
//     {
//       left_motor.pwm = max_speed;
//       right_motor.pwm = 0;
//     }
//     else
//     {
//       left_motor.pwm = 0;
//       right_motor.pwm = max_speed;
//     }

//     left_motor.update();
//     right_motor.update();
//   }

//   left_motor.pwm = 0;
//   left_motor.update();
//   right_motor.pwm = 0;
//   right_motor.update();
// }

void setup()
{
  Mains::main_1();
  // Mains::main_2();
  // Mains::main_3();
}

void loop()
{
  volatile u8 dummy = 0;
  dummy = dummy + 1;
}