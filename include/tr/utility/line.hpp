/// @file
/// @brief Provides `tr::line_segment` and line-related functions.

#pragma once
#include <algorithm>
#include <cmath>
#include <glm/glm.hpp>
#include <optional>
#include <tr/utility/angle.hpp>
#include <tr/utility/default_binary_io.hpp>
#include <type_traits>
//

namespace tr
{
	/// 2D line segment datatype.
	/// @tparam Rep Representation type of the line segment.
	template <std::floating_point Rep>
	struct line_segment
	{
		/// First point of the line segment.
		glm::tvec2<Rep> a;

		/// Second point of the line segment.
		glm::tvec2<Rep> b;

		/// @name Conversion operators
		/// @{

		/// Casts the line segment to a line segment represented with another representation type.
		/// @tparam TargetRep Representation type to cast to.
		/// @return Cast line segment value.
		template <std::floating_point TargetRep>
		[[nodiscard]] explicit constexpr operator line_segment<TargetRep>() const noexcept
		{
			return line_segment<TargetRep>{static_cast<glm::tvec2<TargetRep>>(a), static_cast<glm::tvec2<TargetRep>>(b)};
		}

		/// @}
		/// @name Length
		/// @{

		/// Gets the length of the line segment.
		/// @return `|b-a|`.
		[[nodiscard]] Rep length() const noexcept
		{
			return std::sqrt(length2());
		}

		/// Gets the square length of the line segment.
		/// @return `|b-a|²`.
		[[nodiscard]] constexpr Rep length2() const noexcept
		{
			const glm::tvec2<Rep> delta{b - a};
			return glm::dot(delta, delta);
		}

		/// @}
		/// @name Points
		/// @{

		/// Calculates the closest point to `point` on the line segment.
		/// @tparam PointRep Representation type of the point to project.
		/// @param point Point to project onto the line.
		/// @return Projection of `point` onto the line.
		template <arithmetic PointRep>
		[[nodiscard]] constexpr glm::tvec2<std::common_type_t<Rep, PointRep>> closest_point(glm::tvec2<PointRep> point) const noexcept
		{
			using CommonRep = std::common_type_t<Rep, PointRep>;
			const glm::tvec2<CommonRep> a_cr{a};

			const glm::tvec2<CommonRep> delta{glm::tvec2<CommonRep>{b} - a_cr};
			const glm::tvec2<CommonRep> pdelta{glm::tvec2<CommonRep>{point} - a_cr};
			const CommonRep delta_length2{glm::dot(delta, delta)};
			return a_cr + delta * std::clamp(glm::dot(pdelta, delta) / delta_length2, CommonRep{0}, CommonRep{1});
		}

		/// @}
	};

	/// @name Intersection
	/// @{

	/// Determines whether 3 points are collinear.
	/// @tparam ARep, BRep, CRep Representation types of the points.
	/// @param a, b, c Points to check for collinearity.
	/// @return `true` if the points are collinear, `false` otherwise.
	template <std::floating_point ARep, std::floating_point BRep, std::floating_point CRep>
	[[nodiscard]] constexpr bool collinear(glm::tvec2<ARep> a, glm::tvec2<BRep> b, glm::tvec2<CRep> c) noexcept
	{
		using CommonRep = std::common_type_t<ARep, BRep, CRep>;
		const glm::tvec2<CommonRep> a_cr{a};
		const glm::tvec2<CommonRep> b_cr{b};
		const glm::tvec2<CommonRep> c_cr{c};

		const CommonRep tolerance{std::abs(std::max({a_cr.x, a_cr.y, b_cr.x, b_cr.y, c_cr.x, c_cr.y})) * CommonRep{1e-6}};
		return std::abs((b_cr.x - a_cr.x) * (c_cr.y - a_cr.y) - (c_cr.x - a_cr.x) * (b_cr.y - a_cr.y)) < tolerance;
	}

	/// Determines if two line segments intersect.
	/// @tparam LhsRep, RhsRep Representation types of the line segments.
	/// @param lhs, rhs Line segments to check for intersection.
	/// @return `true` if the lines intersect, `false` otherwise.
	template <std::floating_point LhsRep, std::floating_point RhsRep>
	[[nodiscard]] constexpr bool intersecting(const line_segment<LhsRep>& lhs, const line_segment<RhsRep>& rhs) noexcept
	{
		using CommonRep = std::common_type_t<LhsRep, RhsRep>;
		const line_segment<CommonRep> lhs_cr{static_cast<line_segment<CommonRep>>(lhs)};
		const line_segment<CommonRep> rhs_cr{static_cast<line_segment<CommonRep>>(rhs)};

		const glm::tvec2<CommonRep> delta{rhs_cr.b - rhs_cr.a};
		const glm::tvec2<CommonRep> normal{lhs_cr.b.y - lhs_cr.a.y, -lhs_cr.b.x + lhs_cr.a.x};
		const CommonRep dot{glm::dot(delta, normal)};
		if (std::abs(dot) < CommonRep{1e-6}) {
			return false;
		}
		const CommonRep t{glm::dot(lhs_cr.a - rhs_cr.a, normal) / dot};
		if (t < 0 || t > 1) {
			return false;
		}
		const glm::tvec2<CommonRep> result{rhs_cr.a + delta * t};
		const glm::tvec2<CommonRep> min{glm::min(lhs_cr.a, lhs_cr.b)};
		const glm::tvec2<CommonRep> max{glm::max(lhs_cr.a, lhs_cr.b)};
		return result.x >= min.x && result.x <= max.x && (result.x != lhs_cr.a.x || (result.y >= min.y && result.y <= max.y));
	}

	/// Calculates the intersection point of two line segments.
	/// @tparam LhsRep, RhsRep Representation types of the line segments.
	/// @param lhs, rhs Line segments to check for intersection.
	/// @return Intersection point of the lines, if one exists.
	template <std::floating_point LhsRep, std::floating_point RhsRep>
	[[nodiscard]] constexpr std::optional<glm::tvec2<std::common_type_t<LhsRep, RhsRep>>> intersection(
		const line_segment<LhsRep>& lhs, const line_segment<RhsRep>& rhs) noexcept
	{
		using CommonRep = std::common_type_t<LhsRep, RhsRep>;
		const line_segment<CommonRep> lhs_cr{static_cast<line_segment<CommonRep>>(lhs)};
		const line_segment<CommonRep> rhs_cr{static_cast<line_segment<CommonRep>>(rhs)};

		const glm::tvec2<CommonRep> delta{rhs_cr.b - rhs_cr.a};
		const glm::tvec2<CommonRep> normal{lhs_cr.b.y - lhs_cr.a.y, -lhs_cr.b.x + lhs_cr.a.x};
		const CommonRep dot{glm::dot(delta, normal)};
		if (std::abs(dot) < CommonRep{1e-6}) {
			return std::nullopt;
		}
		const CommonRep t{glm::dot(lhs_cr.a - rhs_cr.a, normal) / dot};
		if (t < 0 || t > 1) {
			return std::nullopt;
		}
		const glm::tvec2<CommonRep> result{rhs_cr.a + delta * t};
		const glm::tvec2<CommonRep> min{glm::min(lhs_cr.a, lhs_cr.b)};
		const glm::tvec2<CommonRep> max{glm::max(lhs_cr.a, lhs_cr.b)};
		if ((result.x < min.x || result.x > max.x) || (result.x == lhs_cr.a.x && (result.y < min.y || result.y > max.y))) {
			return std::nullopt;
		}
		return result;
	}

	/// Calculates the intersection point of a line with a line segment.
	/// @tparam PointRep Representation type of the point on the line.
	/// @tparam AngleRep Representation type of the line angle.
	/// @tparam SegmentRep Representation type of the line segment.
	/// @param lhs_point Point on the first line.
	/// @param lhs_angle Angle of the first line.
	/// @param rhs Line segment to check for intersection.
	/// @return Intersection point of the lines, if one exists.
	template <std::floating_point PointRep, std::floating_point AngleRep, std::floating_point SegmentRep>
	[[nodiscard]] std::optional<glm::tvec2<std::common_type_t<PointRep, AngleRep, SegmentRep>>> intersection(
		glm::tvec2<PointRep> lhs_point, angle<AngleRep> lhs_angle, const line_segment<SegmentRep>& rhs) noexcept
	{
		using CommonRep = std::common_type_t<PointRep, AngleRep, SegmentRep>;
		const glm::tvec2<CommonRep> lhs_point_cr{lhs_point};
		const line_segment<CommonRep> rhs_cr{static_cast<line_segment<CommonRep>>(rhs)};
		const angle<CommonRep> lhs_angle_cr{lhs_angle};

		const glm::tvec2<CommonRep> delta{rhs_cr.b - rhs_cr.a};
		const glm::tvec2<CommonRep> normal{lhs_angle_cr.sin(), -lhs_angle_cr.cos()};
		const CommonRep dot{glm::dot(delta, normal)};
		if (std::abs(dot) < CommonRep{1e-6}) {
			return std::nullopt;
		}
		const CommonRep t{glm::dot(lhs_point_cr - rhs_cr.a, normal) / dot};
		if (t >= 0 && t <= 1) {
			return rhs_cr.a + delta * t;
		}
		return std::nullopt;
	}

	/// Calculates the intersection point of two lines.
	/// @tparam LhsPointRep Representation type of the point on the first line.
	/// @tparam LhsAngleRep Representation type of the first line angle.
	/// @tparam RhsPointRep Representation type of the point on the second line.
	/// @tparam RhsAngleRep Representation type of the second line angle.
	/// @param lhs_point Point on the first line.
	/// @param lhs_angle Angle of the first line.
	/// @param rhs_point Point on the second line.
	/// @param rhs_angle Angle of the second line.
	/// @return Intersection point of the lines, if one exists.
	template <std::floating_point LhsPointRep, std::floating_point LhsAngleRep, std::floating_point RhsPointRep,
			  std::floating_point RhsAngleRep>
	[[nodiscard]] std::optional<glm::tvec2<std::common_type_t<LhsPointRep, LhsAngleRep, RhsPointRep, RhsAngleRep>>> intersection(
		glm::tvec2<LhsPointRep> lhs_point, angle<LhsAngleRep> lhs_angle, glm::tvec2<RhsPointRep> rhs_point,
		angle<RhsAngleRep> rhs_angle) noexcept
	{
		using CommonRep = std::common_type_t<LhsPointRep, LhsAngleRep, RhsPointRep, RhsAngleRep>;
		const glm::tvec2<CommonRep> lhs_point_cr{lhs_point};
		const glm::tvec2<CommonRep> rhs_point_cr{rhs_point};
		const angle<CommonRep> lhs_angle_cr{lhs_angle};
		const angle<CommonRep> rhs_angle_cr{rhs_angle};

		const glm::tvec2<CommonRep> delta{rhs_angle_cr.cos(), rhs_angle_cr.sin()};
		const glm::tvec2<CommonRep> normal{lhs_angle_cr.sin(), -lhs_angle_cr.cos()};
		const CommonRep dot{glm::dot(delta, normal)};
		if (std::abs(dot) < CommonRep{1e-6}) {
			return std::nullopt;
		}
		const CommonRep t{glm::dot(lhs_point_cr - rhs_point_cr, normal) / dot};
		return rhs_point_cr + delta * t;
	}

	/// Calculates the intersection point of two lines.
	/// @tparam PointRep Representation type of the point on the first line.
	/// @tparam AngleRep Representation type of the first line angle.
	/// @tparam ARep Representation type of the first point on the second line.
	/// @tparam BRep Representation type of the second point on the second line.
	/// @param lhs_point Point on the first line.
	/// @param lhs_angle Angle of the first line.
	/// @param rhs_a, rhs_b Points on the second line.
	/// @return Intersection point of the lines, if one exists.
	template <std::floating_point PointRep, std::floating_point AngleRep, std::floating_point ARep, std::floating_point BRep>
	[[nodiscard]] std::optional<glm::tvec2<std::common_type_t<PointRep, AngleRep, ARep, BRep>>> intersection(
		glm::tvec2<PointRep> lhs_point, angle<AngleRep> lhs_angle, glm::tvec2<ARep> rhs_a, glm::tvec2<BRep> rhs_b) noexcept
	{
		using CommonRep = std::common_type_t<PointRep, AngleRep, ARep, BRep>;
		const glm::tvec2<CommonRep> lhs_point_cr{lhs_point};
		const angle<CommonRep> lhs_angle_cr{lhs_angle};
		const glm::tvec2<CommonRep> rhs_a_cr{rhs_a};
		const glm::tvec2<CommonRep> rhs_b_cr{rhs_b};

		const glm::tvec2<CommonRep> delta{rhs_b_cr - rhs_a_cr};
		const glm::tvec2<CommonRep> normal{lhs_angle_cr.sin(), -lhs_angle_cr.cos()};
		const CommonRep dot{glm::dot(delta, normal)};
		if (std::abs(dot) < CommonRep{1e-6}) {
			return std::nullopt;
		}
		const CommonRep t{glm::dot(lhs_point_cr - rhs_a_cr, normal) / dot};
		return rhs_a_cr + delta * t;
	}

	/// Calculates the intersection point of two lines.
	/// @tparam LhsARep, LhsBRep Representation types of the points on the first line.
	/// @tparam RhsARep, RhsBRep Representation types of the points on the second line.
	/// @param lhs_a, lhs_b Points on the first line.
	/// @param rhs_a, rhs_b Points on the second line.
	/// @return Intersection point of the lines, if one exists.
	template <std::floating_point LhsARep, std::floating_point LhsBRep, std::floating_point RhsARep, std::floating_point RhsBRep>
	[[nodiscard]] constexpr std::optional<glm::tvec2<std::common_type_t<LhsARep, LhsBRep, RhsARep, RhsBRep>>> insersection(
		glm::tvec2<LhsARep> lhs_a, glm::tvec2<LhsBRep> lhs_b, glm::tvec2<RhsARep> rhs_a, glm::tvec2<RhsBRep> rhs_b) noexcept
	{
		using CommonRep = std::common_type_t<LhsARep, LhsBRep, RhsARep, RhsBRep>;
		const glm::tvec2<CommonRep> lhs_a_cr{lhs_a};
		const glm::tvec2<CommonRep> lhs_b_cr{lhs_b};
		const glm::tvec2<CommonRep> rhs_a_cr{rhs_a};
		const glm::tvec2<CommonRep> rhs_b_cr{rhs_b};

		const glm::tvec2<CommonRep> delta{rhs_b_cr - rhs_a_cr};
		const glm::tvec2<CommonRep> normal{lhs_b_cr.y - lhs_a_cr.y, -lhs_b_cr.x + lhs_a_cr.x};
		const CommonRep dot{glm::dot(delta, normal)};
		if (std::abs(dot) < CommonRep{1e-6}) {
			return std::nullopt;
		}
		const CommonRep t{glm::dot(lhs_a_cr - rhs_a_cr, normal) / dot};
		return rhs_a_cr + delta * t;
	}

	/// @}
} // namespace tr

//

/// Enables default binary IO for line segments.
/// @tparam Element Type of the elements of the line segment points.
template <std::floating_point Element>
inline constexpr bool tr::enable_default_binary_io<tr::line_segment<Element>>{true};