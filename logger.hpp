#pragma once

#include <string.h>

#include "mystd/integer_types.hpp"
#include "mystd/utility.hpp"
#include "mystd/concepts.hpp"

namespace jkb21::logger::impl {
	using namespace mystd::integer_types;

	template<class T>
	concept memcpy_packable = mystd::concepts::integral<T> || mystd::concepts::floating_point<T>;

	template<memcpy_packable T>
	struct MemcpyPack final
	{
		alignas(T) ubyte buffer[sizeof(T) + 1];

		static auto make(const T& value) noexcept -> MemcpyPack
		{
			MemcpyPack result;
			result.buffer[0] = (mystd::concepts::integral<T> ? 0 : 1) << 6 | sizeof(T);
			memcpy(result.buffer + 1, &value, sizeof(T));
			return result;
		}

		bool push_to(ubyte *const begin, const usize n) const noexcept
		{
			if(sizeof(T) + 1 > n) return false;
			memcpy(begin, this->buffer, sizeof(T) + 1);
			return true;
		}
	};

	struct StringLiteralPack final
	{
		const char * string_literal;
		u8 length;

		template<usize n>
		static auto make(const char (& string_literal)[n]) noexcept -> StringLiteralPack
		{
			return StringLiteralPack{string_literal, n - 1};
		}

		bool push_to(ubyte *const begin, const usize n) const noexcept
		{
			if((usize)(length + 1) > n) return false;
			begin[0] = 0b1000'0000 | length;
			memcpy(begin + 1, string_literal, length);
			return true;
		}
	};

	template<class T>
	concept packable =
	memcpy_packable<T>
	|| requires(const T& imut)
	{
		[]<usize n>(const char (&)[n]){}(imut);
	};

	template<packable T>
	auto pack(const T& value) noexcept
	{
		if constexpr(memcpy_packable<T>)
		{
			return MemcpyPack<T>::make(value);
		}
		else
		{
			return StringLiteralPack::make(value);
		}
	}

	template<usize n>
	struct Logger final
	{
		ubyte buffer[n];
		usize index;

		static auto make() noexcept -> Logger
		{
			return Logger{};
		}

		template<packable ... Args>
		void log(const Args& ... args) noexcept
		{
			bool continue_logging = true;
			((continue_logging ? continue_logging = pack(args).push_to(buffer + index, n - index) : false), ...);
		}
	};
}

namespace jkb21::logger {
	using impl::Logger;
}