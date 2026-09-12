/// @file
/// @brief Provides alignment-related datatypes and functions.

#pragma once
#include <tr/utility/integer.hpp>
#include <tr/utility/macro.hpp>

//

namespace tr
{
	/// Horizontal alignment.
	enum class halign : u8
	{
		/// Left horizontal alignment.
		left = 0,

		/// Center horizontal alignment.
		center = 1,

		/// Right horizontal alignment.
		right = 2
	};

	/// Vertical alignment.
	enum class valign : u8
	{
		/// Top vertical alignment.
		top = 0,

		/// Center vertical alignment.
		center = 3,

		/// Bottom vertical alignment.
		bottom = 6
	};

	/// 2D alignment.
	enum class align : u8
	{
		/// Top-left alignment.
		tl = std::to_underlying(valign::top) + std::to_underlying(halign::left),

		/// Top-center alignment.
		tc = std::to_underlying(valign::top) + std::to_underlying(halign::center),

		/// Top-right alignment.
		tr = std::to_underlying(valign::top) + std::to_underlying(halign::right),

		/// Center-left alignment.
		cl = std::to_underlying(valign::center) + std::to_underlying(halign::left),

		/// Center alignment.
		cc = std::to_underlying(valign::center) + std::to_underlying(halign::center),

		/// Center-right alignment.
		cr = std::to_underlying(valign::center) + std::to_underlying(halign::right),

		/// Bottom-left alignment.
		bl = std::to_underlying(valign::bottom) + std::to_underlying(halign::left),

		/// Bottom-center alignment.
		bc = std::to_underlying(valign::bottom) + std::to_underlying(halign::center),

		/// Bottom-right alignment.
		br = std::to_underlying(valign::bottom) + std::to_underlying(halign::right)
	};

	/// @name Alignment
	/// @{

	/// Gets the horizontal component of a 2D alignment.
	/// @param align 2D alignment.
	/// @return Horizontal component of the alignment.
	[[nodiscard]] constexpr halign to_halign(align align) noexcept
	{
		return static_cast<halign>(std::to_underlying(align) % 3);
	}

	/// Gets the vertical component of a 2D alignment.
	/// @param align 2D alignment.
	/// @return Vertical component of the alignment.
	[[nodiscard]] constexpr valign to_valign(align align) noexcept
	{
		return static_cast<valign>(std::to_underlying(align) - std::to_underlying(align) % 3);
	}

	/// Combines horizontal and vertical alignment into a 2D alignment.
	/// @param valign Vertical component of the alignment.
	/// @param halign Horizontal component of the alignment.
	/// @return Combined 2D alignment.
	[[nodiscard]] constexpr align operator|(const valign& valign, const halign& halign) noexcept
	{
		return static_cast<align>(std::to_underlying(halign) + std::to_underlying(valign));
	}

	/// Combines horizontal and vertical alignment into a 2D alignment.
	/// @param halign Horizontal component of the alignment.
	/// @param valign Vertical component of the alignment.
	/// @return Combined 2D alignment.
	[[nodiscard]] constexpr align operator|(const halign& halign, const valign& valign) noexcept
	{
		return static_cast<align>(std::to_underlying(halign) + std::to_underlying(valign));
	}

	//

	/// Computes the top-left corner of a rectangle given a position, size, and anchor point.
	/// @tparam Element Type of the vector elements.
	/// @param pos Position of the rectangle.
	/// @param size Size of the rectangle.
	/// @param pos_anchor What `pos` represents within the rectangle.
	/// @return Top-left corner of the rectangle.
	template <typename Element>
	[[nodiscard]] constexpr glm::tvec2<Element> tl(glm::tvec2<Element> pos, glm::tvec2<Element> size, align pos_anchor) noexcept
	{
		switch (pos_anchor) {
		case align::tl:
			return pos;
		case align::tc:
			return {pos.x - size.x / 2, pos.y};
		case align::tr:
			return {pos.x - size.x, pos.y};
		case align::cl:
			return {pos.x, pos.y - size.y / 2};
		case align::cc:
			return pos - size / Element{2};
		case align::cr:
			return {pos.x - size.x, pos.y - size.y / 2};
		case align::bl:
			return {pos.x, pos.y - size.y};
		case align::bc:
			return {pos.x - size.x / 2, pos.y - size.y};
		case align::br:
			return pos - size;
		default:
			TR_UNREACHABLE;
		}
	}

	/// @}
} // namespace tr