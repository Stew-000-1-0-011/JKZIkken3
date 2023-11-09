#pragma once

#include "main_header.hpp"

namespace Mains
{
	constexpr auto sample_count = 20;
	constexpr auto threthold = 50;
	constexpr u16 brake_prescaler = 2;
  constexpr u8 multiplier = 3;

	enum class State : u8
	{
		Forward,
		Left,
		Right
	};

	void main_3()
	{
		Serial.begin(2'000'000);
		auto robot = Mains::Robot::make();
		auto state = State::Forward;
		u8 brake_count = 0;
		u16 brake_timer = 0;

		delay(2000);

		while (true)
		{
			u16 sensor_values[5]{};
			for (u8 sample_time = 0; sample_time < sample_count; ++sample_time)
				for (u8 i = 0; i < 5; ++i)
				{
					sensor_values[i] += robot.photos[i].read();
				}

			for (u8 i = 0; i < 5; ++i)
			{
				Serial.print("s");
				Serial.print(i);
				Serial.print(":");
				Serial.print(sensor_values[i]);
				Serial.print(",");
			}
			Serial.println("");

			bool is_black_l = sensor_values[1] > threthold;
			bool is_black_r = sensor_values[3] > threthold;

			if (is_black_l && is_black_r)
			{
				state = State::Forward;
				brake_count = 0;
			}
			else if (!is_black_r)
			{
				state = State::Right;
			}
			else if (!is_black_l)
			{
				state = State::Left;
			}

			if (brake_timer == brake_prescaler - 1)
			{
				brake_timer = 0;
				brake_count = brake_count == 255 ? 255 : brake_count + 1;
			}
			else
			{
				brake_timer = brake_timer + 1;
			}

			switch (state)
			{
			case State::Forward:
				robot.left_motor.forward(255);
				robot.right_motor.forward(255);
				break;

			case State::Left:
				robot.left_motor.forward(mystd::max<u8>(255 - multiplier * brake_count, 0));
				robot.right_motor.forward(255);
				break;

			case State::Right:
				robot.left_motor.forward(255);
				robot.right_motor.forward(mystd::max<u8>(255 - multiplier * brake_count, 0));
			}

			delay(1);
		}
	}
}