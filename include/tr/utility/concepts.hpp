#pragma once
#include "common.hpp"

namespace tr {
	// Circumvents static_assert(false) not being a valid expression.
	template <class...> inline constexpr bool ALWAYS_FALSE = false;

	// Template for string literals passed as template arguments.
	template <std::size_t N> struct template_string_literal {
		char data[N - 1];

		constexpr template_string_literal(const char (&str)[N])
		{
			std::copy_n(str, N - 1, data);
		}
	};

	// Concept wrapper over std::is_arithmetic_v.
	template <class T>
	concept arithmetic = std::is_arithmetic_v<T>;
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

	template <class T> struct return_type {};
	template <class T, class... Ts> struct return_type<T(Ts...)> {
		using type = T;
	};
	template <class T, class... Ts> struct return_type<T (*)(Ts...)> {
		using type = T;
	};
	template <class T, class... Ts> struct return_type<T(Ts...) noexcept> {
		using type = T;
	};
	template <class T, class... Ts> struct return_type<T (*)(Ts...) noexcept> {
		using type = T;
	};
	// Gets the return type of a function.
	template <typename T> using return_type_t = return_type<T>::type;

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

	template <std::size_t S, class = void> struct size_type;
	template <std::size_t S> struct size_type<S, std::enable_if_t<(S > std::numeric_limits<std::uint32_t>::max())>> {
		using type = std::uint64_t;
	};
	template <std::size_t S>
	struct size_type<S,
					 std::enable_if_t<(S > std::numeric_limits<std::uint16_t>::max() && S <= std::numeric_limits<std::uint32_t>::max())>> {
		using type = std::uint32_t;
	};
	template <std::size_t S>
	struct size_type<S,
					 std::enable_if_t<(S > std::numeric_limits<std::uint8_t>::max() && S <= std::numeric_limits<std::uint16_t>::max())>> {
		using type = std::uint16_t;
	};
	template <std::size_t S> struct size_type<S, std::enable_if_t<(S <= std::numeric_limits<std::uint8_t>::max())>> {
		using type = std::uint8_t;
	};
	// Gets the type needed to store an integer constant.
	template <std::size_t S> using size_type_t = size_type<S>::type;
} // namespace tr
