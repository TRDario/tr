/// @file
/// @brief Defines the `tr::enable_default_binary_io` customization point.

#pragma once
#include <tr/utility/common.hpp>

//

namespace tr
{
	/// If specialized to true, a default binary reader and writer are used (the object's bytes are directly read/written).
	/// @tparam Target Type to enable the default binary IO for.
	template <typename Target>
	inline constexpr bool enable_default_binary_io{false};

	/// Enables default binary IO for arithmetic types.
	/// @tparam Arithmetic Built-in arithmetic type.
	template <typename Arithmetic>
		requires std::is_arithmetic_v<Arithmetic>
	inline constexpr bool enable_default_binary_io<Arithmetic>{true};

	// Enables default binary IO for enumerators.
	/// @tparam Enumerator Enumerator type.
	template <typename Enumerator>
		requires std::is_enum_v<Enumerator>
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