#pragma once

namespace mystd::optional::impl {
	struct EmptyByte final{};

	template<class T>
	union Storage
	{
		EmptyByte empty;
		T value;
	};

	template<class T>
	struct optional final
	{
		Storage<T> storage;
		bool has_value;
	};
}

namespace mystd::optional {
	using impl::optional;
}