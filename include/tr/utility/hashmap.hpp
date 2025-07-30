#pragma once
#include "static_string.hpp"

namespace tr {
	template <enumerator T> struct enum_hash {
		auto operator()(T arg) const;
	};
	struct string_hash {
		using is_transparent = std::true_type;

		inline auto operator()(std::string_view str) const;
	};
	struct string_eq {
		using is_transparent = std::true_type;

		constexpr bool operator()(std::string_view l, std::string_view r) const;
	};

	// Typedef for a enumerator-key hash map.
	template <enumerator Key, class Value> using enum_hash_map = std::unordered_map<Key, Value, enum_hash<Key>>;
	// Typedef for a string-key hash map.
	template <class Value> using string_hash_map = std::unordered_map<std::string, Value, string_hash, string_eq>;
	// Typedef for a static string-key hash map.
	template <std::size_t S, class Value>
	using static_string_hash_map = std::unordered_map<static_string<S>, Value, string_hash, string_eq>;
} // namespace tr

template <tr::enumerator T> auto tr::enum_hash<T>::operator()(T arg) const
{
	return std::hash<std::underlying_type_t<T>>{}(std::underlying_type_t<T>(arg));
}

auto tr::string_hash::operator()(std::string_view str) const
{
	return std::hash<std::string_view>{}(str);
}

constexpr bool tr::string_eq::operator()(std::string_view l, std::string_view r) const
{
	return l == r;
}