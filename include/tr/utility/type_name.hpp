/// @file
/// @brief Provides a function to obtain the human-readable name of a type.

#pragma once
#include <tr/utility/common.hpp>

//

namespace tr
{
	/// Gets a human-readable name string for a type.
	/// @note May not return the exact same string on all compilers.
	/// @tparam T Type to get the name of.
	/// @return String view to the name of the type.
	template <typename T>
	[[nodiscard]] consteval std::string_view type_name() noexcept
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

} // namespace tr