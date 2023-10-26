#include "Arduino.h"
#pragma once

#include "mystd/integer_types.hpp"
#include "mystd/math_algo.hpp"

#include "sanitized_arduino.hpp"

namespace jkb21::photo::impl
{
	using namespace mystd::integer_types;
	using u32 = mystd::u32;
  using u16 = mystd::u16;

	/// @brief  @todo: この値は適当
	constexpr u32 photo_max = 1000;
	constexpr u32 photo_min = 800;
	constexpr i8 linepos_max = 64;
  constexpr u8 sensor_read_count = 20;

	struct Photo final
	{
		u8 pin;
		i8 position;

		static auto make(const u8 pin, const i8 position) noexcept -> Photo
		{
			pinMode(pin, INPUT);
			return Photo{pin, position};
		}

		auto read() const noexcept -> u8
		{
			const i32 raw = analogRead(this->pin);
			const u32 cliped = mystd::math_algo::max(photo_min, mystd::math_algo::min(photo_max, raw));
			return 255 * (cliped - photo_min) / (photo_max - photo_min);
		}

    auto raw_read() const noexcept -> u16
    {
			 return analogRead(this->pin);
    }
	};

	template<u8 n>
	struct PhotoLine final
	{
		Photo photos[n];

		static auto make(const Photo (&photos)[n]) noexcept -> PhotoLine
		{
			PhotoLine result;
			for(usize i = 0; i < n; ++i) result.photos[i] = photos[i];
			return result;
		}

    auto is_lost() noexcept -> bool
    {
      i32 sensor_values[n]{};

      for(u8 i = 0; i < sensor_read_count; ++i)
      {
        for(u8 i_n = 0; i_n < n; ++i_n)
        {
          sensor_values[i_n] += this->photos[i_n].read();
        }
      }

      i32 sensor_sum = 0;
      for(const i32 v : sensor_values) sensor_sum += v;

      return sensor_sum < 20 * sensor_read_count * n;
    }

    auto is_not_lost() noexcept -> bool
    {
      i32 sensor_values[n]{};

      for(u8 i = 0; i < sensor_read_count; ++i)
      {
        for(u8 i_n = 0; i_n < n; ++i_n)
        {
          sensor_values[i_n] += this->photos[i_n].read();
        }
      }

      i32 sensor_sum = 0;
      for(const i32 v : sensor_values) sensor_sum += v;

      return sensor_sum > 50 * sensor_read_count * n;
    }

    auto is_all_black() noexcept -> bool
    {
      i32 sensor_values[n]{};

      for(u8 i = 0; i < sensor_read_count; ++i)
      {
        for(u8 i_n = 0; i_n < n; ++i_n)
        {
          sensor_values[i_n] += this->photos[i_n].read();
        }
      }

      i32 sensor_sum = 0;
      for(const i32 v : sensor_values) sensor_sum += v;

      Serial.print("sensor_sum:");
      Serial.println(sensor_sum / sensor_read_count / n);

      return sensor_sum > 245 * sensor_read_count * n;
    }

		auto fusion() noexcept -> i32
		{
      i32 sensor_values[n]{};

      for(u8 i = 0; i < sensor_read_count; ++i)
      {
        for(u8 i_n = 0; i_n < n; ++i_n)
        {
          sensor_values[i_n] += this->photos[i_n].read();
        }
      }

      i32 sensor_sum = 0;
      for(const i32 v : sensor_values) sensor_sum += v;

      i32 linepos_sum = 0;
      for(u8 i_n = 0; i_n < n; ++i_n)
      {
        linepos_sum += (i32)sensor_values[i_n] * this->photos[i_n].position;
      }

      return linepos_sum / sensor_sum;
		}

    void show() noexcept
    {
      for(u8 i = 0; const auto& photo : photos)
      {
        Serial.print("show");
        Serial.print(i);
        Serial.print(":");
        Serial.print(photo.read());
        Serial.print(",");
        ++i;
      }
    }

    void raw_show() noexcept
    {
      for(u8 i = 0; const auto& photo : photos)
      {
        Serial.print("raw_show");
        Serial.print(i);
        Serial.print(":");
        Serial.print(photo.raw_read());
        Serial.print(",");

        ++i;
      }
    }
	};
}

namespace jkb21::photo
{
	using impl::Photo;
	using impl::PhotoLine;
}