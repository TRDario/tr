/// @file
/// @brief Provides matrix-related constants, functions and literals.

#pragma once
#include "angle.hpp"
#include "rectangle.hpp"

//

namespace tr
{
	/// Normalized X-axis vector.
	inline constexpr glm::vec3 x_axis{1.0f, 0.0f, 0.0f};

	/// Normalized Y-axis vector.
	inline constexpr glm::vec3 y_axis{0.0f, 1.0f, 0.0f};

	/// Normalized Z-axis vector.
	inline constexpr glm::vec3 z_axis{0.0f, 0.0f, 1.0f};

	//

	/// Produces an orthographic transform for a rectangular region.
	/// @param region Region to create an orthographic transform for.
	/// @return Orthographic transform mapping to `region`.
	glm::mat4 ortho(rectangle<float> region);

	/// Applies a 2D scaling around a point to a matrix.
	/// @param matrix Base matrix to apply the transformation to.
	/// @param center Point to scale around.
	/// @param scale Scaling multipliers.
	/// @return Result of the transformation applied upon `matrix`.
	glm::mat4 scale_around(const glm::mat4& matrix, glm::vec2 center, glm::vec2 scale);

	/// Applies a 3D scaling around a point to a matrix.
	/// @param matrix Base matrix to apply the transformation to.
	/// @param center Point to scale around.
	/// @param scale Scaling multipliers.
	/// @return Result of the transformation applied upon `matrix`.
	glm::mat4 scale_around(const glm::mat4& matrix, glm::vec3 center, glm::vec3 scale);

	/// Applies a 2D rotation around a point to a matrix.
	/// @param matrix Base matrix to apply the transformation to.
	/// @param center Point to rotate around.
	/// @param rotation Angle of rotation.
	/// @return Result of the transformation applied upon `matrix`.
	glm::mat4 rotate_around(const glm::mat4& matrix, glm::vec2 center, angle rotation);

	/// Applies a 3D rotation around a point to a matrix.
	/// @param matrix Base matrix to apply the transformation to.
	/// @param center Point to rotate around.
	/// @param rotation Angle of rotation.
	/// @param axis Axis of rotation.
	/// @return Result of the transformation applied upon `matrix`.
	glm::mat4 rotate_around(const glm::mat4& matrix, glm::vec3 center, angle rotation, glm::vec3 axis);

	//

	inline namespace literals
	{
		/// Provides matrix multiplication operators.
		inline namespace matrix_operators
		{
			/// @name Literals
			/// @{

			/// Performs matrix multiplication on a 2D vector.
			/// @param v Vector to multiply with.
			/// @param m Matrix to multiply with.
			/// @return Result of the matrix multiplication.
			glm::vec2 operator*(glm::vec2 v, const glm::mat4& m);

			/// Performs matrix multiplication on a 3D vector.
			/// @param v Vector to multiply with.
			/// @param m Matrix to multiply with.
			/// @return Result of the matrix multiplication.
			glm::vec3 operator*(glm::vec3 v, const glm::mat4& m);

			/// Performs matrix multiplication on a 2D vector.
			/// @param m Matrix to multiply with.
			/// @param v Vector to multiply with.
			/// @return Result of the matrix multiplication.
			glm::vec2 operator*(const glm::mat4& m, glm::vec2 v);

			/// Performs matrix multiplication on a 3D vector.
			/// @param m Matrix to multiply with.
			/// @param v Vector to multiply with.
			/// @return Result of the matrix multiplication.
			glm::vec3 operator*(const glm::mat4& m, glm::vec3 v);

			/// @}
		} // namespace matrix_operators
	} // namespace literals
} // namespace tr