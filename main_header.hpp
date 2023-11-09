
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
      pinMode(in1_pin, OUTPUT);
      pinMode(in2_pin, OUTPUT);
      return Motor{in1_pin, in2_pin, is_forward_cw};
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

    void take_off_forward() noexcept
    {
      forward(128);
    }

    void take_off_backward() noexcept
    {
      backward(128);
    }
  };

  struct Photo final {

    static constexpr u16 photo_min = 894;
    static constexpr u16 photo_max = 1020;

    u8 pin;

    static auto make(const u8 pin) noexcept -> Photo
    {
      pinMode(pin, INPUT);
      return Photo{pin};
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

  struct Robot final {
    Motor left_motor;
    Motor right_motor;
    Photo photos[5];

    static auto make() noexcept -> Robot
    {
      return Robot {
        Motor::make(10, 11, true)
        , Motor::make(3, 9, false)
        , {
          Photo::make(A4)
          , Photo::make(A2)
          , Photo::make(A1)
          , Photo::make(A0)
          , Photo::make(A3)
        }
      };
    }
  };
}