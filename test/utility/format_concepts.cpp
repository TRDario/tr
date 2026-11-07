/// @file
/// @brief Tests utility/format_concepts.hpp.

#include <tr/utility/angle.hpp>
#include <tr/utility/format_concepts.hpp>

//

/// @brief Tests `tr::formattable` on a positive case.
static_assert(tr::formattable<std::string_view>, "std::string_view should fulfill tr::formattable, but does not!");

/// @brief Tests `tr::formattable` on a positive case with a wide character type.
static_assert(tr::formattable<std::wstring_view, wchar_t>, "std::wstring_view should fulfill tr::formattable<wchar_t>, but does not!");

/// @brief Tests `tr::formattable` on a negative case.
static_assert(!tr::formattable<std::string_view, wchar_t>);

//

/// @brief Tests `tr::valid_format_string_for` on a positive case.
static_assert(tr::valid_format_string_for<"A: {}, B: {}, C: {}", int, float, char>,
			  "String \"A: {}, B: {}, C: {}\" should be a valid format string for <int, float, char>, but is not!");

/// @brief Tests `tr::valid_format_string_for` on a negative case.
static_assert(!tr::valid_format_string_for<"A: {}, B: {}, C: {}", int, tr::angle<float>, char>,
			  "String \"A: {}, B: {}, C: {}\" should not be a valid format string for <int, tr::angle<float>, char>, but is!");

/// @brief Tests `tr::valid_format_string_for` on a positive case.
static_assert(tr::valid_format_string_for<"A: {}, B: {:d}, C: {}", int, tr::angle<float>, char>,
			  "String \"A: {}, B: {}, C: {}\" should be a valid format string for <int, tr::angle<float>, char>, but is not!");