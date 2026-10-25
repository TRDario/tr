/// @file
/// @brief Provides `tr::triangle`.

#pragma once
#include <tr/utility/default_binary_io.hpp>
#include <tr/utility/polygon.hpp>
#include <tr/utility/vector.hpp>

//

namespace tr
{
	/// 2D triangle structure.
	/// @tparam Rep Representation type of the triangle.
	template <arithmetic Rep>
	struct triangle
	{
		/// First point of the triangle.
		glm::tvec2<Rep> a;

		/// Second point of the triangle.
		glm::tvec2<Rep> b;

		/// Third point of the triangle.
		glm::tvec2<Rep> c;

		/// @name Conversion operators
		/// @{

		template <arithmetic TargetRep>
		[[nodiscard]] explicit constexpr operator triangle<TargetRep>() const noexcept
		{
			return triangle<TargetRep>{static_cast<glm::tvec2<TargetRep>>(a), static_cast<glm::tvec2<TargetRep>>(b),
									   static_cast<glm::tvec2<TargetRep>>(c)};
		}

		/// @}
		/// @name Mathematical operations
		/// @{

		/// Determines the winding order of the triangle.
		/// @return Winding order of the triangle.
		[[nodiscard]] constexpr winding_order winding_order() const noexcept
		{
			return cross(b - a, c - a) <= 0 ? winding_order::ccw : winding_order::cw;
		}

		/// Determines whether a point is contained inside the triangle.
		/// @tparam PointRet Representation type of the point.
		/// @param point Point to check for being in the triangle.
		/// @return `true` if `point` is within the triangle, `false` otherwise.
		template <arithmetic PointRep = Rep>
		[[nodiscard]] constexpr bool contains(glm::tvec2<PointRep> point) const noexcept
		{
			using BaseCommonRep = std::common_type_t<Rep, PointRep>;
			using CommonRep = std::conditional_t<std::integral<BaseCommonRep>, double, BaseCommonRep>;
			const glm::tvec2<CommonRep> a_cr{a};
			const glm::tvec2<CommonRep> b_cr{b};
			const glm::tvec2<CommonRep> c_cr{c};
			const glm::tvec2<CommonRep> point_cr{point};

			const CommonRep dx{point_cr.x - c_cr.x};
			const CommonRep dy{point_cr.y - c_cr.y};
			const CommonRep dx_cb{c_cr.x - b_cr.x};
			const CommonRep dy_bc{b_cr.y - c_cr.y};
			const CommonRep d{dy_bc * (a_cr.x - c_cr.x) + dx_cb * (a_cr.y - c_cr.y)};
			const CommonRep s{dy_bc * dx + dx_cb * dy};
			const CommonRep t{(c_cr.y - a_cr.y) * dx + (a_cr.x - c_cr.x) * dy};
			return d < 0 ? (s <= 0 && t <= 0 && s + t >= d) : (s >= 0 && t >= 0 && s + t <= d);
		}

		/// @}
	};
} // namespace tr

//

/// Enables default binary IO for triangles.
template <typename Rep>
inline constexpr bool tr::enable_default_binary_io<tr::triangle<Rep>>{true};