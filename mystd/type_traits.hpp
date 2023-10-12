#pragma once

namespace mystd::type_traits {
	// std::type_identity is a type that is same as T. use this to avoid ambiguous deducing of T.
	template<class T>
	struct type_identity
	{
		using type = T;
	};

	// alias of type_identity<T>::type.
	template<class T>
	using type_identity_t = typename type_identity<T>::type;

	template <class T, class U>
	struct is_same
	{
		static constexpr bool value = false;
	};

	template <class T>
	struct is_same<T, T>
	{
		static constexpr bool value = true;
	};

	template <class T, class U>
	inline constexpr bool is_same_v = is_same<T, U>::value;
}