#pragma once
#include "integer.hpp"

namespace tr {
	// Circumvents static_assert(false) not being a valid expression.
	template <class...> inline constexpr bool ALWAYS_FALSE = false;

	// Tag class.
	template <typename> struct tag {};

	// Template for string literals passed as template arguments.
	template <usize N> struct template_string_literal {
		char data[N - 1];

		consteval template_string_literal(const char (&str)[N])
		{
			std::copy_n(str, N - 1, data);
		}

		consteval operator std::string_view() const
		{
			return {data, N - 1};
		}

		template <class... Args> consteval operator TR_FORMAT_STRING<Args...>() const
		{
			return std::string_view{*this};
		}
	};

	// Concept wrapper over std::is_arithmetic_v.
	template <class T>
	concept arithmetic = std::is_arithmetic_v<T>;

	// Concept denoting a cv-unqualified object.
	template <class T>
	concept cv_unqualified_object = std::is_object_v<T> && !std::is_const_v<T> && !std::is_volatile_v<T>;
	// Concept denoting a non-const reference.
	template <class T>
	concept non_const_ref = std::is_lvalue_reference_v<T> && !std::is_const_v<std::remove_reference_t<T>>;
	// Concept denoting a reference to const.
	template <class T>
	concept const_ref = std::is_lvalue_reference_v<T> && std::is_const_v<std::remove_reference_t<T>>;
	// Concept wrapper over std::is_enum_v.
	template <class T>
	concept enumerator = std::is_enum_v<T>;
	// Concept wrapper over std::is_standard_layout_v.
	template <class T>
	concept standard_layout = std::is_standard_layout_v<T>;
	// Concept denoting a contiguous range of standard layout objects.
	template <class T>
	concept standard_layout_range = std::ranges::contiguous_range<T> && standard_layout<std::ranges::range_value_t<T>>;
	// Concept denoting an output range.
	template <class R, class T>
	concept sized_output_range = std::ranges::sized_range<R> && std::output_iterator<std::ranges::iterator_t<R>, T>;
	// Concept defining a contiguous range holding a specific type.
	template <class R, class T>
	concept typed_contiguous_range = std::ranges::contiguous_range<R> && std::same_as<T, std::ranges::range_value_t<R>>;

	template <class T, template <class...> class Z> struct is_specialization_of : std::false_type {};
	template <class... Args, template <class...> class Z> struct is_specialization_of<Z<Args...>, Z> : std::true_type {};
	// Concept that denotes a type is a specialization of a certain template.
	template <class T, template <class...> class Z>
	concept specialization_of = is_specialization_of<T, Z>::value;

	template <typename T> struct remove_noexcept {
		using type = T;
	};
	template <typename R, typename... P> struct remove_noexcept<R(P...) noexcept> {
		using type = R(P...);
	};
	// Removes noexcept qualification from a function type.
	template <typename T> using remove_noexcept_t = remove_noexcept<T>::type;

	template <class T> struct arg_type {};
	template <class T, class T1> struct arg_type<T(T1)> {
		using type = T1;
	};
	template <class T, class T1> struct arg_type<T (*)(T1)> {
		using type = T1;
	};
	template <class T, class T1> struct arg_type<T(T1) noexcept> {
		using type = T1;
	};
	template <class T, class T1> struct arg_type<T (*)(T1) noexcept> {
		using type = T1;
	};
	// Gets the type of the argument of a function.
	template <typename T> using arg_type_t = arg_type<T>::type;

	template <usize S, class = void> struct size_type;
	template <usize S> struct size_type<S, std::enable_if_t<(S > UINT32_MAX)>> {
		using type = u64;
	};
	template <usize S> struct size_type<S, std::enable_if_t<(S > UINT16_MAX && S <= UINT32_MAX)>> {
		using type = u32;
	};
	template <usize S> struct size_type<S, std::enable_if_t<(S > UINT8_MAX && S <= UINT16_MAX)>> {
		using type = u16;
	};
	template <usize S> struct size_type<S, std::enable_if_t<(S <= UINT8_MAX)>> {
		using type = u8;
	};
	// Gets the type needed to store an integer constant.
	template <usize S> using size_type_t = size_type<S>::type;

	// Concept checking if a type is in a list.
	template <class T, class... Ts>
	concept one_of = (std::same_as<T, Ts> || ...);
} // namespace tr
