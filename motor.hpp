#pragma once

#include "mystd/integer_types.hpp"
#include "mystd/utility.hpp"

#include "sanitized_arduino.hpp"

namespace jkb21::motor::impl
{
	using namespace mystd::integer_types;

	template<class T>
	struct RetainPrevious final
	{
		T value;
		private:
		T previous;

		public:
		static auto make(const T value) noexcept -> RetainPrevious
		{
			RetainPrevious result;
			result.value = value;
			result.previous = value;
			return result;
		}

		auto check() noexcept -> mystd::utility::Pair<T, T>
		{
			const auto previous = this->previous;
			this->previous = value;
			return {value, previous};
		}
	};

	enum class Mode: u8
	{
		Stop,
		Clockwise,
		CounterClockwise,
		Brake
	};

	struct Motor final
	{
		u8 in1_pin;
		u8 in2_pin;

		RetainPrevious<Mode> mode;
		u8 pwm;

		static auto make(const u8 in1_pin, const u8 in2_pin) noexcept -> Motor
		{
			pinMode(in1_pin, OUTPUT);
			pinMode(in2_pin, OUTPUT);

			return Motor
			{
				.in1_pin = in1_pin,
				.in2_pin = in2_pin,
				.mode = decltype(mode)::make(Mode::Stop),
				.pwm = 0
			};
		}

		void update() noexcept
		{
			const auto mode = this->mode.check();

			// 100us delay when mode changes from brake to cw/ccw or cw/ccw to ccw/cw or vice versa.
			if(mode.first != mode.second)
			{
				analogWrite(in1_pin, 0);
				analogWrite(in2_pin, 0);
				delayMicroseconds(100);
			}

			switch(mode.first)
			{
				case Mode::Stop:
				{
					analogWrite(in1_pin, 0);
					analogWrite(in2_pin, 0);
					break;
				}
				case Mode::Clockwise:
				{
					analogWrite(in1_pin, pwm);
					analogWrite(in2_pin, 0);
					break;
				}
				case Mode::CounterClockwise:
				{
					analogWrite(in1_pin, 0);
					analogWrite(in2_pin, pwm);
					break;
				}
				case Mode::Brake:
				{
					analogWrite(in1_pin, 255);
					analogWrite(in2_pin, 255);
					break;
				}
			}
		}
	};
}

namespace jkb21::motor
{
	using impl::Motor;
	using impl::Mode;
}