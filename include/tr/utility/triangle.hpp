/// @file
/// @brief Provides a triangle datatype.

#pragma once
#include "default_binary_io.hpp"
#include "polygon.hpp"

//

namespace tr
{
	/// 2D triangle datatype.
	struct triangle
	{
		/// First point of the triangle.
		glm::vec2 a;

		/// Second point of the triangle.
		glm::vec2 b;

		/// Third point of the triangle.
		glm::vec2 c;

		//

		/// Determines the winding order of the triangle.
		/// @return Winding order of the triangle.
		winding_order winding_order() const;

		/// Determines whether a point is contained inside the triangle.
		/// @return `true` if `point` is within the triangle, `false` otherwise.
		bool contains(glm::vec2 point) const;
	};
} // namespace tr

//

/// Enables default binary IO for triangles.
template <>
inline constexpr bool tr::enable_default_binary_io<tr::triangle>{true};