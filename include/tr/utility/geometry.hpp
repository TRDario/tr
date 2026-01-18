///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides geometry-related datatypes and functionality.                                                                                //
//                                                                                                                                       //
// Horizontal, vertical, and 2D alignment enumerators are provided. 2D alignments can be composed from and decomposed into components:   //
//     - tr::align align{tr::valign::top | tr::halign::left} -> tr::align::tl                                                            //
//     - tr::to_halign(tr::align::cr) -> tr::halign::right                                                                               //
//     - tr::to_valign(tr::align::cr) -> tr::valign::center                                                                              //
//                                                                                                                                       //
// A winding order enumerator is provided (with values cw and ccw).                                                                      //
//                                                                                                                                       //
// An N-dimensional axis-aligned rectangle structure is provided as tr::rect<S, T> (a few shorthands are also provided in the form       //
// tr::TrectX). Rects are binary readable and writable and can be checked for intersections and whether a point is contained within them://
//     - tr::frect2 rect{} -> {.tl{0, 0}, .size{0, 0}}                                                                                   //
//     - tr::frect2 rect{glm::vec2{500, 500}} -> {.tl{0, 0}, .size{500, 500}}                                                            //
//     - tr::frect2 rect{{100, 100}, {100, 100}} -> {.tl{100, 100}, .size{100, 100}}                                                     //
//     - tr::frect2{{100, 100}, {100, 100}}.contains({150, 150}) -> true                                                                 //
//     - tr::intersecting(tr::frect2{{100, 100}, {100, 100}}, tr::frect2{{50, 125}, {400, 50}}) -> true                                  //
//                                                                                                                                       //
// A 2D triangle structure is provided. Triangles are binary readable and writable, and can be queried for their winding order and       //
// whether a point is contained within them:                                                                                             //
//     - tr::triangle tri{{500, 500}, {600, 600}, {700, 500}}                                                                            //
//     - tri.winding_order() -> tr::winding_order::cw                                                                                    //
//     - tri.contains({600, 550}) -> true                                                                                                //
//                                                                                                                                       //
// A 2D circle structure is provided. Circles are binary readable and writable, and can be checked for containing a point:               //
//     - tr::circle circle{{500, 500}, 100} -> circle with radius 100 centered at (500, 500)                                             //
//     - circle.contains({450, 550}) -> true                                                                                             //
//                                                                                                                                       //
// The X, Y, and Z axes are provided as the constants tr::x_axis, tr::y_axis, tr::z_axis.                                                //
//                                                                                                                                       //
// A number of miscellaneous vector operations are provided:                                                                             //
//     - tr::normal(90_deg) -> {0, 1} (normalized vector corresponding to the angle)                                                     //
//     - tr::magth(5, 90_deg) -> {0, 5} (vector constructed through magnitude + angle)                                                   //
//     - tr::length2({10, 10}) -> 200.0f (squared length of the vector)                                                                  //
//     - tr::dist2({10, 10}, {20, 20}) -> 200.0f (squared distance between vectors)                                                      //
//     - tr::tl({500, 500}, {200, 200}, tr::align::cc) -> {400, 400} (top-left corner of a rectangle described with anchor+size+align)   //
//     - tr::inverse({100, 100}) -> {0.01, 0.01} (inverse vector)                                                                        //
//     - tr::cross({10, 10}, {5, 20}) -> 100 (2D cross product)                                                                          //
//     - tr::collinear({5, 5}, {10, 10}, {20, 20}) -> true (whether the points are collinear)                                            //
//                                                                                                                                       //
// A 2D line segment structure is provided. Line segments are binary readable and writable, the closest point on the line to an          //
// arbitrary points can be determined, and the intersection point of two line segments or lines can be determined:                       //
//     - tr::line_segment{{100, 100}, {200, 100}}.closest_point({150, 200}) -> {150, 100}                                                //
//     - tr::intersecting(tr::line_segment{{100, 100}, {200, 200}}, tr::line_segment{{100, 200}, {200, 100}}) -> true                    //
//     - tr::intersection(tr::line_segment{{100, 100}, {200, 200}}, tr::line_segment{{100, 200}, {200, 100}}) -> {150, 150}              //
//     - tr::intersection(tr::line_segment{{100, 100}, {200, 100}}, {250, 50}, {250, 150}) -> std::nullopt                               //
//     - tr::intersection({100, 100}, {200, 100}, {250, 50}, {250, 150}) -> {250, 100}                                                   //
//     - tr::intersection({100, 100}, {200, 100}, {250, 50}, 90_deg) -> {250, 100}                                                       //
//     - tr::intersection({100, 100}, 0_deg, {250, 50}, 90_deg) -> {250, 100}                                                            //
//                                                                                                                                       //
// A mirror repeat mapping function is provided, defined for arithmetic-arithmetic, vector-vector, or vector-rect combinations:          //
//     - tr::mirror_repeat(9, 0, 5) -> 1                                                                                                 //
//     - tr::mirror_repeat({1020, 970}, {0, 0}, {1000, 1000}) -> {980, 970}                                                              //
//     - tr::mirror_repeat({1020, 970}, tr::frect2{{1000, 1000}}) -> {980, 970}                                                          //
//                                                                                                                                       //
// Additional matrix multiplication operators (to avoid having to cast a vec2 into a vec4 when multiplying with mat4, for example) are   //
// defined in tr::matrix_operators, as well as tr::literals.                                                                             //
//                                                                                                                                       //
// A few convenient matrix transformations are provided:                                                                                 //
//     - tr::ortho(tr::frect2{{1000, 1000}}) -> equivalent to glm::ortho(0, 1000, 1000, 0)                                               //
//     - tr::scale_around(mat, {10, 10}, {2, 2}) -> applies a 2x scaling transformation centered on (10, 10) on mat                      //
//     - tr::scale_around(mat, {10, 10, 10}, {2, 2, 2}) -> applies a 2x scaling transformation centered on (10, 10, 10) on mat           //
//     - tr::rotate_around(mat, {10, 10}, 90_deg) -> applies a 90-degree rotation centered on (10, 10) on mat                            //
//     - tr::rotate_around(mat, {10, 10, 10}, 90_deg, y_axis) -> applies a 90-degree y-axis rotation centered on (10, 10, 10) on mat     //
//                                                                                                                                       //
// Polygons (as spans of glm::vec2) can be checked for their winding order, whether they're simple, whether a point is contained inside  //
// then, or whether two polygons intersect:                                                                                              //
//     - tr::polygon_winding_order(vertices) -> tr::winding_order::cw                                                                    //
//     - tr::simple_polygon(vertices) -> true                                                                                            //
//     - tr::point_in_polygon(vertices, {0, 0}) -> false                                                                                 //
//     - tr::intersecting(vertices1, vertices2) -> true                                                                                  //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "angle.hpp"
#include "binary_io.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Horizontal alignment.
	enum class halign : u8 {
		left,   // Left horizontal alignment.
		center, // Center horizontal alignment.
		right   // Right horizontal alignment.
	};
	// Vertical alignment.
	enum class valign : u8 {
		top,        // Top vertical alignment.
		center = 3, // Center vertical alignment.
		bottom = 6  // Bottom vertical alignment.
	};
	// 2D alignment.
	enum class align : u8 {
		tl = u8(valign::top) + u8(halign::left),      // Top-left alignment.
		tc = u8(valign::top) + u8(halign::center),    // Top-center alignment.
		tr = u8(valign::top) + u8(halign::right),     // Top-right alignment.
		cl = u8(valign::center) + u8(halign::left),   // Center-left alignment.
		cc = u8(valign::center) + u8(halign::center), // Center alignment.
		cr = u8(valign::center) + u8(halign::right),  // Center-right alignment.
		bl = u8(valign::bottom) + u8(halign::left),   // Bottom-left alignment.
		bc = u8(valign::bottom) + u8(halign::center), // Bottom-center alignment.
		br = u8(valign::bottom) + u8(halign::right)   // Bottom-right alignment.
	};
	// Converts a 2D alignment to a horizontal alignment.
	constexpr halign to_halign(align align);
	// Converts a 2D alignment to a vertical alignment.
	constexpr valign to_valign(align align);
	// Combines a horizontal and vertical alignment into a 2D alignment.
	constexpr align operator|(const valign& valign, const halign& halign);
	// Combines a horizontal and vertical alignment into a 2D alignment.
	constexpr align operator|(const halign& halign, const valign& valign);

	// Polygon winding order.
	enum class winding_order : bool {
		cw, // Clockwise winding order.
		ccw // Counter-clockwise winding order.
	};

	// Rectangle object.
	template <int S, class T> struct rect {
		// The offset of the top-left corner of the rect.
		glm::vec<S, T> tl;
		// The size of the rectangle.
		glm::vec<S, T> size;

		// Default-constructs a rect.
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
	template <int S, class T> struct binary_reader<rect<S, T>> : raw_binary_reader<rect<S, T>> {};
	// Rect binary writers.
	template <int S, class T> struct binary_writer<rect<S, T>> : raw_binary_writer<rect<S, T>> {};
	// Determines if two rects intersect.
	template <class T1, class T2> constexpr bool intersecting(const rect2<T1>& r1, const rect2<T2>& r2);

	// 2D triangle.
	struct triangle {
		// The first point of the triangle.
		glm::vec2 a;
		// The second point of the triangle.
		glm::vec2 b;
		// The third point of the triangle.
		glm::vec2 c;

		// Determines the winding order of the triangle.
		winding_order winding_order() const;
		// Determines whether a point is contained inside the triangle.
		bool contains(glm::vec2 point) const;
	};
	// Triangle binary reader.
	template <> struct binary_reader<triangle> : raw_binary_reader<triangle> {};
	// Triangle binary writer.
	template <> struct binary_writer<triangle> : raw_binary_writer<triangle> {};

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
	template <> struct binary_reader<circle> : raw_binary_reader<circle> {};
	// Circle binary writer.
	template <> struct binary_writer<circle> : raw_binary_writer<circle> {};
	// Determines if two circles intersect.
	bool intersecting(const circle& c1, const circle& c2);

	// Normalized X-axis vector.
	inline constexpr glm::vec3 x_axis{1.0f, 0.0f, 0.0f};
	// Normalized Y-axis vector.
	inline constexpr glm::vec3 y_axis{0.0f, 1.0f, 0.0f};
	// Normalized Z-axis vector.
	inline constexpr glm::vec3 z_axis{0.0f, 0.0f, 1.0f};
	// Computes a normalized vector from an angle value.
	glm::vec2 normal(angle th);
	// Creates a vector from a magnitude and angle.
	glm::vec2 magth(float mag, angle th);

	// Gets the squared length of a vector.
	template <int S, class T> constexpr T length2(glm::vec<S, T> a);
	// Gets the squared distance between two points.
	template <int S, class T1, class T2> constexpr std::common_type_t<T1, T2> dist2(glm::vec<S, T1> a, glm::vec<S, T2> b);
	// Computes the top-left corner of a rectangle given a position, size, and anchor point.
	template <class T> constexpr glm::tvec2<T> tl(glm::tvec2<T> pos, glm::tvec2<T> size, align pos_anchor);
	// Gets the inverse of a vector.
	template <int S, arithmetic T> constexpr glm::vec<S, float> inverse(const glm::vec<S, T>& vec);
	// Calculates the 2D cross product of two vectors.
	template <class T1, class T2> constexpr std::common_type_t<T1, T2> cross(glm::tvec2<T1> a, glm::tvec2<T2> b);
	// Determines whether 3 points are collinear.
	bool collinear(glm::vec2 a, glm::vec2 b, glm::vec2 c);

	// 2D line segment.
	struct line_segment {
		// The first point of the line segment.
		glm::vec2 a;
		// The second point of the line segment.
		glm::vec2 b;

		// Calculates the closest point to p on the line segment.
		glm::vec2 closest_point(glm::vec2 p) const;
	};
	// Circle binary reader.
	template <> struct binary_reader<line_segment> : raw_binary_reader<line_segment> {};
	// Circle binary writer.
	template <> struct binary_writer<line_segment> : raw_binary_writer<line_segment> {};
	// Determines if two line segments intersect.
	bool intersecting(const line_segment& l, const line_segment& r);
	// Calculates the intersection point of two line segments.
	std::optional<glm::vec2> intersection(const line_segment& l, const line_segment& r);
	// Calculates the intersection point of a line with a line segment.
	std::optional<glm::vec2> intersection(glm::vec2 lp, angle lth, const line_segment& r);
	// Calculates the intersection point of two lines.
	std::optional<glm::vec2> intersection(glm::vec2 a1, angle th1, glm::vec2 a2, angle th2);
	// Calculates the intersection point of two lines.
	std::optional<glm::vec2> insersection(glm::vec2 a1, glm::vec2 b1, glm::vec2 a2, glm::vec2 b2);
	// Calculates the intersection point of two lines.
	std::optional<glm::vec2> intersection(glm::vec2 a1, angle th1, glm::vec2 a2, glm::vec2 b2);

	// Performs a mirror repeat mapping.
	template <arithmetic T> constexpr T mirror_repeat(T v, T min, T max);
	// Performs a mirror repeat mapping.
	template <int S, arithmetic T> constexpr glm::vec<S, T> mirror_repeat(glm::vec<S, T> v, glm::vec<S, T> min, glm::vec<S, T> max);
	// Performs a mirror repeat mapping of a vector on a rect.
	template <int S, arithmetic T> glm::vec<S, T> constexpr mirror_repeat(glm::vec<S, T> v, const rect<S, T>& rect);

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

	// Finds the winding order of a polygon.
	winding_order polygon_winding_order(std::span<const glm::vec2> vertices);
	// Determines whether a polygon is simple (not self-intersecting).
	bool simple_polygon(std::span<const glm::vec2> vertices);
	// Determines whether a point is contained in a polygon.
	bool point_in_polygon(glm::vec2 p, std::span<const glm::vec2> vertices);
	// Determines whether two polygons are intersecting.
	bool intersecting(std::span<const glm::vec2> a, std::span<const glm::vec2> b);
} // namespace tr

////////////////////////////////////////////////////////////// IMPLEMENTATION /////////////////////////////////////////////////////////////

#include "geometry_impl.hpp" // IWYU pragma: export