#pragma once

// to use fixed bit width integer type
#include <stdint.h>
// to use size_t
#include <stddef.h>

namespace mystd::integer_types {
	// alias types of fixed bit width integer types.
	using u8 = uint8_t;
	using u16 = uint16_t;
	using u32 = uint32_t;
	using u64 = uint64_t;
	using i8 = int8_t;
	using i16 = int16_t;
	using i32 = int32_t;
	using i64 = int64_t;

	using usize = size_t;
	using ubyte = unsigned char;
}

namespace mystd {
	using namespace mystd::integer_types;
}