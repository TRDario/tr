///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides concepts.                                                                                                                    //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "template.hpp"

namespace tr {
	// Concept denoting an arithmetic type.
	template <typename T>
	concept arithmetic = std::is_arithmetic_v<T>;
	// Concept denoting an enumerator type.
	template <typename T>
	concept enumerator = std::is_enum_v<T>;

	// Concept denoting an object type (i.e. not a reference).
	template <typename T>
	concept object = std::is_object_v<T>;
	// Concept denoting a reference.
	template <typename T>
	concept reference = std::is_reference_v<T>;
	// Concept denoting an lvalue reference.
	template <typename T>
	concept lvalue_reference = std::is_lvalue_reference_v<T>;
	// Concept denoting an rvalue reference.
	template <typename T>
	concept rvalue_reference = std::is_rvalue_reference_v<T>;

	// Concept denoting a const-qualified type.
	template <typename T>
	concept const_qualified = std::is_const_v<T>;
	// Concept denoting a volatile-qualified type.
	template <typename T>
	concept volatile_qualified = std::is_volatile_v<T>;
	// Concept denoting a cv-unqualified type.
	template <typename T>
	concept cv_unqualified = !const_qualified<T> && !volatile_qualified<T>;
	// Concept denoting a cv-unqualified object.
	template <typename T>
	concept cv_unqualified_object = object<T> && cv_unqualified<T>;

	// Concept denoting a standard layout type.
	template <typename T>
	concept standard_layout = std::is_standard_layout_v<T>;
	// Concept denoting a contiguous range.
	template <typename T>
	concept standard_layout_range = std::ranges::contiguous_range<T> && standard_layout<std::ranges::range_value_t<T>>;
	// Concept denoting a sized output range.
	template <typename Range, typename Element>
	concept sized_output_range = std::ranges::sized_range<Range> && std::output_iterator<std::ranges::iterator_t<Range>, Element>;
	// Concept denoting a typed contiguous range.
	template <typename Range, typename Element>
	concept typed_contiguous_range = std::ranges::contiguous_range<Range> && std::same_as<Element, std::ranges::range_value_t<Range>>;

	// Concept denoting a specialization of a template with type parameters.
	template <typename T, template <typename...> typename Z>
	concept specialization_of = is_specialization_of<T, Z>::value;
	// Concept denoting a specialization of a template with value parameters.
	template <typename T, template <auto...> typename Z>
	concept specialization_of_v = is_specialization_of_v<T, Z>::value;
	// Concept denoting a specialization of a template with a leading value parameter.
	template <typename T, template <auto, typename...> typename Z>
	concept specialization_of_vt = is_specialization_of_vt<T, Z>::value;
	// Concept denoting a specialization of a template with a leading type and value parameter.
	template <typename T, template <typename, auto, typename...> typename Z>
	concept specialization_of_tv = is_specialization_of_tv<T, Z>::value;
	// Concept denoting a (potentially cv-qualified or reference to a) specialization of a template with type parameters.
	template <typename T, template <typename...> typename Z>
	concept cvref_specialization_of = specialization_of<std::remove_cvref_t<T>, Z>;
	// Concept denoting a (potentially cv-qualified or reference to a) specialization of a template with value parameters.
	template <typename T, template <auto...> typename Z>
	concept cvref_specialization_of_v = specialization_of_v<std::remove_cvref_t<T>, Z>;
	// Concept denoting a (potentially cv-qualified or reference to a) specialization of a template with a leading value parameter.
	template <typename T, template <auto, typename...> typename Z>
	concept cvref_specialization_of_vt = specialization_of_vt<std::remove_cvref_t<T>, Z>;
	// Concept denoting a (potentially cv-qualified or reference to a) specialization of a template with a leading type and value parameter.
	template <typename T, template <typename, auto, typename...> typename Z>
	concept cvref_specialization_of_tv = specialization_of_tv<std::remove_cvref_t<T>, Z>;

	// Concept checking if a type is in a list.
	template <typename T, typename... Ts>
	concept one_of = (std::same_as<T, Ts> || ...);

	// Concept checking whether a type is formattable.
	template <class T, class Context, class Formatter = typename Context::template formatter_type<std::remove_const_t<T>>>
	concept formattable_with = std::semiregular<Formatter> && requires(Formatter& f, Formatter const& cf, T&& t, Context fc,
																	   TR_FMT::basic_format_parse_context<typename Context::char_type> pc) {
		{ f.parse(pc) } -> std::same_as<typename decltype(pc)::iterator>;
		{ cf.format(t, fc) } -> std::same_as<typename Context::iterator>;
	};
	// Concept checking whether a type is formattable.
	template <class T, class CharT = char>
	concept formattable = formattable_with<std::remove_reference_t<T>, TR_FMT::basic_format_context<CharT*, CharT>>;
	// Concept checking whether a format string is valid for a set of types.
	template <string_literal Fmt, class... Args>
	concept valid_format_string_for =
		(formattable<Args, char> && ...) && requires { typename std::integral_constant<int, (std::format_string<Args...>(Fmt.data), 1)>; };
} // namespace tr