/// @file
/// @brief Provides utilities for determining whether a type is a specialization of a template.

#pragma once

//

namespace tr
{
	/// Stores whether `T` is a specialization of `Template` in `::value`.
	/// @tparam T The type to check.
	/// @tparam Template Template taking type parameters.
	template <typename T, template <typename...> typename Template>
	struct is_specialization_of;

	/// Stores whether `T` is a specialization of `Template` in `::value`.
	/// @tparam T The type to check.
	/// @tparam Template Template taking non-type parameters.
	template <typename T, template <auto...> typename Template>
	struct is_specialization_of_v;

	/// Stores whether `T` is a specialization of `Template` in `::value`.
	/// @tparam T The type to check.
	/// @tparam Template Template taking a leading non-type parameter followed by type parameters.
	template <typename T, template <auto, typename...> typename Template>
	struct is_specialization_of_vt;

	/// Stores whether `T` is a specialization of `Template` in `::value`.
	/// @tparam T The type to check.
	/// @tparam Template Template taking a leading type parameter, followed by a non-type parameter, followed by type parameters.
	template <typename T, template <typename, auto, typename...> typename Template>
	struct is_specialization_of_tv;

	//

	/// Specialization of a template with type parameters.
	/// @tparam Template Template taking type parameters.
	template <typename T, template <typename...> typename Template>
	concept specialization_of = is_specialization_of<T, Template>::value;

	/// Specialization of a template with value parameters.
	/// @tparam Template Template taking non-type parameters.
	template <typename T, template <auto...> typename Template>
	concept specialization_of_v = is_specialization_of_v<T, Template>::value;

	/// Specialization of a template with a leading value parameter.
	/// @tparam Template Template taking a leading non-type parameter followed by type parameters.
	template <typename T, template <auto, typename...> typename Template>
	concept specialization_of_vt = is_specialization_of_vt<T, Template>::value;

	/// Specialization of a template with a leading type and value parameter.
	/// @tparam Template Template taking a leading type parameter, followed by a non-type parameter, followed by type parameters.
	template <typename T, template <typename, auto, typename...> typename Template>
	concept specialization_of_tv = is_specialization_of_tv<T, Template>::value;

	/// (Potentially cv-qualified or reference to a) specialization of a template with type parameters.
	/// @tparam Template Template taking type parameters.
	template <typename T, template <typename...> typename Template>
	concept cvref_specialization_of = specialization_of<std::remove_cvref_t<T>, Template>;

	/// (Potentially cv-qualified or reference to a) specialization of a template with value parameters.
	/// @tparam Template Template taking non-type parameters.
	template <typename T, template <auto...> typename Template>
	concept cvref_specialization_of_v = specialization_of_v<std::remove_cvref_t<T>, Template>;

	/// (Potentially cv-qualified or reference to a) specialization of a template with a leading value parameter.
	/// @tparam Template Template taking a leading non-type parameter followed by type parameters.
	template <typename T, template <auto, typename...> typename Template>
	concept cvref_specialization_of_vt = specialization_of_vt<std::remove_cvref_t<T>, Template>;

	/// (Potentially cv-qualified or reference to a) specialization of a template with a leading type and value parameter.
	/// @tparam Template Template taking a leading type parameter, followed by a non-type parameter, followed by type parameters.
	template <typename T, template <typename, auto, typename...> typename Template>
	concept cvref_specialization_of_tv = specialization_of_tv<std::remove_cvref_t<T>, Template>;
} // namespace tr

#include "impl/specialization_of.hpp" // IWYU pragma: export