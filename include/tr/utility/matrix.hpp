///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides matrix-related constants, functions and literals.                                                                            //
//                                                                                                                                       //
// The X, Y, and Z axes are provided as the constants tr::x_axis, tr::y_axis, tr::z_axis.                                                //
//                                                                                                                                       //
//                                                                                                                                       //
// An orthographic transformation for a rectangle is provided:                                                                           //
//     - tr::ortho(tr::rectangle<float>{{1000, 1000}}) -> equivalent to glm::ortho(0, 1000, 1000, 0)                                     //
//                                                                                                                                       //
// Centered scaling and rotation transformations are provided:                                                                           //
//     - tr::scale_around(mat, {10, 10}, {2, 2}) -> applies a 2x scaling transformation centered on (10, 10) on mat                      //
//     - tr::scale_around(mat, {10, 10, 10}, {2, 2, 2}) -> applies a 2x scaling transformation centered on (10, 10, 10) on mat           //
//     - tr::rotate_around(mat, {10, 10}, 90_deg) -> applies a 90-degree rotation centered on (10, 10) on mat                            //
//     - tr::rotate_around(mat, {10, 10, 10}, 90_deg, y_axis) -> applies a 90-degree y-axis rotation centered on (10, 10, 10) on mat     //
//                                                                                                                                       //
// Additional matrix multiplication operators (to avoid having to cast a vec2 into a vec4 when multiplying with mat4, for example) are   //
// defined in tr::matrix_operators, as well as tr::literals.                                                                             //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "angle.hpp"
#include "rectangle.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Normalized X-axis vector.
	inline constexpr glm::vec3 x_axis{1.0f, 0.0f, 0.0f};
	// Normalized Y-axis vector.
	inline constexpr glm::vec3 y_axis{0.0f, 1.0f, 0.0f};
	// Normalized Z-axis vector.
	inline constexpr glm::vec3 z_axis{0.0f, 0.0f, 1.0f};

	// Produces an orthographic transform for a rectangular region.
	glm::mat4 ortho(const rectangle<float>& region);
	// Applies a 2D scaling around a point to a matrix.
	glm::mat4 scale_around(const glm::mat4& matrix, glm::vec2 center, glm::vec2 scale);
	// Applies a 3D scaling around a point to a matrix.
	glm::mat4 scale_around(const glm::mat4& matrix, glm::vec3 center, glm::vec3 scale);
	// Applies a 2D rotation around a point to a matrix.
	glm::mat4 rotate_around(const glm::mat4& matrix, glm::vec2 center, angle rotation);
	// Applies a 3D rotation around a point to a matrix.
	glm::mat4 rotate_around(const glm::mat4& matrix, glm::vec3 center, angle rotation, glm::vec3 axis);

	inline namespace literals {
		// Matrix multiplication operators.
		inline namespace matrix_operators {
			// Performs matrix multiplication on a 2D vector.
			glm::vec2 operator*(const glm::vec2& v, const glm::mat4& m);
			// Performs matrix multiplication on a 3D vector.
			glm::vec3 operator*(const glm::vec3& v, const glm::mat4& m);
			// Performs matrix multiplication on a 2D vector.
			glm::vec2 operator*(const glm::mat4& m, const glm::vec2& v);
			// Performs matrix multiplication on a 3D vector.
			glm::vec3 operator*(const glm::mat4& m, const glm::vec3& v);
		} // namespace matrix_operators
	} // namespace literals
} // namespace tr