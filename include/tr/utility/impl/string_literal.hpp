/// @file
/// @brief Implements string_literal.hpp.

#pragma once
#include "../string_literal.hpp"

//

template <tr::usize Size>
consteval tr::string_literal<Size>::string_literal(const char (&str)[Size])
{
	std::ranges::copy(str, data);
}

//

template <tr::usize Size>
consteval tr::string_literal<Size>::operator const char*() const
{
	return data;
}

template <tr::usize Size>
consteval tr::string_literal<Size>::operator std::string_view() const
{
	return data;
}

template <tr::usize Size>
template <typename... Args>
consteval tr::string_literal<Size>::operator std::format_string<Args...>() const
{
	return std::string_view{*this};
}

//

template <tr::usize Size>
consteval tr::usize tr::string_literal<Size>::size()
{
	return Size - 1;
}

//

template <typename First, typename Second, typename... Rest>
consteval auto tr::concatenate_string_literals(First&& first, Second&& second, Rest&&... rest)
{
	if constexpr (sizeof...(Rest) == 0) {
		const tr::string_literal left{first};
		const tr::string_literal right{second};
		tr::string_literal<decltype(left)::size() + decltype(right)::size() + 1> concatenated{{}};
		std::ranges::copy(left.data, std::ranges::begin(concatenated.data));
		std::ranges::copy(right.data, std::ranges::begin(concatenated.data) + left.size());
		return concatenated;
	}
	else {
		return concatenate_string_literals(concatenate_string_literals(std::forward<First>(first), std::forward<Second>(second)),
										   std::forward<Rest>(rest)...);
	}
}