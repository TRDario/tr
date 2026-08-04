/// @file
/// Provides concepts.

#pragma once
#include "string_literal.hpp"

namespace tr
{
	/// Built-in arithmetic type.
	template <typename T>
	concept arithmetic = std::is_arithmetic_v<T>;

	/// Enumerator type.
	template <typename T>
	concept enumerator = std::is_enum_v<T>;

	/// Object (non-reference) type.
	template <typename T>
	concept object = std::is_object_v<T>;

	/// Pointer type.
	template <typename T>
	concept pointer = std::is_pointer_v<T>;

	/// Reference type.
	template <typename T>
	concept reference = std::is_reference_v<T>;

	/// Lvalue reference type.
	template <typename T>
	concept lvalue_reference = std::is_lvalue_reference_v<T>;

	/// Rvalue reference type.
	template <typename T>
	concept rvalue_reference = std::is_rvalue_reference_v<T>;

	/// Const-qualified type.
	template <typename T>
	concept const_qualified = std::is_const_v<std::remove_reference_t<T>>;

	/// Volatile-qualified type.
	template <typename T>
	concept volatile_qualified = std::is_volatile_v<std::remove_reference_t<T>>;

	/// CV-unqualified type.
	template <typename T>
	concept cv_unqualified = !const_qualified<T> && !volatile_qualified<T>;

	/// CV-unqualified object type.
	template <typename T>
	concept cv_unqualified_object = object<T> && cv_unqualified<T>;

	/// Non-constant lvalue reference type.
	template <typename T>
	concept non_const_lvalue_reference = lvalue_reference<T> && !const_qualified<T>;

	/// Constant lvalue reference type.
	template <typename T>
	concept const_lvalue_reference = lvalue_reference<T> && const_qualified<T>;

	//

	/// Move-assignable type.
	template <typename T>
	concept move_assignable = std::is_move_assignable_v<T>;

	/// Standard layout type.
	template <typename T>
	concept standard_layout = std::is_standard_layout_v<T>;

	//

	/// Contiguous range of standard layout objects.
	template <typename T>
	concept standard_layout_range = std::ranges::contiguous_range<T> && standard_layout<std::ranges::range_value_t<T>>;

	/// Borrowed contiguous range of standard layout objects.
	template <typename T>
	concept borrowed_standard_layout_range = standard_layout_range<T> && std::ranges::borrowed_range<T>;

	/// Borrowed mutable contiguous range of standard layout objects.
	template <typename T>
	concept borrowed_mutable_standard_layout_range = borrowed_standard_layout_range<T> &&
													 !const_qualified<std::ranges::range_reference_t<T>>;

	/// Sized output range type.
	/// @tparam Element Required output type.
	template <typename T, typename Element>
	concept sized_output_range = std::ranges::sized_range<T> && std::output_iterator<std::ranges::iterator_t<T>, Element>;

	/// Immutable contiguous range whose elements are of a certain type.
	/// @tparam Element Required element type.
	template <typename T, typename Element>
	concept typed_contiguous_const_range =
		std::ranges::contiguous_range<T> &&
		std::same_as<const Element, std::add_const_t<std::remove_reference_t<std::ranges::range_reference_t<T>>>>;

	/// Borrowed immutable contiguous range whose elements are of a certain type.
	/// @tparam Element Required element type.
	template <typename T, typename Element>
	concept borrowed_typed_contiguous_const_range = typed_contiguous_const_range<T, Element> && std::ranges::borrowed_range<T>;

	/// Mutable contiguous range whose elements are of a certain type.
	/// @tparam Element Required element type.
	template <typename T, typename Element>
	concept typed_contiguous_mutable_range = std::ranges::contiguous_range<T> &&
											 std::same_as<Element, std::remove_reference_t<std::ranges::range_reference_t<T>>>;

	/// Borrowed mutable contiguous range whose elements are of a certain type.
	/// @tparam Element Required element type.
	template <typename T, typename Element>
	concept borrowed_typed_contiguous_mutable_range = typed_contiguous_mutable_range<T, Element> && std::ranges::borrowed_range<T>;

	//

	/// Type contained within a list of types.
	/// @tparam Ts List of accepted types.
	template <typename T, typename... Ts>
	concept one_of = (std::same_as<T, Ts> || ...);

	//

	/// Formattable type.
	/// @tparam Context Formatting context type.
	/// @tparam Formatter Formatter type.
	template <typename T, typename Context, typename Formatter = typename Context::template formatter_type<std::remove_const_t<T>>>
	concept formattable_with = std::semiregular<Formatter> && requires(Formatter& f, Formatter const& cf, T&& t, Context fc,
																	   std::basic_format_parse_context<typename Context::char_type> pc) {
		{ f.parse(pc) } -> std::same_as<typename decltype(pc)::iterator>;
		{ cf.format(t, fc) } -> std::same_as<typename Context::iterator>;
	};

	/// Formattable type.
	/// @tparam CharT Output character type.
	template <typename T, typename CharT = char>
	concept formattable = formattable_with<std::remove_reference_t<T>, std::basic_format_context<CharT*, CharT>>;

	/// Format string that is valid for a set of types.
	/// @tparam Args List of arguments to the formatting function.
	template <string_literal Fmt, typename... Args>
	concept valid_format_string_for = (formattable<Args, char> && ...) &&
									  requires { typename std::integral_constant<int, (std::format_string<Args...>(Fmt.data), 1)>; };

	//

	/// Hasher compatible with a specific type.
	/// @tparam Hashed Type that must be hashable.
	template <typename T, typename Hashed>
	concept hasher = requires(const Hashed& v) {
		{ T{}(v) } -> std::convertible_to<u64>;
	};

	/// Equality predicate compatible with a specific type.
	/// @tparam Lhs Type on the left-hand side of the equality operation.
	/// @tparam Rhs Type on the right-hand side of the equality operation.
	template <typename T, typename Lhs, typename Rhs = Lhs>
	concept equality_predicate = requires(const Lhs& lhs, const Rhs& rhs) {
		{ T{}(lhs, rhs) } -> std::same_as<bool>;
	};

	/// "Keylike" type compatible with a hash collection.
	/// @tparam Key Primary key type of the collection.
	/// @tparam Hash Hasher type of the collection.
	/// @tparam Eq Equality predicate type of the collection.
	template <typename T, typename Key, typename Hash, typename Eq>
	concept hash_keylike = hasher<Hash, T> && equality_predicate<Eq, Key, T>;
} // namespace tr