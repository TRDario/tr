#pragma once
#include "angle.hpp"

namespace tr {
	/** @ingroup utility
	 *  @defgroup geometry Geometry
	 *  Geometry-related datatypes and functions.
	 *  @{
	 */

	/******************************************************************************************************************
	 * Horizontal alignment.
	 *****************************************************************************************************************/
	enum class halign : std::uint8_t {
		LEFT,
		CENTER,
		RIGHT
	};

	/******************************************************************************************************************
	 * Vertical alignment.
	 *****************************************************************************************************************/
	enum class valign : std::uint8_t {
		TOP,
		CENTER = 3,
		BOTTOM = 6
	};

	/******************************************************************************************************************
	 * 2D alignment.
	 *****************************************************************************************************************/
	enum class align : std::uint8_t {
		TOP_LEFT,
		TOP_CENTER,
		TOP_RIGHT,
		CENTER_LEFT,
		CENTER,
		CENTER_RIGHT,
		BOTTOM_LEFT,
		BOTTOM_CENTER,
		BOTTOM_RIGHT
	};

	/******************************************************************************************************************
	 * Converts a 2D alignment to a horizontal alignment.
	 *
	 * @param align The alignment to decompose.
	 *
	 * @return The horizontal component of the alignment.
	 *****************************************************************************************************************/
	constexpr halign to_halign(align align) noexcept;

	/******************************************************************************************************************
	 * Converts a 2D alignment to a vertical alignment.
	 *
	 * @param align The alignment to decompose.
	 *
	 * @return The vertical component of the alignment.
	 *****************************************************************************************************************/
	constexpr valign to_valign(align align) noexcept;

	/******************************************************************************************************************
	 * Combines a horizontal and vertical alignment into a 2D alignment.
	 *
	 * @param[in] halign The horizontal alignment.
	 * @param[in] valign The vertical alignment.
	 *
	 * @return A 2D alignment.
	 *****************************************************************************************************************/
	constexpr align operator|(const valign& valign, const halign& halign) noexcept;

	/******************************************************************************************************************
	 * Combines a horizontal and vertical alignment into a 2D alignment.
	 *
	 * @param[in] halign The horizontal alignment.
	 * @param[in] valign The vertical alignment.
	 *
	 * @return A 2D alignment.
	 *****************************************************************************************************************/
	constexpr align operator|(const halign& halign, const valign& valign) noexcept;

	/******************************************************************************************************************
	 * Rectangle object.
	 ******************************************************************************************************************/
	template <int S, class T> struct rect {
		/**************************************************************************************************************
		 * The offset of the top-left corner of the rect.
		 **************************************************************************************************************/
		glm::vec<S, T> tl;

		/**************************************************************************************************************
		 * The size of the rectangle.
		 **************************************************************************************************************/
		glm::vec<S, T> size;

		/**************************************************************************************************************
		 * Default-constructs a rect.
		 **************************************************************************************************************/
		constexpr rect() = default;

		/**************************************************************************************************************
		 * Constructs a rect.
		 *
		 * @param[in] tl The offset of the top-left corner of the rect.
		 * @param[in] size The size of the rectangle.
		 **************************************************************************************************************/
		constexpr rect(glm::vec<S, T> tl, glm::vec<S, T> size) noexcept;

		/**************************************************************************************************************
		 * Constructs a rect with the top-left corner at the origin.
		 *
		 * @param[in] size The size of the rectangle.
		 **************************************************************************************************************/
		constexpr rect(glm::vec<S, T> size) noexcept;

		/**************************************************************************************************************
		 * Copy-constructs a rect.
		 *
		 * @param[in] rect The rect to copy (may be of a different base type).
		 **************************************************************************************************************/
		template <class T1> constexpr rect(const rect<S, T1>& rect) noexcept;

		/**************************************************************************************************************
		 * Equality comparison operator.
		 **************************************************************************************************************/
		template <class T1> constexpr bool operator==(const rect<S, T1>&) const noexcept;

		/**************************************************************************************************************
		 * Determines whether a point is contained inside the rect.
		 *
		 * @param[in] point The point to check.
		 *
		 * @return True if the point within the bounds of the rect and false otherwise.
		 **************************************************************************************************************/
		template <class T1> constexpr bool contains(glm::vec<S, T1> point) const noexcept;
	};

	/******************************************************************************************************************
	 * 2D rectangle.
	 ******************************************************************************************************************/
	template <class T> using rect2 = rect<2, T>;

	/******************************************************************************************************************
	 * Shorthand for an int 2D rect.
	 ******************************************************************************************************************/
	using irect2 = rect2<int>;

	/******************************************************************************************************************
	 * Shorthand for a float 2D rect.
	 ******************************************************************************************************************/
	using frect2 = rect2<float>;

	/******************************************************************************************************************
	 * 3D rectangle.
	 ******************************************************************************************************************/
	template <class T> using rect3 = rect<3, T>;

	/******************************************************************************************************************
	 * Shorthand for an int 3D rect.
	 ******************************************************************************************************************/
	using irect3 = rect3<int>;

	/******************************************************************************************************************
	 * Shorthand for a float 3D rect.
	 ******************************************************************************************************************/
	using frect3 = rect3<float>;

	/******************************************************************************************************************
	 * 2D circle.
	 ******************************************************************************************************************/
	template <class T> struct circle {
		/**************************************************************************************************************
		 * The offset of the center of the circle.
		 **************************************************************************************************************/
		glm::tvec2<T> c;

		/**************************************************************************************************************
		 * The radius of the circle.
		 **************************************************************************************************************/
		T r;

		/**************************************************************************************************************
		 * Default-constructs a circle.
		 **************************************************************************************************************/
		constexpr circle() = default;

		/**************************************************************************************************************
		 * Constructs a circle.
		 *
		 * @param[in] c The offset of the center of the circle.
		 * @param[in] r The radius of the circle.
		 **************************************************************************************************************/
		constexpr circle(glm::tvec2<T> c, T r) noexcept;

		/**************************************************************************************************************
		 * Copy-constructs a circle.
		 *
		 * @param[in] circle The circle to copy (may be of a different base type).
		 **************************************************************************************************************/
		template <class T1> constexpr circle(const circle<T1>& circle) noexcept;

		/**************************************************************************************************************
		 * Equality comparison operator.
		 **************************************************************************************************************/
		template <class T1> constexpr bool operator==(const circle<T1>& r) const noexcept;

		/**************************************************************************************************************
		 * Determines whether a point is contained inside the circle.
		 *
		 * @param[in] point The point to check.
		 *
		 * @return True if the point within the bounds of the circle and false otherwise.
		 **************************************************************************************************************/
		template <class T1> constexpr bool contains(glm::tvec2<T1> point) const noexcept;
	};

	/******************************************************************************************************************
	 * Shorthand for a float circle.
	 ******************************************************************************************************************/
	using fcircle = circle<float>;

	//

	/******************************************************************************************************************
	 * Creates a vector from a magnitude and angle.
	 *
	 * @param[in] mag The magnitude of the vector.
	 * @param[in] th The angle of the vector.
	 *
	 * @return A vector with the given magnitude and angle.
	 *****************************************************************************************************************/
	template <class T> inline glm::tvec2<T> magth(T mag, tr::angle<T> th) noexcept;

	/******************************************************************************************************************
	 * Computes the top-left corner of a rectangle given a position, size, and anchor point.
	 *
	 * @param[in] pos The position of the rectangle.
	 * @param[in] size The size of the rectangle.
	 * @param[in] pos_anchor The pivot point of the rectangle.
	 *
	 * @return The top-left corner of the rectangle.
	 *****************************************************************************************************************/
	template <class T> constexpr glm::tvec2<T> tl(glm::tvec2<T> pos, glm::tvec2<T> size, tr::align pos_anchor) noexcept;

	/******************************************************************************************************************
	 * Determines whether 3 points are collinear.
	 *
	 * @param[in] a, b, c The points to check for collinearity.
	 *
	 * @return True if the points are collinear, and false otherwise.
	 ******************************************************************************************************************/
	template <class T1, class T2, class T3> constexpr bool collinear(glm::tvec2<T1> a, glm::tvec2<T2> b, glm::tvec2<T3> c) noexcept;

	/******************************************************************************************************************
	 * Determines if two rects intersect.
	 *
	 * @param[in] r1, r2 The rects to check for intersection.
	 *
	 * @return True if the rects intersect, and false otherwise.
	 ******************************************************************************************************************/
	template <class T1, class T2> bool intersecting(const rect2<T1>& r1, const rect2<T2>& r2) noexcept;

	/******************************************************************************************************************
	 * Determines if two circles intersect.
	 *
	 * @param[in] c1, c2 The circles to check for intersection.
	 *
	 * @return True if the circles intersect, and false otherwise.
	 ******************************************************************************************************************/
	template <class T> bool intersecting(const circle<T>& c1, const circle<T>& c2) noexcept;

	/******************************************************************************************************************
	 * Calculates the closest point to p on the line ab.
	 *
	 * @param[in] p A point.
	 * @param[in] a, b The start and end point of the line segment.
	 *
	 * @return The closest point to p located on the line ab.
	 ******************************************************************************************************************/
	template <std::floating_point T> constexpr glm::tvec2<T> closest_point(glm::tvec2<T> p, glm::tvec2<T> a, glm::tvec2<T> b) noexcept;

	/******************************************************************************************************************
	 * Calculates the closest point to p on the line ab.
	 *
	 * @param[in] p A point.
	 * @param[in] a, b The start and end point of the line segment.
	 *
	 * @return The closest point to p located on the line ab.
	 ******************************************************************************************************************/
	template <std::integral T> constexpr glm::dvec2 closest_point(glm::tvec2<T> p, glm::tvec2<T> a, glm::tvec2<T> b) noexcept;

	/******************************************************************************************************************
	 * Calculates the intersection point of two line segments.
	 *
	 * @param[in] a1, b1 Delimiting points of the first line segment.
	 * @param[in] a2, b2 Delimiting points of the second line segment.
	 *
	 * @return The intersection point, if one exists.
	 ******************************************************************************************************************/
	template <std::floating_point T>
	constexpr std::optional<glm::tvec2<T>> segment_intersect(glm::tvec2<T> a1, glm::tvec2<T> b1, glm::tvec2<T> a2,
															 glm::tvec2<T> b2) noexcept;

	/******************************************************************************************************************
	 * Calculates the intersection point of two line segments.
	 *
	 * @param[in] a1, b1 Delimiting points of the first line segment.
	 * @param[in] a2, b2 Delimiting points of the second line segment.
	 *
	 * @return The intersection point, if one exists.
	 ******************************************************************************************************************/
	template <std::integral T>
	constexpr std::optional<glm::dvec2> segment_intersect(glm::tvec2<T> a1, glm::tvec2<T> b1, glm::tvec2<T> a2, glm::tvec2<T> b2) noexcept;

	/******************************************************************************************************************
	 * Calculates the intersection point of a line with a line segment.
	 *
	 * @param[in] a1, th1 Point and angle of the first line.
	 * @param[in] a2, b2 Delimiting points of the second line segment.
	 *
	 * @return The intersection point, if one exists.
	 ******************************************************************************************************************/
	template <std::floating_point T>
	std::optional<glm::tvec2<T>> segment_intersect(glm::tvec2<T> a1, angle<T> th1, glm::tvec2<T> a2, glm::tvec2<T> b2) noexcept;

	/******************************************************************************************************************
	 * Calculates the intersection point of a line with a line segment.
	 *
	 * @param[in] a1, th1 Point and angle of the first line.
	 * @param[in] a2, b2 Delimiting points of the second line segment.
	 *
	 * @return The intersection point, if one exists.
	 ******************************************************************************************************************/
	template <std::integral T>
	std::optional<glm::dvec2> segment_intersect(glm::tvec2<T> a1, dangle th1, glm::tvec2<T> a2, glm::tvec2<T> b2) noexcept;

	/******************************************************************************************************************
	 * Calculates the intersection point of two lines.
	 *
	 * @param[in] a1, th1 Point and angle of the first line.
	 * @param[in] a2, th2 Point and angle of the second line.
	 *
	 * @return The intersection point, if one exists.
	 ******************************************************************************************************************/
	template <std::floating_point T>
	std::optional<glm::tvec2<T>> intersect(glm::tvec2<T> a1, angle<T> th1, glm::tvec2<T> a2, angle<T> th2) noexcept;

	/******************************************************************************************************************
	 * Calculates the intersection point of two lines.
	 *
	 * @param[in] a1, th1 Point and angle of the first line.
	 * @param[in] a2, th2 Point and angle of the second line.
	 *
	 * @return The intersection point, if one exists.
	 ******************************************************************************************************************/
	template <std::integral T> std::optional<glm::dvec2> intersect(glm::tvec2<T> a1, dangle th1, glm::tvec2<T> a2, dangle th2) noexcept;

	/******************************************************************************************************************
	 * Calculates the intersection point of two lines.
	 *
	 * @param[in] a1, b1 Two points on the first line.
	 * @param[in] a2, b2 Two points on the second line.
	 *
	 * @return The intersection point, if one exists.
	 ******************************************************************************************************************/
	template <std::floating_point T>
	constexpr std::optional<glm::tvec2<T>> intersect(glm::tvec2<T> a1, glm::tvec2<T> b1, glm::tvec2<T> a2, glm::tvec2<T> b2) noexcept;

	/******************************************************************************************************************
	 * Calculates the intersection point of two lines.
	 *
	 * @param[in] a1, th1 Point and angle of the first line.
	 * @param[in] a2, b2 Two points on the second line.
	 *
	 * @return The intersection point, if one exists.
	 ******************************************************************************************************************/
	template <std::floating_point T>
	std::optional<glm::tvec2<T>> intersect(glm::tvec2<T> a1, angle<T> th1, glm::tvec2<T> a2, glm::tvec2<T> b2) noexcept;

	/******************************************************************************************************************
	 * Calculates the 2D cross product of two vectors.
	 *
	 * @param[in] a, b The two vectors.
	 *
	 * @return The 2D cross product of the vectors, standard promotion rules apply.
	 ******************************************************************************************************************/
	template <class T1, class T2> constexpr std::common_type_t<T1, T2> cross2(glm::tvec2<T1> a, glm::tvec2<T2> b) noexcept;

	/******************************************************************************************************************
	 * Performs a mirror repeat mapping.
	 *
	 * @param[in] v The value to map.
	 * @param[in] min, max The bounds of the range.
	 *
	 * @return v mirror mapped to [min, max).
	 ******************************************************************************************************************/
	template <arithmetic T> T mirror_repeat(T v, T min, T max) noexcept;

	/******************************************************************************************************************
	 * Performs a mirror repeat mapping.
	 *
	 * @param[in] v The value to map.
	 * @param[in] min, max The bounds of the range.
	 *
	 * @return v mirror mapped to [min, max).
	 ******************************************************************************************************************/
	template <int S, class T>
	glm::vec<S, T> mirror_repeat(const glm::vec<S, T>& v, const glm::vec<S, T>& min, const glm::vec<S, T>& max) noexcept;

	/******************************************************************************************************************
	 * Performs a mirror repeat mapping of a vector on a rect.
	 *
	 * @param[in] v The value to map.
	 * @param[in] rect The bounds of the range.
	 *
	 * @return v mirror mapped to rect.
	 ******************************************************************************************************************/
	template <std::size_t S, class T> glm::vec<S, T> mirror_repeat(const glm::vec<S, T>& v, const rect<S, T>& rect) noexcept;

	/******************************************************************************************************************
	 * Gets the inverse of a vector.
	 *
	 * @param[in] vec The vector to invert.
	 *
	 * @return The inverse vector.
	 ******************************************************************************************************************/
	template <std::size_t S, std::floating_point T> constexpr glm::vec<S, T> inverse(const glm::vec<S, T>& vec) noexcept;

	/******************************************************************************************************************
	 * Gets the inverse of a vector.
	 *
	 * @param[in] vec The vector to invert.
	 *
	 * @return The inverse vector.
	 ******************************************************************************************************************/
	template <std::size_t S, std::integral T> constexpr glm::vec<S, double> inverse(const glm::vec<S, T>& vec) noexcept;

	/******************************************************************************************************************
	 * Matrix multiplication operators.
	 ******************************************************************************************************************/
	inline namespace matrix_operators {
		/**************************************************************************************************************
		 * Performs matrix multiplication on a 2D vector.
		 *
		 * @param[in] v A 2D vector.
		 * @param[in] m A matrix.
		 *
		 * @return `v * m`.
		 **************************************************************************************************************/
		template <class T> constexpr glm::tvec2<T> operator*(const glm::tvec2<T>& v, const glm::tmat4x4<T>& m) noexcept;

		/**************************************************************************************************************
		 * Performs matrix multiplication on a 3D vector.
		 *
		 * @param[in] v A 3D vector.
		 * @param[in] m A matrix.
		 *
		 * @return `v * m`.
		 **************************************************************************************************************/
		template <class T> constexpr glm::tvec3<T> operator*(const glm::tvec3<T>& v, const glm::tmat4x4<T>& m) noexcept;

		/**************************************************************************************************************
		 * Performs matrix multiplication on a 2D vector.
		 *
		 * @param[in] m A matrix.
		 * @param[in] v A 2D vector.
		 *
		 * @return `m * v`.
		 **************************************************************************************************************/
		template <class T> constexpr glm::tvec2<T> operator*(const glm::tmat4x4<T>& m, const glm::tvec2<T>& v) noexcept;

		/**************************************************************************************************************
		 * Performs matrix multiplication on a 3D vector.
		 *
		 * @param[in] m A matrix.
		 * @param[in] v A 3D vector.
		 *
		 * @return `m * v`.
		 **************************************************************************************************************/
		template <class T> constexpr glm::tvec3<T> operator*(const glm::tmat4x4<T>& m, const glm::tvec3<T>& v) noexcept;
	} // namespace matrix_operators

	/******************************************************************************************************************
	 * Normalized X-axis vector.
	 ******************************************************************************************************************/
	inline constexpr glm::vec3 X_AXIS{1.0f, 0.0f, 0.0f};

	/******************************************************************************************************************
	 * Normalized Y-axis vector.
	 ******************************************************************************************************************/
	inline constexpr glm::vec3 Y_AXIS{0.0f, 1.0f, 0.0f};

	/******************************************************************************************************************
	 * Normalized Z-axis vector.
	 ******************************************************************************************************************/
	inline constexpr glm::vec3 Z_AXIS{0.0f, 0.0f, 1.0f};

	/******************************************************************************************************************
	 * Produces an orthographic transform for a region from (0, 0) to (bl.x, bl.y).
	 *
	 * @param[in] bl The bottom-left corner of the rectangle.
	 *
	 * @return An orthographic transform.
	 ******************************************************************************************************************/
	template <class T = float> glm::tmat4x4<T> ortho(const glm::tvec2<T>& bl) noexcept;

	/******************************************************************************************************************
	 * Produces an orthographic transform for a rectangular region.
	 *
	 * @param[in] rect The rectangle to produce an orthographic transform for.
	 *
	 * @return An orthographic transform.
	 ******************************************************************************************************************/
	template <class T = float> glm::tmat4x4<T> ortho(const tr::rect2<T>& rect) noexcept;

	/******************************************************************************************************************
	 * Applies a 2D scaling around a point to a matrix.
	 *
	 * @param[in] mat The matrix to apply the scaling to.
	 * @param[in] c The point to scale around.
	 * @param[in] s The scale factor.
	 *
	 * @return A matrix with the applied scaling.
	 ******************************************************************************************************************/
	template <class T> glm::tmat4x4<T> scale_around(const glm::tmat4x4<T>& mat, const glm::tvec2<T>& c, const glm::tvec2<T>& s) noexcept;

	/******************************************************************************************************************
	 * Applies a 3D scaling around a point to a matrix.
	 *
	 * @param[in] mat The matrix to apply the scaling to.
	 * @param[in] c The point to scale around.
	 * @param[in] s The scale factor.
	 *
	 * @return A matrix with the applied scaling.
	 ******************************************************************************************************************/
	template <class T> glm::tmat4x4<T> scale_around(const glm::tmat4x4<T>& mat, const glm::tvec3<T>& c, const glm::tvec3<T>& s) noexcept;

	/******************************************************************************************************************
	 * Applies a 2D rotation around a point to a matrix.
	 *
	 * @param[in] mat The matrix to apply the rotation to.
	 * @param[in] c The point to rotation around.
	 * @param[in] th The rotation angle.
	 *
	 * @return A matrix with the applied rotation.
	 ******************************************************************************************************************/
	template <class T> glm::tmat4x4<T> rotate_around(const glm::tmat4x4<T>& mat, const glm::tvec2<T>& c, const angle<T>& th) noexcept;

	/******************************************************************************************************************
	 * Applies a 3D rotation around a point to a matrix.
	 *
	 * @param[in] mat The matrix to apply the rotation to.
	 * @param[in] c The point to rotation around.
	 * @param[in] th The rotation angle.
	 * @param[in] axis The axis to rotate around.
	 *
	 * @return A matrix with the applied rotation.
	 ******************************************************************************************************************/
	template <class T>
	glm::tmat4x4<T> rotate_around(const glm::tmat4x4<T>& mat, const glm::tvec3<T>& c, const angle<T>& th,
								  const glm::tvec3<T>& axis) noexcept;

	/// @}
} // namespace tr

#include "geometry_impl.hpp"