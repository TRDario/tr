/// @file
/// @brief Provides a function to obtain the human-readable name of a type.

#pragma once
#include <tr/utility/common.hpp>

//

namespace tr
{
	/// Gets a human-readable name string for a type.
	/// @note May not return the exact same string on all compilers.
	/// @tparam Target Type to get the name of.
	/// @return String view to the name of the type.
	template <typename Target>
	[[nodiscard]] consteval std::string_view type_name() noexcept
	{
#ifdef __clang__
		constexpr std::string_view function_name{std::source_location::current().function_name()};
#ifdef _MSC_VER
		return std::string_view{function_name.begin() + 55, function_name.end() - 1};
#else
		return std::string_view{function_name.begin() + 43, function_name.end() - 1};
#endif
#elifdef _MSC_VER
		constexpr std::string_view function_name{std::source_location::current().function_name()};
		return std::string_view{function_name.begin() + 93, function_name.end() - 7};
#elifdef __GNUC__
		constexpr std::string_view function_name{
			[]<typename>() { return std::source_location::current().function_name(); }.template operator()<Target>()};
		return std::string_view{function_name.begin() + 14, function_name.begin() + function_name.find("lambda") - 6};
#else
		static_assert(false, "tr::type_name is not implemented for this compiler!");
#endif
	}

} // namespace tr