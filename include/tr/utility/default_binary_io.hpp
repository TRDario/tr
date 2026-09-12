/// @file
/// @brief Defines the `tr::enable_default_binary_io` customization point.

#pragma once
#include <tr/utility/concepts.hpp>

//

namespace tr
{
	/// If specialized to true, a default binary reader and writer are used (the object's bytes are directly read/written).
	/// @tparam T Type to enable the default binary IO for.
	template <cv_unqualified_object T>
	inline constexpr bool enable_default_binary_io{false};

	/// Enables default binary IO for arithmetic types.
	/// @tparam Arithmetic Built-in arithmetic type.
	template <arithmetic Arithmetic>
		requires(cv_unqualified_object<Arithmetic>)
	inline constexpr bool enable_default_binary_io<Arithmetic>{true};

	// Enables default binary IO for enumerators.
	/// @tparam Enumerator Enumerator type.
	template <enumerator Enumerator>
		requires(cv_unqualified_object<Enumerator>)
	inline constexpr bool enable_default_binary_io<Enumerator>{true};

	/// Enables default binary IO for vectors.
	/// @tparam Dimensions Vector dimensions.
	/// @tparam Element Vector element type.
	template <int Dimensions, typename Element>
	inline constexpr bool enable_default_binary_io<glm::vec<Dimensions, Element>>{true};

	/// Enables default binary IO for matrices.
	/// @tparam Columns Matrix columns.
	/// @tparam Columns Matrix rows.
	/// @tparam Element Matrix element type.
	template <int Columns, int Rows, typename Element>
	inline constexpr bool enable_default_binary_io<glm::mat<Columns, Rows, Element>>{true};
} // namespace tr