/// @file
/// @brief Defines the `tr::enable_default_binary_io` customization point.

#pragma once
#include <tr/utility/common.hpp>

//

namespace tr
{
	/// @name Default binary IO
	/// @{

	/// @brief If specialized to `true`, a default binary reader and writer are used (the object's bytes are directly read/written).
	/// @tparam Target Type to enable the default binary IO for.
	template <typename Target>
	inline constexpr bool enable_default_binary_io{false};

	/// @brief Enables default binary IO for arithmetic types.
	/// @tparam Arithmetic Built-in arithmetic type.
	template <typename Arithmetic>
		requires std::is_arithmetic_v<Arithmetic>
	inline constexpr bool enable_default_binary_io<Arithmetic>{true};

	/// @brief Enables default binary IO for enumerators.
	/// @tparam Enumerator Enumerator type.
	template <typename Enumerator>
		requires std::is_enum_v<Enumerator>
	inline constexpr bool enable_default_binary_io<Enumerator>{true};

	/// @brief Enables default binary IO for vectors.
	/// @tparam Dimensions Vector dimensions.
	/// @tparam Rep Vector representation type.
	template <int Dimensions, typename Rep>
	inline constexpr bool enable_default_binary_io<glm::vec<Dimensions, Rep>>{true};

	/// @brief Enables default binary IO for matrices.
	/// @tparam Columns Number of columns in the matrix.
	/// @tparam Rows Number of rows in the matrix.
	/// @tparam Rep Matrix representation type.
	template <int Columns, int Rows, typename Rep>
	inline constexpr bool enable_default_binary_io<glm::mat<Columns, Rows, Rep>>{true};

	/// @}
} // namespace tr