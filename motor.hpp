#pragma once

#include "fake_arduino.hpp"
#include "mystd.hpp"

// import integer_types to global namespace.

namespace jkb21::impl
{
	using namespace mystd::integer_types;

	namespace Wrong
	{
		struct Motor final
		{
			u8 pwm_pin;
			u8 in1_pin;
			u8 in2_pin;

			// pin initialization
			void enable() const
			{
				pinMode(pwm_pin, OUTPUT);
				pinMode(in1_pin, OUTPUT);
				pinMode(in2_pin, OUTPUT);
			}

			// update motor
			void update_speed(i32 speed) const
			{
				const u32 absolute_speed = mystd::abs(speed);
				const bool is_negative = mystd::signbit(speed);
				
				// signedness
				if(is_negative)
				{
				digitalWrite(in1_pin, LOW);
				digitalWrite(in2_pin, HIGH);
				}
				else
				{
				digitalWrite(in1_pin, HIGH);
				digitalWrite(in2_pin, LOW);
				}


				const u8 fixed_speed = mystd::min(absolute_speed, 255);

				// absolute
				analogWrite(pwm_pin, fixed_speed);
			}
		};
	}

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
		Mode mode;

		static auto make(const u8 in1_pin, const u8 in2_pin) noexcept -> Motor
		{
			pinMode(in1_pin, OUTPUT);
			pinMode(in2_pin, OUTPUT);
			return Motor{in1_pin, in2_pin, Mode::Stop};
		}

		void update_speed(const i32 speed) const noexcept
		{
			const u32 absolute_speed = mystd::abs(speed);
			const bool is_negative = mystd::signbit(speed);

			switch(mode)
			{
			case Mode::Stop:
				digitalWrite(in1_pin, LOW);
				digitalWrite(in2_pin, LOW);
				break;
			case Mode::Clockwise:
				digitalWrite(in1_pin, HIGH);
				digitalWrite(in2_pin, LOW);
				break;
			case Mode::CounterClockwise:
				digitalWrite(in1_pin, LOW);
				digitalWrite(in2_pin, HIGH);
				break;
			case Mode::Brake:
				digitalWrite(in1_pin, HIGH);
				digitalWrite(in2_pin, HIGH);
				break;
			}

			const u8 fixed_speed = mystd::min(absolute_speed, 255);

			analogWrite(in1_pin, fixed_speed);
		}
	};
}

namespace jkb21
{
	using impl::Motor;
}