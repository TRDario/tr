///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements template.hpp.                                                                                                              //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../template.hpp"

////////////////////////////////////////////////////////////// STRING LITERAL /////////////////////////////////////////////////////////////

template <tr::usize Size>
consteval tr::string_literal<Size>::string_literal(const char (&str)[Size])
{
	std::ranges::copy(str, data);
}

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

template <tr::usize Size>
consteval tr::usize tr::string_literal<Size>::size()
{
	return Size - 1;
}

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

//////////////////////////////////////////////////////////////// TYPE NAME ////////////////////////////////////////////////////////////////

template <typename T>
consteval std::string_view tr::type_name()
{
#ifdef __clang__
	constexpr std::string_view function_name{std::source_location::current().function_name()};
	return std::string_view{function_name.begin() + 38, function_name.end() - 1};
#elifdef(_MSC_VER)
	constexpr std::string_view function_name{std::source_location::current().function_name()};
	return std::string_view{function_name.begin() + 88, function_name.end() - 7};
#elifdef(__GNUC__)
	constexpr std::string_view function_name{
		[]<typename U>() { return std::source_location::current().function_name(); }.template operator()<T>()};
	return std::string_view{function_name.begin() + 14, function_name.begin() + function_name.find("lambda") - 6};
#else
	static_assert(false, "tr::type_name is not implemented for this compiler!");
#endif
}

//////////////////////////////////////////////////////////////// SIZE TYPE ////////////////////////////////////////////////////////////////

template <tr::usize S>
	requires(S > UINT32_MAX)
struct tr::size_type<S>
{
	using type = u64;
};

template <tr::usize S>
	requires(S > UINT16_MAX && S <= UINT32_MAX)
struct tr::size_type<S>
{
	using type = u32;
};

template <tr::usize S>
	requires(S > UINT8_MAX && S <= UINT16_MAX)
struct tr::size_type<S>
{
	using type = u16;
};

template <tr::usize S>
	requires(S <= UINT8_MAX)
struct tr::size_type<S>
{
	using type = u8;
};