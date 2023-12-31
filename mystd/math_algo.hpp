#pragma once

#include "initializer_list.hpp"
#include "type_traits.hpp"

namespace mystd::math_algo {
	// one of std::min overloads.
	template<class T>
	inline constexpr auto min(const T& l, const type_traits::type_identity_t<T>& r) noexcept -> T
	{
		// conditional operator. this is a ternary operator.
		return l < r ? l : r;
	}

	template<class T>
	inline constexpr auto max(const T& l, const type_traits::type_identity_t<T>& r) noexcept -> T
	{
		return l < r ? r : l;
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

	inline constexpr auto safe_add(const auto l, const auto r)
	{
		return (r > 0 && l > 127 - r) ? 127 : (r < 0 && l < -128 - r) ? -128 : l + r;
	}
}