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
	constexpr u32 photo_min = 770;
	constexpr i8 linepos_max = 64;

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
      // Serial.print("Photo::read ");
      // Serial.println(raw);
			const u32 cliped = mystd::math_algo::max(photo_min, mystd::math_algo::min(photo_max, raw));
			return 255 * cliped / (photo_max - photo_min);
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

		auto fusion() noexcept -> i8
		{
			i16 sensor_sum = 0;

			u8 sensor_values[n];

			for(u8 i = 0; i < n; ++i)
			{
				sensor_values[i] = this->photos[i].read();
				sensor_sum += sensor_values[i];
			}

			i16 linepos_sum = 0;
			for(u8 i = 0; i < n; ++i)
			{
				linepos_sum += (i16)sensor_values[i] * this->photos[i].position;
			}

			return linepos_sum / sensor_sum;
		}

    void show() noexcept
    {
      for(const auto& photo : photos)
      {
        Serial.print(photo.read());
        Serial.print(", ");
      }
      Serial.println();
    }

    void raw_show() noexcept
    {
      for(const auto& photo : photos)
      {
        Serial.print(photo.raw_read());
        Serial.print(", ");
      }
      Serial.println();
    }
	};
}

namespace jkb21::photo
{
	using impl::Photo;
	using impl::PhotoLine;
}