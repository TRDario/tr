/// @file
/// @brief Provides `tr::circle` and related functionality.

/// @ingroup math
/// @defgroup circle Circle
/// @brief Contains `tr::circle` and related functionality.

#pragma once
#include <tr/utility/arithmetic_concepts.hpp>
#include <tr/utility/binary_io.hpp>
#include <tr/utility/vector.hpp>

//

namespace tr
{
	/// @ingroup circle
	/// @brief Circle in 2D space defined by a center and radius.
	/// @tparam Rep Representation type used by the circle.
	template <std::floating_point Rep>
	struct circle
	{
		/// @brief Offset of the center of the circle.
		glm::tvec2<Rep> center;

		/// @brief Radius of the circle.
		Rep radius;

		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs a circle centered at (0, 0) with radius 0.
		[[nodiscard]] constexpr circle() noexcept
			: center{0, 0}
			, radius{0}
		{
		}

		/// @brief Constructs a circle centered at (0, 0).
		/// @param radius Radius of the circle.
		[[nodiscard]] constexpr circle(Rep radius) noexcept
			: center{0, 0}
			, radius{radius}
		{
		}

		/// @brief Constructs a circle.
		/// @param center Point the circle is centered at.
		/// @param radius Radius of the circle.
		[[nodiscard]] constexpr circle(glm::tvec2<Rep> center, Rep radius) noexcept
			: center{center}
			, radius{radius}
		{
		}

		/// @brief Constructs a circle by copying from a circle of a different representation type.
		/// @tparam RhsRep Representation type of the right-hand side circle.
		/// @param circle Circle to copy from.
		template <std::floating_point RhsRep>
		[[nodiscard]] constexpr circle(const circle<RhsRep>& circle) noexcept
			: center{static_cast<glm::tvec2<RhsRep>>(circle.center)}
			, radius{static_cast<Rep>(circle.radius)}
		{
		}

		/// @}
		/// @name Operations
		/// @{

		/// @brief Checks whether a point is contained inside the circle.
		/// @tparam PointRep Representation type of the point.
		/// @param point Point to check.
		/// @return `true` if `point` is inside the circle, `false` otherwise.
		template <arithmetic PointRep>
		[[nodiscard]] constexpr bool contains(glm::tvec2<PointRep> point) const noexcept
		{
			return distance2(center, point) <= sqr(radius);
		}

		/// @brief Checks whether the circle intersects another circle.
		/// @tparam RhsRep Representation types of the right-hand side circle.
		/// @param rhs Circle to check for intersection.
		/// @return `true` if `rhs` intersects `*this`, `false` otherwise.
		template <std::floating_point RhsRep>
		[[nodiscard]] bool intersects(const circle<RhsRep>& rhs) const noexcept
		{
			return distance2(center, rhs.center) <= sqr(radius + rhs.radius);
		}

		/// @}
	};
} // namespace tr

//

/// @ingroup circle
/// @brief Specialization of `std::tuple_size` for `tr::circle`.
template <typename Rep>
struct std::tuple_size<tr::circle<Rep>> : std::integral_constant<std::size_t, 2>
{
};

/// @ingroup circle
/// @brief Specialization of `std::tuple_element` for `tr::circle`.
/// @tparam Index Index of the element.
template <std::size_t Index, typename Rep>
struct std::tuple_element<Index, tr::circle<Rep>> : std::tuple_element<Index, std::tuple<glm::tvec2<Rep>, Rep>>
{
};

namespace tr
{
	/// @ingroup circle
	/// @brief Returns an element from a circle.
	/// @tparam Index Index of the element to return.
	/// @tparam Rectangle Potentially cvref-qualified instance of `tr::circle`.
	/// @param circle Circle to return an element from.
	/// @return Element from `circle`.
	template <usize Index, typename Circle>
		requires cvref_specialization_of<Circle, circle>
	[[nodiscard]] decltype(auto) get(Circle&& circle) noexcept
	{
		if constexpr (Index == 0) {
			return std::forward_like<Circle>(circle.center);
		}
		else if constexpr (Index == 1) {
			return std::forward_like<Circle>(circle.radius);
		}
		else {
			TR_UNREACHABLE;
		}
	}
} // namespace tr