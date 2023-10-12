#pragma once

#error "Too heavy on 8bit microcontroller."

#include "utility.hpp"

namespace mystd::option::impl {
	struct None final{};

	template<class T>
	union Storage
	{
		None empty;
		T value;
	};

	template<class T>
	struct Option final
	{
		static_assert(!type_traits::is_reference_v<T>, "T must not be a reference type.");

		Storage<T> storage;
		bool has_value;

		private:
		Option() noexcept
			: storage()
			, has_value(false)
		{}

		public:
		Option(const Option&) = delete;
		Option& operator=(const Option&) = delete;

		Option(Option&& other) noexcept
			: storage()
			, has_value(other.has_value)
		{
			if(has_value)
			{
				new (&storage.value) T(mystd::utility::move(other.storage.value));
			}
		}
		
		Option& operator=(Option&& other) noexcept
		{
			if(has_value)
			{
				storage.value.~T();
			}
			
			has_value = other.has_value;

			if(has_value)
			{
				new (&storage.value) T(mystd::utility::move(other.storage.value));
			}
			return *this;
		}

		~Option()
		{
			if(has_value)
			{
				storage.value.~T();
			}
		}

		auto some(T&& value) -> Option
		{
			Option result;
			result.has_value = true;
			new (&result.storage.value) T(mystd::utility::move(value));
			return result;
		}

		auto none() -> Option
		{
			return Option();
		}
	};
}

namespace mystd::option {
	using impl::Option;
}