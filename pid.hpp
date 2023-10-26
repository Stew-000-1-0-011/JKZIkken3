#pragma once

#include "mystd/integer_types.hpp"
#include "mystd/math_algo.hpp"
#include "mystd/concepts.hpp"
#include "mystd/limits.hpp"

namespace jkb21::pid::impl
{
	using namespace mystd::integer_types;

	template<mystd::concepts::integral T>
	struct Pid final
	{
		const T kp;
		const T ki;
		const T kd;
		T integral;

		static auto make(const T kp, const T ki, const T kd) noexcept -> Pid
		{
			return Pid{kp, ki, kd, 0};
		}

		auto calc(const T error, const T previous_error) const noexcept -> i32
		{
			return kp * error + ki * integral + kd * (error - previous_error);
		}

		void update(const i8 error) noexcept
		{
			integral += error;
			integral = mystd::math_algo::min(mystd::math_algo::max(integral, mystd::limits::Limits<T>::min / 2), mystd::limits::Limits<T>::max / 2);
		}
	};
}

namespace jkb21::pid
{
	using impl::Pid;
}