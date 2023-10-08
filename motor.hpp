#pragma once

#include "fake_arduino.hpp"
#include "mystd.hpp"

// import integer_types to global namespace.

namespace jkb21::impl
{
	using namespace mystd::integer_types;

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

namespace jkb21
{
	using impl::Motor;
}