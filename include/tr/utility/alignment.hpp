/// @file
/// @brief Provides alignment-related datatypes and functions.

/// @ingroup math
/// @defgroup alignment Alignment
/// @brief Contains alignment-related datatypes and functions.

#pragma once
#include <tr/utility/integer.hpp>
#include <tr/utility/macro.hpp>

//

namespace tr
{
	/// @ingroup alignment
	/// @brief Horizontal alignment.
	enum class halign : u8
	{
		/// @brief Left horizontal alignment.
		left = 0,

		/// @brief Center horizontal alignment.
		center = 1,

		/// @brief Right horizontal alignment.
		right = 2
	};

	/// @ingroup alignment
	/// @brief Vertical alignment.
	enum class valign : u8
	{
		/// @brief Top vertical alignment.
		top = 0,

		/// @brief Center vertical alignment.
		center = 3,

		/// @brief Bottom vertical alignment.
		bottom = 6
	};

	/// @ingroup alignment
	/// @brief 2D alignment.
	enum class align : u8
	{
		/// @brief Top-left alignment.
		tl = std::to_underlying(valign::top) + std::to_underlying(halign::left),

		/// @brief Top-center alignment.
		tc = std::to_underlying(valign::top) + std::to_underlying(halign::center),

		/// @brief Top-right alignment.
		tr = std::to_underlying(valign::top) + std::to_underlying(halign::right),

		/// @brief Center-left alignment.
		cl = std::to_underlying(valign::center) + std::to_underlying(halign::left),

		/// @brief Center alignment.
		cc = std::to_underlying(valign::center) + std::to_underlying(halign::center),

		/// @brief Center-right alignment.
		cr = std::to_underlying(valign::center) + std::to_underlying(halign::right),

		/// @brief Bottom-left alignment.
		bl = std::to_underlying(valign::bottom) + std::to_underlying(halign::left),

		/// @brief Bottom-center alignment.
		bc = std::to_underlying(valign::bottom) + std::to_underlying(halign::center),

		/// @brief Bottom-right alignment.
		br = std::to_underlying(valign::bottom) + std::to_underlying(halign::right)
	};

	//

	/// @ingroup alignment
	/// @brief Returns the horizontal component of a 2D alignment.
	/// @param align 2D alignment.
	/// @return Horizontal component of the alignment.
	[[nodiscard]] constexpr halign to_halign(align align) noexcept
	{
		return static_cast<halign>(std::to_underlying(align) % 3);
	}

	/// @ingroup alignment
	/// @brief Returns the vertical component of a 2D alignment.
	/// @param align 2D alignment.
	/// @return Vertical component of the alignment.
	[[nodiscard]] constexpr valign to_valign(align align) noexcept
	{
		return static_cast<valign>(std::to_underlying(align) - std::to_underlying(align) % 3);
	}

	/// @ingroup alignment
	/// @brief Combines horizontal and vertical alignment into a 2D alignment.
	/// @param valign Vertical component of the alignment.
	/// @param halign Horizontal component of the alignment.
	/// @return Combined 2D alignment.
	[[nodiscard]] constexpr align operator|(const valign& valign, const halign& halign) noexcept
	{
		return static_cast<align>(std::to_underlying(halign) + std::to_underlying(valign));
	}

	/// @ingroup alignment
	/// @brief Combines horizontal and vertical alignment into a 2D alignment.
	/// @param halign Horizontal component of the alignment.
	/// @param valign Vertical component of the alignment.
	/// @return Combined 2D alignment.
	[[nodiscard]] constexpr align operator|(const halign& halign, const valign& valign) noexcept
	{
		return static_cast<align>(std::to_underlying(halign) + std::to_underlying(valign));
	}

	//

	/// @ingroup alignment
	/// @brief Returns the top-left corner of a rectangle given a position, size, and anchor point.
	/// @tparam Rep Vector representation type.
	/// @param pos Position of the rectangle.
	/// @param size Size of the rectangle.
	/// @param pos_anchor What `pos` represents within the rectangle.
	/// @return Top-left corner of the rectangle.
	template <typename Rep>
	[[nodiscard]] constexpr glm::tvec2<Rep> tl(glm::tvec2<Rep> pos, glm::tvec2<Rep> size, align pos_anchor) noexcept
	{
		switch (pos_anchor) {
		case align::tl:
			return pos;
		case align::tc:
			return glm::tvec2<Rep>{pos.x - size.x / 2, pos.y};
		case align::tr:
			return glm::tvec2<Rep>{pos.x - size.x, pos.y};
		case align::cl:
			return glm::tvec2<Rep>{pos.x, pos.y - size.y / 2};
		case align::cc:
			return pos - size / Rep{2};
		case align::cr:
			return glm::tvec2<Rep>{pos.x - size.x, pos.y - size.y / 2};
		case align::bl:
			return glm::tvec2<Rep>{pos.x, pos.y - size.y};
		case align::bc:
			return glm::tvec2<Rep>{pos.x - size.x / 2, pos.y - size.y};
		case align::br:
			return pos - size;
		default:
			TR_UNREACHABLE;
		}
	}
} // namespace tr