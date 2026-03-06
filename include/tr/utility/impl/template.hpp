///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements template.hpp.                                                                                                              //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../template.hpp"

////////////////////////////////////////////////////////////// STRING LITERAL /////////////////////////////////////////////////////////////

template <tr::usize Size> consteval tr::string_literal<Size>::string_literal(const char (&str)[Size])
{
	std::ranges::copy(str, data);
}

template <tr::usize Size> consteval tr::string_literal<Size>::operator const char*() const
{
	return data;
}

template <tr::usize Size> consteval tr::string_literal<Size>::operator std::string_view() const
{
	return data;
}

template <tr::usize Size> template <typename... Args> consteval tr::string_literal<Size>::operator TR_FORMAT_STRING<Args...>() const
{
	return std::string_view{*this};
}

//////////////////////////////////////////////////////////////// TYPE NAME ////////////////////////////////////////////////////////////////

template <typename T> consteval std::string_view tr::type_name()
{
#ifdef __clang__
	constexpr std::string_view function_name{std::source_location::current().function_name()};
	return std::string_view{function_name.begin() + 38, function_name.end() - 1};
#elif defined(_MSC_VER)
	constexpr std::string_view function_name{std::source_location::current().function_name()};
	return std::string_view{function_name.begin() + 88, function_name.end() - 7};
#elif defined(__GNUC__)
	constexpr std::string_view function_name{
		[]<typename U>() { return std::source_location::current().function_name(); }.template operator()<T>()};
	return std::string_view{function_name.begin() + 14, function_name.begin() + function_name.find("lambda") - 6};
#else
	static_assert(false, "tr::type_name is not implemented for this compiler!");
#endif
}

//////////////////////////////////////////////////////////////// SIZE TYPE ////////////////////////////////////////////////////////////////

template <tr::usize S> struct tr::size_type<S, std::enable_if_t<(S > UINT32_MAX)>> {
	using type = u64;
};
template <tr::usize S> struct tr::size_type<S, std::enable_if_t<(S > UINT16_MAX && S <= UINT32_MAX)>> {
	using type = u32;
};
template <tr::usize S> struct tr::size_type<S, std::enable_if_t<(S > UINT8_MAX && S <= UINT16_MAX)>> {
	using type = u16;
};
template <tr::usize S> struct tr::size_type<S, std::enable_if_t<(S <= UINT8_MAX)>> {
	using type = u8;
};

/////////////////////////////////////////////////////////// IS SPECIALIZATION OF //////////////////////////////////////////////////////////

template <typename T, template <typename...> typename Template> struct tr::is_specialization_of : std::false_type {};
template <typename... Args, template <typename...> typename Template>
struct tr::is_specialization_of<Template<Args...>, Template> : std::true_type {};

template <typename T, template <auto...> typename Template> struct tr::is_specialization_of_v : std::false_type {};
template <auto... Values, template <auto...> typename Template>
struct tr::is_specialization_of_v<Template<Values...>, Template> : std::true_type {};

template <typename T, template <auto, typename...> typename Template> struct tr::is_specialization_of_vt : std::false_type {};
template <auto S, typename... Args, template <auto, typename...> typename Template>
struct tr::is_specialization_of_vt<Template<S, Args...>, Template> : std::true_type {};

template <typename T, template <typename, auto, typename...> typename Template> struct tr::is_specialization_of_tv : std::false_type {};
template <typename T, auto S, typename... Args, template <typename, auto, typename...> typename Template>
struct tr::is_specialization_of_tv<Template<T, S, Args...>, Template> : std::true_type {};

///////////////////////////////////////////////////////////// FUNCTION TRAITS /////////////////////////////////////////////////////////////

template <typename Return, typename... Args> struct tr::function_traits<Return(Args...)> {
	using return_type = Return;
	using args_tuple = std::tuple<Args...>;
	template <usize N> using nth_arg_type = std::tuple_element_t<N, args_tuple>;
};
template <typename Return, typename Class, typename... Args> struct tr::function_traits<Return (Class::*)(Args...)> {
	using return_type = Return;
	using class_type = Class;
	using args_tuple = std::tuple<Args...>;
	template <usize N> using nth_arg_type = std::tuple_element_t<N, args_tuple>;
};
template <typename Return, typename... Args> struct tr::function_traits<Return (*)(Args...)> : function_traits<Return(Args...)> {};
template <typename Return, typename... Args> struct tr::function_traits<Return (&)(Args...)> : function_traits<Return(Args...)> {};
template <typename Return, typename... Args> struct tr::function_traits<Return(Args...) noexcept> : function_traits<Return(Args...)> {};
template <typename Return, typename... Args> struct tr::function_traits<Return (*)(Args...) noexcept> : function_traits<Return(Args...)> {};
template <typename Return, typename... Args> struct tr::function_traits<Return (&)(Args...) noexcept> : function_traits<Return(Args...)> {};
template <typename Return, typename Class, typename... Args>
struct tr::function_traits<Return (Class::*)(Args...) const> : function_traits<Return (Class::*)(Args...)> {};
template <typename Return, typename Class, typename... Args>
struct tr::function_traits<Return (Class::*)(Args...) &> : function_traits<Return (Class::*)(Args...)> {};
template <typename Return, typename Class, typename... Args>
struct tr::function_traits<Return (Class::*)(Args...) const&> : function_traits<Return (Class::*)(Args...)> {};
template <typename Return, typename Class, typename... Args>
struct tr::function_traits<Return (Class::*)(Args...) &&> : function_traits<Return (Class::*)(Args...)> {};
template <typename Return, typename Class, typename... Args>
struct tr::function_traits<Return (Class::*)(Args...) const&&> : function_traits<Return (Class::*)(Args...)> {};
template <typename Return, typename Class, typename... Args>
struct tr::function_traits<Return (Class::*)(Args...) noexcept> : function_traits<Return (Class::*)(Args...)> {};
template <typename Return, typename Class, typename... Args>
struct tr::function_traits<Return (Class::*)(Args...) const noexcept> : function_traits<Return (Class::*)(Args...)> {};
template <typename Return, typename Class, typename... Args>
struct tr::function_traits<Return (Class::*)(Args...) & noexcept> : function_traits<Return (Class::*)(Args...)> {};
template <typename Return, typename Class, typename... Args>
struct tr::function_traits<Return (Class::*)(Args...) const & noexcept> : function_traits<Return (Class::*)(Args...)> {};
template <typename Return, typename Class, typename... Args>
struct tr::function_traits<Return (Class::*)(Args...) && noexcept> : function_traits<Return (Class::*)(Args...)> {};
template <typename Return, typename Class, typename... Args>
struct tr::function_traits<Return (Class::*)(Args...) const && noexcept> : function_traits<Return (Class::*)(Args...)> {};