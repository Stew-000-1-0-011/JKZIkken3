#pragma once

#include "integer_types.hpp"

namespace mystd::limits::impl
{
	template<class T>
	struct Limits;
	
	template<>
	struct Limits<u8>
	{
		static constexpr u8 min = 0;
		static constexpr u8 max = 255;
	};

	template<>
	struct Limits<i8>
	{
		static constexpr i8 min = -128;
		static constexpr i8 max = 127;
	};

	template<>
	struct Limits<u16>
	{
		static constexpr u16 min = 0;
		static constexpr u16 max = 65535;
	};

	template<>
	struct Limits<i16>
	{
		static constexpr i16 min = -32768;
		static constexpr i16 max = 32767;
	};

	template<>
	struct Limits<u32>
	{
		static constexpr u32 min = 0;
		static constexpr u32 max = 4294967295;
	};

	template<>
	struct Limits<i32>
	{
		static constexpr i32 min = -2147483648;
		static constexpr i32 max = 2147483647;
	};
}

namespace mystd::limits
{
	using impl::Limits;
}