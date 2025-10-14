#pragma once
#include "angle.hpp"
#include "binary_io.hpp"

namespace tr {
	// Horizontal alignment.
	enum class halign : u8 {
		LEFT,
		CENTER,
		RIGHT
	};
	// Vertical alignment.
	enum class valign : u8 {
		TOP,
		CENTER = 3,
		BOTTOM = 6
	};
	// 2D alignment.
	enum class align : u8 {
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
	// Converts a 2D alignment to a horizontal alignment.
	constexpr halign to_halign(align align);
	// Converts a 2D alignment to a vertical alignment.
	constexpr valign to_valign(align align);
	// Combines a horizontal and vertical alignment into a 2D alignment.
	constexpr align operator|(const valign& valign, const halign& halign);
	// Combines a horizontal and vertical alignment into a 2D alignment.
	constexpr align operator|(const halign& halign, const valign& valign);

	// Rectangle object.
	template <int S, class T> struct rect {
		// The offset of the top-left corner of the rect.
		glm::vec<S, T> tl;
		// The size of the rectangle.
		glm::vec<S, T> size;

		constexpr rect() = default;
		// Constructs a rect from a top-left corner and size.
		constexpr rect(glm::vec<S, T> tl, glm::vec<S, T> size);
		// Constructs a rect with the top-left corner at the origin.
		constexpr rect(glm::vec<S, T> size);
		// Copy-constructs a rect.
		template <class T1> constexpr rect(const rect<S, T1>& rect);

		template <class T1> constexpr bool operator==(const rect<S, T1>&) const;

		// Determines whether a point is contained inside the rect.
		template <class T1> constexpr bool contains(glm::vec<S, T1> point) const;
	};
	// 2D rectangle.
	template <class T> using rect2 = rect<2, T>;
	// Shorthand for an int 2D rect.
	using irect2 = rect2<int>;
	// Shorthand for a float 2D rect.
	using frect2 = rect2<float>;
	// 3D rectangle.
	template <class T> using rect3 = rect<3, T>;
	// Shorthand for an int 3D rect.
	using irect3 = rect3<int>;
	// Shorthand for a float 3D rect.
	using frect3 = rect3<float>;
	// Rect binary readers.
	template <int S, class T> struct binary_reader<rect<S, T>> : default_binary_reader<rect<S, T>> {};
	// Rect binary writers.
	template <int S, class T> struct binary_writer<rect<S, T>> : default_binary_writer<rect<S, T>> {};
	// Determines if two rects intersect.
	template <class T1, class T2> constexpr bool intersecting(const rect2<T1>& r1, const rect2<T2>& r2);

	// 2D triangle.
	struct triangle {
		glm::vec2 a;
		glm::vec2 b;
		glm::vec2 c;

		// Determines whether a point is contained inside the triangle.
		bool contains(glm::vec2 point) const;
	};
	// Triangle binary reader.
	template <> struct binary_reader<triangle> : default_binary_reader<triangle> {};
	// Triangle binary writer.
	template <> struct binary_writer<triangle> : default_binary_writer<triangle> {};

	// 2D circle.
	struct circle {
		// The offset of the center of the circle.
		glm::vec2 c;
		// The radius of the circle.
		float r;

		// Determines whether a point is contained inside the circle.
		bool contains(glm::vec2 point) const;
	};
	// Circle binary reader.
	template <> struct binary_reader<circle> : default_binary_reader<circle> {};
	// Circle binary writer.
	template <> struct binary_writer<circle> : default_binary_writer<circle> {};
	// Determines if two circles intersect.
	bool intersecting(const circle& c1, const circle& c2);

	// Normalized X-axis vector.
	inline constexpr glm::vec3 X_AXIS{1.0f, 0.0f, 0.0f};
	// Normalized Y-axis vector.
	inline constexpr glm::vec3 Y_AXIS{0.0f, 1.0f, 0.0f};
	// Normalized Z-axis vector.
	inline constexpr glm::vec3 Z_AXIS{0.0f, 0.0f, 1.0f};
	// Computes a normalized vector from an angle value.
	glm::vec2 normal(angle th);
	// Creates a vector from a magnitude and angle.
	glm::vec2 magth(float mag, angle th);

	// Computes the top-left corner of a rectangle given a position, size, and anchor point.
	template <class T> constexpr glm::tvec2<T> tl(glm::tvec2<T> pos, glm::tvec2<T> size, align pos_anchor);
	// Gets the inverse of a vector.
	template <int S, arithmetic T> constexpr glm::vec<S, float> inverse(const glm::vec<S, T>& vec);
	// Calculates the 2D cross product of two vectors.
	template <class T1, class T2> constexpr std::common_type_t<T1, T2> cross2(glm::tvec2<T1> a, glm::tvec2<T2> b);

	// Determines whether 3 points are collinear.
	bool collinear(glm::vec2 a, glm::vec2 b, glm::vec2 c);
	// Calculates the closest point to p on the line ab.
	glm::vec2 closest_point(glm::vec2 p, glm::vec2 a, glm::vec2 b);
	// Calculates the intersection point of two line segments.
	std::optional<glm::vec2> segment_intersect(glm::vec2 a1, glm::vec2 b1, glm::vec2 a2, glm::vec2 b2);
	// Calculates the intersection point of a line with a line segment.
	std::optional<glm::vec2> segment_intersect(glm::vec2 a1, angle th1, glm::vec2 a2, glm::vec2 b2);
	// Calculates the intersection point of two lines.
	std::optional<glm::vec2> intersect(glm::vec2 a1, angle th1, glm::vec2 a2, angle th2);
	// Calculates the intersection point of two lines.
	std::optional<glm::vec2> intersect(glm::vec2 a1, glm::vec2 b1, glm::vec2 a2, glm::vec2 b2);
	// Calculates the intersection point of two lines.
	std::optional<glm::vec2> intersect(glm::vec2 a1, angle th1, glm::vec2 a2, glm::vec2 b2);

	// Performs a mirror repeat mapping.
	template <arithmetic T> constexpr T mirror_repeat(T v, T min, T max);
	// Performs a mirror repeat mapping.
	template <int S, arithmetic T> constexpr glm::vec<S, T> mirror_repeat(glm::vec<S, T> v, glm::vec<S, T> min, glm::vec<S, T> max);
	// Performs a mirror repeat mapping of a vector on a rect.
	template <int S, arithmetic T> glm::vec<S, T> constexpr mirror_repeat(glm::vec<S, T> v, const rect<S, T>& rect);

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

	// Produces an orthographic transform for a rectangular region.
	glm::mat4 ortho(const frect2& rect);
	// Applies a 2D scaling around a point to a matrix.
	glm::mat4 scale_around(const glm::mat4& mat, const glm::vec2& c, const glm::vec2& s);
	// Applies a 3D scaling around a point to a matrix.
	glm::mat4 scale_around(const glm::mat4& mat, const glm::vec3& c, const glm::vec3& s);
	// Applies a 2D rotation around a point to a matrix.
	glm::mat4 rotate_around(const glm::mat4& mat, const glm::vec2& c, const angle& th);
	// Applies a 3D rotation around a point to a matrix.
	glm::mat4 rotate_around(const glm::mat4& mat, const glm::vec3& c, const angle& th, const glm::vec3& axis);

	// Polygon winding order.
	enum class winding_order : bool {
		CW,
		CCW
	};
	// Finds the winding order of a polygon.
	winding_order polygon_winding_order(std::span<const glm::vec2> vertices);
} // namespace tr

#include "geometry_impl.hpp" // IWYU pragma: keep