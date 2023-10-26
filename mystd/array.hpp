#pragma once

#include "integer_types.hpp"

namespace mystd::array::impl
{
	template<class T, usize n>
	struct Array final
	{
		T elements[n]{};

		auto operator[](const usize index) noexcept -> T&
		{
			return elements[index];
		}

		auto operator[](const usize index) noexcept -> const T&
		{
			return elements[index];
		}
	};
}

namespace mystd::array
{
	using impl::Array;
}