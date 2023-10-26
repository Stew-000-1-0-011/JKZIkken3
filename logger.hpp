#pragma once

#error Serial.printも同様の処理をしているらしい。不要。

// #include <string.h>

// #include "mystd/integer_types.hpp"
// #include "mystd/utility.hpp"
// #include "mystd/concepts.hpp"

// namespace jkb21::logger::impl
// {
// 	using namespace mystd::integer_types;

// 	template <class T>
// 	concept memcpy_packable = mystd::concepts::integral<T> || mystd::concepts::floating_point<T>;

// 	template <memcpy_packable T>
// 	struct MemcpyPack final
// 	{
// 		alignas(T) ubyte buffer[sizeof(T) + 1];

// 		static auto make(const T &value) noexcept -> MemcpyPack
// 		{
// 			constexpr auto get_log2 = [](usize x) noexcept -> u8
// 			{
// 				u8 ret = 0;
// 				while (x >>= 1) ++ret;
// 				return ret + 1;
// 			};

// 			MemcpyPack result;
// 			result.buffer[0] = (mystd::concepts::integral<T> ? 1 : 0) << 6
// 			| (mystd::concepts::unsigned_integral<T> ? 1 : 0) << 5
// 			| get_log2(sizeof(T));
// 			memcpy(result.buffer + 1, &value, sizeof(T));
// 			return result;
// 		}

// 		bool push_to(ubyte *const begin, const usize n) const noexcept
// 		{
// 			if (sizeof(T) + 1 > n)
// 				return false;
// 			memcpy(begin, this->buffer, sizeof(T) + 1);
// 			return true;
// 		}
// 	};

// 	struct StringLiteralPack final
// 	{
// 		const char *string_literal;
// 		u8 length;

// 		template <usize n>
// 		requires (n < (1 << 7))
// 		static auto make(const char (&string_literal)[n]) noexcept -> StringLiteralPack
// 		{
// 			return StringLiteralPack{string_literal, n};
// 		}

// 		bool push_to(ubyte *const begin, const usize n) const noexcept
// 		{
// 			if ((usize)(length + 1) > n) return false;

// 			begin[0] = 0b1000'0000 | length;
// 			memcpy(begin + 1, string_literal, length);
// 			return true;
// 		}
// 	};

// 	template <class T>
// 	concept packable =
// 		memcpy_packable<T> || requires(const T &imut) {
// 			[]<usize n>(const char(&)[n]) {}(imut);
// 		};

// 	template <packable T>
// 	auto pack(const T &value) noexcept
// 	{
// 		if constexpr (memcpy_packable<T>)
// 		{
// 			return MemcpyPack<T>::make(value);
// 		}
// 		else
// 		{
// 			return StringLiteralPack::make(value);
// 		}
// 	}

// 	enum class DepackState : u8
// 	{
// 		ReadHead,
// 		ReadingMemcpy,
// 		ReadingStringLiteral
// 	};

// 	template <usize n>
// 	struct Logger final
// 	{
// 		ubyte buffer[n];
// 		usize index;

// 		static auto make() noexcept -> Logger
// 		{
// 			return Logger{};
// 		}

// 		template <packable... Args>
// 		void log(const Args &...args) noexcept
// 		{
// 			bool continue_logging = true;
// 			((continue_logging ? continue_logging = pack(args).push_to(buffer + index, n - index) : false), ...);
// 		}

// 		void flush() noexcept
// 		{
// 			DepackState state{DepackState::ReadHead};

// 			for (usize i = 0; i < index; ++i)
// 			{
// 				switch (state)
// 				{
// 					case DepackState::ReadHead:
// 					if (buffer[i] == 0)
// 					{
// 						return;
// 					}
// 					else if (buffer[i] & 0b1000'0000)
// 					{
// 						state = DepackState::ReadingStringLiteral;
// 						const u8 length = buffer[i] & 0b0111'1111;
// 						Serial.print(buffer + i + 1);
// 						i += length;
// 					}
// 					else
// 					{
// 						state = DepackState::ReadingMemcpy;
// 						is_float = buffer[i] & 0b0100'0000;
// 						is_signed = buffer[i] & 0b0010'0000;
// 						memcpy_size = 1 << ((buffer[i] & 0b0001'1111) - 1);
// 					}
// 					break;

// 					case DepackState::ReadingMemcpy:
					
// 				}
// 			}
// 		}
// 	};
// }

// namespace jkb21::logger
// {
// 	using impl::Logger;
// }