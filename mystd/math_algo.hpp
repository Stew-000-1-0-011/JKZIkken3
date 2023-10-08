#pragma once

#include "type_traits.hpp"

namespace mystd {
	// one of std::min overloads.
	template<class T>
	inline constexpr auto min(const T& l, const type_identity_t<T>& r) noexcept -> T
	{
		// conditional operator. this is a ternary operator.
		return l < r ? l : r;
	}

	// one of std::abs overloads.
	template<class T>
	inline constexpr auto abs(const T& v) noexcept -> T
	{
		return v < 0 ? -v : v;
	}


	// one of std::signbit overloads.
	template<class T>
	inline constexpr auto signbit(const T& v) noexcept -> bool
	{
		return v < 0;
	}
}