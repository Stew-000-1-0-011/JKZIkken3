namespace mystd {
	// std::type_identity is a type that is same as T. use this to avoid ambiguous deducing of T.
	template<class T>
	struct type_identity
	{
		using type = T;
	};

	// alias of type_identity<T>::type.
	template<class T>
	using type_identity_t = type_identity<T>::type;
}