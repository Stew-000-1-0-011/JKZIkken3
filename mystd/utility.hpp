#pragma once

#include "type_traits.hpp"
#include "integer_types.hpp"

namespace mystd::utility::impl {
	template<class T>
	inline auto forward(type_traits::type_identity_t<T>&& value) noexcept -> T&&
	{
		return static_cast<T&&>(value);
	}

	template<class T>
	inline auto move(T&& value) noexcept -> T&&
	{
		return static_cast<T&&>(value);
	}

	template<class T1, class T2>
	struct Pair final
	{
		T1 first;
		T2 second;
	};
}

namespace mystd::utility {
	using impl::forward;
	using impl::move;
	using impl::Pair;
}