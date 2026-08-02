/// @file
/// @brief Implements specialization_of.hpp.

#pragma once
#include "../specialization_of.hpp"

//

namespace tr
{
	/// Specialization check specialization with a negative result.
	/// @tparam T The type to check.
	/// @tparam Template Unrelated template.
	template <typename T, template <typename...> typename Template>
	struct is_specialization_of : std::false_type
	{
	};

	/// Specialization check specialization with a positive result.
	/// @tparam T The type to check.
	/// @tparam Template Parent template.
	template <typename... Args, template <typename...> typename Template>
	struct is_specialization_of<Template<Args...>, Template> : std::true_type
	{
	};

	//

	/// Specialization check specialization with a negative result.
	/// @tparam T The type to check.
	/// @tparam Template Unrelated template.
	template <typename T, template <auto...> typename Template>
	struct is_specialization_of_v : std::false_type
	{
	};

	/// Specialization check specialization with a positive result.
	/// @tparam T The type to check.
	/// @tparam Template Parent template.
	template <auto... Values, template <auto...> typename Template>
	struct is_specialization_of_v<Template<Values...>, Template> : std::true_type
	{
	};

	//

	/// Specialization check specialization with a negative result.
	/// @tparam T The type to check.
	/// @tparam Template Unrelated template.
	template <typename T, template <auto, typename...> typename Template>
	struct is_specialization_of_vt : std::false_type
	{
	};

	/// Specialization check specialization with a positive result.
	/// @tparam T The type to check.
	/// @tparam Template Parent template.
	template <auto S, typename... Args, template <auto, typename...> typename Template>
	struct is_specialization_of_vt<Template<S, Args...>, Template> : std::true_type
	{
	};

	//

	/// Specialization check specialization with a negative result.
	/// @tparam T The type to check.
	/// @tparam Template Unrelated template.
	template <typename T, template <typename, auto, typename...> typename Template>
	struct is_specialization_of_tv : std::false_type
	{
	};

	/// Specialization check specialization with a positive result.
	/// @tparam T The type to check.
	/// @tparam Template Parent template.
	template <typename T, auto S, typename... Args, template <typename, auto, typename...> typename Template>
	struct is_specialization_of_tv<Template<T, S, Args...>, Template> : std::true_type
	{
	};
} // namespace tr
