/// @file
/// @brief Provides matrix-related constants, functions and literals.

#pragma once
#include <tr/utility/angle.hpp>
#include <tr/utility/rectangle.hpp>

//

namespace tr
{
	/// @ingroup math
	/// @brief Normalized X-axis vector.
	inline constexpr glm::vec3 x_axis{1.0f, 0.0f, 0.0f};

	/// @ingroup math
	/// @brief Normalized Y-axis vector.
	inline constexpr glm::vec3 y_axis{0.0f, 1.0f, 0.0f};

	/// @ingroup math
	/// @brief Normalized Z-axis vector.
	inline constexpr glm::vec3 z_axis{0.0f, 0.0f, 1.0f};

	//

	/// @ingroup math
	/// @brief Returns an orthographic transform corresponding to a rectangular region.
	/// @param region Region to create an orthographic transform for.
	/// @return Orthographic transform mapping to `region`.
	[[nodiscard]] glm::mat4 ortho(rectangle<float> region) noexcept;

	/// @ingroup math
	/// @brief Applies a 2D scaling around a point to a matrix.
	/// @param matrix Base matrix to apply the transformation to.
	/// @param center Point to scale around.
	/// @param scale Scaling multipliers.
	/// @return Result of the transformation applied upon `matrix`.
	[[nodiscard]] glm::mat4 scale_around(const glm::mat4& matrix, glm::vec2 center, glm::vec2 scale) noexcept;

	/// @ingroup math
	/// @brief Applies a 3D scaling around a point to a matrix.
	/// @param matrix Base matrix to apply the transformation to.
	/// @param center Point to scale around.
	/// @param scale Scaling multipliers.
	/// @return Result of the transformation applied upon `matrix`.
	[[nodiscard]] glm::mat4 scale_around(const glm::mat4& matrix, glm::vec3 center, glm::vec3 scale) noexcept;

	/// @ingroup math
	/// @brief Applies a 2D rotation around a point to a matrix.
	/// @param matrix Base matrix to apply the transformation to.
	/// @param center Point to rotate around.
	/// @param rotation Angle of rotation.
	/// @return Result of the transformation applied upon `matrix`.
	[[nodiscard]] glm::mat4 rotate_around(const glm::mat4& matrix, glm::vec2 center, angle<float> rotation) noexcept;

	/// @ingroup math
	/// @brief Applies a 3D rotation around a point to a matrix.
	/// @param matrix Base matrix to apply the transformation to.
	/// @param center Point to rotate around.
	/// @param rotation Angle of rotation.
	/// @param axis Axis of rotation.
	/// @return Result of the transformation applied upon `matrix`.
	[[nodiscard]] glm::mat4 rotate_around(const glm::mat4& matrix, glm::vec3 center, angle<float> rotation, glm::vec3 axis) noexcept;

	//

	inline namespace literals
	{
		/// @ingroup math
		/// @brief Namespace containing matrix multiplication operators.
		inline namespace matrix_operators
		{
			/// @brief Performs matrix multiplication on a 2D vector.
			/// @details Equivalent to `m * glm::vec4{v, 0, 1}`.
			/// @param m Matrix to multiply with.
			/// @param v Vector to multiply with.
			/// @return Result of the matrix multiplication.
			[[nodiscard]] glm::vec2 operator*(const glm::mat4& m, glm::vec2 v) noexcept;

			/// @brief Performs matrix multiplication on a 3D vector.
			/// @details Equivalent to `m * glm::vec4{v, 1}`.
			/// @param m Matrix to multiply with.
			/// @param v Vector to multiply with.
			/// @return Result of the matrix multiplication.
			[[nodiscard]] glm::vec3 operator*(const glm::mat4& m, glm::vec3 v) noexcept;
		} // namespace matrix_operators
	} // namespace literals
} // namespace tr