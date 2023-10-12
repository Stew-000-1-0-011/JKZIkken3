#pragma once

#include "type_traits.hpp"
#include "integer_types.hpp"

namespace mystd::concepts {
	template<class T, class U>
	concept same_as = type_traits::is_same_v<T, U>;

	template<class T>
	concept integral =
	same_as<T, i8> || same_as<T, i16> || same_as<T, i32> || same_as<T, i64>
	|| same_as<T, u8> || same_as<T, u16> || same_as<T, u32> || same_as<T, u64>
	|| same_as<T, usize> || same_as<T, ubyte> || same_as<T, char>;

	template<class T>
	concept floating_point = same_as<T, float> || same_as<T, double>;
}